#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/engine.hpp"

#include "husky-sql/rexnode/aggregators/abstract_aggregator.hpp"
#include "husky-sql/rexnode/aggregate_call.hpp"

namespace husky {
namespace sql {

class GroupAggregators {
   public:
    using SValT = std::vector<std::string>;
    explicit GroupAggregators(const std::vector<std::unique_ptr<AggregateCall>>& aggregate_calls) {
        for (auto& aggregate_call : aggregate_calls) {
            aggregators_.push_back(std::move(aggregate_call->get_aggregator()));
        }
    }

    inline size_t size() const { return aggregators_.size(); }

    void aggregate(SValT& val, const SValT& inc) const {
        for (int i = 0; i < inc.size(); ++i) {
            aggregators_[i]->aggregate(val[i], inc[i]);
        }
    }

   private:
    std::vector<std::unique_ptr<AbstractAggregator>> aggregators_;
};

}  // namespace sql
}  // namespace husky