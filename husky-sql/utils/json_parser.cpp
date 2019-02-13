#include "utils/json_parser.hpp"

#include "table/table.hpp"

#include <fstream>

namespace husky {
namespace sql {

JsonParser::JsonParser(const std::string & json_path): json_path_(json_path) { }

std::unique_ptr<AbstractRelNode> JsonParser::get_first_rel_node() {
	json j = get_json();
	return get_rel_node(j);
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
			std::vector<string> row_data;
			for(int i = 0; i < column_count; i++) {
				std::string data_cell;
				std::getline(linestream, data_cell, '\t');
				row_data.push_back(data_cell);
			}
			table->add_row_data(row_data); 
		}
		hlts.set_table(std::move(table));

		/* set projection */
		json projection = j["projection"];
		std::vector<int> proj_index;
		for(auto& j_proj : projection) {
			proj_index.push_back(j_proj.get<int>());
		}
		hlts.set_projection_index(proj_index);

		/* set condition */
		json j_condition = j["condition"];

		/* set inputs */
		json j_inputs = j["inputs"];
		for(auto& j_input : j_inputs) {
			auto input_rel = get_rel_node(j_input);
			hlts.add_input_rel_node(std::move(input_rel));
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

} // namespace sql
} // namespace husky