#include <cstdlib>
#include <ctime>
#include "Util.h"

namespace Util {

// ============================================= //

namespace {
    bool firstRandom = true;
};

// ============================================= //

double getRandom()
{
    // Initialize seed if we are getting the first random number
    if (firstRandom)
    {
        firstRandom = false;
#if _WIN32
        srand(time(NULL));
#else
        srand48(time(NULL));
#endif
    }

    // Compute the random number and return it
#if _WIN32
    return ((double)rand()) / RAND_MAX;
#else
    return drand48();
#endif
}

// ============================================= //

};
