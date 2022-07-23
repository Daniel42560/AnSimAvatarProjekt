#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <CG1_Tools/CompatibilityTools.h>
#include <CG1_Tools/ShaderTools.h>

#include <iostream>

//------------------------------ Example Dragon Fractal
//--------------------------------------------------//
#include <glm/glm.hpp>
#include <vector>
//-----------------------------------------------------------------------------------------------------//

const int width = 800;
const int height = 600;

static GLFWwindow* window;

void put_pixel(int x, int y) {
    float p[2];

    p[0] = -1 + x * 2.0f / (width);
    p[1] = -1 + y * 2.0f / (height);
    glBufferData(GL_ARRAY_BUFFER, sizeof(p), &p, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_POINTS, 0, 1);
}

//------------------------------ Example Dragon Fractal
//--------------------------------------------------//
void interpolate(const glm::vec2& a, const glm::vec2& b, int num_points) {
    for (int i = 0; i < num_points; i++) {
        float t = float(i) / float(num_points - 1);
        glm::vec2 p = (1.0f - t) * a + t * b;
        put_pixel(p.x, p.y);
    }
}
//-----------------------------------------------------------------------------------------------------//
int main() {
    // Init Window
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();

    window =
        glfwCreateWindow(width, height, "02_Dragon_Fractal", nullptr, nullptr);
    glfwSetWindowPos(window, 600, 50);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    cg1::ShaderTools::createColorShaderProgram(1.0, 0.0, 0.0);

    GLuint vertexbuffer, vertexarray;

    glGenVertexArrays(1, &vertexarray);
    glBindVertexArray(vertexarray);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    //------------------------------ Example Dragon Fractal
    //--------------------------------------------------//
    std::vector<glm::vec2> lines; // points of the lines
    // dragon fractal order 0
    lines.push_back(glm::vec2(200, 300));
    lines.push_back(glm::vec2(300, 300));

    // define order of the dragon fractal
    int o = 10;
    for (int k = 0; k < o; k++) {
        std::vector<glm::vec2> l;
        l.push_back(lines[0]);

        for (size_t i = 0; i < lines.size() - 1; i++) {
            glm::vec2 a = lines[i];
            glm::vec2 b = lines[i + 1];

            glm::vec2 line = b - a;

            float halfLength = glm::length(line) / 2.0f;

            glm::vec2 middle = a + line * 0.5f;

            glm::vec2 n = glm::normalize(glm::vec2(a.y - b.y, b.x - a.x));

            if (i % 2 != 0) {
                n = -n;
            }

            glm::vec2 p = middle + halfLength * n;
            l.push_back(p);
            l.push_back(b);
        }

        lines = l;
    }

    //-----------------------------------------------------------------------------------------------------//

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        //------------------------------ Example Dragon Fractal
        //--------------------------------------------------//
        for (unsigned int i = 0; i < lines.size() - 1; i++) {
            interpolate(lines.at(i), lines.at(i + 1), 100);
        }
        //-----------------------------------------------------------------------------------------------------//
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
