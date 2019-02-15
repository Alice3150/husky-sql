#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>

namespace husky {
namespace sql {

class Condition {
	public:
		Condition() { }
		~Condition() { }
		class Operand
		{
			public:
				explicit Operand(const std::string & type): type_(type) {}
				~Operand() {}
				inline void set_type(const std::string & type) { type_ = type; }
				inline void set_name(const std::string & name) { name_ = name; }
				inline void set_datatype(const std::string & datatype) { datatype_ = datatype; }
				inline void set_value(const std::string & value) { value_ = value; }
				inline void set_index(int index) { index_ = index; }
				inline void add_operand(std::unique_ptr<Operand> operand) { operands_.push_back(std::move(operand)); }

				inline std::string get_type() { return type_; }
				inline std::string get_name() { return name_; }
				inline std::string get_dataype() { return datatype_; }
				inline std::string get_value() { return value_; }
				inline int get_index() { return index_; }
				inline Operand * get_operand(int index) { return operands_[index].get(); }
				inline int get_operands_count() { return operands_.size(); }

			private:
				int index_; // index as parent's operand
				std::string type_; // function, field, constant
				std::string name_;
				std::string datatype_;
				std::string value_;
				std::vector<std::unique_ptr<Operand>> operands_;
		};
		void set_head_operand(std::unique_ptr<Operand> head) { head_operand_ = std::move(head); }
		/* return ture if the row satisfies the condition; otherwise false */
		bool check_row(const std::vector<std::string> & row);

	private:
		std::unique_ptr<Operand> head_operand_;

		bool check_operand(const std::vector<std::string> & row, Operand * op);
};

} // namespace sql
} // namespace husky