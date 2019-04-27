Husky SQL
=========

[![Build Status](https://travis-ci.org/TatianaJin/husky-sql.svg?branch=master)](https://travis-ci.org/TatianaJin/husky-sql)

Dependencies
-------------
Husky SQL has the following minimal dependencies:

* nlohmann/json (Version >= 3.2.0, it will be installed automatically)
* All dependencies of [husky](https://github.com/husky-team/husky)

Build
-------------
Download the latest source code of Husky SQL:

```bash
git clone --recursive https://github.com/husky-team/husky-sql.git
cd husky-sql
```

Do an out-of-source build using CMake:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release # CMAKE_BUILD_TYPE: Release, Debug, RelWithDebInfo
make help                           # List all build target

make -j{N}                          # Make using N threads
```

Configuration & Running
-------------
For information of configuring and running Husky, please take a look at the [Husky README](https://github.com/husky-team/husky/blob/master/README.md).

##### Get husky query plan JSON (calcite/output/plan.json)
```bash
make HuskyQueryPlanToJson
```
##### Get query execution 
```bash
make ExecuteSimpleQuery
```
##### Conf file
```bash
  master_host=w10
  master_port=56789
  comm_port=45678

  hdfs_namenode=proj99
  hdfs_namenode_port=9000

  serve=0

  json_url=hdfs://localhost:9000/husky-sql/input/plan.json
  output_path=hdfs://proj99:9000/husky-sql/output/

  [worker]
  info=w1:4
  info=w2:4
  info=w3:4
  info=w4:4
  info=w5:4
```

