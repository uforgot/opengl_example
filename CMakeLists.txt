cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME opengl_example)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "TEST")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)
set(CMAKE_TLS_VERIFY TRUE)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/common.cpp src/common.h 
  src/shader.cpp src/shader.h
  src/program.cpp src/program.h
  src/context.cpp src/context.h
)

# MAC OS 일 경우 프레임워크 링크 추가
if(APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE
  "-framework Foundation"
  "-framework IOKit"
  "-framework CoreVideo" 
  "-framework OpenGL" 
  "-framework IOKit" 
  "-framework Cocoa" 
  "-framework Carbon"
  )
endif()

include(Dependency.cmake)

target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

add_dependencies(${PROJECT_NAME} ${DEP_LIST})