cmake_minimum_required(VERSION 3.16)
project(evogrid LANGUAGES CXX)

#–– Tell CMake to use straight ISO C++17––
set(CMAKE_CXX_STANDARD     17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS    OFF)   # disable GNU/Clang extensions

file(GLOB SOURCES "src/*.cpp")
find_package(raylib REQUIRED)

add_executable(evogrid ${SOURCES})

target_include_directories(evogrid
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
#–– Explicitly require the C++17 feature on our target ––
target_compile_features(evogrid PRIVATE cxx_std_17)

target_link_libraries(evogrid PRIVATE raylib)
if(APPLE)
  target_link_libraries(evogrid PRIVATE
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreAudio"
    "-framework CoreVideo"
  )
endif()

add_custom_target(run
  COMMAND $<TARGET_FILE:evogrid>
  DEPENDS evogrid
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)