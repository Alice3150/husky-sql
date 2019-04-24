#pragma once

#include <string>

#include "glog/logging.h"

namespace husky {
namespace sql {

class AbstractAggregator {
   public:
    virtual void aggregate(std::string& val, const std::string& inc) {
        LOG(ERROR) << "Dummy base class for measure types not implemented";
    }
};

}  // namespace sql
}  // namespace husky