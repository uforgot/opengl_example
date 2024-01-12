# ExternalProject 관련 명령어 셋추가
include(ExternalProject)

#Dependency 관련 변수 설정
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

#spdlog: fast logger library
ExternalProject_Add(
    dep_spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.12.0"
    GIT_SHALLOW 1
    UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
)

set(DEP_LIST ${DEP_LIST} dep_spdlog)
# 왜인지 모르겠으나 맥에서는 안됨
# set(DEP_LIBS ${DEP_LIBS} spdlog$<$<CONFIG:Debug>:d>)
set(DEP_LIBS ${DEP_LIBS} spdlog)

#glfw : OpenGL window manager
ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.3.9"
    GIT_SHALLOW 1
    UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND ""
    CMAKE_ARGS 
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
)

set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)

#glad : OpenGL loader
ExternalProject_Add(
    dep_glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad.git"
    GIT_TAG "v0.1.34"
    GIT_SHALLOW 1
    UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND ""
    CMAKE_ARGS 
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DGLAD_INSTALL=ON
)

set(DEP_LIST ${DEP_LIST} dep_glad)
set(DEP_LIBS ${DEP_LIBS} glad)

#stb: image loader
ExternalProject_Add(
    dep_stb
    GIT_REPOSITORY "https://github.com/nothings/stb.git"
    GIT_SHALLOW 1
    UPDATE_COMMAND "" PATCH_COMMAND "" 
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/dep_stb-prefix/src/dep_stb/stb_image.h ${DEP_INSTALL_DIR}/include/stb/stb_image.h
)

set(DEP_LIST ${DEP_LIST} dep_stb)

#glm: OpenGL math library
ExternalProject_Add(
    dep_glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG "0.9.9.8"
    GIT_SHALLOW 1
    UPDATE_COMMAND "" PATCH_COMMAND "" CONFIGURE_COMMAND "" BUILD_COMMAND "" TEST_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_BINARY_DIR}/dep_glm-prefix/src/dep_glm/glm ${DEP_INSTALL_DIR}/include/glm
)

set(DEP_LIST ${DEP_LIST} dep_glm)

#imgui: OpenGL GUI library
add_library(imgui
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp 
    imgui/imgui.cpp 
    imgui/imgui_impl_glfw.cpp 
    imgui/imgui_impl_opengl3.cpp 
)

target_include_directories(imgui PRIVATE ${DEP_INCLUDE_DIR})
add_dependencies(imgui ${DEP_LIST})
set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(DEP_LIST ${DEP_LIST} imgui)
set(DEP_LIBS ${DEP_LIBS} imgui)