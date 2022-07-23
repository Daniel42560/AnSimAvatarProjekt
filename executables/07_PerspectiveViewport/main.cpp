#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/ext.hpp>
#include <string>

#include <CG1_Tools/CVK_ShaderSet.h>
#include <CG1_Tools/CompatibilityTools.h>

#include "Teapot.h"

#include <iostream>

using namespace glm;

const int width = 800;
const int height = 800;

int main() {
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();
    GLFWwindow* window = glfwCreateWindow(
        width, height, "07_Perspective_Viewport", nullptr, nullptr);
    glfwSetWindowPos(window, 600, 50);
    glfwMakeContextCurrent(window);

    // Initialisierung von glad
    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }
    glClearColor(1, 1, 1, 1);

    /// Shader kompillieren
    const char* shadernames[2] = {
        SHADERS_PATH "/07_PerspectiveViewport/depth.vert",
        SHADERS_PATH "/07_PerspectiveViewport/depth.frag"};
    cg1::ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT,
                                 shadernames);

    // Aktiviere Programm
    shaderProgram.useProgram();

    // Uniforms fuer Matrizen, near, far
    GLint projectionMatrixHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "projectionMatrix");
    GLint viewMatrixHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "viewMatrix");
    GLint modelMatrixHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "modelMatrix");
    GLint nearHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "near");
    GLint farHandle = glGetUniformLocation(shaderProgram.getProgramID(), "far");

    // VBO fuer Vertexdaten
    GLuint vertexBufferHandle;
    // Erzeuge Buffer und fuelle mit Datan
    glGenBuffers(1, &vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 3 * Teapot::numPoints * sizeof(GL_FLOAT),
                 &Teapot::data[0], GL_STATIC_DRAW);

    // Indexbuffer
    GLuint indexListHandle;
    glGenBuffers(1, &indexListHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexListHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 Teapot::numIndices * sizeof(GL_UNSIGNED_INT),
                 &Teapot::index[0], GL_STATIC_DRAW);

    // VAO
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);

    // Vertexattribute fuer Shader
    glBindVertexArray(vertexArrayHandle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexListHandle);

    // Kameraparameter
    float near = 0.1f;
    float far = 4.0f;

    // Matrizen
    mat4 modelMatrix(1.0f);
    mat4 viewMatrix(1.0f);
    mat4 projectionMatrix(1.0f);
    // Nicht gebraucht, kann direkt gesetzt werden
    glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE, value_ptr(modelMatrix));

    // Wird fuer die Einfaerbung im Shader benutzt
    glUniform1f(nearHandle, near);
    glUniform1f(farHandle, far);

    // Framebuffergroesse
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    // Zeichnet als Wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Renderloop
    while (!glfwWindowShouldClose(window)) {

        // Wichtig! Sonst keine Tiefentests
        glEnable(GL_DEPTH_TEST);

        // Wichtig! Tiefe muss auch geloescht werden
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Links unten
        glViewport(0, 0, w / 2, h / 2);
        viewMatrix = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f),
                            vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewMatrixHandle, 1, GL_FALSE,
                           value_ptr(viewMatrix));
        projectionMatrix = perspective(glm::radians(60.0f),
                                       float(width) / float(height), near, far);
        glUniformMatrix4fv(projectionMatrixHandle, 1, GL_FALSE,
                           value_ptr(projectionMatrix));

        glDrawElements(GL_TRIANGLES, Teapot::numIndices, GL_UNSIGNED_INT,
                       nullptr);

        // Rechts unten
        glViewport(w / 2, 0, w / 2, h / 2);
        viewMatrix = lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f),
                            vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewMatrixHandle, 1, GL_FALSE,
                           value_ptr(viewMatrix));
        projectionMatrix = perspective(glm::radians(80.0f),
                                       float(width) / float(height), near, far);
        glUniformMatrix4fv(projectionMatrixHandle, 1, GL_FALSE,
                           value_ptr(projectionMatrix));

        glDrawElements(GL_TRIANGLES, Teapot::numIndices, GL_UNSIGNED_INT,
                       nullptr);

        // Links oben
        glViewport(0, h / 2, w / 2, h / 2);
        viewMatrix = lookAt(vec3(0.0, 0.0, 1.25), vec3(0.0f, 0.0, 0.0f),
                            vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewMatrixHandle, 1, GL_FALSE,
                           value_ptr(viewMatrix));
        projectionMatrix = perspective(glm::radians(120.0f),
                                       float(width) / float(height), near, far);
        glUniformMatrix4fv(projectionMatrixHandle, 1, GL_FALSE,
                           value_ptr(projectionMatrix));

        // Rechts oben
        glDrawElements(GL_TRIANGLES, Teapot::numIndices, GL_UNSIGNED_INT,
                       nullptr);

        glViewport(w / 2, h / 2, w / 2, h / 2);
        viewMatrix = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f),
                            vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewMatrixHandle, 1, GL_FALSE,
                           value_ptr(viewMatrix));
        projectionMatrix = ortho(-2.0f, 2.0f, -2.0f, 2.0f, near, far);
        glUniformMatrix4fv(projectionMatrixHandle, 1, GL_FALSE,
                           value_ptr(projectionMatrix));

        glDrawElements(GL_TRIANGLES, Teapot::numIndices, GL_UNSIGNED_INT,
                       nullptr);

        // Tausche front und backbuffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vertexArrayHandle);
    glDeleteBuffers(1, &indexListHandle);
    glDeleteBuffers(1, &vertexBufferHandle);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
