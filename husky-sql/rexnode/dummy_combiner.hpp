#pragma once

#include <string>
#include <vector>

namespace husky {
namespace sql {

class DummyCombiner {
    using ValT = std::vector<std::string>;

	public:
	    static void combine(ValT& val, const ValT& inc) {
            /* do nothing */
	    }
};

}  // namespace sql
}  // namespace husky