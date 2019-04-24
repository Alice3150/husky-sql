#include "husky-sql/utils/save_to_hdfs.hpp"

#include "io/hdfs_manager.hpp"

#include <vector>
#include <memory>
#include <sstream>

namespace husky {
namespace sql {

void save_to_hdfs(ObjList<RowKV>& records, const std::string& output_path) {
    // get path and namenode info
    std::string protocol = "hdfs://";
    // CHECK_EQ(output_path.substr(0, protocol.size()), protocol) << "output path must start with hdfs://";
    auto colon = output_path.find(":", protocol.size());
    // CHECK_EQ(true, colon != output_path.npos) << "namenode host and port should be specified as hostname:port";
    auto host = output_path.substr(protocol.size(), colon - protocol.size());
    auto path_start = output_path.find("/", colon);
    auto port = output_path.substr(colon + 1, path_start - colon - 1);

    std::string hdfs_output_path = output_path.substr(path_start);
    // husky::LOG_I << "save rel node into: host-" + host + " port-" + port + " path-" + hdfs_output_path;

    list_execute(records, {}, {}, [&host, &port, &hdfs_output_path](RowKV& record) {
        // husky::io::HDFS::Write(host, port, (char*) record.id().data(), record.id().size(), hdfs_output_path,
                               // Context::get_global_tid());
        std::stringstream ss;
        for (auto& row_data : record.get_data()) {
            ss << row_data << "\t";
        }
        ss << "\n";
        husky::io::HDFS::Write(host, port, ss.str(), hdfs_output_path, Context::get_global_tid());
    });
    husky::io::HDFS::CloseFile(host, port);
}

}  // namespace sql
}  // namespace husky