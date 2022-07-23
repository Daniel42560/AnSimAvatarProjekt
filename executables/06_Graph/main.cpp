#include <CG1_Tools/CVK_ShaderSet.h>
#include <CG1_Tools/CompatibilityTools.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

const int width = 800;
const int height = 600;



int main() {
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();
    GLFWwindow* window =
        glfwCreateWindow(width, height, "06_ExampleGraph", nullptr, nullptr);
    glfwSetWindowPos(window, 600, 50);
    glfwMakeContextCurrent(window);


    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }
    glClearColor(1, 1, 1, 1);

    std::vector<GLfloat> vertices = {
        0.5f, -0.5f, 0.0f,  0.0,0.0,
        0.5f, -0.5f, 0.0f,  0.0,0.0f
        0.0f, 0.0f, 0.0f,    0.0,0.0
    };

    GLuint vertexBufferHandle;
    glGenBuffers(1, &vertexBufferHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);


    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // compile a shader program using CG1_Tools/CVK_ShaderSet.cpp
    const char* shadernames[2] = {SHADERS_PATH "/06_Graph/graph.vert",
                                  SHADERS_PATH "/06_Graph/graph.frag"};
    cg1::ShaderSet* shader = new cg1::ShaderSet(
        VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames);

    // use the shader program
    shader->useProgram();

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix =
        glm::lookAt(glm::vec3(0.5f, 2.0f, -3.0f), glm::vec3(0.5f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f), float(width) / float(height), 0.1f, 50.0f);

    GLint modelHandle = glGetUniformLocation(shader->getProgramID(), "model");
    GLint viewHandle = glGetUniformLocation(shader->getProgramID(), "view");
    GLint projectionHandle =
        glGetUniformLocation(shader->getProgramID(), "projection");

    glUniformMatrix4fv(modelHandle, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewHandle, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionHandle, 1, GL_FALSE,
                       glm::value_ptr(projectionMatrix));


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // renderloop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // show what's been drawn
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    delete shader;

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
