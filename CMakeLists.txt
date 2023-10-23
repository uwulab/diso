cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(diso C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PKG_ARIA2 REQUIRED libaria2)


set(CMAKE_CXX_STANDARD 23)
add_executable(diso
    "src/main.cpp"
)
target_include_directories(diso PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party")
target_include_directories(diso PUBLIC ${PKG_ARIA2_INCLUDE_DIRS})
target_link_directories(diso PUBLIC ${PKG_ARIA2_LIBRARY_DIRS})
target_link_libraries(diso ${PKG_ARIA2_LIBRARIES})
target_compile_options(diso PUBLIC ${PKG_ARIA2_CFLAGS_OTHER})

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /wd4996 /wd4267 /wd4068")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-sign-compare -Wno-unused-parameter -Wno-missing-field-initializers -Wno-deprecated-declarations")
endif()
