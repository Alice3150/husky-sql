#include "husky-sql/relnode/husky_logical_table_scan.hpp"

#include "core/engine.hpp"
#include "husky-sql/table/row.hpp" // row object in husky

namespace husky {
namespace sql {

void HuskyLogicalTableScan::get_output(std::vector<std::vector<std::string> > & output) const {
	// auto& row_objlist = ObjListStore::creat_objlist<Row>();
	// auto& ch = ChannelStore::create_push_channel<std::vector<std::string>>(row_objlist, dst_list);

    auto& table_data = table_->get_data();

    // add rows that satisfy condition and apply projection
    // int index = 0;
    for(auto& row_data : table_data) {
    	if(condition_->check_row(row_data)) {
    		// keep this row
    		// Row row(index);
    		auto& proj_index = get_projection_index();
    		std::vector<std::string> row_after_projection;
    		for(int i : proj_index) {
    			row_after_projection.push_back(row_data[i]);
    		}
    		output.push_back(row_after_projection);
    		// row.set_row_data(row_after_projection);
    		// row_objlist.add_object(row);

    		// index++;
    	}
    }
}

} // namespace sql
} // namespace husky