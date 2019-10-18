#include "pch.h"
#include "Util.hpp"
#include <chrono>

static bool timeZesoCalculated = false;
static std::chrono::system_clock::time_point time0;

double TimeUtils::getTimestamp()
{
	if (timeZesoCalculated) {
		time0 = std::chrono::system_clock::now();
		timeZesoCalculated = true;
	}

	auto tstamp = std::chrono::system_clock::now() - time0;

	long long delta = std::chrono::duration_cast<std::chrono::nanoseconds>(tstamp).count();

	return delta * 0.000000001;
}
