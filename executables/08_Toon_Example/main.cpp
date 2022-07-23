#include <CG1_Tools/CVK_ShaderSet.h>
#include <CG1_Tools/CompatibilityTools.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Teapot.h"

#include <array>
#include <iostream>

using namespace glm;

const int width = 800;
const int height = 600;

// light
struct Light {
    Light(vec3 p) : position(p) {}
    vec3 position = {};
};
static Light light(vec3(-2.8, 8.2, 8.0f));

static bool mouseButtonLeftPressed = false;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)window;
    (void)mods;

    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
        mouseButtonLeftPressed = true;
    else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
        mouseButtonLeftPressed = false;
}

static double xpos_old = -1.0;
static double ypos_old = -2.0;
void mouseCursorCallback(GLFWwindow*, double xpos, double ypos) {

    if (mouseButtonLeftPressed) {
        double lightChangeX = (xpos - xpos_old) / 30.0;
        xpos_old = xpos;

        light.position.x += static_cast<float>(lightChangeX);

        double lightChangeY = (ypos - ypos_old) / 30.0;
        ypos_old = ypos;

        light.position.y -= static_cast<float>(lightChangeY);
    }
}

void run(GLFWwindow* window) {
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);

    // compile a shader program using CG1_Tools/CVK_ShaderSet.cpp
    const char* shadernames[2] = {SHADERS_PATH "/08_Toon_Example/toon.vert",
                                  SHADERS_PATH "/08_Toon_Example/toon.frag"};
    cg1::ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT,
                                 shadernames);

    // get the uniform locations
    GLint viewHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "view");
    GLint projectionHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "projection");
    GLint modelHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "model");
    GLint lightHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "lightPosition");
    GLint colorHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "color");

    // generate a vertex buffer object (VBO) for vertices, normals, indices
    std::array<GLuint, 3> vertexBufferHandles;
    glGenBuffers(3, vertexBufferHandles.data());

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * _points * sizeof(GL_FLOAT), &_v[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * _points * sizeof(GL_FLOAT), &_n[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferHandles[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices * sizeof(GL_UNSIGNED_INT),
                 &_index[0], GL_STATIC_DRAW);

    // generate a vertex array object (VAO)
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferHandles[2]);

    // use the shader program
    shaderProgram.useProgram();

    // setting up the camera parameters
    mat4 viewMatrix = lookAt(vec3(4.0, 5.0, 5.0f), vec3(0.0f, 0.0f, 0.0f),
                             vec3(0.0f, 1.0f, 0.0f));
    mat4 projectionMatrix = perspective(
        glm::radians(40.0f), static_cast<float>(width) / height, 0.1f, 50.0f);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseCursorCallback);

    // renderloop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // transfer uniform values from cpu to the gpu.
        glUniformMatrix4fv(viewHandle, 1, GL_FALSE, value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionHandle, 1, GL_FALSE,
                           value_ptr(projectionMatrix));
        glUniform3fv(lightHandle, 1, value_ptr(light.position));

        {
            glDisable(GL_DEPTH_TEST);
            vec3 color = vec3(0.0f, 0.0f, 0.0f);
            glUniform3fv(colorHandle, 1, value_ptr(color));
            mat4 modelMatrix = mat4(1.0f);
            modelMatrix = glm::scale(modelMatrix, vec3(1.075f));
            glUniformMatrix4fv(modelHandle, 1, GL_FALSE,
                               value_ptr(modelMatrix));

            // draw teapot
            glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_INT, nullptr);
        }

        {
            glEnable(GL_DEPTH_TEST);

            vec3 color = vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(colorHandle, 1, value_ptr(color));
            mat4 modelMatrix = mat4(1.0f);
            glUniformMatrix4fv(modelHandle, 1, GL_FALSE,
                               value_ptr(modelMatrix));

            // draw teapot
            glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_INT, nullptr);
        }

        // show what's been drawn
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteBuffers(3, vertexBufferHandles.data());
    glDeleteVertexArrays(1, &vertexArrayHandle);
    // shader will be cleaned up by its destructor
}

int main() {
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();
    GLFWwindow* window =
        glfwCreateWindow(width, height, "08_Toon_Example", nullptr, nullptr);
    glfwSetWindowPos(window, 600, 50);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }

    run(window);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
