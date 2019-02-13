#pragma once

#include "husky-sql/relnode/abstract_rel_node.hpp"

#include <vector>
#include <string>
#include <memory>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace husky {
namespace sql {

class JsonParser
{
	public:
		explicit JsonParser(const std::string & json_path);
		~JsonParser() { }
		std::unique_ptr<AbstractRelNode> get_first_rel_node();
		json get_json();

	private:
	    std::string json_path_;
	    std::string hdfs_dir_; // TODO - support read from hdfs

	    std::unique_ptr<AbstractRelNode> get_rel_node(json j);
};

} // namespace sql
} // namespace husky