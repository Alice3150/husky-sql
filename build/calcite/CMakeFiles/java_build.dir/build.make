# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /data/opt/brew/Cellar/cmake/3.10.1/bin/cmake

# The command to remove a file.
RM = /data/opt/brew/Cellar/cmake/3.10.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/opt/tmp/1155107959/husky-sql

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/opt/tmp/1155107959/husky-sql/build

# Utility rule file for java_build.

# Include the progress variables for this target.
include calcite/CMakeFiles/java_build.dir/progress.make

calcite/CMakeFiles/java_build: calcite/husky-calcite-1.0.0.jar


calcite/husky-calcite-1.0.0.jar:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Build the java directory"
	cd /data/opt/tmp/1155107959/husky-sql/calcite && mvn -Dbuild.dir=/data/opt/tmp/1155107959/husky-sql/build -DskipTests package

java_build: calcite/CMakeFiles/java_build
java_build: calcite/husky-calcite-1.0.0.jar
java_build: calcite/CMakeFiles/java_build.dir/build.make

.PHONY : java_build

# Rule to build all files generated by this target.
calcite/CMakeFiles/java_build.dir/build: java_build

.PHONY : calcite/CMakeFiles/java_build.dir/build

calcite/CMakeFiles/java_build.dir/clean:
	cd /data/opt/tmp/1155107959/husky-sql/build/calcite && $(CMAKE_COMMAND) -P CMakeFiles/java_build.dir/cmake_clean.cmake
.PHONY : calcite/CMakeFiles/java_build.dir/clean

calcite/CMakeFiles/java_build.dir/depend:
	cd /data/opt/tmp/1155107959/husky-sql/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/opt/tmp/1155107959/husky-sql /data/opt/tmp/1155107959/husky-sql/calcite /data/opt/tmp/1155107959/husky-sql/build /data/opt/tmp/1155107959/husky-sql/build/calcite /data/opt/tmp/1155107959/husky-sql/build/calcite/CMakeFiles/java_build.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : calcite/CMakeFiles/java_build.dir/depend

