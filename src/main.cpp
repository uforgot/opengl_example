#include "context.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height) {
    auto context = (Context*)glfwGetWindowUserPointer(window);
    context->Reshape(width, height);
}

void OnCursorPos(GLFWwindow* window, double x, double y) {
    auto context = (Context*)glfwGetWindowUserPointer(window);
    context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    auto context = (Context*)glfwGetWindowUserPointer(window);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

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

void OnCharEvent(GLFWwindow* window, unsigned int ch){
    ImGui_ImplGlfw_CharCallback(window, ch);
}

void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
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

    // Setup Dear ImGui context
    auto imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    auto context = Context::Create();
    if(!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowUserPointer(window, context.get());

    OnFramebufferSizeChanged(window, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);
    
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChanged);
    glfwSetMouseButtonCallback(window, OnMouseButton);
    glfwSetCursorPosCallback(window, OnCursorPos);
    glfwSetKeyCallback(window, OnKeyEvent);

    glfwSetScrollCallback(window, OnScroll);
    glfwSetCharCallback(window, OnCharEvent);



    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        context->ProcessInput(window);
        context->Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    context.reset();

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);

    glfwTerminate();
    
    return 0;
}