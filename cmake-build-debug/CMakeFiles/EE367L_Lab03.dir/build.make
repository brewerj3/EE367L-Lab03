# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.Vt27FBWxga

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.Vt27FBWxga/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/EE367L_Lab03.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/EE367L_Lab03.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/EE367L_Lab03.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EE367L_Lab03.dir/flags.make

CMakeFiles/EE367L_Lab03.dir/main.c.o: CMakeFiles/EE367L_Lab03.dir/flags.make
CMakeFiles/EE367L_Lab03.dir/main.c.o: /tmp/tmp.Vt27FBWxga/main.c
CMakeFiles/EE367L_Lab03.dir/main.c.o: CMakeFiles/EE367L_Lab03.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Vt27FBWxga/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/EE367L_Lab03.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/EE367L_Lab03.dir/main.c.o -MF CMakeFiles/EE367L_Lab03.dir/main.c.o.d -o CMakeFiles/EE367L_Lab03.dir/main.c.o -c /tmp/tmp.Vt27FBWxga/main.c

CMakeFiles/EE367L_Lab03.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EE367L_Lab03.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.Vt27FBWxga/main.c > CMakeFiles/EE367L_Lab03.dir/main.c.i

CMakeFiles/EE367L_Lab03.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EE367L_Lab03.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.Vt27FBWxga/main.c -o CMakeFiles/EE367L_Lab03.dir/main.c.s

# Object files for target EE367L_Lab03
EE367L_Lab03_OBJECTS = \
"CMakeFiles/EE367L_Lab03.dir/main.c.o"

# External object files for target EE367L_Lab03
EE367L_Lab03_EXTERNAL_OBJECTS =

EE367L_Lab03: CMakeFiles/EE367L_Lab03.dir/main.c.o
EE367L_Lab03: CMakeFiles/EE367L_Lab03.dir/build.make
EE367L_Lab03: CMakeFiles/EE367L_Lab03.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.Vt27FBWxga/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable EE367L_Lab03"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EE367L_Lab03.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EE367L_Lab03.dir/build: EE367L_Lab03
.PHONY : CMakeFiles/EE367L_Lab03.dir/build

CMakeFiles/EE367L_Lab03.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EE367L_Lab03.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EE367L_Lab03.dir/clean

CMakeFiles/EE367L_Lab03.dir/depend:
	cd /tmp/tmp.Vt27FBWxga/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.Vt27FBWxga /tmp/tmp.Vt27FBWxga /tmp/tmp.Vt27FBWxga/cmake-build-debug /tmp/tmp.Vt27FBWxga/cmake-build-debug /tmp/tmp.Vt27FBWxga/cmake-build-debug/CMakeFiles/EE367L_Lab03.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EE367L_Lab03.dir/depend

