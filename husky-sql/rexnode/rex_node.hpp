#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>

namespace husky {
namespace sql {

class RexNode
{
	public:
		explicit RexNode(const std::string & type): type_(type) {}
		~RexNode() {}
		inline void set_type(const std::string & type) { type_ = type; }
		inline void set_name(const std::string & name) { name_ = name; }
		inline void set_datatype(const std::string & datatype) { datatype_ = datatype; }
		inline void set_value(const std::string & value) { value_ = value; }
		inline void set_index(int index) { index_ = index; }
		inline void add_input_rex_node(std::unique_ptr<RexNode> rex_node) { input_rex_nodes_.push_back(std::move(rex_node)); }

		inline std::string get_type() const { return type_; }
		inline std::string get_name() const { return name_; }
		inline std::string get_dataype() const { return datatype_; }
		inline std::string get_value() const { return value_; }
		inline int get_index() const { return index_; }
		inline RexNode * get_input_rex_node(int index) { return input_rex_nodes_[index].get(); }
		inline int get_rex_nodes_count() const { return input_rex_nodes_.size(); }

		bool check_row(const std::vector<std::string> & row);
		std::string compute_expr(const std::vector<std::string> & row);

	private:
		bool check_operand(const std::vector<std::string> & row, RexNode * op);
		std::pair<std::string, std::string> compute_operand(const std::vector<std::string> & row, RexNode * op);
		long long date_to_days(const std::string & date);
		std::string days_to_date(long long days);

		int index_;
		std::string type_; // function, inputRef, constant
		std::string name_;
		std::string datatype_; // INT, CHAR, VARCHAR, DECIMAL, FLOAT, DATE, INTERVAL_DAY
		std::string value_;
		std::vector<std::unique_ptr<RexNode>> input_rex_nodes_;
};

} // namespace sql
} // namespace husky