#pragma once

#include "husky-sql/relnode/abstract_rel_node.hpp"
#include "husky-sql/rexnode/aggregate_call.hpp"
#include "husky-sql/utils/row_kv.hpp"

#include <vector>
#include <string>
#include <memory>

namespace husky {
namespace sql {

class HuskyLogicalAggregate: public AbstractRelNode {
	public:
		explicit HuskyLogicalAggregate() {
			std::string name = "husky_logical_aggregate";
			name_ = name; 
		}
		~HuskyLogicalAggregate() {}

		/* setter and getter */
		inline void add_aggregate_call(std::unique_ptr<AggregateCall> agg_call) { 
			if(agg_call->get_arguments_size() != 0) {
				/* TODO - assume only one argument */
				aggregate_input_index_.push_back(agg_call->get_arguments()[0]);
			} else {
				aggregate_input_index_.push_back(-1); // -1 indicates no args.
			}
			aggregate_calls_.push_back(std::move(agg_call)); 
		}
		inline void add_group_index(int group_index) { group_index_.push_back(group_index); }

		inline std::vector<std::unique_ptr<AggregateCall> > const & get_aggregate_calls() const { return aggregate_calls_; }
		inline const std::vector<int> & get_group_index() const { return group_index_; }
		inline const std::vector<int> & get_aggregate_input_index() const { return aggregate_input_index_; }

		ObjList<RowKV>& get_output() const override;

	private:
		std::vector<std::unique_ptr<AggregateCall> > aggregate_calls_;
		std::vector<int> aggregate_input_index_;
		std::vector<int> group_index_;
};

} // namespace sql
} // namespace husky