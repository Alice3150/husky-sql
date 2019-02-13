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

include(ExternalProject)

set(EXTERNAL_INCLUDE "")
# set(EXTERNAL_LIB "")
set(EXTERNAL_DEFINITION "")
set(THIRDPARTY_DIR ${PROJECT_BINARY_DIR}/third_party)
set(THIRDPARTY_LOG_OPTIONS LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1 LOG_DOWNLOAD 1)

set(JSON_VERSION "3.2.0")

### JSON ###

if (NOT "${JSON_INCLUDE_DIR}" STREQUAL "")
  file(TO_CMAKE_PATH ${JSON_INCLUDE_DIR} _json_path)
endif()

find_path (JSON_INCLUDE_DIR NAMES "nlohmann/json.hpp" HINTS ${_json_path})

if (JSON_INCLUDE_DIR)
  message (STATUS "Found nlohmann/json.hpp:")
  message (STATUS "  (Headers)       ${JSON_INCLUDE_DIR}")
  list(APPEND EXTERNAL_INCLUDE "${JSON_INCLUDE_DIR}")
else()
  ExternalProject_Add(json_ep
    PREFIX ${THIRDPARTY_DIR}/nlohmann
    DOWNLOAD_DIR ${THIRDPARTY_DIR}/nlohmann
    DOWNLOAD_NO_EXTRACT true
    SOURCE_DIR ""
    BINARY_DIR ""
    URL "https://github.com/nlohmann/json/releases/download/v${JSON_VERSION}/json.hpp"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    ${THIRDPARTY_LOG_OPTIONS})
  list(APPEND EXTERNAL_DEPENDENCIES json_ep)
  list(APPEND EXTERNAL_INCLUDE "${THIRDPARTY_DIR}")
endif()

### END JSON ###

include_directories(${EXTERNAL_INCLUDE})
add_definitions(${EXTERNAL_DEFINITION})