# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/hagairaja/Documents/Installer/CLion/CLion-2018.3.4/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/hagairaja/Documents/Installer/CLion/CLion-2018.3.4/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/grafika-opengl-showcase.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/grafika-opengl-showcase.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/grafika-opengl-showcase.dir/flags.make

CMakeFiles/grafika-opengl-showcase.dir/main.cpp.o: CMakeFiles/grafika-opengl-showcase.dir/flags.make
CMakeFiles/grafika-opengl-showcase.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/grafika-opengl-showcase.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grafika-opengl-showcase.dir/main.cpp.o -c /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/main.cpp

CMakeFiles/grafika-opengl-showcase.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grafika-opengl-showcase.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/main.cpp > CMakeFiles/grafika-opengl-showcase.dir/main.cpp.i

CMakeFiles/grafika-opengl-showcase.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grafika-opengl-showcase.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/main.cpp -o CMakeFiles/grafika-opengl-showcase.dir/main.cpp.s

CMakeFiles/grafika-opengl-showcase.dir/glad.c.o: CMakeFiles/grafika-opengl-showcase.dir/flags.make
CMakeFiles/grafika-opengl-showcase.dir/glad.c.o: ../glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/grafika-opengl-showcase.dir/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/grafika-opengl-showcase.dir/glad.c.o   -c /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/glad.c

CMakeFiles/grafika-opengl-showcase.dir/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/grafika-opengl-showcase.dir/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/glad.c > CMakeFiles/grafika-opengl-showcase.dir/glad.c.i

CMakeFiles/grafika-opengl-showcase.dir/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/grafika-opengl-showcase.dir/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/glad.c -o CMakeFiles/grafika-opengl-showcase.dir/glad.c.s

# Object files for target grafika-opengl-showcase
grafika__opengl__showcase_OBJECTS = \
"CMakeFiles/grafika-opengl-showcase.dir/main.cpp.o" \
"CMakeFiles/grafika-opengl-showcase.dir/glad.c.o"

# External object files for target grafika-opengl-showcase
grafika__opengl__showcase_EXTERNAL_OBJECTS =

grafika-opengl-showcase: CMakeFiles/grafika-opengl-showcase.dir/main.cpp.o
grafika-opengl-showcase: CMakeFiles/grafika-opengl-showcase.dir/glad.c.o
grafika-opengl-showcase: CMakeFiles/grafika-opengl-showcase.dir/build.make
grafika-opengl-showcase: CMakeFiles/grafika-opengl-showcase.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable grafika-opengl-showcase"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grafika-opengl-showcase.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/grafika-opengl-showcase.dir/build: grafika-opengl-showcase

.PHONY : CMakeFiles/grafika-opengl-showcase.dir/build

CMakeFiles/grafika-opengl-showcase.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/grafika-opengl-showcase.dir/cmake_clean.cmake
.PHONY : CMakeFiles/grafika-opengl-showcase.dir/clean

CMakeFiles/grafika-opengl-showcase.dir/depend:
	cd /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug/CMakeFiles/grafika-opengl-showcase.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/grafika-opengl-showcase.dir/depend

