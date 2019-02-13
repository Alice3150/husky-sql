#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>

namespace husky {
namespace sql {

class Condition {
	public:
		Condition(const std::string & first_op_type) { }
		~Condition() { }
		/* return ture if the row is not empty after filterd; otherwise false */
		bool row_filter(std::vector<std::string> & row);

	private:
		class Operand
		{
			public:
				explicit Operand(const std::string & type);
				~Operand() {}
				inline void set_type(const std::string & type) { type_ = type; }
				inline void set_name(const std::string & name) { name_ = name; }
				inline void set_datatype(const std::string & datatype) { datatype_ = datatype; }
				inline void set_value(const std::string & value) { value_ = value; }
				inline void add_operand(std::unique_ptr<Operand> operand) { operands_.push_back(std::move(operand)); }

				inline std::string get_type() { return type_; }
				inline std::string get_name() { return name_; }
				inline std::string get_dataype() { return datatype_; }
				inline std::string get_value() { return value_; }
				inline Operand * get_operand(int index) { return operands_[index].get(); }
				inline int get_operand_count() { return operands_.size(); }

			private:
				int index; // index as parent's operand
				std::string type_; // function, field, constant
				std::string name_;
				std::string datatype_;
				std::string value_;
				std::vector<std::unique_ptr<Operand>> operands_;
		};

		std::unique_ptr<Operand> head_operand;
};

} // namespace sql
} // namespace husky