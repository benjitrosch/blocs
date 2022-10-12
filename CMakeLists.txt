project(blocs)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    src/*.cpp
    include/blocs/*.h
    extensions/*.h
)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if (EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-s USE_SDL=2")
else()
    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    set(FETCHCONTENT_UPDATES_DISCONNECTED_SDL2 ON)

    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL
        GIT_TAG         release-2.26.0
        GIT_PROGRESS    TRUE
    )
    FetchContent_MakeAvailable(SDL2)

    set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2-static)
endif()

set(STB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb)
add_library(stb ${STB_DIR}/src/stb.c)
target_include_directories(stb
    PUBLIC
    ${SDL2_SOURCE_DIR}/include
    PRIVATE
    ${STB_DIR}/include
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    include
    PUBLIC
    ${STB_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${SDL_LIBRARIES}
    PUBLIC
    stb
)
