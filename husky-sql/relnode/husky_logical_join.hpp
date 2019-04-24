#pragma once

#include "husky-sql/relnode/abstract_rel_node.hpp"
#include "husky-sql/rexnode/rex_node.hpp"
#include "husky-sql/utils/row_kv.hpp"

#include <vector>
#include <string>
#include <memory>

namespace husky {
namespace sql {

class HuskyLogicalJoin: public AbstractRelNode {
	public:
		explicit HuskyLogicalJoin() {
			std::string name = "husky_logical_join";
			name_ = name; 
		}
		~HuskyLogicalJoin() {}

		/* setter and getter */
		inline void set_condition(std::unique_ptr<RexNode> condition) { 
			condition_ = std::move(condition);
		}
		inline void set_join_type(const std::string & join_type) { join_type_ = join_type; }

		inline std::unique_ptr<RexNode> const & get_condition() const { return condition_; }
		inline std::string  get_join_type() const { return join_type_; }

		ObjList<RowKV>& get_output() const override;

	private:
		std::unique_ptr<RexNode> condition_;
		std::string join_type_; // INNER, FULL, LEFT, RIGHT
};

} // namespace sql
} // namespace husky