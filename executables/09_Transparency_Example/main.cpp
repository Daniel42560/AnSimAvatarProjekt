#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <CG1_Tools/CVK_ShaderSet.h>
#include <CG1_Tools/CompatibilityTools.h>

// Import Matrix Transformationen
#include <glm/gtc/matrix_transform.hpp>
// Fuer value_ptr
#include <glm/gtc/type_ptr.hpp>

// Import vector
#include <iostream>
#include <vector>

const int width = 800;
const int height = 800;

void run(GLFWwindow* window) {

    // Vertex Daten
    // Einzelne 2D Vektoren liegen direkt hintereinander
    // im Speicher und koennen so direkt
    // an OpenGL weitergegeben werden
    std::vector<glm::vec2> vertices = {
        glm::vec2(-1, -1), glm::vec2(1, -1), glm::vec2(1, 1),
        glm::vec2(-1, -1), glm::vec2(1, 1),  glm::vec2(-1, 1),

    };

    glClearColor(0, 0, 0, 1);

    // Lade Shader
    const char* shadernames[2] = {SHADERS_PATH "/09_Transparency/minimal.vert",
                                  SHADERS_PATH "/09_Transparency/minimal.frag"};
    cg1::ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT,
                                 shadernames);

    // Generiere VBO
    GLuint vertexBufferHandle;
    glGenBuffers(1, &vertexBufferHandle);

    // Lade Daten ins VBO
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    // Anzahl von Bytes ist = Anzahl von Elementen * Bytes pro Element
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2),
                 vertices.data(), GL_STATIC_DRAW);

    // Generiere Vertex Array Objekt
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);

    // Beschreibung der Vertex-Daten
    glBindVertexArray(vertexArrayHandle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Deaktiviere VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Aktiviere Shader
    shaderProgram.useProgram();

    // Aktivieren des Tiefentests sorgt dafür, dass Zeichenreihenfolge zu
    // Problemen führen kann, wenn die Reihenfolge anders ist. Dafür einfach die
    // Reihenfolge unten ändern
    // glEnable(GL_DEPTH_TEST);

    // Aktiviere Blending
    glEnable(GL_BLEND);

    // Verschiedene Blend Funktionen
    // Alpha Blending
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Additives Blending
    glBlendFunc(GL_ONE, GL_ONE);

    // Renderloop
    while (!glfwWindowShouldClose(window)) {

        // Macht Anzeige frei
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Uniform location
        GLint mID = glGetUniformLocation(shaderProgram.getProgramID(), "M");
        GLint cID = glGetUniformLocation(shaderProgram.getProgramID(), "color");

        // Zeichne von hinten nach vorne

        // Blaues Quadrat
        {
            // Transformationen
            // Lese Reihenfolge von unten nach oben
            glm::mat4 M(1.f);
            M = glm::translate(M, glm::vec3(-0.35f, -0.35f, 0.75f));
            M = glm::scale(M, glm::vec3(0.5f, 0.5f, 1.0f));

            // Sende Daten
            glUniformMatrix4fv(mID, 1, false, glm::value_ptr(M));
            glUniform4fv(cID, 1,
                         glm::value_ptr(glm::vec4(0.0f, 0.0f, 1.0f, 0.75f)));
            // Zeichnen
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }

        // Rotes Quadrat
        {

            // Transformationen
            // Lese Reihenfolge von unten nach oben
            glm::mat4 M(1.0f);
            M = glm::translate(M, glm::vec3(0.2f, 0.2f, 0.5f));
            M = glm::scale(M, glm::vec3(0.25f, 0.25f, 1.0f));

            // Sende Daten
            glUniformMatrix4fv(mID, 1, false, glm::value_ptr(M));
            glUniform4fv(cID, 1,
                         glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f)));
            // Zeichnen
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
        // Grünes Quadrat
        {

            // Transformationen
            // Lese Reihenfolge von unten nach oben
            glm::mat4 M(1.f);
            M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.25f));
            M = glm::scale(M, glm::vec3(0.25f, 0.25f, 1.0f));

            // Sende Daten
            glUniformMatrix4fv(mID, 1, false, glm::value_ptr(M));

            glUniform4fv(cID, 1,
                         glm::value_ptr(glm::vec4(0.0f, 1.0f, 0.0f, 0.25f)));
            // Zeichnen
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteVertexArrays(1, &vertexArrayHandle);
    glDeleteBuffers(1, &vertexBufferHandle);
}

int main() {

    // Initialisierung
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();

    GLFWwindow* window =
        glfwCreateWindow(width, height, "09_Transparency", nullptr, nullptr);
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
