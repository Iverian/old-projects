#pragma once

#include <iostream>

namespace util
{
	static constexpr auto accuracy = 1e-9;

	bool fuzzy_cmp(const double& x, const double& y);
	void echo(std::istream& p_is, std::ostream& p_os);
    bool is_zero(const double& x);
}
