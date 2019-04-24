#pragma once

#include "core/engine.hpp"

#include "husky-sql/utils/row_kv.hpp"

#include <string>

namespace husky {
namespace sql {

void save_to_hdfs(ObjList<RowKV>& records, const std::string& output_path);

} // namespace sql
} // namespace husky