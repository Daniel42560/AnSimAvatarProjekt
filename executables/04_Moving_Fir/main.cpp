#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include <CG1_Tools/CVK_ShaderSet.h>
#include <CG1_Tools/CompatibilityTools.h>
#include <CG1_Tools/ShaderTools.h>
#include <vector>
#include <iostream>
#include "Particles.h"

const int width = 1000;
const int height = 800;
float dt = 0.005f;

const int numberOfParticles = 1000;
//const float normalizer = 2.0f / numberOfParticles;

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
    particles.createParticles(numberOfParticles);
   
     // compile a shader program using CG1_Tools/CVK_ShaderSet.cpp
    const char* shadernames[2] = {SHADERS_PATH "/04_Moving_Fir/fir.vert",
                                  SHADERS_PATH "/04_Moving_Fir/fir.frag"};
    cg1::ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT,
                                 shadernames);

    // use the shader program
    shaderProgram.useProgram();

    // Uniforms --------------------------------------------------------------------------------------------

    GLint t = glGetUniformLocation(shaderProgram.getProgramID(), "time");
    
    //------------------------------------------------------------------------------------------------------//
     

    // renderloop
    while (!glfwWindowShouldClose(window)) {
       
        
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        
        glUniform1f(t, static_cast<float>(glfwGetTime()));
        particles.render();

        // Set the value of the uniform variables

        //glDrawArrays(GL_POINTS,0, numberOfParticles);
        

        // show what's been drawn
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
