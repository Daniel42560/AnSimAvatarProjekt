#include <CG1_Tools/CVK_ShaderSet.h>
#include <CG1_Tools/CompatibilityTools.h>
#include <CG1_Tools/TextureTools.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>
#include <iostream>

const int width = 800;
const int height = 600;

// 3d bezier points

// bottom row
static glm::vec3 pbl = glm::vec3(-1, 1, -1);
static glm::vec3 pbm = glm::vec3(0, 0, -1);
static glm::vec3 pbr = glm::vec3(1, 0, -1);

// mid row
static glm::vec3 pml = glm::vec3(-1, 0.5, 0);
static glm::vec3 pmm = glm::vec3(0, 1, 0);
static glm::vec3 pmr = glm::vec3(1, 0.5, 0);

// top row
static glm::vec3 ptl = glm::vec3(-1, 0, 1);
static glm::vec3 ptm = glm::vec3(0, -1, 1);
static glm::vec3 ptr = glm::vec3(1, 0, 1);

glm::vec3 bezierQuad(const glm::vec3& p0, const glm::vec3& p1,
                     const glm::vec3& p2, float t) {
    float it = 1.0f - t;

    return it * it * p0 + 2.0f * it * t * p1 + t * t * p2;
}

glm::vec3 surfacePoint(float u, float v) {
    glm::vec3 pb = bezierQuad(pbl, pbm, pbr, u);
    glm::vec3 pm = bezierQuad(pml, pmm, pmr, u);
    glm::vec3 pt = bezierQuad(ptl, ptm, ptr, u);

    return bezierQuad(pb, pm, pt, v);
}

void run(GLFWwindow* window) {
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    const int resolution = 600;

    int resm = resolution - 1;
    // placeholder

    for (int u = 0; u < resm; u++) {
        for (int v = 0; v < resm; v++) {

            float xt = static_cast<float>(u) / static_cast<float>(resm);
            float zt = static_cast<float>(v) / static_cast<float>(resm);

            glm::vec3 p = surfacePoint(xt, zt);
            vertices.push_back(p);

            glm::vec3 uNext = surfacePoint(
                static_cast<float>(u + 1) / static_cast<float>(resm), zt);
            glm::vec3 vNext = surfacePoint(xt, static_cast<float>(v + 1) /
                                                   static_cast<float>(resm));

            glm::vec3 pu = uNext - p;
            glm::vec3 pv = vNext - p;

            glm::vec3 n = glm::normalize(glm::cross(pv, pu));

            normals.push_back(n);
            uvs.push_back(glm::vec2(xt, zt));
        }
    }

    glPointSize(2.0f);

    // generate vertex array object (vao)
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);

    // generate vertex buffer object (vbo) for vertices
    std::array<GLuint, 3> vertexBufferHandles;
    glGenBuffers(vertexBufferHandles.size(), vertexBufferHandles.data());

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandles[2]);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // shader program using CG1_Tools/CVK_ShaderSet.cpp
    const char* shadernames[2] = {SHADERS_PATH "/10_BezierSurface/patch.vert",
                                  SHADERS_PATH "/10_BezierSurface/patch.frag"};
    cg1::ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT,
                                 shadernames);

    // use shader
    shaderProgram.useProgram();

    // uniform locations
    GLint viewHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "view");
    GLint projectionHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "projection");
    GLint modelHandle =
        glGetUniformLocation(shaderProgram.getProgramID(), "model");
    GLint texHandle = glGetUniformLocation(shaderProgram.getProgramID(), "tex");

    // camera parameters
    glm::mat4 viewMatrix =
        glm::lookAt(glm::vec3(3.0f, 2.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(40.0f), static_cast<float>(width) / height, 0.1f, 50.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // transfer uniforms to GPU
    glUniformMatrix4fv(viewHandle, 1, GL_FALSE, value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionHandle, 1, GL_FALSE,
                       value_ptr(projectionMatrix));
    glUniformMatrix4fv(modelHandle, 1, GL_FALSE, value_ptr(modelMatrix));

    GLuint texID =
        cg1::TextureTools::loadTexture(RESOURCES_PATH "/checkerboard.png");

    // renderloop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rotate the surface around with time
        modelMatrix = glm::rotate(
            modelMatrix,
            glm::radians(glm::cos(static_cast<float>(glfwGetTime()))),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelHandle, 1, GL_FALSE, value_ptr(modelMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        glUniform1i(texHandle, 0);
        // draw
        glDrawArrays(GL_POINTS, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteVertexArrays(1, &vertexArrayHandle);
    glDeleteBuffers(vertexBufferHandles.size(), vertexBufferHandles.data());
}
int main() {
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();
    GLFWwindow* window = glfwCreateWindow(
        width, height, "10_BezierSurface_Example", nullptr, nullptr);
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