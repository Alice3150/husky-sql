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

# Include any dependencies generated for this target.
include husky/examples/CMakeFiles/LinearRegression.dir/depend.make

# Include the progress variables for this target.
include husky/examples/CMakeFiles/LinearRegression.dir/progress.make

# Include the compile flags for this target's objects.
include husky/examples/CMakeFiles/LinearRegression.dir/flags.make

husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o: husky/examples/CMakeFiles/LinearRegression.dir/flags.make
husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o: ../husky/examples/linear_regression.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && /data/opt/gcc-6.1.0-bottle/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LinearRegression.dir/linear_regression.cpp.o -c /data/opt/tmp/1155107959/husky-sql/husky/examples/linear_regression.cpp

husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinearRegression.dir/linear_regression.cpp.i"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && /data/opt/gcc-6.1.0-bottle/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/opt/tmp/1155107959/husky-sql/husky/examples/linear_regression.cpp > CMakeFiles/LinearRegression.dir/linear_regression.cpp.i

husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinearRegression.dir/linear_regression.cpp.s"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && /data/opt/gcc-6.1.0-bottle/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/opt/tmp/1155107959/husky-sql/husky/examples/linear_regression.cpp -o CMakeFiles/LinearRegression.dir/linear_regression.cpp.s

husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o.requires:

.PHONY : husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o.requires

husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o.provides: husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o.requires
	$(MAKE) -f husky/examples/CMakeFiles/LinearRegression.dir/build.make husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o.provides.build
.PHONY : husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o.provides

husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o.provides.build: husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o


# Object files for target LinearRegression
LinearRegression_OBJECTS = \
"CMakeFiles/LinearRegression.dir/linear_regression.cpp.o"

# External object files for target LinearRegression
LinearRegression_EXTERNAL_OBJECTS =

husky/LinearRegression: husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o
husky/LinearRegression: husky/examples/CMakeFiles/LinearRegression.dir/build.make
husky/LinearRegression: husky/lib/libhusky-lib.a
husky/LinearRegression: husky/io/libhusky-io.a
husky/LinearRegression: husky/core/libhusky-core.a
husky/LinearRegression: husky/base/libhusky-base.a
husky/LinearRegression: husky/lib/libglog.a
husky/LinearRegression: /data/opt/lib/libzmq.so
husky/LinearRegression: /data/opt/brew/lib/libtcmalloc.so
husky/LinearRegression: /data/opt/brew/lib/libboost_system-mt.so
husky/LinearRegression: /data/opt/brew/lib/libboost_filesystem-mt.so
husky/LinearRegression: /data/opt/brew/lib/libboost_regex-mt.so
husky/LinearRegression: /data/opt/brew/lib/libboost_thread-mt.so
husky/LinearRegression: /data/opt/brew/lib/libboost_program_options-mt.so
husky/LinearRegression: /data/opt/brew/lib/libboost_date_time-mt.so
husky/LinearRegression: /data/opt/brew/lib/libboost_chrono-mt.so
husky/LinearRegression: /data/opt/brew/lib/libboost_atomic-mt.so
husky/LinearRegression: /data/opt/tmp/tati/install/lib/libhdfs3.so
husky/LinearRegression: /usr/lib64/librt.so
husky/LinearRegression: /data/opt/lib/libmongoclient.so
husky/LinearRegression: /data/opt/lib/libthrift.so
husky/LinearRegression: husky/examples/CMakeFiles/LinearRegression.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../LinearRegression"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LinearRegression.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
husky/examples/CMakeFiles/LinearRegression.dir/build: husky/LinearRegression

.PHONY : husky/examples/CMakeFiles/LinearRegression.dir/build

husky/examples/CMakeFiles/LinearRegression.dir/requires: husky/examples/CMakeFiles/LinearRegression.dir/linear_regression.cpp.o.requires

.PHONY : husky/examples/CMakeFiles/LinearRegression.dir/requires

husky/examples/CMakeFiles/LinearRegression.dir/clean:
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && $(CMAKE_COMMAND) -P CMakeFiles/LinearRegression.dir/cmake_clean.cmake
.PHONY : husky/examples/CMakeFiles/LinearRegression.dir/clean

husky/examples/CMakeFiles/LinearRegression.dir/depend:
	cd /data/opt/tmp/1155107959/husky-sql/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/opt/tmp/1155107959/husky-sql /data/opt/tmp/1155107959/husky-sql/husky/examples /data/opt/tmp/1155107959/husky-sql/build /data/opt/tmp/1155107959/husky-sql/build/husky/examples /data/opt/tmp/1155107959/husky-sql/build/husky/examples/CMakeFiles/LinearRegression.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : husky/examples/CMakeFiles/LinearRegression.dir/depend

