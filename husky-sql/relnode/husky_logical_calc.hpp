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

class HuskyLogicalCalc: public AbstractRelNode {
	public:
		explicit HuskyLogicalCalc() {
			std::string name = "husky_logical_calc";
			name_ = name; 
			is_projection_ = false;
			is_filter_ = false; 
		}
		~HuskyLogicalCalc() {}

		/* setter and getter */
		inline void add_projection(std::unique_ptr<RexNode> projection) { 
			projections_.push_back(std::move(projection)); 
			is_projection_ = true;
		}
		inline void set_condition(std::unique_ptr<RexNode> condition) { 
			condition_ = std::move(condition); 
			is_filter_ = true;
		}

		inline std::vector<std::unique_ptr<RexNode> > const & get_projections() const { return projections_; }
		inline std::unique_ptr<RexNode> const & get_condition() const { return condition_; }
		inline const bool is_projection() const { return is_projection_; }
		inline const bool is_filter() const { return is_filter_; }

		ObjList<RowKV>& get_output() const override;

	private:
		std::vector<std::unique_ptr<RexNode> > projections_; // refers to an expression
		std::unique_ptr<RexNode> condition_; // refers to an expression
		bool is_projection_;
		bool is_filter_;
};

} // namespace sql
} // namespace husky