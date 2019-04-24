#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base/thread_support.hpp"

#include "husky-sql/rexnode/group_aggregators.hpp"

namespace husky {
namespace sql {

class AggregateCombiner {
    using ValT = std::vector<std::string>;

	public:
	    static void combine(ValT& val, const ValT& inc) {
            if(aggs_ != nullptr) {
                aggs_->aggregate(val, inc);
            }
	    }

    inline static void set_aggregators(const std::shared_ptr<GroupAggregators>& aggs) {
        base::call_once_each_time([aggs = std::move(aggs)]() { aggs_ = aggs; });
    }

   private:
    static std::shared_ptr<GroupAggregators> aggs_;
};

}  // namespace sql
}  // namespace husky