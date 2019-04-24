#pragma once

#include <string>
#include <set>

#include "glog/logging.h"

#include "husky-sql/rexnode/aggregators/abstract_aggregator.hpp"

namespace husky {
namespace sql {
class CountAggregator : public AbstractAggregator {
    public:
        void aggregate(std::string& val_str, const std::string& inc_str) override {
            /* TODO: only support COUNT(*), how about specified field? */
            val_str = std::to_string(std::stoll(val_str) + 1);
        }
};

}  // namespace sql
}  // namespace husky