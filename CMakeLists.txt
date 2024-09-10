cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(snake)
# file(GLOB_RECURSE SRC_FILES src/*.cpp)

# find_package(SDL2 REQUIRED)
# find_package(SDL2_ttf REQUIRED)

# include_directories(snake ${SDL2_INCLUDE_DIR})
# include_directories(snake ${SDL2_TTF_INCLUDE_DIR})

add_executable(snake ./src/main.cpp ./src/Vector2.cpp)

target_link_libraries(snake SDL2 SDL2_ttf)
target_include_directories(snake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


# include(FetchContent)
# FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 871ed52d350214a034f6ef8a3b8f51c5ce1bd400) # The commit hash for 1.9.0. Replace with the latest from: https://github.com/libcpr/cpr/releases
# FetchContent_MakeAvailable(cpr)
