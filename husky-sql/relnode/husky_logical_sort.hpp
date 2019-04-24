#pragma once

#include "husky-sql/relnode/abstract_rel_node.hpp"
#include "husky-sql/utils/row_kv.hpp"

#include <vector>
#include <string>
#include <memory>
#include <utility>

namespace husky {
namespace sql {

class HuskyLogicalSort: public AbstractRelNode {
	public:
		explicit HuskyLogicalSort() {
			std::string name = "husky_logical_sort";
			name_ = name; 
		}
		~HuskyLogicalSort() {}

		/* setter and getter */
		inline void add_sort(const std::string & field_type, int field_index) { 
			sort_.push_back(make_pair(field_type, field_index)); 
		}
		inline void add_dir(const std::string & dir) { dir_.push_back(dir); }

		inline const std::vector<std::pair<std::string, int> > & get_sort() const { return sort_; }
		inline const std::vector<std::string> & get_dir() const { return dir_; }


		ObjList<RowKV>& get_output() const override;

	private:
		std::vector<std::pair<std::string, int> > sort_; /* pair: <field type, field index>*/
		std::vector<std::string> dir_;
};

} // namespace sql
} // namespace husky