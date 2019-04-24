#include "husky-sql/rexnode/aggregate_call.hpp"

#include "husky-sql/rexnode/aggregators/sum_aggregators.hpp"
#include "husky-sql/rexnode/aggregators/count_aggregators.hpp"
#include "husky-sql/rexnode/aggregators/avg_aggregators.hpp"

namespace husky {
namespace sql {
	std::unique_ptr<AbstractAggregator> AggregateCall::get_aggregator() {
		if(this->get_function_name() == "SUM") {
			if(this->get_datatype() == "DECIMAL") {
				return std::make_unique<DecimalSumAggregator>();
			} else if(this->get_datatype() == "FLOAT") {
				return std::make_unique<FloatSumAggregator>();
			} else if(this->get_datatype() == "INTEGER" || this->get_datatype() == "BIGINT") {
				return std::make_unique<LongSumAggregator>();
			}
		} else if(this->get_function_name() == "AVG") {
			if(this->get_datatype() == "DECIMAL") {
				return std::make_unique<DecimalAvgAggregator>();
			} else if(this->get_datatype() == "FLOAT") {
				return std::make_unique<FloatAvgAggregator>();
			} else if(this->get_datatype() == "INTEGER" || this->get_datatype() == "BIGINT") {
				return std::make_unique<LongAvgAggregator>();
			}
		} else if(this->get_function_name() == "COUNT") {
			return std::make_unique<CountAggregator>();
		} else {
			/* TODO - must return something */
			return std::make_unique<LongSumAggregator>();
		}
	}

} // namespace sql
} // namespace husky