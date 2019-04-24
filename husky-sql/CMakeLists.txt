# Copyright 2018 Husky Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/husky ${HUSKY_EXTERNAL_INCLUDE} ${EXTERNAL_INCLUDE})
set(husky-sql husky extension-husky-io extension-husky-master) 

file(GLOB husky-sql-src
  relnode/husky_logical_table_scan.cpp
  relnode/husky_logical_calc.cpp
  relnode/husky_logical_aggregate.cpp
  relnode/husky_logical_sort.cpp
  relnode/husky_logical_join.cpp
  rexnode/rex_node.cpp
  rexnode/aggregate_call.cpp
  rexnode/aggregate_combiner.cpp
  utils/json_parser.cpp
  utils/save_to_hdfs.cpp
  )

husky_cache_variable(husky-sql-src ${husky-sql-src})
add_library(husky-sql-inner STATIC ${husky-sql-src})
target_link_libraries(husky-sql-inner ${husky-sql})
target_link_libraries(husky-sql-inner ${HUSKY_EXTERNAL_LIB})
target_link_libraries(husky-sql-inner ${EXTERNAL_LIB})