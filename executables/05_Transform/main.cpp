#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>

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

GLFWwindow* window;

// Vertex Daten
// Einzelne 2D Vektoren liegen direkt hintereinander
// im Speicher und koennen so direkt
// an OpenGL weitergegeben werden
std::vector<glm::vec2> vertices = {
    glm::vec2(0, 7) / 14.0f,   glm::vec2(0, 4) / 14.0f,
    glm::vec2(2, 5) / 14.0f,   glm::vec2(0, 4) / 14.0f,
    glm::vec2(2, 2) / 14.0f,   glm::vec2(2, 5) / 14.0f,
    glm::vec2(2, 2) / 14.0f,   glm::vec2(3, 2) / 14.0f,
    glm::vec2(2, 5) / 14.0f,   glm::vec2(3, 2) / 14.0f,
    glm::vec2(5, 2) / 14.0f,   glm::vec2(2, 5) / 14.0f,
    glm::vec2(3, 2) / 14.0f,   glm::vec2(4, 1) / 14.0f,
    glm::vec2(5, 2) / 14.0f,   glm::vec2(3, 2) / 14.0f,
    glm::vec2(3, 0) / 14.0f,   glm::vec2(4, 1) / 14.0f,
    glm::vec2(3, 0) / 14.0f,   glm::vec2(5, 0) / 14.0f,
    glm::vec2(4, 1) / 14.0f,   glm::vec2(5, 0) / 14.0f,
    glm::vec2(5, 1) / 14.0f,   glm::vec2(4, 1) / 14.0f,
    glm::vec2(5, 2) / 14.0f,   glm::vec2(7, 3) / 14.0f,
    glm::vec2(2, 5) / 14.0f,   glm::vec2(5, 2) / 14.0f,
    glm::vec2(7, 1) / 14.0f,   glm::vec2(7, 3) / 14.0f,
    glm::vec2(5, 2) / 14.0f,   glm::vec2(6, 0) / 14.0f,
    glm::vec2(7, 1) / 14.0f,   glm::vec2(6, 0) / 14.0f,
    glm::vec2(8, 0) / 14.0f,   glm::vec2(7, 1) / 14.0f,
    glm::vec2(7, 1) / 14.0f,   glm::vec2(8, 0) / 14.0f,
    glm::vec2(8, 1) / 14.0f,   glm::vec2(5, 2) / 14.0f,
    glm::vec2(7, 3) / 14.0f,   glm::vec2(2, 5) / 14.0f,
    glm::vec2(7, 3) / 14.0f,   glm::vec2(6, 10) / 14.0f,
    glm::vec2(2, 5) / 14.0f,   glm::vec2(7, 3) / 14.0f,
    glm::vec2(8, 5) / 14.0f,   glm::vec2(6, 10) / 14.0f,
    glm::vec2(8, 5) / 14.0f,   glm::vec2(8, 6) / 14.0f,
    glm::vec2(6, 10) / 14.0f,  glm::vec2(8, 6) / 14.0f,
    glm::vec2(8, 7) / 14.0f,   glm::vec2(6, 10) / 14.0f,
    glm::vec2(8, 7) / 14.0f,   glm::vec2(8, 8) / 14.0f,
    glm::vec2(6, 10) / 14.0f, // 18
    glm::vec2(8, 6) / 14.0f,   glm::vec2(9, 7) / 14.0f,
    glm::vec2(8, 7) / 14.0f,   glm::vec2(8, 6) / 14.0f,
    glm::vec2(9, 6) / 14.0f,   glm::vec2(9, 7) / 14.0f,
    glm::vec2(8, 8) / 14.0f,   glm::vec2(9, 9) / 14.0f,
    glm::vec2(6, 10) / 14.0f,  glm::vec2(8, 8) / 14.0f,
    glm::vec2(11, 8) / 14.0f,  glm::vec2(9, 9) / 14.0f,
    glm::vec2(9, 9) / 14.0f,   glm::vec2(11, 8) / 14.0f,
    glm::vec2(11, 9) / 14.0f, // 25
    glm::vec2(9, 9) / 14.0f,   glm::vec2(9, 10) / 14.0f,
    glm::vec2(6, 10) / 14.0f,  glm::vec2(9, 10) / 14.0f,
    glm::vec2(6, 12) / 14.0f,  glm::vec2(6, 10) / 14.0f,
    glm::vec2(6, 12) / 14.0f,  glm::vec2(9, 10) / 14.0f,
    glm::vec2(7, 14) / 14.0f,  glm::vec2(9, 10) / 14.0f,
    glm::vec2(11, 14) / 14.0f, glm::vec2(7, 14) / 14.0f,
    glm::vec2(9, 10) / 14.0f,  glm::vec2(13, 13) / 14.0f,
    glm::vec2(11, 14) / 14.0f, glm::vec2(9, 10) / 14.0f,
    glm::vec2(13, 10) / 14.0f, glm::vec2(13, 13) / 14.0f,
};

// Gibt den Mittelpunkt des Objekts an
glm::vec2 center(0.0f);

// Berechnet den Mittelpunkt
void computeCenter() {
    // Mittelpunkt ist Mittelwert aller Punkte
    // Addiere alle Punkte auf und teile durch Anzahl
    glm::vec2 sum(0.0f, 0.0f);

    // Iteriere ueber Punkte
    for (size_t i = 0; i < vertices.size(); i++) {
        sum += vertices[i];
    }

    // Teile durch Anzahl
    // Mache Anzahl zu float
    center = sum / static_cast<float>(vertices.size());
}

int main() {

    // Initialisierung
    glfwInit();
    cg1::CompatibilityTools::useOpenGL33CoreProfile();
    window = glfwCreateWindow(width, height, "05_Transform", nullptr, nullptr);
    glfwSetWindowPos(window, 600, 50);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }
    glClearColor(0, 0, 0, 1);

    // Lade Shader
    const char* shadernames[2] = {SHADERS_PATH "/05_Transform/minimal.vert",
                                  SHADERS_PATH "/05_Transform/minimal.frag"};
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

    // Berechne Zentrum fuer Transformationen
    computeCenter();

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

    // Zeitvariable
    float t = 0.0f;
    float lastTime = static_cast<float>(glfwGetTime());
    // Renderloop
    while (!glfwWindowShouldClose(window)) {

        float currentTime = static_cast<float>(glfwGetTime());
        t += (currentTime - lastTime);
        lastTime = currentTime;
        // Macht Anzeige frei
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Uniform location
        GLint mID = glGetUniformLocation(shaderProgram.getProgramID(), "M");

        // Transformationen
        // Lese Reihenfolge von unten nach oben

        // Drehe um Achse
        glm::mat4 M(1.0f);

        M = glm::translate(M, glm::vec3(center, 0.0f));
        M = glm::rotate(M, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        M = glm::scale(M, glm::vec3(1.0, 0.2, 1.));
        M = glm::translate(M, glm::vec3(-center, 0.0f));

        // Alternative Schreibweise
        //		{
        //			glm::mat4 T1 = glm::translate(glm::mat4(1.0f),
        // glm::vec3(-center, 0.0f)); 			glm::mat4 S =
        // glm::scale(glm::mat4(1.0f),
        // glm::vec3(1.0, 0.2, 1.)); 			glm::mat4 R=
        // glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f,
        // 0.0f, 1.0f)); 			glm::mat4 T2 =
        // glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));
        //
        //			// Reihenfolge von Links nach rechts entspricht
        // Reihenfolge von Unten nach oben in Code 			M =
        // T2*R*S*T1;
        //		}

        // Sende Daten
        glUniformMatrix4fv(mID, 1, false, glm::value_ptr(M));

        // Zeichnen
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // Neue Transformation

        M = glm::mat4(1.0f);

        M = glm::translate(M, glm::vec3(-0.5, -0.5, 0.0f));
        M = glm::rotate(M, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        M = glm::translate(M, glm::vec3(-center, 0.0f));

        // Sende Daten
        glUniformMatrix4fv(mID, 1, false, glm::value_ptr(M));

        // Zeichnen
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // Neue Transformation
        M = glm::mat4(1.0f);

        M = glm::translate(M, glm::vec3(-0.5, 0.5, 0.0f));
        M = glm::rotate(M, t * glm::pi<float>() * 2.0f / 10.0f,
                        glm::vec3(1.0f, 0.0f, 0.0f));
        M = glm::scale(M, glm::vec3(0.5, 0.5, 0.5));
        M = glm::translate(M, glm::vec3(-center, 0.0f));

        // Sende Daten
        glUniformMatrix4fv(mID, 1, false, glm::value_ptr(M));

        // Zeichnen
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Zaehle Zeit mit festem Wert hoch.
        // Damit laeuft es je nach Rechner mit unterschiedlicher
        // Geschwindigkeit, je nachdem wieviele Bilder pro Sekunde gezeichnet
        // werden koennen. Fuer eine gleiche Bewegung ueberall muesste noch die
        // vergangene Zeit beruecksichtigt werden
    }

    // clean up
    glDeleteVertexArrays(1, &vertexArrayHandle);
    glDeleteBuffers(1, &vertexBufferHandle);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
