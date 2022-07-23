#include <CG1_Tools/TextureTools.h>
#include <CVK_1_Minimal/CVK_Framework.h>

#include <array>
#include <iostream>
#include <memory>

const int width = 800;
const int height = 800;

void charCallback(GLFWwindow* window, unsigned int key) {
    (void)window;
    if (key == 'W')
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (key == 'w')
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

std::unique_ptr<CVK::Node> init_scene() {
    std::unique_ptr<CVK::Node> scene_node =
        std::make_unique<CVK::Node>("Scene");

    auto geometry = std::make_shared<CVK::Cube>();
    // auto geometry = std::make_shared<CVK::Plane>();
    //    auto geometry = std::make_shared<CVK::Teapot>();
    auto node = std::make_unique<CVK::Node>("Node");
    node->setModelMatrix(
        glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.72, 0)));
    node->setGeometry(geometry);
    scene_node->addChild(std::move(node));

    return scene_node;
}
std::array<GLuint, 2> loadTextures() {
    // load a fancy texture
    return {cg1::TextureTools::loadTexture(RESOURCES_PATH "/cube.png"),
            cg1::TextureTools::loadTexture(RESOURCES_PATH "/circle.png")};
}

void run(GLFWwindow* window) {
    CVK::State::getInstance()->setBackgroundColor(CVK::white);
    glm::vec3 BgCol = CVK::State::getInstance()->getBackgroundColor();
    glClearColor(BgCol.r, BgCol.g, BgCol.b, 0.0);
    glEnable(GL_DEPTH_TEST);

    // load, compile and link Shader
    const char* shadernames[2] = {SHADERS_PATH "/09_TexturedCube/cube.vert",
                                  SHADERS_PATH "/09_TexturedCube/cube.frag"};
    auto minimalShader = std::make_shared<CVK::ShaderMinimal>(
        CVK::VERTEX_SHADER_BIT | CVK::FRAGMENT_SHADER_BIT, shadernames);
    CVK::State::getInstance()->setShader(minimalShader);

    // init camera and scene
    auto projection = std::make_shared<CVK::Perspective>(
        60.0f, static_cast<float>(width) / height, 0.1f, 100.f);
    // CVK::Ortho projection(-5, 5, -5, 5, -5, 50);
    auto cam = std::make_shared<CVK::Trackball>(width, height, projection);
    cam->setRadius(5);
    CVK::State::getInstance()->setCamera(cam);

    GLint textureID0 =
        glGetUniformLocation(minimalShader->getProgramID(), "tex0");
    GLint textureID1 =
        glGetUniformLocation(minimalShader->getProgramID(), "tex1");

    std::unique_ptr<CVK::Node> scene_node = init_scene();
    std::array<GLuint, 2> textureHandles = loadTextures();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Update Camera
        cam->update(window);

        // set our "tex" sampler to texture unit 0
        glUniform1i(textureID0, 0);
        glUniform1i(textureID1, 1);
        // set texture unit 0 active
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureHandles[0]);

        // repeat texture coordinates
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // clamp uv to the outermost edge but still inside
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // clamp to border and set border color
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        // GL_CLAMP_TO_BORDER); glTexParameteri(GL_TEXTURE_2D,
        // GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // glm::vec4 borderColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
        //                  glm::value_ptr(borderColor));

        // set the magnification and minifying filter
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureHandles[1]);

        // Use Shader and define camera uniforms
        minimalShader->update();

        scene_node->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    // other GL objects that are wrapped in C++ classes automatically get
    // deleted
    glDeleteTextures(textureHandles.size(), textureHandles.data());
}
int main() {
    // Init GLFW and GLEW
    glfwInit();
    CVK::useOpenGL33CoreProfile();
    GLFWwindow* window =
        glfwCreateWindow(width, height, "09_Texture_Example", nullptr, nullptr);
    glfwSetWindowPos(window, 100, 50);
    glfwMakeContextCurrent(window);
    glfwSetCharCallback(window, charCallback);

    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }

    run(window);

    CVK::State::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
