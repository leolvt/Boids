set(BOIDS_SRC 
    ${BOIDS_SRC} 
    World.cpp
    Util.cpp
    Flock.cpp
    Boid.cpp
    main.cpp
)

set(CMAKE_CXX_FLAGS "-std=c++0x")

add_executable(Boids ${BOIDS_SRC})
target_link_libraries(Boids ${BOIDS_LIBS})
