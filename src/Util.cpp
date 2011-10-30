#include <iostream>
#include <cstdlib>
#include <ctime>

#include <glm/gtx/vector_angle.hpp>

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

double normNegSigmoid(double val, double limit)
{
    // Compute a normalization factor based on a sigmoid function.
    // When val -> limit,  fac -> 1,
    // when val -> 0,      fac -> 0
    double fac = 1/(1 + glm::exp(-9/limit*(limit/2-val)));
    return fac;
}

// ============================================= //

double normSigmoid(double val, double limit)
{
    // Compute a normalization factor based on a sigmoid function.
    // When val -> limit,  fac -> 1,
    // when val -> 0,      fac -> 0
    double fac = 1/(1 + glm::exp(9/limit*(limit/2-val)));
    return fac;
}

// ============================================= //

glm::vec3 normalize(glm::vec3 vec, double limit)
{
    // Compute the length of the vector
    double length = glm::length(vec);

    // Compute a normalization factor based on a sigmoid function.
    // When length -> limit,  fac -> 1,
    // when length -> 0,      fac -> 0
    double fac = Util::normSigmoid(length, limit);

    // Return the vector normalized by the factor above
    vec /= length + 0.000001;
    vec *= fac;

    return vec;
}

// ============================================= //

glm::vec3 normalizeNeg(glm::vec3 vec, double limit)
{
    // Compute the length of the vector
    double length = glm::length(vec);

    // Compute a normalization factor based on a sigmoid function.
    // When length -> limit,  fac -> 1,
    // when length -> 0,      fac -> 0
    double fac = Util::normNegSigmoid(length, limit);

    // Return the vector normalized by the factor above
    vec /= length + 0.000001;
    vec *= fac;

    return vec;
}

// ============================================= //

double computeAngle(glm::vec3 vec1, glm::vec3 vec2)
{
    vec1 /= glm::length(vec1) + 0.00001;
    vec2 /= glm::length(vec2) + 0.00001;
    return glm::angle(vec2, vec1);
}

// ============================================= //

};
