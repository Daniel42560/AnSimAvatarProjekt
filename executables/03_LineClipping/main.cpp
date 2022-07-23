#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include <CG1_Tools/CompatibilityTools.h>
#include <CG1_Tools/ShaderTools.h>

#include <iostream>
#include <vector>

/*********** Declaration of global variables *********************/
// Note: For illustration purposes only, global variables aren't good style

// window size
const int width = 800;
const int height = 600;

// clip object
std::vector<glm::vec2> polygon;

// start and end of line
glm::vec2 a(-0.5, -0.5);
glm::vec2 b(0.5, 0.5);

// final line clipped by clip object
std::vector<glm::vec2> line;
// simple helper to allow not drawing the line, when its fully outside
bool lineVisible;

// state variables for the Cyrus-Beck algorithm
float tIn, tOut;

// buffer-IDs
GLuint vertexbuffer[2];

/*********** functions for Cyrus-Beck algorithm ****************/

// Iniitalize drawable line and activate visibility
void initLine() {
    line.clear(); // delete old values
    line.push_back(a);
    line.push_back(b);
    lineVisible = true;
}

// clip the line by the edge given by clipStart and clipEnd
bool clipEdge(const glm::vec2& clipStart, const glm::vec2& clipEnd) {

    glm::vec2 k = clipEnd - clipStart;

    // normal vector
    glm::vec2 n;
    n.x = k.y;
    n.y = -k.x;

    glm::vec2 w0 = a - clipStart;
    glm::vec2 w1 = b - clipStart;

    float c0 = glm::dot(w0, n);
    float c1 = glm::dot(w1, n);

    // cases fully in and fully out
    if (c0 <= 0 && c1 <= 0) {
        return true;
    } else if (c0 > 0 && c1 > 0) {
        return false;
    }

    // compute intersection and reuse values already computed
    glm::vec2 v = b - a;
    float denominator = glm::dot(v, n);
    float t = -c0 / denominator;

    // classify inside/outside by dot(n,v)
    // we need the smallest tOut value (so we use t < tOut as an additional
    // condition) and the largest tIn value (so we use t> tIn as an additional
    // condition)
    if (denominator > 0 && t < tOut) {
        tOut = t;
    } else if (denominator < 0 && t > tIn) {
        tIn = t;
    }

    return true;
}

// clip line by the complete polygon
void clipLine() {
    // initialize t values so they just represent the given line
    tIn = 0.0f;
    tOut = 1.0f;

    // go over all edges
    for (size_t i = 0; i < polygon.size(); ++i) {

        // use % to "flip" over the last point to the first
        lineVisible = clipEdge(polygon[i], polygon[(i + 1) % polygon.size()]);

        // stop if the line is fully outside or clipped
        if (!lineVisible || tIn > tOut) {
            lineVisible = false;
            return;
        }
    }

    // line was clipped -> compute final endpoints for drawing
    glm::vec2 v = b - a;

    line[0] = a + tIn * v;
    line[1] = a + tOut * v;
}

/*********** Mouse-Callback function *****************************************/

static void mouseCallback(GLFWwindow* window, int button, int action,
                          int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // get mouse coordinates
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // transform window in OpenGL coordinates
        xpos = (xpos / width) * 2.0 - 1.0;
        ypos = (ypos / height) * -2.0 + 1.0;

        // maximum number of endpoints reached?
        if (polygon.size() >= 4) {
            polygon.clear();
            initLine();
        }

        // add vertex
        polygon.push_back(glm::vec2(xpos, ypos));

        // clip line
        if (polygon.size() >= 3) {
            initLine();
            clipLine();
        }
    }
}

/*********** functions for drawing ******************************************/
static void drawLine() {
    // bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    // copy vertex data into currently active buffer
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec2), line.data(),
                 GL_STATIC_DRAW);
    // enable the first attribute
    glEnableVertexAttribArray(0);
    // specify how that attribute is stored in the buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // render buffer
    glDrawArrays(GL_LINE_LOOP, 0, 2);
}

static void drawPolygon() {
    // draw the polygon as a line strip
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, polygon.size() * sizeof(glm::vec2),
                 polygon.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINE_LOOP, 0, polygon.size());
}

void initBuffers() {
    // create two buffers for the clip polygon and the line
    glGenBuffers(2, &vertexbuffer[0]);

    GLuint vertexarray;
    glGenVertexArrays(1, &vertexarray);
    glBindVertexArray(vertexarray);
}

/*********** Main function with render loop
 * ****************************************/
int main() {

    // init
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();
    GLFWwindow* window =
        glfwCreateWindow(width, height, "03_LineClipping", nullptr, nullptr);
    glfwSetWindowPos(window, 600, 50);
    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    cg1::ShaderTools::createColorShaderProgram(1.0, 0.0, 0.0);

    // activate mouse callback for window
    glfwSetMouseButtonCallback(window, mouseCallback);

    initLine();
    initBuffers();

    // example polygon
    polygon.push_back(glm::vec2(-0.2, -0.2));
    polygon.push_back(glm::vec2(0.2, -0.2));
    polygon.push_back(glm::vec2(0.2, 0.2));
    polygon.push_back(glm::vec2(-0.2, 0.2));
    clipLine();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawPolygon();
        if (lineVisible)
            drawLine();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
