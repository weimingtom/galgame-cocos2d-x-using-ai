cmake_minimum_required(VERSION 3.10)
project(GalgameEngineDemo)

set(APP_NAME GalgameDemo)

# ============================================
# Point COCOS2D_ROOT to your cocos2d-x install
# e.g. set(COCOS2D_ROOT "/path/to/cocos2d-x")
# ============================================
if(NOT DEFINED COCOS2D_ROOT)
    message(WARNING "COCOS2D_ROOT not set. Set it to your cocos2d-x directory.")
    set(COCOS2D_ROOT "${CMAKE_SOURCE_DIR}/../cocos2d")
endif()
set(COCOS2DX_ROOT_PATH ${COCOS2D_ROOT})
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #add_definitions(-DLINUX)
    include_directories(before /usr/include/GLFW)
    #link_directories(before ${COCOS2D_ROOT}/build/lib)
    #link_libraries(GLEW)
endif()

# --- Cocos2d-x setup ---
list(APPEND CMAKE_MODULE_PATH "${COCOS2D_ROOT}/cmake/Modules")
include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos 
	${ENGINE_BINARY_PATH}/cocos/core
)

set(GAME_SRC
    Classes/AppDelegate.cpp
    Classes/AppDelegate.h
    Classes/GameScene.cpp
    Classes/GameScene.h
    Classes/ScriptEngine.cpp
    Classes/ScriptEngine.h
    Classes/DialogueBox.cpp
    Classes/DialogueBox.h
    Classes/CharacterSprite.cpp
    Classes/CharacterSprite.h
    Classes/ChoiceMenu.cpp
    Classes/ChoiceMenu.h
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
list(APPEND GAME_SRC proj.linux/main.cpp)
endif()

set(GAME_HEADERS
    Classes/AppDelegate.h
    Classes/GameScene.h
    Classes/ScriptEngine.h
    Classes/DialogueBox.h
    Classes/CharacterSprite.h
    Classes/ChoiceMenu.h
)

# --- Define the app ---
add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEADERS})

# --- Cocos2d-x linking ---
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#GLU GLEW X11 
target_link_libraries(${APP_NAME} cocos2d GL jpeg)
else()
target_link_libraries(${APP_NAME} cocos2d)
endif()

# --- Include directories ---
target_include_directories(${APP_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Classes
    ${COCOS2D_ROOT}/cocos
    ${COCOS2D_ROOT}/cocos/base
    ${COCOS2D_ROOT}/cocos/2d
    ${COCOS2D_ROOT}/cocos/platform
    ${COCOS2D_ROOT}/cocos/audio/include
    ${COCOS2D_ROOT}/external
    ${COCOS2D_ROOT}/extensions
)

# --- Resources ---
add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources"
    "$<TARGET_FILE_DIR:${APP_NAME}>/Resources"
)

# --- Platform settings ---
if(WIN32)
    set_target_properties(${APP_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
elseif(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
    )
endif()
