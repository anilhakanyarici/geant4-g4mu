cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(g4mu)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB PWDheaders ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB PWDsources ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${sources} ${PWDsources} ${headers} ${PWDheaders})
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES})
