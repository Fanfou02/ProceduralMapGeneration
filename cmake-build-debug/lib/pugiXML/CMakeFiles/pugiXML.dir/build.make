# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/zyuiop/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/191.7141.37/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/zyuiop/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/191.7141.37/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zyuiop/CLionProjects/ProceduralMapGeneration

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug

# Include any dependencies generated for this target.
include lib/pugiXML/CMakeFiles/pugiXML.dir/depend.make

# Include the progress variables for this target.
include lib/pugiXML/CMakeFiles/pugiXML.dir/progress.make

# Include the compile flags for this target's objects.
include lib/pugiXML/CMakeFiles/pugiXML.dir/flags.make

lib/pugiXML/CMakeFiles/pugiXML.dir/pugixml.cpp.o: lib/pugiXML/CMakeFiles/pugiXML.dir/flags.make
lib/pugiXML/CMakeFiles/pugiXML.dir/pugixml.cpp.o: ../lib/pugiXML/pugixml.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/pugiXML/CMakeFiles/pugiXML.dir/pugixml.cpp.o"
	cd /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/lib/pugiXML && /usr/lib/hardening-wrapper/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pugiXML.dir/pugixml.cpp.o -c /home/zyuiop/CLionProjects/ProceduralMapGeneration/lib/pugiXML/pugixml.cpp

lib/pugiXML/CMakeFiles/pugiXML.dir/pugixml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pugiXML.dir/pugixml.cpp.i"
	cd /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/lib/pugiXML && /usr/lib/hardening-wrapper/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zyuiop/CLionProjects/ProceduralMapGeneration/lib/pugiXML/pugixml.cpp > CMakeFiles/pugiXML.dir/pugixml.cpp.i

lib/pugiXML/CMakeFiles/pugiXML.dir/pugixml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pugiXML.dir/pugixml.cpp.s"
	cd /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/lib/pugiXML && /usr/lib/hardening-wrapper/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zyuiop/CLionProjects/ProceduralMapGeneration/lib/pugiXML/pugixml.cpp -o CMakeFiles/pugiXML.dir/pugixml.cpp.s

# Object files for target pugiXML
pugiXML_OBJECTS = \
"CMakeFiles/pugiXML.dir/pugixml.cpp.o"

# External object files for target pugiXML
pugiXML_EXTERNAL_OBJECTS =

libpugiXML.a: lib/pugiXML/CMakeFiles/pugiXML.dir/pugixml.cpp.o
libpugiXML.a: lib/pugiXML/CMakeFiles/pugiXML.dir/build.make
libpugiXML.a: lib/pugiXML/CMakeFiles/pugiXML.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../libpugiXML.a"
	cd /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/lib/pugiXML && $(CMAKE_COMMAND) -P CMakeFiles/pugiXML.dir/cmake_clean_target.cmake
	cd /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/lib/pugiXML && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pugiXML.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/pugiXML/CMakeFiles/pugiXML.dir/build: libpugiXML.a

.PHONY : lib/pugiXML/CMakeFiles/pugiXML.dir/build

lib/pugiXML/CMakeFiles/pugiXML.dir/clean:
	cd /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/lib/pugiXML && $(CMAKE_COMMAND) -P CMakeFiles/pugiXML.dir/cmake_clean.cmake
.PHONY : lib/pugiXML/CMakeFiles/pugiXML.dir/clean

lib/pugiXML/CMakeFiles/pugiXML.dir/depend:
	cd /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zyuiop/CLionProjects/ProceduralMapGeneration /home/zyuiop/CLionProjects/ProceduralMapGeneration/lib/pugiXML /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/lib/pugiXML /home/zyuiop/CLionProjects/ProceduralMapGeneration/cmake-build-debug/lib/pugiXML/CMakeFiles/pugiXML.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/pugiXML/CMakeFiles/pugiXML.dir/depend

