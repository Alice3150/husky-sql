#include "husky-sql/relnode/husky_logical_aggregate.hpp"

#include "core/engine.hpp"

#include "glog/logging.h"
#include "glog/stl_logging.h"

#include "husky-sql/rexnode/aggregate_combiner.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <map>

namespace husky {
namespace sql {

ObjList<RowKV>& HuskyLogicalAggregate::get_output() const {
	auto& input_rel_nodes = this->get_inputs();
	auto& input_rel_node = input_rel_nodes[0];

	auto& input_objlist = input_rel_node->get_output();
	auto& output_objlist = ObjListStore::create_objlist<RowKV>();

	auto& group_ch = ChannelStore::create_push_combined_channel<std::vector<std::string>, AggregateCombiner>(input_objlist, output_objlist);

	/* set aggregators*/
	auto& aggregate_calls = this->get_aggregate_calls();
	AggregateCombiner::set_aggregators(std::make_shared<GroupAggregators>(aggregate_calls));

	/* aggregate */
	auto& group_index = this->get_group_index();
	auto& aggregate_input_index = this->get_aggregate_input_index();
	list_execute(input_objlist, {}, {&group_ch}, [&group_index, &aggregate_input_index, &group_ch](RowKV& record) {
		auto& row_data = record.get_data();

		/* make group key string*/
		std::string group_key_str  = row_data[group_index[0]];
		for(int index = 1; index < group_index.size(); index++) {
			group_key_str = group_key_str + "_" + row_data[group_index[index]];
		}
		
		/* set inc */
		std::vector<std::string> inc;
		for(int index : aggregate_input_index) {
			if(index != -1) {
				inc.push_back(row_data[index]);
			} else {
				inc.push_back("0");
			}
		}

		group_ch.push(inc, group_key_str);
		
	});

	long long count = 0;
	list_execute(output_objlist, {&group_ch}, {}, [&group_ch, &count](RowKV& record) {
		std::string group_key = record.id();
		std::vector<std::string> groups;
		boost::split(groups, group_key, boost::is_any_of("_"), boost::token_compress_on);

		std::vector<std::string> aggregation = group_ch.get(record);
		std::vector<std::string> result;
		result.insert( result.end(), groups.begin(), groups.end() );
		result.insert( result.end(), aggregation.begin(), aggregation.end() );

		husky::LOG_I << "Aggregate Row: " << result;

		record.set_data(result);
		count++;
	});

	ObjListStore::drop_objlist(input_objlist.get_id());
    return output_objlist;
}

} // namespace sql
} // namespace husky