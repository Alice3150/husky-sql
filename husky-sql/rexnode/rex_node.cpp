#include "husky-sql/rexnode/rex_node.hpp"

#include <sstream>
#include <ctime>
#include <utility>
#include <iomanip>
#include <algorithm>

#include "core/engine.hpp"

namespace husky {
namespace sql {

bool RexNode::check_row(const std::vector<std::string> & row) {
	return check_operand(row, this);
}

std::string RexNode::compute_expr(const std::vector<std::string> & row) {
	return compute_operand(row, this).first;
}

bool RexNode::check_operand(const std::vector<std::string> & row, RexNode * op) {
	if(op->get_type() == "function" && op->get_name() == "AND") {
		return check_operand(row, op->get_input_rex_node(0)) && check_operand(row, op->get_input_rex_node(1));
	} else if(op->get_type() == "function" && op->get_name() == "OR") {
		return check_operand(row, op->get_input_rex_node(0)) || check_operand(row, op->get_input_rex_node(1));
	} else if(op->get_type() == "function" && (op->get_name() == "=" || op->get_name() == "<" || op->get_name() == ">" || op->get_name() == "<=" || op->get_name() == ">=")) {
		RexNode * left_op = op->get_input_rex_node(0);
		auto left_pair = compute_operand(row, left_op);
		std::string left_value = left_pair.first;
		std::string left_datatype = left_pair.second;

		RexNode * right_op = op->get_input_rex_node(1);
		auto right_pair = compute_operand(row, right_op);
		std::string right_value = right_pair.first;
		std::string right_datatype = right_pair.second;

	 	if(left_datatype == "INTEGER" && right_datatype == "INTEGER") {
	 		husky::LOG_I << "check_operand: left_value: " << left_value << " right_value: " << right_value;
	 		if(op->get_name() == "=") {
	 			return std::stoi(left_value) == std::stoi(right_value);
	 		} else if(op->get_name() == "<") {
	 			return std::stoi(left_value) < std::stoi(right_value);
	 		} else if(op->get_name() == ">") {
	 			return std::stoi(left_value) > std::stoi(right_value);
	 		} else if(op->get_name() == "<=") {
	 			return std::stoi(left_value) <= std::stoi(right_value);
	 		} else {
	 			/* op->get_name() == ">=" */
	 			return std::stoi(left_value) >= std::stoi(right_value);
	 		} 
	 	} else if(left_datatype == "FLOAT" && right_datatype == "FLOAT"){
	 		if(op->get_name() == "=") {
	 			return std::stof(left_value) == std::stof(right_value);
	 		} else if(op->get_name() == "<") {
	 			return std::stof(left_value) < std::stof(right_value);
	 		} else if(op->get_name() == ">") {
	 			return std::stof(left_value) > std::stof(right_value);
	 		} else if(op->get_name() == "<=") {
	 			return std::stof(left_value) <= std::stof(right_value);
	 		} else {
	 			/* op->get_name() == ">=" */
	 			return std::stof(left_value) >= std::stof(right_value);
	 		}
	 	} else if(left_datatype == "DECIMAL" && right_datatype == "DECIMAL") {
	 		if(op->get_name() == "=") {
	 			return std::stod(left_value) == std::stod(right_value);
	 		} else if(op->get_name() == "<") {
	 			return std::stod(left_value) < std::stod(right_value);
	 		} else if(op->get_name() == ">") {
	 			return std::stod(left_value) > std::stod(right_value);
	 		} else if(op->get_name() == "<=") {
	 			return std::stod(left_value) <= std::stod(right_value);
	 		} else {
	 			/* op->get_name() == ">=" */
	 			return std::stod(left_value) >= std::stod(right_value);
	 		}
	 	} else if((left_datatype == "CHAR" && right_datatype == "CHAR") || (left_datatype == "VARCHAR" && right_datatype == "VARCHAR")) {
	 		return left_value == right_value; /* TODO - NOT equal case */
	 	} else if(left_datatype == "DATE" && right_datatype == "DATE") {
	 		// husky::LOG_I << "Compare DATE: " << right_value << " with " << left_value;
			
			long long left_days = date_to_days(left_value);
			long long right_days = date_to_days(right_value);

			if(op->get_name() == "=") {
	 			return left_days == right_days;
	 		} else if(op->get_name() == "<") {
	 			return left_days < right_days;
	 		} else if(op->get_name() == ">") {
	 			return left_days > right_days;
	 		} else if(op->get_name() == "<=") {
	 			return left_days <= right_days;
	 		} else{
	 			/* op->get_name() == ">=" */
	 			return left_days >= right_days;
	 		}
	 	} else {
	 		husky::LOG_E << "Can not find matching datatype";
	 		return false; /* TODO */
	 	}
	} else {
		husky::LOG_E << "Can not find match operand type";
		return false;
	} 
}

/* return pair<value_str, datatype_str> */
std::pair<std::string, std::string> RexNode::compute_operand(const std::vector<std::string> & row, RexNode * op) {
	if(op->get_type() == "inputRef") {
		int index = op->get_index();
		return std::make_pair(row[index], op->get_dataype());
	} else if(op->get_type() == "constant") {
		std::string datatype = op->get_dataype();
		return std::make_pair(op->get_value(), op->get_dataype());
	} else if(op->get_type() == "function" && (op->get_name() == "+" || op->get_name() == "-" || op->get_name() == "*" || op->get_name() == "/")) {
		RexNode * left_op = op->get_input_rex_node(0);
		auto left_pair = compute_operand(row, left_op);
		std::string left_value = left_pair.first;
		std::string left_datatype = left_pair.second;

		RexNode * right_op = op->get_input_rex_node(1);
		auto right_pair = compute_operand(row, right_op);
		std::string right_value = right_pair.first;
		std::string right_datatype = right_pair.second;
 		
 		if(left_datatype == "INTEGER" && right_datatype == "INTEGER") {
 			if(op->get_name() == "+") {
 				return std::make_pair(std::to_string(std::stoi(left_value) + std::stoi(right_value)), "INTEGER");
 			} else if(op->get_name() == "-") {
 				return std::make_pair(std::to_string(std::stoi(left_value) - std::stoi(right_value)), "INTEGER");
 			} else if(op->get_name() == "*") {
 				return std::make_pair(std::to_string(std::stoi(left_value) * std::stoi(right_value)), "INTEGER");
 			} else {
 				/* op->get_name() == "/" */
 				return std::make_pair(std::to_string(std::stoi(left_value) / std::stoi(right_value)), "INTEGER");
 			}
 		} else if(left_datatype == "FLOAT" && right_datatype == "FLOAT") {
 			if(op->get_name() == "+") {
 				return std::make_pair(std::to_string(std::stof(left_value) + std::stof(right_value)), "FLOAT");
 			} else if(op->get_name() == "-") {
 				return std::make_pair(std::to_string(std::stof(left_value) - std::stof(right_value)), "FLOAT");
 			} else if(op->get_name() == "*") {
 				return std::make_pair(std::to_string(std::stof(left_value) * std::stof(right_value)), "FLOAT");
 			} else {
 				/* op->get_name() == "/" */
 				return std::make_pair(std::to_string(std::stof(left_value) / std::stof(right_value)), "FLOAT");
 			}
 		} else if(left_datatype == "DECIMAL" && right_datatype == "DECIMAL") {
 			if(op->get_name() == "+") {
 				return std::make_pair(std::to_string(std::stod(left_value) + std::stod(right_value)), "DECIMAL");
 			} else if(op->get_name() == "-") {
 				return std::make_pair(std::to_string(std::stod(left_value) - std::stod(right_value)), "DECIMAL");
 			} else if(op->get_name() == "*") {
 				return std::make_pair(std::to_string(std::stod(left_value) * std::stod(right_value)), "DECIMAL");
 			} else{
 				/* op->get_name() == "/" */
 				return std::make_pair(std::to_string(std::stod(left_value) / std::stod(right_value)), "DECIMAL");
 			}
 		} else if(left_datatype == "DATE" && right_datatype == "DATE") {

			long long left_days = date_to_days(left_value);
			long long right_days = date_to_days(right_value);

			long long result_days;
			if(op->get_name() == "+") {
				result_days = left_days + right_days;
			} else {
				/* op->get_name() == "-" */
				result_days = left_days - right_days;
			}

			std::string result_str = days_to_date(result_days);
			// husky::LOG_I << right_value << " +/- " << left_value << " = " << result_str;
			return std::make_pair(result_str, "DATE");	
 		} else if(left_datatype == "DATE" && right_datatype == "INTERVAL_DAY") {
			
 			long long left_days = date_to_days(left_value);
			long long right_days = std::stoll(right_value) / 86400000;

			long long result_days;
			if(op->get_name() == "+") {
				result_days = left_days + right_days;
			} else {
				/* op->get_name() == "-" */
				result_days = left_days - right_days;
			}

			std::string result_str = days_to_date(result_days);

			// husky::LOG_I << right_value << " +/- " << left_days << " days = " << result_str;
			return std::make_pair(result_str, "DATE");	
 		} else {
 			husky::LOG_E << "Can not find match operand type";
 			std::string msg = "error";
			std::string msg_type =  "VARCHAR";
			return std::make_pair(msg, msg_type);
 		}

	} else if(op->get_type() == "function" && op->get_name() == "CAST") {
		RexNode * operand = op->get_input_rex_node(0);
		return compute_operand(row, operand);
	} else {
		husky::LOG_E << "Can not find match operand type";
		std::string msg = "error";
		std::string msg_type =  "VARCHAR";
		return std::make_pair(msg, msg_type);
	}
}

long long RexNode::date_to_days(const std::string & date) {
	long long y = std::stoll(date.substr(0, 4));
	long long m = std::stoll(date.substr(5, 2));
	long long d = std::stoll(date.substr(8, 2));

	m = (m + 9) % 12;
	y = y - m/10;

	return 365*y + y/4 - y/100 + y/400 + (m*306 + 5)/10 + ( d - 1 );
}

std::string RexNode::days_to_date(long long days) {
	long long  y = (10000*days + 14780)/3652425;
	long long ddd = days - (365*y + y/4 - y/100 + y/400);
	if (ddd < 0) {
		y = y - 1;
	 	ddd = days - (365*y + y/4 - y/100 + y/400);
	}
	long long mi = (100*ddd + 52)/3060;
	long long mm = (mi + 2)%12 + 1;
	y = y + (mi + 2)/12;
	long long dd = ddd - (mi*306 + 5)/10 + 1;

	std::string y_str = std::to_string(y);
	std::string mm_str;
	if(mm < 10) {
		mm_str = "0" + std::to_string(mm);
	} else {
		mm_str = std::to_string(mm);
	}
	std::string dd_str;
	if(dd < 10) {
		dd_str = "0" + std::to_string(dd);
	} else {
		dd_str = std::to_string(dd);
	}

	return y_str + "-" + mm_str + "-" + dd_str;
}

} // namespace sql
} // namespace husky