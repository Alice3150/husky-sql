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
include husky/examples/CMakeFiles/VectorDeprecated.dir/depend.make

# Include the progress variables for this target.
include husky/examples/CMakeFiles/VectorDeprecated.dir/progress.make

# Include the compile flags for this target's objects.
include husky/examples/CMakeFiles/VectorDeprecated.dir/flags.make

husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o: husky/examples/CMakeFiles/VectorDeprecated.dir/flags.make
husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o: ../husky/examples/vector_deprecated.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && /data/opt/gcc-6.1.0-bottle/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o -c /data/opt/tmp/1155107959/husky-sql/husky/examples/vector_deprecated.cpp

husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.i"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && /data/opt/gcc-6.1.0-bottle/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/opt/tmp/1155107959/husky-sql/husky/examples/vector_deprecated.cpp > CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.i

husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.s"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && /data/opt/gcc-6.1.0-bottle/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/opt/tmp/1155107959/husky-sql/husky/examples/vector_deprecated.cpp -o CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.s

husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o.requires:

.PHONY : husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o.requires

husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o.provides: husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o.requires
	$(MAKE) -f husky/examples/CMakeFiles/VectorDeprecated.dir/build.make husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o.provides.build
.PHONY : husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o.provides

husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o.provides.build: husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o


# Object files for target VectorDeprecated
VectorDeprecated_OBJECTS = \
"CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o"

# External object files for target VectorDeprecated
VectorDeprecated_EXTERNAL_OBJECTS =

husky/VectorDeprecated: husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o
husky/VectorDeprecated: husky/examples/CMakeFiles/VectorDeprecated.dir/build.make
husky/VectorDeprecated: husky/lib/libhusky-lib.a
husky/VectorDeprecated: husky/io/libhusky-io.a
husky/VectorDeprecated: husky/core/libhusky-core.a
husky/VectorDeprecated: husky/base/libhusky-base.a
husky/VectorDeprecated: husky/lib/libglog.a
husky/VectorDeprecated: /data/opt/lib/libzmq.so
husky/VectorDeprecated: /data/opt/brew/lib/libtcmalloc.so
husky/VectorDeprecated: /data/opt/brew/lib/libboost_system-mt.so
husky/VectorDeprecated: /data/opt/brew/lib/libboost_filesystem-mt.so
husky/VectorDeprecated: /data/opt/brew/lib/libboost_regex-mt.so
husky/VectorDeprecated: /data/opt/brew/lib/libboost_thread-mt.so
husky/VectorDeprecated: /data/opt/brew/lib/libboost_program_options-mt.so
husky/VectorDeprecated: /data/opt/brew/lib/libboost_date_time-mt.so
husky/VectorDeprecated: /data/opt/brew/lib/libboost_chrono-mt.so
husky/VectorDeprecated: /data/opt/brew/lib/libboost_atomic-mt.so
husky/VectorDeprecated: /data/opt/tmp/tati/install/lib/libhdfs3.so
husky/VectorDeprecated: /usr/lib64/librt.so
husky/VectorDeprecated: /data/opt/lib/libmongoclient.so
husky/VectorDeprecated: /data/opt/lib/libthrift.so
husky/VectorDeprecated: husky/examples/CMakeFiles/VectorDeprecated.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/opt/tmp/1155107959/husky-sql/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../VectorDeprecated"
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/VectorDeprecated.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
husky/examples/CMakeFiles/VectorDeprecated.dir/build: husky/VectorDeprecated

.PHONY : husky/examples/CMakeFiles/VectorDeprecated.dir/build

husky/examples/CMakeFiles/VectorDeprecated.dir/requires: husky/examples/CMakeFiles/VectorDeprecated.dir/vector_deprecated.cpp.o.requires

.PHONY : husky/examples/CMakeFiles/VectorDeprecated.dir/requires

husky/examples/CMakeFiles/VectorDeprecated.dir/clean:
	cd /data/opt/tmp/1155107959/husky-sql/build/husky/examples && $(CMAKE_COMMAND) -P CMakeFiles/VectorDeprecated.dir/cmake_clean.cmake
.PHONY : husky/examples/CMakeFiles/VectorDeprecated.dir/clean

husky/examples/CMakeFiles/VectorDeprecated.dir/depend:
	cd /data/opt/tmp/1155107959/husky-sql/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/opt/tmp/1155107959/husky-sql /data/opt/tmp/1155107959/husky-sql/husky/examples /data/opt/tmp/1155107959/husky-sql/build /data/opt/tmp/1155107959/husky-sql/build/husky/examples /data/opt/tmp/1155107959/husky-sql/build/husky/examples/CMakeFiles/VectorDeprecated.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : husky/examples/CMakeFiles/VectorDeprecated.dir/depend

