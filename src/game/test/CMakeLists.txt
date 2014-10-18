#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(myproject)

# directory of the project
set(DEV_ROOT_PATH ../../)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -DDEBUG" )

################################################################################
# includes dirs
include_directories(${DEV_ROOT_PATH})


# headers
set(HDRS
    ${DEV_ROOT_PATH}/game/AnimHandler.h
    ${DEV_ROOT_PATH}/game/Animation.h
    ${DEV_ROOT_PATH}/game/GameHandler.h
    ${DEV_ROOT_PATH}/game/Submarine.h
    ${DEV_ROOT_PATH}/game/GameEntity.h
    ${DEV_ROOT_PATH}/game/ResourceManager.h
)
    
# sources
set(SRCS
    ${DEV_ROOT_PATH}/game/test/test.cpp
    ${DEV_ROOT_PATH}/game/AnimHandler.cpp
    ${DEV_ROOT_PATH}/game/GameHandler.cpp
    ${DEV_ROOT_PATH}/game/Submarine.cpp
    ${DEV_ROOT_PATH}/game/GameEntity.cpp
    ${DEV_ROOT_PATH}/game/ResourceManager.cpp
)



# Define sources and executable
set(EXECUTABLE_NAME "sfmltest")
add_executable(${EXECUTABLE_NAME} ${HDRS} ${SRCS} test.cpp)


# Detect and add SFML
set(SFML_LIBRARIES
    sfml-graphics sfml-window sfml-system
)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


