#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(submarines)

# directory of the project
set(DEV_ROOT_PATH ./)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -DDEBUG -Wall ")

################################################################################
# includes dirs
include_directories(${DEV_ROOT_PATH})

# define modules specific files

################################################################################
## Game

set(GAME_HDRS
    ${DEV_ROOT_PATH}/game/anim/AnimHandler.h
    ${DEV_ROOT_PATH}/game/anim/Animation.h
    ${DEV_ROOT_PATH}/game/scene/SceneManager.h
    ${DEV_ROOT_PATH}/game/scene/SceneObject.h
    ${DEV_ROOT_PATH}/game/scene/AnimatedSceneObject.h
    ${DEV_ROOT_PATH}/game/effects/Effect.h
    ${DEV_ROOT_PATH}/game/player/Submarine.h
    ${DEV_ROOT_PATH}/game/enemies/EnemyUnit.h
    ${DEV_ROOT_PATH}/game/enemies/FishEnemyUnit.h
    ${DEV_ROOT_PATH}/game/resources/ResourceManager.h
    ${DEV_ROOT_PATH}/game/elements/ElementObject.h
    ${DEV_ROOT_PATH}/game/elements/BombElement.h
)
    
# sources
set(GAME_SRCS
    ${DEV_ROOT_PATH}/game/test/test.cpp
    ${DEV_ROOT_PATH}/game/anim/AnimHandler.cpp
    ${DEV_ROOT_PATH}/game/scene/SceneManager.cpp
    ${DEV_ROOT_PATH}/game/scene/SceneObject.cpp
    ${DEV_ROOT_PATH}/game/scene/AnimatedSceneObject.cpp
    ${DEV_ROOT_PATH}/game/effects/Effect.cpp
    ${DEV_ROOT_PATH}/game/player/Submarine.cpp
    ${DEV_ROOT_PATH}/game/enemies/EnemyUnit.cpp
    ${DEV_ROOT_PATH}/game/enemies/FishEnemyUnit.cpp
    ${DEV_ROOT_PATH}/game/resources/ResourceManager.cpp
    ${DEV_ROOT_PATH}/game/elements/ElementObject.cpp
    ${DEV_ROOT_PATH}/game/elements/BombElement.cpp
)

################################################################################
## OpenCV

set(IMGP_HDRS
    ${DEV_ROOT_PATH}/img_configurer/Configurer.h
    ${DEV_ROOT_PATH}/img_reader/ImageReader.h
    ${DEV_ROOT_PATH}/img_analyzer/ImageAnalyzer.h
)

set(IMGP_SRCS
    ${DEV_ROOT_PATH}/img_configurer/Configurer.cpp
    ${DEV_ROOT_PATH}/img_reader/ImageReader.cpp
    ${DEV_ROOT_PATH}/img_analyzer/ImageAnalyzer.cpp
    ${DEV_ROOT_PATH}/opencv_test/test.cpp
)



# headers
set(HDRS
#    ${GAME_HDRS}
    ${IMGP_HDRS}
)
    
# sources
set(SRCS
#   ${GAME_SRCS}
    ${IMGP_SRCS}
)



# Define sources and executable
set(EXECUTABLE_NAME "submarines")
add_executable(${EXECUTABLE_NAME} ${HDRS} ${SRCS})


# Detect and add SFML
set(SFML_LIBRARIES
    sfml-graphics sfml-window sfml-system
)

# detect opencv
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${OpenCV_LIBS})


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


