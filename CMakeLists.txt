cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(diso C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PKG_ARIA2 REQUIRED libaria2)
pkg_check_modules(PKG_LIBXML2 REQUIRED libxml-2.0)
pkg_check_modules(PKG_OpenSSL REQUIRED OpenSSL)
pkg_check_modules(PKG_LIBSSH2 REQUIRED libssh2)
pkg_check_modules(PKG_LIBCARES REQUIRED libcares)
pkg_check_modules(PKG_SQLITE3 REQUIRED sqlite3)

set(CMAKE_CXX_STANDARD 23)
add_executable(diso
    "src/main.cpp"
)
target_include_directories(diso PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party")
target_include_directories(diso PUBLIC 
    ${PKG_ARIA2_INCLUDE_DIRS}
    ${PKG_LIBXML2_INCLUDE_DIRS}
    ${PKG_OpenSSL_INCLUDE_DIRS}
    ${PKG_LIBSSH2_INCLUDE_DIRS}
    ${PKG_LIBCARES_INCLUDE_DIRS}
    ${PKG_SQLITE3_INCLUDE_DIRS}
)
target_link_directories(diso PUBLIC
    ${PKG_ARIA2_LIBRARY_DIRS}
    ${PKG_LIBXML2_LIBRARY_DIRS}
    ${PKG_OpenSSL_LIBRARY_DIRS}
    ${PKG_LIBSSH2_LIBRARY_DIRS}
    ${PKG_LIBCARES_LIBRARY_DIRS}
    ${PKG_SQLITE3_LIBRARY_DIRS}
)
target_link_libraries(diso
    ${PKG_ARIA2_LIBRARIES}
    ${PKG_LIBXML2_LIBRARIES}
    ${PKG_OpenSSL_LIBRARIES}
    ${PKG_LIBSSH2_LIBRARIES}
    ${PKG_LIBCARES_LIBRARIES}
    ${PKG_SQLITE3_LIBRARIES}
)
target_compile_options(diso PUBLIC
    ${PKG_ARIA2_CFLAGS_OTHER}
    ${PKG_LIBXML2_CFLAGS_OTHER}
    ${PKG_OpenSSL_CFLAGS_OTHER}
    ${PKG_LIBSSH2_CFLAGS_OTHER}
    ${PKG_LIBCARES_CFLAGS_OTHER}
    ${PKG_SQLITE3_CFLAGS_OTHER}
)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /wd4996 /wd4267 /wd4068")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    if (UNIX AND APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreFoundation -framework Security -lz")
    endif()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-sign-compare -Wno-unused-parameter -Wno-missing-field-initializers -Wno-deprecated-declarations")
endif()
