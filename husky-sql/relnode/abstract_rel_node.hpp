#pragma once

#include <string>
#include <vector>
#include <memory>

#include "husky-sql/utils/row_kv.hpp"

#include "core/engine.hpp"

namespace husky {
namespace sql {

class AbstractRelNode {
	public:
		AbstractRelNode() { }
    	~AbstractRelNode() { }

    	inline void set_column_names(const std::vector<std::string> & names) { column_names_ = names; }

		inline void add_input_rel_node(std::unique_ptr<AbstractRelNode> input) { inputs_.push_back(std::move(input)); }
		inline std::vector<std::unique_ptr<AbstractRelNode> > const & get_inputs() const { return inputs_; }
		virtual ObjList<RowKV>& get_output() const = 0;
		inline const std::vector<std::string> & get_column_names() const { return column_names_; }

	protected:
		std::string name_;
		std::vector<std::unique_ptr<AbstractRelNode> > inputs_;
		std::vector<std::string> column_names_;
};

} // namespace sql
} // namespace husky