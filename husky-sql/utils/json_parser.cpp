#include "husky-sql/utils/json_parser.hpp"

#include "husky-sql/table/table.hpp"
#include "husky-sql/relnode/husky_logical_table_scan.hpp"

#include <fstream>
#include <sstream>

namespace husky {
namespace sql {

AbstractRelNode * JsonParser::get_first_rel_node() {
	json j = get_json();
	return get_rel_node(j).get();
}

std::unique_ptr<AbstractRelNode> JsonParser::get_rel_node(json j) {
	std::string name = j["name"].get<std::string>();
	if(name == "HuskyLogicalTableScan") {
		auto hlts = std::make_unique<HuskyLogicalTableScan>();

		/* set table */
		std::string table_name = j["table"].get<std::string>();
		std::string url = j["url"].get<std::string>();
		std::unique_ptr<Table> table = std::make_unique<Table>(table_name, url);
		json j_fields = j["fields"];
		int column_count = 0;
		for(auto& j_field : j_fields) {
			int f_index = j_field["index"].get<int>();
			std::string f_name = j_field["name"].get<std::string>();
			std::string f_datatype = j_field["datatype"].get<std::string>();

			table->add_column(f_index, f_name, f_datatype);
			column_count++;
		}
		std::ifstream table_file(url);
		std::string line;
		while(std::getline(table_file, line)) {
			std::stringstream linestream(line);
			std::vector<std::string> row_data;
			for(int i = 0; i < column_count; i++) {
				std::string data_cell;
				std::getline(linestream, data_cell, '\t');
				row_data.push_back(data_cell);
			}
			table->add_row_data(row_data); 
		}
		hlts->set_table(std::move(table));

		/* set projection */
		json projection = j["projection"];
		std::vector<int> proj_index;
		for(auto& j_proj : projection) {
			proj_index.push_back(j_proj.get<int>());
		}
		hlts->set_projection_index(proj_index);

		/* set condition */
		json j_condition = j["condition"];
		std::unique_ptr<Condition::Operand> head_operand = get_operand(j_condition);
		std::unique_ptr<Condition> condition = std::make_unique<Condition>();
		condition->set_head_operand(std::move(head_operand));
		hlts->set_condition(std::move(condition));

		/* set inputs */
		json j_inputs = j["inputs"];
		for(auto& j_input : j_inputs) {
			auto input_rel = get_rel_node(j_input);
			hlts->add_input_rel_node(std::move(input_rel));
		}

		return hlts;
	} else {
		return std::unique_ptr<AbstractRelNode>{};
	}
}

json JsonParser::get_json() {
	std::ifstream i(json_path_);
	json j;
	i >> j;
	return j;
}

std::unique_ptr<Condition::Operand> JsonParser::get_operand(json j) {
	std::string type = j["type"].get<std::string>();
	std::unique_ptr<Condition::Operand> operand = std::make_unique<Condition::Operand>(type);
	
	if(type == "function") {
		std::string name = j["name"].get<std::string>();
		operand->set_name(name);
	} else if(type == "field") {
		std::string datatype = j["datatype"].get<std::string>();
		int index = j["index"].get<int>();
		operand->set_datatype(datatype);
		operand->set_index(index);
	} else if(type == "constant") {
		std::string value = j["value"].get<std::string>();
		operand->set_value(value);
	}
	
	json j_operands = j["operands"];
	for(auto& j_op : j_operands) {
		operand->add_operand(std::move(get_operand(j_op)));
	}

	return operand;
}

} // namespace sql
} // namespace husky