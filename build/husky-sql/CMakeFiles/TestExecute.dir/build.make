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
include husky-sql/CMakeFiles/TestExecute.dir/depend.make

# Include the progress variables for this target.
include husky-sql/CMakeFiles/TestExecute.dir/progress.make

# Include the compile flags for this target's objects.
include husky-sql/CMakeFiles/TestExecute.dir/flags.make

husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o: husky-sql/CMakeFiles/TestExecute.dir/flags.make
husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o: ../husky-sql/execute.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky-sql && /data/opt/gcc-6.1.0-bottle/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TestExecute.dir/execute.cpp.o -c /data/opt/tmp/1155107959/husky-sql/husky-sql/execute.cpp

husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TestExecute.dir/execute.cpp.i"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky-sql && /data/opt/gcc-6.1.0-bottle/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/opt/tmp/1155107959/husky-sql/husky-sql/execute.cpp > CMakeFiles/TestExecute.dir/execute.cpp.i

husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TestExecute.dir/execute.cpp.s"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky-sql && /data/opt/gcc-6.1.0-bottle/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/opt/tmp/1155107959/husky-sql/husky-sql/execute.cpp -o CMakeFiles/TestExecute.dir/execute.cpp.s

husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o.requires:

.PHONY : husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o.requires

husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o.provides: husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o.requires
	$(MAKE) -f husky-sql/CMakeFiles/TestExecute.dir/build.make husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o.provides.build
.PHONY : husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o.provides

husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o.provides.build: husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o


# Object files for target TestExecute
TestExecute_OBJECTS = \
"CMakeFiles/TestExecute.dir/execute.cpp.o"

# External object files for target TestExecute
TestExecute_EXTERNAL_OBJECTS =

husky-sql/TestExecute: husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o
husky-sql/TestExecute: husky-sql/CMakeFiles/TestExecute.dir/build.make
husky-sql/TestExecute: husky/libhusky.a
husky-sql/TestExecute: husky/lib/libglog.a
husky-sql/TestExecute: /data/opt/lib/libzmq.so
husky-sql/TestExecute: /data/opt/brew/lib/libtcmalloc.so
husky-sql/TestExecute: /data/opt/brew/lib/libboost_system-mt.so
husky-sql/TestExecute: /data/opt/brew/lib/libboost_filesystem-mt.so
husky-sql/TestExecute: /data/opt/brew/lib/libboost_regex-mt.so
husky-sql/TestExecute: /data/opt/brew/lib/libboost_thread-mt.so
husky-sql/TestExecute: /data/opt/brew/lib/libboost_program_options-mt.so
husky-sql/TestExecute: /data/opt/brew/lib/libboost_date_time-mt.so
husky-sql/TestExecute: /data/opt/brew/lib/libboost_chrono-mt.so
husky-sql/TestExecute: /data/opt/brew/lib/libboost_atomic-mt.so
husky-sql/TestExecute: /data/opt/tmp/tati/install/lib/libhdfs3.so
husky-sql/TestExecute: /usr/lib64/librt.so
husky-sql/TestExecute: /data/opt/lib/libmongoclient.so
husky-sql/TestExecute: /data/opt/lib/libthrift.so
husky-sql/TestExecute: husky-sql/CMakeFiles/TestExecute.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TestExecute"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky-sql && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestExecute.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
husky-sql/CMakeFiles/TestExecute.dir/build: husky-sql/TestExecute

.PHONY : husky-sql/CMakeFiles/TestExecute.dir/build

husky-sql/CMakeFiles/TestExecute.dir/requires: husky-sql/CMakeFiles/TestExecute.dir/execute.cpp.o.requires

.PHONY : husky-sql/CMakeFiles/TestExecute.dir/requires

husky-sql/CMakeFiles/TestExecute.dir/clean:
	cd /data/opt/tmp/1155107959/husky-sql/build/husky-sql && $(CMAKE_COMMAND) -P CMakeFiles/TestExecute.dir/cmake_clean.cmake
.PHONY : husky-sql/CMakeFiles/TestExecute.dir/clean

husky-sql/CMakeFiles/TestExecute.dir/depend:
	cd /data/opt/tmp/1155107959/husky-sql/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/opt/tmp/1155107959/husky-sql /data/opt/tmp/1155107959/husky-sql/husky-sql /data/opt/tmp/1155107959/husky-sql/build /data/opt/tmp/1155107959/husky-sql/build/husky-sql /data/opt/tmp/1155107959/husky-sql/build/husky-sql/CMakeFiles/TestExecute.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : husky-sql/CMakeFiles/TestExecute.dir/depend

