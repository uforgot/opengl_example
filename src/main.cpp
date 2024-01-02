#include <iostream>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("Framebuffer size changed: {}x{}", width, height);
    glViewport(0, 0, width, height);
}

int main(int argc, const char** argv) {
    SPDLOG_INFO("Start program");
    
    // #ifdef __APPLE__
    // /* We need to explicitly ask for a 3.2 context on OS X */
    // glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    // glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // #endif 

    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    #ifdef __APPLE__
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }


    auto glVersion = glGetString(GL_VERSION);
    //SPDLOG_INFO("GL Version: {}", glVersion); < 이상하게 fmt error가 난다.
    std::cout << "GL Version: " << glVersion << std::endl;        

    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChanged);
    

    // SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}