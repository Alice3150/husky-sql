#pragma once

#include "husky-sql/relnode/abstract_rel_node.hpp"
#include "husky-sql/table/table.hpp"
#include "husky-sql/rexnode/condition.hpp"

#include <vector>
#include <string>
#include <memory>

namespace husky {
namespace sql {

class HuskyLogicalTableScan: public AbstractRelNode {
	public:
		HuskyLogicalTableScan() {
			std::string name = "HuskyLogicalTableScan";
			name_ = name; 
		}
		~HuskyLogicalTableScan() {}

		/* setter and getter */
		inline void set_table(std::unique_ptr<Table> table) { table_ = std::move(table); }
		inline void set_projection_index(const std::vector<int> & projection_index) { projection_index_ = projection_index; }
		inline void set_condition(std::unique_ptr<Condition> condition) { condition_ = std::move(condition); }

		inline Table * get_table() { return table_.get(); }
		inline Condition * get_condition() { return condition_.get(); }
		inline const std::vector<int> & get_projection_index() const { return projection_index_; }

		void get_output(std::vector<std::vector<std::string> > & output) const override;

	private:
		std::unique_ptr<Table> table_;
		std::unique_ptr<Condition> condition_;
		std::vector<int> projection_index_;
};

} // namespace sql
} // namespace husky