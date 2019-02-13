

set(command "/data/opt/brew/Cellar/cmake/3.10.1/bin/cmake;-Dmake=${make};-Dconfig=${config};-P;/data/opt/tmp/1155107959/husky-sql/build/third_party/nlohmann/src/json_ep-stamp/json_ep-download-Release-impl.cmake")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/data/opt/tmp/1155107959/husky-sql/build/third_party/nlohmann/src/json_ep-stamp/json_ep-download-out.log"
  ERROR_FILE "/data/opt/tmp/1155107959/husky-sql/build/third_party/nlohmann/src/json_ep-stamp/json_ep-download-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /data/opt/tmp/1155107959/husky-sql/build/third_party/nlohmann/src/json_ep-stamp/json_ep-download-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "json_ep download command succeeded.  See also /data/opt/tmp/1155107959/husky-sql/build/third_party/nlohmann/src/json_ep-stamp/json_ep-download-*.log")
  message(STATUS "${msg}")
endif()
