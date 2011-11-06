#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

namespace Boids {

namespace World {

// ============================================= //

void initialize();
void run();
void terminate();

/* Callback functions */
void keyPressed(int key, int status);
void resized(int width, int height);

// ============================================= //

};
};

#endif // WORLD_H_INCLUDED
