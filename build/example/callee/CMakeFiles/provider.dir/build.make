# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yefimov/project/myrpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yefimov/project/myrpc/build

# Include any dependencies generated for this target.
include example/callee/CMakeFiles/provider.dir/depend.make

# Include the progress variables for this target.
include example/callee/CMakeFiles/provider.dir/progress.make

# Include the compile flags for this target's objects.
include example/callee/CMakeFiles/provider.dir/flags.make

example/callee/CMakeFiles/provider.dir/userservice.cc.o: example/callee/CMakeFiles/provider.dir/flags.make
example/callee/CMakeFiles/provider.dir/userservice.cc.o: ../example/callee/userservice.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yefimov/project/myrpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/callee/CMakeFiles/provider.dir/userservice.cc.o"
	cd /home/yefimov/project/myrpc/build/example/callee && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/provider.dir/userservice.cc.o -c /home/yefimov/project/myrpc/example/callee/userservice.cc

example/callee/CMakeFiles/provider.dir/userservice.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/provider.dir/userservice.cc.i"
	cd /home/yefimov/project/myrpc/build/example/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yefimov/project/myrpc/example/callee/userservice.cc > CMakeFiles/provider.dir/userservice.cc.i

example/callee/CMakeFiles/provider.dir/userservice.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/provider.dir/userservice.cc.s"
	cd /home/yefimov/project/myrpc/build/example/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yefimov/project/myrpc/example/callee/userservice.cc -o CMakeFiles/provider.dir/userservice.cc.s

example/callee/CMakeFiles/provider.dir/__/entrywebserver.pb.cc.o: example/callee/CMakeFiles/provider.dir/flags.make
example/callee/CMakeFiles/provider.dir/__/entrywebserver.pb.cc.o: ../example/entrywebserver.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yefimov/project/myrpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object example/callee/CMakeFiles/provider.dir/__/entrywebserver.pb.cc.o"
	cd /home/yefimov/project/myrpc/build/example/callee && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/provider.dir/__/entrywebserver.pb.cc.o -c /home/yefimov/project/myrpc/example/entrywebserver.pb.cc

example/callee/CMakeFiles/provider.dir/__/entrywebserver.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/provider.dir/__/entrywebserver.pb.cc.i"
	cd /home/yefimov/project/myrpc/build/example/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yefimov/project/myrpc/example/entrywebserver.pb.cc > CMakeFiles/provider.dir/__/entrywebserver.pb.cc.i

example/callee/CMakeFiles/provider.dir/__/entrywebserver.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/provider.dir/__/entrywebserver.pb.cc.s"
	cd /home/yefimov/project/myrpc/build/example/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yefimov/project/myrpc/example/entrywebserver.pb.cc -o CMakeFiles/provider.dir/__/entrywebserver.pb.cc.s

# Object files for target provider
provider_OBJECTS = \
"CMakeFiles/provider.dir/userservice.cc.o" \
"CMakeFiles/provider.dir/__/entrywebserver.pb.cc.o"

# External object files for target provider
provider_EXTERNAL_OBJECTS =

../bin/provider: example/callee/CMakeFiles/provider.dir/userservice.cc.o
../bin/provider: example/callee/CMakeFiles/provider.dir/__/entrywebserver.pb.cc.o
../bin/provider: example/callee/CMakeFiles/provider.dir/build.make
../bin/provider: ../lib/libmprpc.a
../bin/provider: example/callee/CMakeFiles/provider.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yefimov/project/myrpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../bin/provider"
	cd /home/yefimov/project/myrpc/build/example/callee && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/provider.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/callee/CMakeFiles/provider.dir/build: ../bin/provider

.PHONY : example/callee/CMakeFiles/provider.dir/build

example/callee/CMakeFiles/provider.dir/clean:
	cd /home/yefimov/project/myrpc/build/example/callee && $(CMAKE_COMMAND) -P CMakeFiles/provider.dir/cmake_clean.cmake
.PHONY : example/callee/CMakeFiles/provider.dir/clean

example/callee/CMakeFiles/provider.dir/depend:
	cd /home/yefimov/project/myrpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yefimov/project/myrpc /home/yefimov/project/myrpc/example/callee /home/yefimov/project/myrpc/build /home/yefimov/project/myrpc/build/example/callee /home/yefimov/project/myrpc/build/example/callee/CMakeFiles/provider.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/callee/CMakeFiles/provider.dir/depend

