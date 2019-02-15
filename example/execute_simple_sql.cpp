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

#include "glog/logging.h"

#include "core/engine.hpp"

#include "husky-sql/relnode/abstract_rel_node.hpp"
#include "husky-sql/relnode/husky_logical_table_scan.hpp"
#include "husky-sql/rexnode/condition.hpp"
#include "husky-sql/table/table.hpp"
#include "husky-sql/utils/json_parser.hpp"

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
 * json_path=hdfs://localhost:9000/husky_sql/physical_plan/plan.json
 * table_path=hdfs://hdfs://localhost:9000/husky_sql/table/table
 * output_path=hdfs://proj99:9000/kylin/kylin_metadata/kylin-86dffb72-3bf9-4150-b9bd-52332d9a7af5/simple_sales_model/cuboid/
 *
 * [worker]
 * info=w1:4
 */

using husky::Context;
using husky::sql::AbstractRelNode;
using husky::sql::HuskyLogicalTableScan;
using husky::sql::Condition;
using husky::sql::Table;
using husky::sql::JsonParser;

void execute() {
    // load plan.json
    std::string json_url = Context::get_param("json_url");
    JsonParser json_parser(json_url);
    AbstractRelNode * first_rel_node = json_parser.get_first_rel_node();
    // execute physical plan
    std::vector<std::vector<std::string> > output;
    first_rel_node->get_output(output);
    LOG(INFO) << "Output result: ";
    // for(int i = 0; i < output.size(); i++) {
    //     LOG(INFO) << output[i];
    // }
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