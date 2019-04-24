#include "husky-sql/rexnode/aggregate_combiner.hpp"

namespace husky {
namespace sql {

std::shared_ptr<GroupAggregators> AggregateCombiner::aggs_ = nullptr;

}  // namespace sql
}  // namespace husky