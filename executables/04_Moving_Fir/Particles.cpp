#include "Particles.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>


void Particles::createParticles(GLuint numberOfParticles)
{

    // Buffer = selection of Memory
    // ArrayBuffer = selection of Memory used for a list / array 


    NumberOfParticlesIn = numberOfParticles;
    std::vector<GLfloat> ParticlePositionData;    
    std::vector<glm::vec3> acceleration;
    dt = 0.005f;

    GLfloat normalizer = 2.0F / NumberOfParticlesIn;            // sicher gehen dass jedes Partikel auch im Fenster ist (Nummer der Partikel muss auf -1 bis +1 abgebildet werden)
    

    for(size_t i = 0; i < numberOfParticles*3; i++)
    {
        std::vector<float> velocity_test{0.005f,0.0f,0.0f};
        velocity.push_back(velocity_test[0]);
        velocity.push_back(velocity_test[1]);
        velocity.push_back(velocity_test[2]);
    }
    
    for(size_t i = 0; i < numberOfParticles; i++)
    {
        ParticlePositionNew.push_back(-1.0f + i * normalizer);      // x-Position
        ParticlePositionNew.push_back(sin(i * normalizer));     // y-Position
        ParticlePositionNew.push_back(0.0f);                   // z-Position
    }
    /*
    GLuint ParticlePositionHandle;
    glGenBuffers(1, &ParticlePositionHandle);
    glBindBuffer(GL_ARRAY_BUFFER, ParticlePositionHandle);    
    glBufferData(GL_ARRAY_BUFFER, ParticlePositionNew.size() * sizeof(GLfloat),
                 ParticlePositionNew.data(), GL_STATIC_DRAW);
    */             
    //------------------------------------------------------------------------------------------------------//

    
    // generate a vertex array object (VAO)
    GLuint vertexArrayHandle;
    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);    
    /*
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);  
    */
    // Velocity Buffer Handle

    GLuint velocityHandle;
    glGenBuffers(1, &velocityHandle);
    glBindBuffer(GL_ARRAY_BUFFER, velocityHandle);    
    glBufferData(GL_ARRAY_BUFFER, velocity.size() * sizeof(GLfloat),
                 velocity.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); 

    // unbind the VBO, we don't need it anymore
    
    //----------------------------------------------------------------------------------------------------
    

}
void Particles::calcParticlePosition()
    {      
        ParticlePositionOld.clear();  
        for(size_t i = 0; i < NumberOfParticlesIn*3; i++)
        {
            ParticlePositionOld.push_back(ParticlePositionNew[i]);
        }
        ParticlePositionNew.clear();
        for(size_t i = 0; i < NumberOfParticlesIn * 3; i++)
        {
            float newPosition = ParticlePositionOld[i] + velocity[i];
            ParticlePositionNew.push_back(newPosition);
        }        
    }

void Particles::render()
{     
    GLuint ParticlePositionHandle;
    glGenBuffers(1, &ParticlePositionHandle);
    glBindBuffer(GL_ARRAY_BUFFER, ParticlePositionHandle);    
    glBufferData(GL_ARRAY_BUFFER, ParticlePositionNew.size() * sizeof(GLfloat),
                 ParticlePositionNew.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr); 
      
    glDrawArrays(GL_POINTS, 0, NumberOfParticlesIn);      // malt (aktives Objekt) Pixel / größe der gezeichneten Pixel veränderbar
    calcParticlePosition();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}