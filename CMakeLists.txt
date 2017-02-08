cmake_minimum_required(VERSION 2.6)
project(HD44780Lcd)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#--------------------------------------------------------------------------

add_library(HD44780Lcd STATIC lib/FileDescriptor.cxx
							  lib/Lcd.cxx
							  lib/LcdI2C.cxx
							  lib/LcdColumnGraph.cxx
							  lib/LcdPixel.cxx
							  lib/LcdRowGraph.cxx
							  lib/LcdRowSlider.cxx)

include_directories(${PROJECT_SOURCE_DIR}/lib)

set(EXTRA_LIBS ${EXTRA_LIBS} HD44780Lcd)

#--------------------------------------------------------------------------

add_executable(clock examples/clock.cxx examples/LinuxKeys.cxx)
target_link_libraries(clock HD44780Lcd)

add_executable(column_graph examples/column_graph.cxx
							examples/LinuxKeys.cxx)
target_link_libraries(column_graph HD44780Lcd)

add_executable(custom_chars examples/custom_chars.cxx
							examples/LinuxKeys.cxx)
target_link_libraries(custom_chars HD44780Lcd)

add_executable(ipaddress examples/ipaddress.cxx)
target_link_libraries(ipaddress HD44780Lcd)

add_executable(life examples/life.cxx examples/LinuxKeys.cxx)
target_link_libraries(life HD44780Lcd)

add_executable(pixel examples/pixel.cxx examples/LinuxKeys.cxx)
target_link_libraries(pixel HD44780Lcd)

add_executable(row_graph examples/row_graph.cxx examples/LinuxKeys.cxx)
target_link_libraries(row_graph HD44780Lcd)

add_executable(row_slider examples/row_slider.cxx examples/LinuxKeys.cxx)
target_link_libraries(row_slider HD44780Lcd)

