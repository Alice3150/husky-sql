#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>

namespace husky {
namespace sql {

class Table {
	public:
		Table(const std::string & name, const std::string & url): name_(name), url_(url) {}
		~Table() { }
		
		/* setters and getters*/
		inline int get_row_count() const { return data_.size(); }
		inline int get_column_count() const { return data_[0].size(); }
		inline const std::vector<std::vector<std::string> > & get_data() const { return data_; }
		inline void add_column(int index, const std::string& name, const std::string& datatype) {
			columns_.push_back(std::make_unique<Column>(index, name, datatype));
		}
		inline void add_row_data(const std::vector<std::string> & row) {
			data_.push_back(row);
		}

		class Column {
			public:
				Column(int index, std::string name, std::string datatype): index_(index), name_(name), datatype_(datatype) { }

				/* Setters and Getters*/
				inline void set_index(int index) { index_ = index; }
				inline void set_name(const std::string & name) { name_ = name; }
				inline void set_datatype(const std::string & datatype) { datatype_ = datatype; }

				inline int get_index() const { return index_; }
				inline std::string get_name() const { return name_; }
 				inline std::string get_dataype() const { return datatype_; }

			private:
				int index_;
				std::string name_;
				std::string datatype_;
		};

	private:
		std::string name_;
		std::string url_;
		std::vector<std::vector<std::string> > data_;
		std::vector<std::unique_ptr<Column> > columns_;

		void read_table() { /* TODO */ }
};

} // namespace sql
} // namespace husky