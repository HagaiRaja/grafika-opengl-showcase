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
include CMakeFiles/grafika_opengl_showcase.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/grafika_opengl_showcase.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/grafika_opengl_showcase.dir/flags.make

CMakeFiles/grafika_opengl_showcase.dir/main.cpp.o: CMakeFiles/grafika_opengl_showcase.dir/flags.make
CMakeFiles/grafika_opengl_showcase.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/grafika_opengl_showcase.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/grafika_opengl_showcase.dir/main.cpp.o -c /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/main.cpp

CMakeFiles/grafika_opengl_showcase.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/grafika_opengl_showcase.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/main.cpp > CMakeFiles/grafika_opengl_showcase.dir/main.cpp.i

CMakeFiles/grafika_opengl_showcase.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/grafika_opengl_showcase.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/main.cpp -o CMakeFiles/grafika_opengl_showcase.dir/main.cpp.s

# Object files for target grafika_opengl_showcase
grafika_opengl_showcase_OBJECTS = \
"CMakeFiles/grafika_opengl_showcase.dir/main.cpp.o"

# External object files for target grafika_opengl_showcase
grafika_opengl_showcase_EXTERNAL_OBJECTS =

grafika_opengl_showcase: CMakeFiles/grafika_opengl_showcase.dir/main.cpp.o
grafika_opengl_showcase: CMakeFiles/grafika_opengl_showcase.dir/build.make
grafika_opengl_showcase: CMakeFiles/grafika_opengl_showcase.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable grafika_opengl_showcase"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/grafika_opengl_showcase.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/grafika_opengl_showcase.dir/build: grafika_opengl_showcase

.PHONY : CMakeFiles/grafika_opengl_showcase.dir/build

CMakeFiles/grafika_opengl_showcase.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/grafika_opengl_showcase.dir/cmake_clean.cmake
.PHONY : CMakeFiles/grafika_opengl_showcase.dir/clean

CMakeFiles/grafika_opengl_showcase.dir/depend:
	cd /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug /home/hagairaja/Documents/GrafKom/grafika-opengl-showcase/cmake-build-debug/CMakeFiles/grafika_opengl_showcase.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/grafika_opengl_showcase.dir/depend

