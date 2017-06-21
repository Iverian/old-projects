#include <utility.h>

#include <cmath>
#include <string>

namespace util {
static constexpr auto bufsize = 255;
}
using namespace std;

bool util::fuzzy_cmp(const double& x, const double& y) { return abs(x - y) < accuracy; }

void util::echo(istream& p_is, ostream& p_os)
{
    string buf;
    buf.reserve(bufsize);
    while (p_is) {
        getline(p_is, buf);
        p_os << buf << endl;
    }
}

bool ::util::is_zero(const double& x) { return util::fuzzy_cmp(x, 0.); }
