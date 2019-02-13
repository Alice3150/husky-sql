#include "husky-sql/rexnode/condition.hpp"

namespace husky {
namespace sql {

Condition::Operand::Operand(const std::string & type): type_(type) {
	switch (type) {
		case "function": {
			
		}
			break;
		case "field": {
			
		}
			break;
		case "constant": {
			
		}
			break;

		default:
			break;
	}
}

bool Condition::filter_row(std::vector<string> & row) {
	return true;
}

} // namespace sql
} // namespace husky