cmake_minimum_required(VERSION 3.16...3.27)
project(GCRSimulation)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(GCRSimulation GCRSimulation.cc ${sources} ${headers})
target_link_libraries(GCRSimulation ${Geant4_LIBRARIES})

file(COPY ${PROJECT_SOURCE_DIR}/macs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
