#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include <CG1_Tools/CVK_ShaderSet.h>
#include <CG1_Tools/CompatibilityTools.h>
#include <CG1_Tools/ShaderTools.h>

#include <iostream>
#include "Particles.h"

const int width = 600;
const int height = 600;

static GLFWwindow* window;
Particles particles;

int main() {
    // Init Window
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();

    window = glfwCreateWindow(width, height, "04_MovingFir", nullptr, nullptr);
    glfwSetWindowPos(window, 600, 50);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }
    glClearColor(1, 1, 1, 1);
    particles.createParticles();

   
    // generate a vertex buffer object (VBO)
    std::vector<GLfloat> vertices = {-0.4f, -0.4f, 0.0f,
                                      0.4f, -0.4f, 0.0f,  
                                      -0.4f, 0.0f, 0.0f,
                                      0.4f, 0.0f, 0.0f};

    GLuint vertexBufferHandle;
    glGenBuffers(1, &vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);
    //------------------------------------------------------------------------------------------------------//

    
    // generate a vertex array object (VAO)
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);    

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //generate a color buffer object
    std::vector<GLfloat> colors = {1.0f, 0.0f, 0.0f, 1.0f,
                                    0.0f, 1.0f, 0.0f, 1.0f, 
                                    0.0f, 0.0f, 1.0f, 1.0f,
                                    0.5f, 0.5f, 0.5f, 1.0f
                                    };

    GLuint colorBufferHandle;
    glGenBuffers(1, &colorBufferHandle);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat),
                 colors.data(), GL_STATIC_DRAW);

    

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    // specify the vertex data of the bound VBO and store it in the bound VAO

    // unbind the VBO, we don't need it anymore
    glBindBuffer(GL_ARRAY_BUFFER, 0);

     // compile a shader program using CG1_Tools/CVK_ShaderSet.cpp
    const char* shadernames[2] = {SHADERS_PATH "/04_Moving_Fir/fir.vert",
                                  SHADERS_PATH "/04_Moving_Fir/fir.frag"};
    cg1::ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT,
                                 shadernames);

    // use the shader program
    shaderProgram.useProgram();

    // Retrieve the locations of the uniform variables within the program. Once
    // you have that location you can set the value Note: Access to uniform
    // variables is available after linking the program
    GLint h = glGetUniformLocation(shaderProgram.getProgramID(), "height");
    GLint t = glGetUniformLocation(shaderProgram.getProgramID(), "time");
    GLint posOffset = glGetUniformLocation(shaderProgram.getProgramID(), "positionOffset");
    //------------------------------------------------------------------------------------------------------//

    // renderloop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        particles.render();

        // Set the value of the uniform variables
        glUniform1f(t, static_cast<float>(glfwGetTime()));
        glUniform4f(posOffset, 1.0f, 1.0f, 1.0f, 2.0f);

        // Draw three triangles with different heights
        glUniform1f(h, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 1, 3);
        //------------------------------------------------------------------------------------------------------//

        // show what's been drawn
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
