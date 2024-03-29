#include "context.h"

void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("Framebuffer size changed: {} x {}", width, height);
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    SPDLOG_INFO("Key: {}, scancode: {}, action: {} mods: {}{}{}", 
        key, scancode, 
        action == GLFW_PRESS ? "Pressed" : 
        action == GLFW_RELEASE ? "Released" : 
        action == GLFW_REPEAT ? "Repeat" : "Unknown", 
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-"
        );

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
}

int main(int argc, const char** argv) {
    SPDLOG_INFO("Start program");

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
    std::cout << "--> GL Version: " << glVersion << std::endl;        

    auto context = Context::Create();
    if(!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    OnFramebufferSizeChanged(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChanged);
    glfwSetKeyCallback(window, OnKeyEvent);

    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        context->Render();
        glfwSwapBuffers(window);
    }

    context.reset();
    glfwTerminate();
    
    return 0;
}