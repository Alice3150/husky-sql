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
set(husky-sql husky) 

file(GLOB husky-sql-src
  excute.cpp
  relnode/husky_logical_table_scan.cpp
  utils/table.cpp
  )

husky_cache_variable(husky-sql-src ${husky-sql-src})
add_library(husky-sql-inner SHARED ${husky-sql-src})
target_link_libraries(husky-sql-inner ${husky-sql})
target_link_libraries(husky-sql-inner ${HUSKY_EXTERNAL_LIB})

### Functional Test

# TestModel
add_executable(TestExecute execute.cpp)
target_link_libraries(TestExecute ${husky-sql})
target_link_libraries(TestExecute ${HUSKY_EXTERNAL_LIB} ${EXTERNAL_LIB})
add_dependencies(TestExecute ${EXTERNAL_DEPENDENCIES})