# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wencheng/crypto/key_management

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wencheng/crypto/key_management/build

# Include any dependencies generated for this target.
include src/CMakeFiles/km_src.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/km_src.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/km_src.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/km_src.dir/flags.make

src/CMakeFiles/km_src.dir/key_manage.c.o: src/CMakeFiles/km_src.dir/flags.make
src/CMakeFiles/km_src.dir/key_manage.c.o: /home/wencheng/crypto/key_management/src/key_manage.c
src/CMakeFiles/km_src.dir/key_manage.c.o: src/CMakeFiles/km_src.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wencheng/crypto/key_management/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/km_src.dir/key_manage.c.o"
	cd /home/wencheng/crypto/key_management/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/km_src.dir/key_manage.c.o -MF CMakeFiles/km_src.dir/key_manage.c.o.d -o CMakeFiles/km_src.dir/key_manage.c.o -c /home/wencheng/crypto/key_management/src/key_manage.c

src/CMakeFiles/km_src.dir/key_manage.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/km_src.dir/key_manage.c.i"
	cd /home/wencheng/crypto/key_management/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wencheng/crypto/key_management/src/key_manage.c > CMakeFiles/km_src.dir/key_manage.c.i

src/CMakeFiles/km_src.dir/key_manage.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/km_src.dir/key_manage.c.s"
	cd /home/wencheng/crypto/key_management/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wencheng/crypto/key_management/src/key_manage.c -o CMakeFiles/km_src.dir/key_manage.c.s

src/CMakeFiles/km_src.dir/kmdb.c.o: src/CMakeFiles/km_src.dir/flags.make
src/CMakeFiles/km_src.dir/kmdb.c.o: /home/wencheng/crypto/key_management/src/kmdb.c
src/CMakeFiles/km_src.dir/kmdb.c.o: src/CMakeFiles/km_src.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wencheng/crypto/key_management/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/km_src.dir/kmdb.c.o"
	cd /home/wencheng/crypto/key_management/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/km_src.dir/kmdb.c.o -MF CMakeFiles/km_src.dir/kmdb.c.o.d -o CMakeFiles/km_src.dir/kmdb.c.o -c /home/wencheng/crypto/key_management/src/kmdb.c

src/CMakeFiles/km_src.dir/kmdb.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/km_src.dir/kmdb.c.i"
	cd /home/wencheng/crypto/key_management/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wencheng/crypto/key_management/src/kmdb.c > CMakeFiles/km_src.dir/kmdb.c.i

src/CMakeFiles/km_src.dir/kmdb.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/km_src.dir/kmdb.c.s"
	cd /home/wencheng/crypto/key_management/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wencheng/crypto/key_management/src/kmdb.c -o CMakeFiles/km_src.dir/kmdb.c.s

# Object files for target km_src
km_src_OBJECTS = \
"CMakeFiles/km_src.dir/key_manage.c.o" \
"CMakeFiles/km_src.dir/kmdb.c.o"

# External object files for target km_src
km_src_EXTERNAL_OBJECTS =

src/libkm_src.so: src/CMakeFiles/km_src.dir/key_manage.c.o
src/libkm_src.so: src/CMakeFiles/km_src.dir/kmdb.c.o
src/libkm_src.so: src/CMakeFiles/km_src.dir/build.make
src/libkm_src.so: src/CMakeFiles/km_src.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wencheng/crypto/key_management/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C shared library libkm_src.so"
	cd /home/wencheng/crypto/key_management/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/km_src.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/km_src.dir/build: src/libkm_src.so
.PHONY : src/CMakeFiles/km_src.dir/build

src/CMakeFiles/km_src.dir/clean:
	cd /home/wencheng/crypto/key_management/build/src && $(CMAKE_COMMAND) -P CMakeFiles/km_src.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/km_src.dir/clean

src/CMakeFiles/km_src.dir/depend:
	cd /home/wencheng/crypto/key_management/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wencheng/crypto/key_management /home/wencheng/crypto/key_management/src /home/wencheng/crypto/key_management/build /home/wencheng/crypto/key_management/build/src /home/wencheng/crypto/key_management/build/src/CMakeFiles/km_src.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/km_src.dir/depend

