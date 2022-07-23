#pragma once // gets included once

// Die h Datei ist als grober Überblick da die Funktionen werden zur cpp Datei ausgelagert

#include <vector> 
#include <random>
#include <glad/glad.h>

class Particles
{
    public:
    void createParticles();
    void render();

    private:
    GLuint ParticleObjectLocation;
    GLuint NumberOfParticles;
};