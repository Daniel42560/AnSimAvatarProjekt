#include <CVK_1_Minimal/CVK_Framework.h>

#include <iostream>

using namespace glm;

const int width = 800;
const int height = 600;

void render(GLFWwindow* window) {
    // allocate objects after rendering context initiation
    //***************************************************
    // Geometry
    //***************************************************

    //        auto geometry = std::make_shared<CVK::Sphere>(0.3f);
    //        float modelScale = 0.5f;
    //        float globalScale = 0.5f;

    auto geometry = std::make_shared<CVK::Teapot>();
    float modelScale = 0.25f;
    float globalScale = 0.5f;

    //        auto geometry = std::make_shared<CVK::Cone>(0.3f,0.1f,0.3f,20);
    //    float modelScale = 0.5f;
    //        float globalScale = 1.0f;
    //***************************************************

    // load, compile and link Shader
    const char* shadernames[2] = {SHADERS_PATH "/11_Hierarchy/phong.vert",
                                  SHADERS_PATH "/11_Hierarchy/phong.frag"};
    auto phongShader = std::make_shared<CVK::ShaderMinimal>(
        CVK::VERTEX_SHADER_BIT | CVK::FRAGMENT_SHADER_BIT, shadernames);
    GLint viewMatrixHandle =
        glGetUniformLocation(phongShader->getProgramID(), "viewMatrix");
    GLint projectionMatrixHandle =
        glGetUniformLocation(phongShader->getProgramID(), "projectionMatrix");
    GLint modelMatrixHandle =
        glGetUniformLocation(phongShader->getProgramID(), "modelMatrix");
    GLint lightPosHandle =
        glGetUniformLocation(phongShader->getProgramID(), "lightPosition");

    CVK::State::getInstance()->setShader(phongShader);
    phongShader->useProgram();
    mat4 viewMatrix = lookAt(vec3(0.0, 0.0, 5.0f), vec3(0.0f, 0.0, 0.0f),
                             vec3(0.0f, 1.0f, 0.0f));
    mat4 projectionMatrix = perspective(
        glm::radians(40.0f), static_cast<float>(width) / height, 0.01f, 50.0f);

    glUniformMatrix4fv(viewMatrixHandle, 1, GL_FALSE, value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixHandle, 1, GL_FALSE,
                       value_ptr(projectionMatrix));

    glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE,
                       value_ptr(glm::mat4(1.0f)));

    glUniform3fv(lightPosHandle, 1,
                 glm::value_ptr(glm::vec3(4.0f, 4.0f, 4.0f)));
    // OpenGL parameters
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    //---------------- Hierachy Example
    //-------------------------------------------------
    // Create root node in hierarchy (sun)

    // Set inital transformations
    auto sunNode = std::make_unique<CVK::Node>("Sun");
    sunNode->setGeometry(geometry);
    sunNode->setModelMatrix(
        glm::scale(glm::mat4(1.0f), glm::vec3(globalScale)));

    // Create node in relation to sunNode
    auto earthNode = std::make_unique<CVK::Node>("Earth");
    earthNode->setGeometry(geometry);

    earthNode->setModelMatrix(
        glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)),
                   glm::vec3(modelScale)));

    // Create node in relation to earthNode
    auto moonNode = std::make_unique<CVK::Node>("Moon");
    moonNode->setGeometry(geometry);

    moonNode->setModelMatrix(
        glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)),
                   glm::vec3(modelScale)));

    // add to children
    earthNode->addChild(std::move(moonNode));
    sunNode->addChild(std::move(earthNode));

    float startTime = static_cast<float>(glfwGetTime());
    float totalTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // delta time
        float deltaT = static_cast<float>(glfwGetTime()) - startTime;
        startTime = static_cast<float>(glfwGetTime());
        totalTime += deltaT;

        // rotate in relation to time

        sunNode->setModelMatrix(glm::rotate(sunNode->getModelMatrix(),
                                            glm::radians(30.0f) * deltaT,
                                            glm::vec3(0.0, 1.0, 0.0)));
        CVK::Node& earth = *sunNode->getChilds()[0];
        earth.setModelMatrix(glm::rotate(earth.getModelMatrix(),
                                         glm::radians(30.0f) * deltaT,
                                         glm::vec3(0.0, 1.0, 0.0)));

        CVK::Node& moon = *earth.getChilds()[0];
        moon.setModelMatrix(glm::rotate(moon.getModelMatrix(),
                                        glm::radians(90.0f) * deltaT,
                                        glm::vec3(1.0, 1.0, 1.0)));

        // render root node
        sunNode->render();
        //--------------------------------------------------------------------------------------//

        // render the same without node structure

        phongShader->useProgram();

        // sun
        glm::mat4 M_w_s(1.0f);
        M_w_s = glm::translate(M_w_s, glm::vec3(0, -1, 0));
        M_w_s = glm::rotate(M_w_s, glm::radians(30.0f) * totalTime,
                            glm::vec3(0.0f, 1.0f, 0.0f));
        M_w_s = glm::scale(M_w_s, glm::vec3(globalScale));
        glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE,
                           glm::value_ptr(M_w_s));
        geometry->render();

        // earth
        glm::mat4 M_s_e(1.0f);
        M_s_e = glm::scale(glm::translate(M_s_e, glm::vec3(2.0f, 0.0f, 0.0f)),
                           glm::vec3(modelScale));
        M_s_e = glm::rotate(M_s_e, glm::radians(30.0f) * totalTime,
                            glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 M_w_e = M_w_s * M_s_e;
        glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE,
                           glm::value_ptr(M_w_e));

        geometry->render();

        // moon
        glm::mat4 M_e_m(1.0f);
        M_e_m = glm::scale(glm::translate(M_e_m, glm::vec3(2.0f, 0.0f, 0.0f)),
                           glm::vec3(modelScale));
        M_e_m = glm::rotate(M_e_m, glm::radians(90.0f) * totalTime,
                            glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 M_w_m = M_w_s * M_s_e * M_e_m;
        glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE,
                           glm::value_ptr(M_w_m));

        geometry->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
int main() {
    // Init GLFW and glad
    glfwInit();
    CVK::useOpenGL33CoreProfile();
    GLFWwindow* window = glfwCreateWindow(
        width, height, "11_SunEarthMoon_Example", nullptr, nullptr);
    glfwSetWindowPos(window, 100, 50);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }

    render(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
