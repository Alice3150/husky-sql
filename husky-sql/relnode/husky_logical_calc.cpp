#include "husky-sql/relnode/husky_logical_calc.hpp"

#include "core/engine.hpp"
#include "husky-sql/utils/save_to_hdfs.hpp"

#include "glog/logging.h"
#include "glog/stl_logging.h"

namespace husky {
namespace sql {

ObjList<RowKV>& HuskyLogicalCalc::get_output() const {
	auto& projections = this->get_projections();
	auto& condition = this->get_condition();

	auto& input_rel_nodes = this->get_inputs();
	auto& input_rel_node = input_rel_nodes[0];

	auto& input_objlist = input_rel_node->get_output();
	auto& output_objlist = ObjListStore::create_objlist<RowKV>();
	auto& record_ch = ChannelStore::create_push_channel<std::vector<std::string> >(input_objlist, output_objlist);

	/* For each row, check condition and compute projection*/
	list_execute(input_objlist, {}, {&record_ch}, [&projections, &condition, &record_ch, this](RowKV& record) {
		auto& row_data = record.get_data();
		std::vector<std::string> result;
		/* check condition */
		if(!(this->is_filter()) || condition->check_row(row_data)) {
			/* keep this row */
			if(this->is_projection()) {
				for(auto& projection : projections) {
					std::string proj_result = projection->compute_expr(row_data);
					result.push_back(proj_result);
				}
			} else {
				result.insert(result.end(), row_data.begin(), row_data.end());
			}
			record_ch.push(result, record.id());
		}
	});

	list_execute(output_objlist, {&record_ch}, {}, [&record_ch](RowKV& record) {
		auto row_data = record_ch.get(record)[0];
		record.set_data(row_data);
	});

	ObjListStore::drop_objlist(input_objlist.get_id());

    return output_objlist;
}

} // namespace sql
} // namespace husky