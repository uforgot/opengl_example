#include "context.h"
#include "image.h"


ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init()) {
        return nullptr;
    }
    return std::move(context);
}

bool Context::Init() {
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);

    m_vertexLayout = VertexLayout::Create();
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);
    
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t)*6);


    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader) {
        return false;
    }

    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({vertShader, fragShader});
    if (!m_program) {
        return false;
    }
    
    SPDLOG_INFO("program id: {}", m_program->Get());
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    // auto image = Image::Create(512,512);
    // image->SetCheckImage(16,16);
    auto image = Image::Load("./image/container.jpg");
    if (!image) return false;
    m_texture = Texture::CreateFromImage(image.get());

    auto image2 = Image::Load("./image/awesomeface.png");
    if (!image2) return false;
    m_texture2 = Texture::CreateFromImage(image2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 1);

    // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    // auto rot = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0,0.0,1.0));
    // auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
    // vec = trans * rot * scale * vec;
    // SPDLOG_INFO("vec: {}, {}, {}", vec.x, vec.y, vec.z);

    auto transform = glm::rotate( glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)), glm::radians(30.0f), glm::vec3(0.0,0.0,1.0));
    // auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.2f, 0.0f));
    auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);


    // static float time = 0.0f;
    // float t = sinf(time) * 0.5f + 0.5f;
    // auto loc = glGetUniformLocation(m_program->Get(), "color");
    m_program->Use();
    // glUniform4f(loc, t*t, 2.0f *t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // time += 0.016f;
}