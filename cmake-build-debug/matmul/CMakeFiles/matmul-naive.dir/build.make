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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug

# Include any dependencies generated for this target.
include matmul/CMakeFiles/matmul-naive.dir/depend.make

# Include the progress variables for this target.
include matmul/CMakeFiles/matmul-naive.dir/progress.make

# Include the compile flags for this target's objects.
include matmul/CMakeFiles/matmul-naive.dir/flags.make

matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o: matmul/CMakeFiles/matmul-naive.dir/flags.make
matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o: ../matmul/dgemm-naive.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o"
	cd /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/matmul && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/matmul-naive.dir/dgemm-naive.c.o   -c /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/matmul/dgemm-naive.c

matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/matmul-naive.dir/dgemm-naive.c.i"
	cd /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/matmul && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/matmul/dgemm-naive.c > CMakeFiles/matmul-naive.dir/dgemm-naive.c.i

matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/matmul-naive.dir/dgemm-naive.c.s"
	cd /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/matmul && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/matmul/dgemm-naive.c -o CMakeFiles/matmul-naive.dir/dgemm-naive.c.s

matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o.requires:

.PHONY : matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o.requires

matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o.provides: matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o.requires
	$(MAKE) -f matmul/CMakeFiles/matmul-naive.dir/build.make matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o.provides.build
.PHONY : matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o.provides

matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o.provides.build: matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o


# Object files for target matmul-naive
matmul__naive_OBJECTS = \
"CMakeFiles/matmul-naive.dir/dgemm-naive.c.o"

# External object files for target matmul-naive
matmul__naive_EXTERNAL_OBJECTS =

matmul/libmatmul-naive.a: matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o
matmul/libmatmul-naive.a: matmul/CMakeFiles/matmul-naive.dir/build.make
matmul/libmatmul-naive.a: matmul/CMakeFiles/matmul-naive.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libmatmul-naive.a"
	cd /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/matmul && $(CMAKE_COMMAND) -P CMakeFiles/matmul-naive.dir/cmake_clean_target.cmake
	cd /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/matmul && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/matmul-naive.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
matmul/CMakeFiles/matmul-naive.dir/build: matmul/libmatmul-naive.a

.PHONY : matmul/CMakeFiles/matmul-naive.dir/build

matmul/CMakeFiles/matmul-naive.dir/requires: matmul/CMakeFiles/matmul-naive.dir/dgemm-naive.c.o.requires

.PHONY : matmul/CMakeFiles/matmul-naive.dir/requires

matmul/CMakeFiles/matmul-naive.dir/clean:
	cd /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/matmul && $(CMAKE_COMMAND) -P CMakeFiles/matmul-naive.dir/cmake_clean.cmake
.PHONY : matmul/CMakeFiles/matmul-naive.dir/clean

matmul/CMakeFiles/matmul-naive.dir/depend:
	cd /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/matmul /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/matmul /Users/ajinkyachalke/Desktop/csc698/term-project/CSC-698-Term-Project/cmake-build-debug/matmul/CMakeFiles/matmul-naive.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : matmul/CMakeFiles/matmul-naive.dir/depend
