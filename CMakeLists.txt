project(Babel CXX)
cmake_minimum_required(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${CMAKE_INCLUDE_PATH})

################ BABEL SERVER BINARY CONFIG ##################

# SOURCES FILES
file(GLOB_RECURSE BABEL_SERVER_SOURCES "src/server/*.cpp")

# HEADER FILES
file(GLOB_RECURSE BABEL_SERVER_HEADERS "inc/server/*.hpp")
set (BABEL_SERVER_INCLUDE_DIRS "")
foreach (_headerFile ${BABEL_SERVER_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND BABEL_SERVER_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES BABEL_SERVER_INCLUDE_DIRS)

# BINARY CONFIG
add_executable(babel_server ${BABEL_SERVER_SOURCES})
SET_TARGET_PROPERTIES(babel_server PROPERTIES COMPILE_FLAGS "-D BABEL_SERVER")

# LIB INCLUDE
target_include_directories(babel_server PRIVATE ${BABEL_SERVER_INCLUDE_DIRS})
target_link_libraries(babel_server ${CONAN_LIBS})


################ BABEL CLIENT BINARY CONFIG ##################

# SOURCES FILES
file(GLOB_RECURSE BABEL_CLIENT_SOURCES "src/client/*.cpp")

# HEADER FILES
file(GLOB_RECURSE BABEL_CLIENT_HEADERS "inc/client/*.hpp")
set (BABEL_CLIENT_INCLUDE_DIRS "")
foreach (_headerFile ${BABEL_CLIENT_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND BABEL_CLIENT_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES BABEL_CLIENT_INCLUDE_DIRS)

# BINARY CONFIG
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_executable(babel_client ${BABEL_CLIENT_SOURCES})
SET_TARGET_PROPERTIES(babel_client PROPERTIES COMPILE_FLAGS "-D BABEL_CLIENT")

# LIB INCLUDE
target_include_directories(babel_client PRIVATE ${BABEL_CLIENT_INCLUDE_DIRS})
target_link_libraries(babel_client ${CONAN_LIBS} Qt5::Widgets)
