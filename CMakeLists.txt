cmake_minimum_required(VERSION 3.10)
project(Preprocess)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

find_package(VTK REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include ${VTK_USE_FILE} /usr/local/include)

link_directories(/usr/local/lib)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} metis ${VTK_LIBRARIES})
