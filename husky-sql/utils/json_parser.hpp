#pragma once

#include "husky-sql/relnode/abstract_rel_node.hpp"
#include "husky-sql/rexnode/rex_node.hpp"

#include "core/engine.hpp"

#include "husky-plugins/io/input/file_broadcast_inputformat.hpp"

#include "nlohmann/json.hpp"

#include <vector>
#include <string>
#include <memory>

namespace husky {
namespace sql {

using json = nlohmann::json;

class JsonParser
{
	public:
		explicit JsonParser(const std::string & json_path) {
			std::string protocol = "hdfs://";
	        auto pos = json_path.find("/", json_path.find(protocol) + protocol.size());
	        hdfs_dir_ = protocol + json_path.substr(pos);
	        husky::LOG_I << hdfs_dir_;
		}
		~JsonParser() { }
		std::unique_ptr<AbstractRelNode> get_first_rel_node();
		json get_json();

	private:
	    std::string hdfs_dir_;
	    io::FileBroadcastInputFormat infmt_;

	    std::unique_ptr<AbstractRelNode> get_rel_node(json j);
	    std::unique_ptr<RexNode> get_rex_node(json j);
};

} // namespace sql
} // namespace husky