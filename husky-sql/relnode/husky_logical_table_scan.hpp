#pragma once

#include "husky-sql/relnode/abstract_rel_node.hpp"
#include "husky-sql/table/table.hpp"
#include "husky-sql/rexnode/rex_node.hpp"
#include "husky-sql/utils/row_kv.hpp"

#include <vector>
#include <string>
#include <memory>

namespace husky {
namespace sql {

class HuskyLogicalTableScan: public AbstractRelNode {
	public:
		explicit HuskyLogicalTableScan() {
			std::string name = "husky_logical_table_scan";
			name_ = name; 
			is_projection_ = false;
			is_filter_ = false; 
		}
		~HuskyLogicalTableScan() {}

		/* setter and getter */
		inline void set_table(std::unique_ptr<Table> table) { table_ = std::move(table); }
		inline void set_projection_index(const std::vector<int> & projection_index) { 
			projection_index_ = projection_index; 
			is_projection_ = true;
		}
		inline void set_condition(std::unique_ptr<RexNode> condition) { 
			condition_ = std::move(condition);  
			is_filter_ = true;
		}

		inline Table * get_table() { return table_.get(); }
		inline RexNode * get_condition() { return condition_.get(); }
		inline const std::vector<int> & get_projection_index() const { return projection_index_; }
		inline const bool is_projection() const { return is_projection_; }
		inline const bool is_filter() const { return is_filter_; }


		ObjList<RowKV>& get_output() const override;

	private:
		std::unique_ptr<Table> table_;
		std::unique_ptr<RexNode> condition_;
		std::vector<int> projection_index_;
		bool is_projection_;
		bool is_filter_;
};

} // namespace sql
} // namespace husky