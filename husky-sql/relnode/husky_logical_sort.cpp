#include "husky-sql/relnode/husky_logical_sort.hpp"

#include "husky-sql/rexnode/dummy_combiner.hpp"

#include "glog/logging.h"
#include "glog/stl_logging.h"

#include "core/engine.hpp"

namespace husky {
namespace sql {

ObjList<RowKV>& HuskyLogicalSort::get_output() const {
	auto& input_rel_nodes = this->get_inputs();
	auto& input_rel_node = input_rel_nodes[0];

	auto& input_objlist = input_rel_node->get_output();
	auto& output_objlist = ObjListStore::create_objlist<RowKV>();
	auto& record_ch = ChannelStore::create_push_combined_channel<std::vector<std::string>, DummyCombiner>(input_objlist, output_objlist);

	auto& sort = this->get_sort();
	auto& dir = this->get_dir();

	list_execute(input_objlist, {}, {&record_ch}, [&sort, &dir, &record_ch](RowKV& record) {
		auto& row_data = record.get_data();
		std::vector<std::string> result;

		std::string sort_key = "";
		for(auto& sort_pair : sort) {
			sort_key = sort_key + "_" + row_data[sort_pair.second];
		}
		record_ch.push(row_data, sort_key);
	});

	list_execute(output_objlist, {&record_ch}, {}, [&record_ch](RowKV& record) {
		auto& row_data = record_ch.get(record);
		// LOG(INFO) << "Sort Row: " << row_data;
		record.set_data(row_data);
	});

	ObjListStore::drop_objlist(input_objlist.get_id());

    return output_objlist;
}

} // namespace sql
} // namespace husky