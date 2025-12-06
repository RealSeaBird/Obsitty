cmake_minimum_required(VERSION 3.20)
project(obsitty)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find or fetch FTXUI
include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

# Your source files
add_executable(obsitty main.cpp)

# Link FTXUI - CMake will handle all dependencies
target_link_libraries(obsitty
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
