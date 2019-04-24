#pragma once

#include <string>

#include "glog/logging.h"

#include "husky-sql/rexnode/aggregators/abstract_aggregator.hpp"

namespace husky {
namespace sql {
class DecimalSumAggregator : public AbstractAggregator {
   public:
    void aggregate(std::string& val_str, const std::string& inc_str) override {
        val_str = std::to_string(std::stod(val_str) + std::stod(inc_str));
    }
};

class LongSumAggregator : public AbstractAggregator {
    void aggregate(std::string& val_str, const std::string& inc_str) override {
        val_str = std::to_string(std::stoll(val_str) + std::stoll(inc_str));
    }
};

class FloatSumAggregator : public AbstractAggregator {
   public:
    void aggregate(std::string& val_str, const std::string& inc_str) override {
        val_str = std::to_string(std::stof(val_str) + std::stof(inc_str));
    }
};

}  // namespace sql
}  // namespace husky