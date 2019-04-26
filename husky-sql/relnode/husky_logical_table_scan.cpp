#include "husky-sql/relnode/husky_logical_table_scan.hpp"

#include "husky-sql/utils/save_to_hdfs.hpp"

#include "core/engine.hpp"

#include "glog/logging.h"
#include "glog/stl_logging.h"

namespace husky {
namespace sql {

ObjList<RowKV>& HuskyLogicalTableScan::get_output() const {
    const std::vector<std::vector<std::string> > table_data = table_->get_data();
    auto& record_objlist = ObjListStore::create_objlist<RowKV>();
    auto& record_ch = ChannelStore::create_push_channel<std::vector<std::string> >(record_objlist, record_objlist);
    /* add rows that satisfy condition and apply projection */
    for(auto& row_data : table_data) {
        std::vector<std::string> row_without_quotes; // For CHAR, VARCHAR, DATE type field, remove double quotes from string.
        for(auto& cell : row_data) {
            std::string value = cell;
            value.erase(std::remove(value.begin(), value.end(), '\"' ), value.end());
            row_without_quotes.push_back(value);
        }
        
        if(!(this->is_filter()) || condition_->check_row(row_without_quotes)) {
            // keep this row
            std::vector<std::string> row_after_projection; 
            if(this->is_projection()) {
                auto& proj_index = get_projection_index();
                for(int i : proj_index) {
                    row_after_projection.push_back(row_without_quotes[i]);
                }
            } else {
                /* no projection */
                row_after_projection.insert(row_after_projection.end(), 
                    row_without_quotes.begin(), 
                    row_without_quotes.end());
            }
            std::string key = row_after_projection[0];
            for(int index = 1; index < row_after_projection.size(); index++) {
                key = key + "_" + row_after_projection[index];
            }
            record_ch.push(row_after_projection, key);
        }
    }
    record_ch.out();  // flush channel

    list_execute(record_objlist, {&record_ch}, {}, [&record_ch](RowKV& record) {
        record.set_data(record_ch.get(record)[0]);
    });

    table_->clear();

    return record_objlist;
}

} // namespace sql
} // namespace husky