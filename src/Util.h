#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <glm/glm.hpp>

// ============================================= //

namespace Util {

const double PI = 3.14592653589793;

// ============================================= //

double getRandom();
double normSigmoid(double val, double limit);
double normNegSigmoid(double val, double limit);
glm::vec3 normalize(glm::vec3 vec, double limit);
glm::vec3 normalizeNeg(glm::vec3 vec, double limit);

};

// ============================================= //

#endif // UTIL_H_INCLUDED
