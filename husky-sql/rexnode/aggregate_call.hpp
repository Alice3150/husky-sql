#pragma once

#include <vector>
#include <string>
#include <memory>

#include "husky-sql/rexnode/aggregators/abstract_aggregator.hpp"

namespace husky {
namespace sql {

class AggregateCall {
	public:
		AggregateCall() { }
		~AggregateCall() { }
		
		/* setters and getters*/
		inline void set_arguments(const std::vector<int> & args) { arguments_ = args; }
		inline void set_function_name(const std::string & func_name) { function_name_ = func_name; }
		inline void set_aggregate_name(const std::string & agg_name) { aggregate_name_ = agg_name; }
		inline void set_datatype(const std::string & datatype) { datatype_ = datatype; }

		inline const std::vector<int> & get_arguments() const { return arguments_; }
		inline const std::string get_function_name() const { return function_name_; }
		inline const std::string get_aggregate_name() const { return aggregate_name_; }
		inline const std::string get_datatype() const { return datatype_; }
		inline int get_arguments_size() const { return arguments_.size(); }

		std::unique_ptr<AbstractAggregator> get_aggregator();

	private:
		std::vector<int> arguments_; /* refers to input field index*/
		std::string function_name_;
		std::string aggregate_name_;
		std::string datatype_; /* output datatype */
};

} // namespace sql
} // namespace husky