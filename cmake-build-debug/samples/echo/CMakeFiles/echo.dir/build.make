# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\firen\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\212.5457.51\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\firen\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\212.5457.51\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug

# Include any dependencies generated for this target.
include samples/echo/CMakeFiles/echo.dir/depend.make
# Include the progress variables for this target.
include samples/echo/CMakeFiles/echo.dir/progress.make

# Include the compile flags for this target's objects.
include samples/echo/CMakeFiles/echo.dir/flags.make

samples/echo/CMakeFiles/echo.dir/main.cpp.obj: samples/echo/CMakeFiles/echo.dir/flags.make
samples/echo/CMakeFiles/echo.dir/main.cpp.obj: ../samples/echo/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object samples/echo/CMakeFiles/echo.dir/main.cpp.obj"
	cd /d C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo && C:\PROGRA~2\GNUARM~1\102021~1.07\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\echo.dir\main.cpp.obj -c C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\samples\echo\main.cpp

samples/echo/CMakeFiles/echo.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/echo.dir/main.cpp.i"
	cd /d C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo && C:\PROGRA~2\GNUARM~1\102021~1.07\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\samples\echo\main.cpp > CMakeFiles\echo.dir\main.cpp.i

samples/echo/CMakeFiles/echo.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/echo.dir/main.cpp.s"
	cd /d C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo && C:\PROGRA~2\GNUARM~1\102021~1.07\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\samples\echo\main.cpp -o CMakeFiles\echo.dir\main.cpp.s

# Object files for target echo
echo_OBJECTS = \
"CMakeFiles/echo.dir/main.cpp.obj"

# External object files for target echo
echo_EXTERNAL_OBJECTS =

samples/echo/echo.elf: samples/echo/CMakeFiles/echo.dir/main.cpp.obj
samples/echo/echo.elf: samples/echo/CMakeFiles/echo.dir/build.make
samples/echo/echo.elf: libEVTd.a
samples/echo/echo.elf: libs/HALf3/libHALf3d.a
samples/echo/echo.elf: samples/echo/CMakeFiles/echo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable echo.elf"
	cd /d C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\echo.dir\link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Building echo.hex "
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Building echo.bin"
	cd /d C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo && "C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.07\bin\arm-none-eabi-objcopy.exe" -Oihex C:/Users/firen/Documents/GitHub/Electric-Vehicle-Team/EVT-core/cmake-build-debug/samples/echo/echo.elf echo.hex
	cd /d C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo && "C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.07\bin\arm-none-eabi-objcopy.exe" -Obinary C:/Users/firen/Documents/GitHub/Electric-Vehicle-Team/EVT-core/cmake-build-debug/samples/echo/echo.elf echo.bin

# Rule to build all files generated by this target.
samples/echo/CMakeFiles/echo.dir/build: samples/echo/echo.elf
.PHONY : samples/echo/CMakeFiles/echo.dir/build

samples/echo/CMakeFiles/echo.dir/clean:
	cd /d C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo && $(CMAKE_COMMAND) -P CMakeFiles\echo.dir\cmake_clean.cmake
.PHONY : samples/echo/CMakeFiles/echo.dir/clean

samples/echo/CMakeFiles/echo.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\samples\echo C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo C:\Users\firen\Documents\GitHub\Electric-Vehicle-Team\EVT-core\cmake-build-debug\samples\echo\CMakeFiles\echo.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : samples/echo/CMakeFiles/echo.dir/depend

