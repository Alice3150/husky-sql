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

execute_process(COMMAND mvn versions:set -DnewVersion=${HUSKYSQL_VERSION} -DgenerateBackupPoms=false WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(HuskySQL_JARS husky-calcite-${HUSKYSQL_VERSION}.jar)

add_custom_command( OUTPUT ${HuskySQL_JARS} COMMAND mvn -Dbuild.dir=${PROJECT_BINARY_DIR} -DskipTests package WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMENT "Build the java directory" VERBATIM)

add_custom_target(java_build ALL DEPENDS ${HuskySQL_JARS})

add_test( NAME java_test COMMAND mvn -Pcmake -Dbuild.dir=${CMAKE_CURRENT_BINARY_DIR} test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

install( FILES ${HuskySQL_JARS} DESTINATION share)

# Executables
add_custom_target(QueryPlan ALL mvn exec:java -Dexec.mainClass="husky.sql.calcite.planner.SimpleQueryPlanner" -Dexec.args=\"\'select TRANS_ID,ITEM_ID,PRICE,SELLER_ID,SELLER_NAME from Parts where PRICE > 2.0 and ITEM_ID = 2 or SELLER_ID = 4\'\" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${HuskySQL_JARS})

add_custom_target(ExecuteQuery ALL mvn exec:java -Dexec.mainClass="husky.sql.calcite.Execute" -Dexec.args=\"\'select TRANS_ID,ITEM_ID,PRICE,SELLER_ID,SELLER_NAME from Parts where PRICE > 2.0 and ITEM_ID = 2 or SELLER_ID = 4\'\" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${HuskySQL_JARS})

# Executables
set(as_string "A")
add_custom_target(HuskyQueryPlan ALL mvn exec:java -Dexec.mainClass="husky.sql.calcite.planner.HuskyQueryPlanner" -Dexec.args=\"\'select L_RETURNFLAG, L_LINESTATUS, sum(L_QUANTITY) as SUM_QTY, sum(L_EXTENDEDPRICE) as SUM_BASE_PRICE, sum(L_EXTENDEDPRICE * (1 - L_DISCOUNT)) as SUM_DISC_PRICE, sum(L_EXTENDEDPRICE * (1 - L_DISCOUNT) * (1 + L_TAX)) as SUM_CHARGE, avg(L_QUANTITY) as AVG_QTY, avg(L_EXTENDEDPRICE) as AVG_PRICE, avg(L_DISCOUNT) as AVG_DISC, count(*) as COUNT_ORDER from Lineitem where L_ORDERKEY < 1000 group by L_RETURNFLAG, L_LINESTATUS order by L_RETURNFLAG, L_LINESTATUS \'\" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${HuskySQL_JARS})

# Executables
add_custom_target(HuskyQueryPlanToJson ALL mvn exec:java -Dexec.mainClass="husky.sql.calcite.HuskyQueryPlanToJson" -Dexec.args=\"\'select count(P_PARTKEY),P_BRAND from Part where P_PARTKEY < 1000 group by P_BRAND order by count(P_PARTKEY) asc\' \'output/plan.json\'\" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${HuskySQL_JARS})