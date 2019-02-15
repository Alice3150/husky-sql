#pragma once

#include <string>
#include <vector>
#include <memory>

namespace husky {
namespace sql {

class AbstractRelNode {
	public:
		AbstractRelNode() { }
    	~AbstractRelNode() { }
		inline void add_input_rel_node(std::unique_ptr<AbstractRelNode> input) { inputs_.push_back(std::move(input)); }
		inline std::vector<std::unique_ptr<AbstractRelNode> > const & get_inputs() const { return inputs_; }
		virtual void get_output(std::vector<std::vector<std::string> > & output) const = 0;

	protected:
		std::string name_;
		std::vector<std::unique_ptr<AbstractRelNode> > inputs_;
};

} // namespace sql
} // namespace husky