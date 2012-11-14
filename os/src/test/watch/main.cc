/*
 * \brief  Test the stop watch
 * \author Martin Stein
 * \date   2011-05-30
 */

/*
 * Copyright (C) 2011-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/printf.h>
#include <util/watch.h>

using namespace Genode;

struct Test_watch : Watch
{
	unsigned test(unsigned const factor)
	{
		unsigned const count = factor * 200*1000;
		start();
		for (unsigned volatile i = 0; i < count; i++) ;
		return read();
	}

	void deviation_ms(unsigned const factor, unsigned const tests,
	                  unsigned & min, unsigned & avg, unsigned & max)
	{
		unsigned v = test(factor);
		float avg_float = (float)v / tests;
		min = v;
		max = v;
		for(unsigned i = 1; i < tests; i++) {
			v = test(factor);
			if (v < min) min = v;
			if (v > max) max = v;
			avg_float += (float)v / tests;
		}
		avg = Timer::tics_to_ms((unsigned)avg_float);
		min = Timer::tics_to_ms(min);
		max = Timer::tics_to_ms(max);
	}
};


int main()
{
	printf("--- watch test started ---\n");
	enum { TESTS = 8, TESTS_PER_TEST = 15, F = 1000 };
	static Test_watch watch;
	unsigned min, avg, max, factor = 10;

	watch.deviation_ms(factor, TESTS_PER_TEST, min, avg, max);
	float freq = (float)avg / factor;
	float min_per = freq, avg_per = freq / TESTS, max_per = freq;
	PINF("Deviation for factor %u: [%u..%u..%u] (%u:%u)",
	     factor, min, avg, max, avg - min, max - avg);

	for (unsigned tests = TESTS - 1; tests; tests--)
	{
		factor *= 2;
		watch.deviation_ms(factor, TESTS_PER_TEST, min, avg, max);

		freq = (float)avg / factor;
		if (freq < min_per) min_per = freq;
		if (freq > max_per) max_per = freq;
		avg_per += freq / TESTS;

		PINF("Deviation for factor %u: [%u..%u..%u] (%u:%u)",
		     factor, min, avg, max, avg - min, max - avg);
	}
	float per_ldev      = avg_per - min_per;
	float per_rdev      = max_per - avg_per;
	unsigned min_per_l  = (unsigned)min_per;
	unsigned max_per_l  = (unsigned)max_per;
	unsigned avg_per_l  = (unsigned)avg_per;
	unsigned per_ldev_l = (unsigned)per_ldev;
	unsigned per_rdev_l = (unsigned)per_rdev;
	unsigned min_per_r  = (min_per - min_per_l) * F;
	unsigned max_per_r  = (max_per - max_per_l) * F;
	unsigned avg_per_r  = (avg_per - avg_per_l) * F;
	unsigned per_ldev_r = (per_ldev - per_ldev_l) * F;
	unsigned per_rdev_r = (per_rdev - per_rdev_l) * F;

	PINF("Period analyse: [%u.%03u..%u.%03u..%u.%03u] (%u.%03u:%u.%03u)",
	     min_per_l, min_per_r,
	     avg_per_l, avg_per_r,
	     max_per_l, max_per_r,
	     per_ldev_l, per_ldev_r,
	     per_rdev_l, per_rdev_r);

	return 0;
}

