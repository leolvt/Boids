#include "World.h"

int main()
{
    Boids::World::initialize();
    Boids::World::run();
    Boids::World::terminate();
    return 0;
}
