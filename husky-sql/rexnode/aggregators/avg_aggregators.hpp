#pragma once

#include <string>

#include "glog/logging.h"

#include "husky-sql/rexnode/aggregators/abstract_aggregator.hpp"

namespace husky {
namespace sql {
class DecimalAvgAggregator : public AbstractAggregator {
    public:
        DecimalAvgAggregator() : count_(0) {}
        ~DecimalAvgAggregator() {}

        void aggregate(std::string& val_str, const std::string& inc_str) override {
            val_str = std::to_string((std::stod(val_str) * count_ + std::stod(inc_str)) / (count_ + 1) );
            count_++;
        }

    private:
        unsigned long long count_;
};

class LongAvgAggregator : public AbstractAggregator {
   public:
    LongAvgAggregator() : count_(0) {}
    ~LongAvgAggregator() {}

    void aggregate(std::string& val_str, const std::string& inc_str) override {
        val_str = std::to_string(std::stoll(val_str) + std::stoll(inc_str));
        count_++;
    }

    private:
        unsigned long long count_;
};

class FloatAvgAggregator : public AbstractAggregator {
   public:
    FloatAvgAggregator() : count_(0) {}
    ~FloatAvgAggregator() {}

    void aggregate(std::string& val_str, const std::string& inc_str) override {
        val_str = std::to_string(std::stof(val_str) + std::stof(inc_str));
        count_++;
    }

    private:
        unsigned long long count_;
};

}  // namespace sql
}  // namespace husky