#include "husky-sql/rexnode/condition.hpp"

namespace husky {
namespace sql {

bool Condition::check_row(const std::vector<std::string> & row) {
	return check_operand(row, head_operand_.get());
}

bool Condition::check_operand(const std::vector<std::string> & row, Operand * op) {
	if(op->get_type() == "function" && op->get_name() == "AND") {
		return check_operand(row, op->get_operand(0)) && check_operand(row, op->get_operand(0));
	} else if(op->get_type() == "function" && op->get_name() == "OR") {
		return check_operand(row, op->get_operand(0)) || check_operand(row, op->get_operand(0));
	} else if(op->get_type() == "function" && (op->get_name() == "=" || op->get_name() == "<" || op->get_name() == ">")) {
		 Operand * field_op = op->get_operand(0);
		 int index = field_op->get_index();
		 std::string datatype = field_op->get_dataype();
	 	if(datatype == "INTEGER") {
	 		if(op->get_name() == "=") {
	 			return std::stoi(row[index]) == std::stoi(op->get_operand(1)->get_value());
	 		} else if(op->get_name() == "<") {
	 			return std::stoi(row[index]) < std::stoi(op->get_operand(1)->get_value());
	 		} else {
	 			return std::stoi(row[index]) > std::stoi(op->get_operand(1)->get_value());
	 		}
	 	} else if(datatype == "FLOAT"){
	 		if(op->get_name() == "=") {
	 			return std::stof(row[index]) == std::stof(op->get_operand(1)->get_value());
	 		} else if(op->get_name() == "<") {
	 			return std::stof(row[index]) < std::stof(op->get_operand(1)->get_value());
	 		} else {
	 			return std::stof(row[index]) > std::stof(op->get_operand(1)->get_value());
	 		}
	 	} else {
	 		return false; /* TODO */
	 	}
	} 
}

} // namespace sql
} // namespace husky