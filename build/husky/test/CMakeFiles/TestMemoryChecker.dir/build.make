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
include husky/test/CMakeFiles/TestMemoryChecker.dir/depend.make

# Include the progress variables for this target.
include husky/test/CMakeFiles/TestMemoryChecker.dir/progress.make

# Include the compile flags for this target's objects.
include husky/test/CMakeFiles/TestMemoryChecker.dir/flags.make

husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o: husky/test/CMakeFiles/TestMemoryChecker.dir/flags.make
husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o: ../husky/test/test-memory-checker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/test && /data/opt/gcc-6.1.0-bottle/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o -c /data/opt/tmp/1155107959/husky-sql/husky/test/test-memory-checker.cpp

husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.i"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/test && /data/opt/gcc-6.1.0-bottle/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/opt/tmp/1155107959/husky-sql/husky/test/test-memory-checker.cpp > CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.i

husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.s"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/test && /data/opt/gcc-6.1.0-bottle/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/opt/tmp/1155107959/husky-sql/husky/test/test-memory-checker.cpp -o CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.s

husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o.requires:

.PHONY : husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o.requires

husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o.provides: husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o.requires
	$(MAKE) -f husky/test/CMakeFiles/TestMemoryChecker.dir/build.make husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o.provides.build
.PHONY : husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o.provides

husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o.provides.build: husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o


# Object files for target TestMemoryChecker
TestMemoryChecker_OBJECTS = \
"CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o"

# External object files for target TestMemoryChecker
TestMemoryChecker_EXTERNAL_OBJECTS =

husky/TestMemoryChecker: husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o
husky/TestMemoryChecker: husky/test/CMakeFiles/TestMemoryChecker.dir/build.make
husky/TestMemoryChecker: husky/lib/libhusky-lib.a
husky/TestMemoryChecker: husky/io/libhusky-io.a
husky/TestMemoryChecker: husky/core/libhusky-core.a
husky/TestMemoryChecker: husky/base/libhusky-base.a
husky/TestMemoryChecker: husky/lib/libglog.a
husky/TestMemoryChecker: /data/opt/lib/libzmq.so
husky/TestMemoryChecker: /data/opt/brew/lib/libtcmalloc.so
husky/TestMemoryChecker: /data/opt/brew/lib/libboost_system-mt.so
husky/TestMemoryChecker: /data/opt/brew/lib/libboost_filesystem-mt.so
husky/TestMemoryChecker: /data/opt/brew/lib/libboost_regex-mt.so
husky/TestMemoryChecker: /data/opt/brew/lib/libboost_thread-mt.so
husky/TestMemoryChecker: /data/opt/brew/lib/libboost_program_options-mt.so
husky/TestMemoryChecker: /data/opt/brew/lib/libboost_date_time-mt.so
husky/TestMemoryChecker: /data/opt/brew/lib/libboost_chrono-mt.so
husky/TestMemoryChecker: /data/opt/brew/lib/libboost_atomic-mt.so
husky/TestMemoryChecker: /data/opt/tmp/tati/install/lib/libhdfs3.so
husky/TestMemoryChecker: /usr/lib64/librt.so
husky/TestMemoryChecker: /data/opt/lib/libmongoclient.so
husky/TestMemoryChecker: /data/opt/lib/libthrift.so
husky/TestMemoryChecker: husky/test/CMakeFiles/TestMemoryChecker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../TestMemoryChecker"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestMemoryChecker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
husky/test/CMakeFiles/TestMemoryChecker.dir/build: husky/TestMemoryChecker

.PHONY : husky/test/CMakeFiles/TestMemoryChecker.dir/build

husky/test/CMakeFiles/TestMemoryChecker.dir/requires: husky/test/CMakeFiles/TestMemoryChecker.dir/test-memory-checker.cpp.o.requires

.PHONY : husky/test/CMakeFiles/TestMemoryChecker.dir/requires

husky/test/CMakeFiles/TestMemoryChecker.dir/clean:
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/test && $(CMAKE_COMMAND) -P CMakeFiles/TestMemoryChecker.dir/cmake_clean.cmake
.PHONY : husky/test/CMakeFiles/TestMemoryChecker.dir/clean

husky/test/CMakeFiles/TestMemoryChecker.dir/depend:
	cd /data/opt/tmp/1155107959/husky-sql/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/opt/tmp/1155107959/husky-sql /data/opt/tmp/1155107959/husky-sql/husky/test /data/opt/tmp/1155107959/husky-sql/build /data/opt/tmp/1155107959/husky-sql/build/husky/test /data/opt/tmp/1155107959/husky-sql/build/husky/test/CMakeFiles/TestMemoryChecker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : husky/test/CMakeFiles/TestMemoryChecker.dir/depend

