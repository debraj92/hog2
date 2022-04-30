# CMake generated Testfile for 
# Source directory: /Users/debrajray/MyComputer/RTS/hog2_debraj/hog2/apps/game1/test
# Build directory: /Users/debrajray/MyComputer/RTS/hog2_debraj/hog2/apps/game1/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/Users/debrajray/MyComputer/RTS/hog2_debraj/hog2/apps/game1/test/ExampleTests[1]_include.cmake")
add_test(ExampleTests "/Users/debrajray/MyComputer/RTS/hog2_debraj/hog2/apps/game1/test/ExampleTests")
set_tests_properties(ExampleTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/debrajray/MyComputer/RTS/hog2_debraj/hog2/apps/game1/test/CMakeLists.txt;56;add_test;/Users/debrajray/MyComputer/RTS/hog2_debraj/hog2/apps/game1/test/CMakeLists.txt;0;")
subdirs("googletest")
