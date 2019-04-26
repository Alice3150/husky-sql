#include "husky-sql/relnode/husky_logical_join.hpp"

#include "core/engine.hpp"

#include "glog/logging.h"
#include "glog/stl_logging.h"

#include <map>

namespace husky {
namespace sql {

ObjList<RowKV>& HuskyLogicalJoin::get_output() const {
	auto& input_rel_nodes = this->get_inputs();
	auto& left_rel_node = input_rel_nodes[0];
	auto& right_rel_node = input_rel_nodes[1];

	auto& left_objlist = left_rel_node->get_output();
	auto& processed_left_objlist = ObjListStore::create_objlist<RowKV>();
	auto& right_objlist = right_rel_node->get_output();
	auto& processed_right_objlist = ObjListStore::create_objlist<RowKV>();
	auto& output_objlist = ObjListStore::create_objlist<RowKV>();

	/* make join field as key*/
	auto& pre_left_ch = ChannelStore::create_push_channel<std::vector<std::string> >(left_objlist, processed_left_objlist);
	auto& pre_right_ch = ChannelStore::create_push_channel<std::vector<std::string> >(right_objlist, processed_right_objlist);
	
	auto& join_temp_ch = ChannelStore::create_push_channel<std::vector<std::string> >(processed_right_objlist, processed_left_objlist) ;
	auto& join_ch = ChannelStore::create_push_channel<std::vector<std::string> >(processed_left_objlist, output_objlist) ;
	/* set left key*/
	auto& condition = this->get_condition();
	int left_inner_index = condition->get_input_rex_node(0)->get_index();
	int right_inner_index = condition->get_input_rex_node(1)->get_index();
	if(left_inner_index > right_inner_index) {
		int temp = left_inner_index;
		left_inner_index = right_inner_index;
		right_inner_index = temp;
	}

	int left_column_count = 0;
	list_execute(left_objlist, {}, {&pre_left_ch}, [&left_inner_index, &left_column_count, &pre_left_ch](RowKV& record) {
		auto row_data = record.get_data();
		left_column_count = row_data.size();

		std::string key_str = row_data[left_inner_index];
		
		pre_left_ch.push(row_data, key_str);
		
	});

	list_execute(processed_left_objlist, {&pre_left_ch}, {}, [&pre_left_ch](RowKV& record) {
		record.set_data(pre_left_ch.get(record)[0]);
	});

	ObjListStore::drop_objlist(left_objlist.get_id());

	/* set right key*/
	right_inner_index = right_inner_index - left_column_count;
	list_execute(right_objlist, {}, {&pre_right_ch}, [&right_inner_index, &pre_right_ch](RowKV& record) {
		auto row_data = record.get_data();

		std::string key_str = row_data[right_inner_index];
		
		pre_right_ch.push(row_data, key_str);
	});

	list_execute(processed_right_objlist, {&pre_right_ch}, {}, [&pre_right_ch](RowKV& record) {
		record.set_data(pre_right_ch.get(record)[0]);
	});

	ObjListStore::drop_objlist(right_objlist.get_id());

	/* join processed_left_objlist and processed_right_objlist */
	list_execute(processed_right_objlist, {}, {&join_temp_ch}, [&condition, &join_temp_ch](RowKV& record) {
		auto& row_data = record.get_data();

		join_temp_ch.push(row_data, record.id()); // push right row data
	});

	list_execute(processed_left_objlist, {&join_temp_ch}, {&join_ch}, [&condition, &join_temp_ch, &join_ch](RowKV& record) {
		auto& get_right_record = join_temp_ch.get(record);
		auto left_row_data = record.get_data();
		if(get_right_record.size() != 0 && left_row_data.size() != 0) {
			auto right_row_data = get_right_record[0];
			std::vector<std::string> result;
			result.insert( result.end(), left_row_data.begin(), left_row_data.end() );
			result.insert( result.end(), right_row_data.begin(), right_row_data.end() );

			join_ch.push(result, record.id());
		}
	});

	ObjListStore::drop_objlist(processed_right_objlist.get_id());

	list_execute(output_objlist, {&join_ch}, {}, [&join_ch](RowKV& record) {
		record.set_data(join_ch.get(record)[0]); // push right row data
	});

	ObjListStore::drop_objlist(processed_left_objlist.get_id());
    return output_objlist;
}

} // namespace sql
} // namespace husky