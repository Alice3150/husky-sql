#pragma once

#include <string>

#include "glog/logging.h"

#include "husky-sql/rexnode/aggregators/abstract_aggregator.hpp"

namespace husky {
namespace sql {
class BoolMinAggregator : public AbstractAggregator {
    public:
        void aggregate(std::string& val_str, const std::string& inc_str) override {
        	if(val_str == "true" || inc_str == "true") {
        		val_str = "true";
        	}
        }
};

}  // namespace sql
}  // namespace husky