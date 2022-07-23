#include "Particles.h"

void Particles::createParticles(GLuint numberOfParticles)
{

    // Buffer = selection of Memory
    // ArrayBuffer = selection of Memory used for a list / array 


    NumberOfParticlesIn = numberOfParticles;
    std::vector<GLfloat> ParticlePositionData;

    GLfloat normalizer = 2.0F / NumberOfParticlesIn;            // sicher gehen dass jedes Partikel auch im Fenster ist (Nummer der Partikel muss auf -1 bis +1 abgebildet werden)
    /*
    for(size_t i = 0; i < NumberOfParticlesIn; i++)
    {
        ParticlePositionData.push_back(-1.0f + i * Minimiser); // für die x Position
        ParticlePositionData.push_back(sin(i  * Minimiser));   // für die y Position
        ParticlePositionData.push_back(0.0f);                  // für die z Position
    }
    */
    std::vector<GLfloat> ParticlePosition;
    for(size_t i = 0; i < numberOfParticles; i++)
    {
        ParticlePosition.push_back(-1.0f + i * normalizer);      // x-Position
        ParticlePosition.push_back(sin(i * normalizer));     // y-Position
        ParticlePosition.push_back(0.0f);                   // z-Position
    }

    GLuint ParticlePositionHandle;
    glGenBuffers(1, &ParticlePositionHandle);
    glBindBuffer(GL_ARRAY_BUFFER, ParticlePositionHandle);    
    glBufferData(GL_ARRAY_BUFFER, ParticlePosition.size() * sizeof(GLfloat),
                 ParticlePosition.data(), GL_STATIC_DRAW);
    //------------------------------------------------------------------------------------------------------//

    
    // generate a vertex array object (VAO)
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);    

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    

    // unbind the VBO, we don't need it anymore
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*
    glGenVertexArrays(1, &ParticleObjectLocation);    // Speichert Partikel in die Graphikartenmemory
    glBindVertexArray(ParticleObjectLocation);

    GLuint ParticlePositionBufferLocation;            // Ort des Objekts wird gebunden und später zeitgleich mit Objekt entbunden
    glGenBuffers(1, &ParticlePositionBufferLocation);
    glBindBuffer(GL_ARRAY_BUFFER, ParticlePositionBufferLocation);
    glBufferData(GL_ARRAY_BUFFER, ParticlePositionData.size() * sizeof(GLfloat), ParticlePositionData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexArrayAttrib(ParticleObjectLocation, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);               // Unbind Buffer
    glBindVertexArray(0);
    */

}
void Particles::render()
{
    //glBindVertexArray(ParticleObjectLocation);          // Aktiviert Objekt an denen man arbeitet
    glDrawArrays(GL_POINTS, 0, NumberOfParticlesIn);      // malt (aktives Objekt) Pixel / größe der gezeichneten Pixel veränderbar
    //glBindVertexArray(0);                               // Objekt wieder deaktivieren

}