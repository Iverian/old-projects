#include <math_helper.h>
#include <utility.h>

using namespace math;
using namespace std;

double math::sign(double x) { return (x < 0) ? -1. : util::is_zero(x) ? 0. : 1.; }

double math::fraction(double x) { return fabs(x) - floor(fabs(x)); }
