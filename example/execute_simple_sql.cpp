// Copyright 2018 Husky Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include <vector>
#include <iostream>

#include "glog/logging.h"
#include "glog/stl_logging.h"

#include "core/engine.hpp"

#include "husky-sql/relnode/abstract_rel_node.hpp"
#include "husky-sql/relnode/husky_logical_table_scan.hpp"
#include "husky-sql/table/table.hpp"
#include "husky-sql/table/table_reader_factory.hpp"
#include "husky-sql/utils/json_parser.hpp"
#include "husky-sql/utils/row_kv.hpp"
#include "husky-sql/utils/save_to_hdfs.hpp"

/* Example Config
 * master_host=proj99
 * master_port=23215
 * comm_port=12314
 *
 * hdfs_namenode=proj99
 * hdfs_namenode_port=9000
 *
 * serve=0
 *
 * json_path=hdfs://localhost:9000/husky_sql/input/plan.json
 * output_path=hdfs://proj99:9000/husky_sql/output/
 *
 * [worker]
 * info=w1:4
 */

using husky::Context;
using husky::ObjList;
using husky::sql::AbstractRelNode;
using husky::sql::HuskyLogicalTableScan;
using husky::sql::Table;
using husky::sql::TableReaderFactory;
using husky::sql::JsonParser;
using husky::sql::RowKV;

void execute() {
    // load plan.json
    LOG(INFO) << "Loading JSON...";
    std::string json_url = Context::get_param("json_url");
    std::string output_path = Context::get_param("output_path");
    JsonParser json_parser(json_url);
    std::unique_ptr<AbstractRelNode> first_rel_node = json_parser.get_first_rel_node();

    // execute physical plan
    ObjList<RowKV>& output_objlist = first_rel_node->get_output();
    save_to_hdfs(output_objlist, output_path);
}

int main(int argc, char** argv) {
    google::LogToStderr();

    std::vector<std::string> args;
    args.push_back("hdfs_namenode");
    args.push_back("hdfs_namenode_port");
    args.push_back("json_url");
    args.push_back("output_path");

    if (husky::init_with_args(argc, argv, args)) {
        husky::run_job(execute);
        return 0;
    }
}
