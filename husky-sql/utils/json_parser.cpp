#include "husky-sql/utils/json_parser.hpp"

#include "husky-sql/table/table.hpp"
#include "husky-sql/relnode/husky_logical_table_scan.hpp"
#include "husky-sql/relnode/husky_logical_calc.hpp"
#include "husky-sql/relnode/husky_logical_sort.hpp"
#include "husky-sql/relnode/husky_logical_aggregate.hpp"
#include "husky-sql/relnode/husky_logical_join.hpp"
#include "husky-sql/table/table_reader_factory.hpp"
#include "husky-sql/rexnode/aggregate_call.hpp"

#include <utility>

namespace husky {
namespace sql {

std::unique_ptr<AbstractRelNode> JsonParser::get_first_rel_node() {
	json j = get_json();
	return get_rel_node(j);
}

std::unique_ptr<AbstractRelNode> JsonParser::get_rel_node(json j) {
	std::string name = j["name"].get<std::string>();
	if(name == "HuskyLogicalTableScan") {
		auto hlts = std::make_unique<HuskyLogicalTableScan>();

		/* set table */
		json j_table = j["table"];
		// std::string model_name = j_table[0]; 
		std::string table_name = j_table[1];
		husky::LOG_I << "Set HuskyLogicalTableScan for table " << table_name;
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
		
		std::string input_format = "ORC"; // TODO
		auto reader = TableReaderFactory::get_table_reader(input_format);
	    reader->read_data(url);
	    table->set_raw_data(reader->get_data());

		hlts->set_table(std::move(table));

		/* set projection and column names*/
		Table * h_table = hlts->get_table();
		json j_projection = j["projection"];
		if(!j_projection.empty()) {
			std::vector<int> proj_index;
			std::vector<std::string> column_names;
			for(auto& j_proj : j_projection) {
				int index = j_proj.get<int>();
				proj_index.push_back(index);
				column_names.push_back(h_table->get_column_name(index));
			}
			hlts->set_projection_index(proj_index);
			hlts->set_column_names(column_names);
		} else {
			/* no projection*/
			hlts->set_column_names(h_table->get_all_column_names());
		}

		/* set condition */
		json j_condition = j["condition"];
		if(!j_condition.is_null()) {
			std::unique_ptr<RexNode> condition = get_rex_node(j_condition);
			// std::unique_ptr<Condition> condition = std::make_unique<Condition>();
			// condition->set_head_operand(std::move(head_operand));
			hlts->set_condition(std::move(condition));
		}

		return hlts;
	} else if(name == "HuskyLogicalCalc") {
		husky::LOG_I << "Set HuskyLogicalCalc";
		auto hlc = std::make_unique<HuskyLogicalCalc>();

		/* set projection*/
		json j_projections = j["projection"];
		for(auto& j_proj : j_projections) {
			std::unique_ptr<RexNode> projection = get_rex_node(j_proj);
			hlc->add_projection(std::move(projection));
		}

		/* set condition */
		json j_cond = j["condition"];
		if(!j_cond.is_null()) {
			std::unique_ptr<RexNode> condition = get_rex_node(j_cond);
			hlc->set_condition(std::move(condition));
		}

		/* set inputs */
		json j_inputs = j["inputs"];
		for(auto& j_input : j_inputs) {
			auto input_rel = get_rel_node(j_input);
			hlc->add_input_rel_node(std::move(input_rel));
		}

		return hlc;
	} else if(name == "HuskyLogicalAggregate") {
		husky::LOG_I << "Set HuskyLogicalAggregate";
		auto hla = std::make_unique<HuskyLogicalAggregate>();

		/* set group */
		json j_groups = j["group"];
		for(auto& j_group : j_groups) {
			int group_index = j_group.get<int>();
			hla->add_group_index(group_index);
		}

		/* set aggregate calls*/
		json j_agg_calls = j["aggregateCalls"];
		for(auto& j_agg_call : j_agg_calls) {
			auto agg_call = std::make_unique<AggregateCall>(); 

			json j_args = j_agg_call["args"];
			if(!j_args.empty()) {
				std::vector<int> args;
				for(auto& j_arg : j_args) {
					args.push_back(j_arg.get<int>());
				}
				agg_call->set_arguments(args);
			}

			std::string function_name = j_agg_call["function"].get<std::string>();
			agg_call->set_function_name(function_name);

			if(!j_agg_call["name"].is_null()) {
				std::string agg_name = j_agg_call["name"].get<std::string>();
				agg_call->set_aggregate_name(agg_name);
			}

			std::string datatype = j_agg_call["datatype"].get<std::string>();
			agg_call->set_datatype(datatype);

			hla->add_aggregate_call(std::move(agg_call));
		}

		/* set inputs */
		json j_inputs = j["inputs"];
		for(auto& j_input : j_inputs) {
			auto input_rel = get_rel_node(j_input);
			hla->add_input_rel_node(std::move(input_rel));
		}

		return hla;
	} else if(name == "HuskyLogicalSort") {
		husky::LOG_I << "Set HuskyLogicalSort";
		auto hls = std::make_unique<HuskyLogicalSort>();

		/* set sort field */
		json j_sorts = j["sort"];
		for(auto& j_sort : j_sorts) {
			std::string datatype = j_sort["datatype"].get<std::string>();
			int index = j_sort["index"].get<int>();
			hls->add_sort(datatype, index);
		}
		
		/* set dir */
		json j_dirs = j["dir"];
		for(auto& j_dir : j_dirs) {
			std::string dir = j_dir.get<std::string>();
			hls->add_dir(dir);
		}
		
		/* set inputs */
		json j_inputs = j["inputs"];
		for(auto& j_input : j_inputs) {
			auto input_rel = get_rel_node(j_input);
			hls->add_input_rel_node(std::move(input_rel));
		}
		
		return hls;
	} else if(name == "HuskyLogicalJoin") {
		husky::LOG_I << "Set HuskyLogicalJoin";
		auto hlj = std::make_unique<HuskyLogicalJoin>();

		/* set condition */
		json j_condition = j["condition"];
		if(!j_condition.is_null()) {
			std::unique_ptr<RexNode> condition = get_rex_node(j_condition);
			hlj->set_condition(std::move(condition));
		}
		
		/* set join type */
		std::string join_type = j["type"].get<std::string>();
		hlj->set_join_type(join_type);
		
		/* set inputs */
		json j_inputs = j["inputs"];
		for(auto& j_input : j_inputs) {
			auto input_rel = get_rel_node(j_input);
			hlj->add_input_rel_node(std::move(input_rel));
		}
		
		return hlj;
	} else {
		return std::unique_ptr<AbstractRelNode>{};
	}
}

json JsonParser::get_json() {
	std::vector<json> ret;
    infmt_.set_input(hdfs_dir_);
    auto parse_json = [&ret](boost::string_ref& chunk) {
        auto start_pos = chunk.find("{");
        auto end_pos = chunk.rfind("}");
        ret.push_back(json::parse(chunk.substr(start_pos, end_pos - start_pos + 1)));
    };
    husky::load(infmt_, parse_json);
    return ret.empty() ? json() : ret.front();
}

std::unique_ptr<RexNode> JsonParser::get_rex_node(json j) {
	std::string type = j["type"].get<std::string>();
	std::unique_ptr<RexNode> rex_node = std::make_unique<RexNode>(type);
	
	if(type == "function") {
		std::string name = j["name"].get<std::string>();
		rex_node->set_name(name);
		json j_operands = j["operands"];
		for(auto& j_op : j_operands) {
			rex_node->add_input_rex_node(std::move(get_rex_node(j_op)));
		}
	} else if(type == "constant") {
		std::string value = j["value"].get<std::string>();
		value.erase(std::remove(value.begin(), value.end(), '\'' ), value.end());
		std::string datatype = j["datatype"].get<std::string>();
		rex_node->set_value(value);
		rex_node->set_datatype(datatype);
	} else if(type == "inputRef") {
		std::string datatype = j["datatype"].get<std::string>();
		int index = j["index"].get<int>();
		rex_node->set_datatype(datatype);
		rex_node->set_index(index);
	}

	return rex_node;
}

} // namespace sql
} // namespace husky