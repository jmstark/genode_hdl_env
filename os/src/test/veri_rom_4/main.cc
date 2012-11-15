/*
 * \brief   Test device that is in fact an emulated verilog design
 * \author  Martin Stein
 * \date    2012-05-04
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/printf.h>
#include <io_mem_session/connection.h>
#include <irq_session/connection.h>
#include <util/watch.h>

using namespace Genode;

/*

Ergebnisse
##########

Test 4
------

Durchschnittliche Dauer der IRQ-Zustellung. Der Emulator erzeugt einen Thread
der erst eine Weile nach dem Ankommen des await_irq submitted. Beim submitten
wird der Timer aktiviert. Der Client nimmt liest dann die Zeit vom Timer sobald
er deblockiert.

Szenario: real vea9x4

Keine Verzögerung am Submitter:

[init -> vinit -> test] Test result
[init -> vinit -> test]    IRQs received 1000
[init -> vinit -> test]    IRQ tics      47046
[init -> vinit -> test]    avg IRQ tics  47.046
[init -> vinit -> test]    avg IRQ ms    0.047

1'000'000 ops Verzögerung am Submitter

[init -> vinit -> test] Test result
[init -> vinit -> test]    IRQs received 1000
[init -> vinit -> test]    IRQ tics      42619
[init -> vinit -> test]    avg IRQ tics  42.618
[init -> vinit -> test]    avg IRQ ms    0.042

2'000'000 ops Verzögerung am Submitter

[init -> vinit -> test] Test result
[init -> vinit -> test]    IRQs received 1000
[init -> vinit -> test]    IRQ tics      42533
[init -> vinit -> test]    avg IRQ tics  42.533
[init -> vinit -> test]    avg IRQ ms    0.042

4'000'000 ops Verzögerung am Submitter

[init -> vinit -> test] Test result
[init -> vinit -> test]    IRQs received 1000
[init -> vinit -> test]    IRQ tics      42529
[init -> vinit -> test]    avg IRQ tics  42.528
[init -> vinit -> test]    avg IRQ ms    0.042

*/

static Watch watch(0);
static unsigned tics = 0;

int main(int argc, char **argv)
{
	printf("--- test device emulation ---\n");

	Irq_connection irq(123);
	enum { TESTS = 1000 };

	for(unsigned i = 0; i< TESTS; i++)
	{
		irq.wait_for_irq(watch.Io_mem_connection::dataspace());
		tics += watch.read();
	}

	float avg_tics      = (float)tics / TESTS;
	float avg_ms        = Watch::tics_to_ms(avg_tics);
	unsigned avg_tics_l = (unsigned)avg_tics;
	unsigned avg_tics_r = 1000 * (avg_tics - avg_tics_l);
	unsigned avg_ms_l   = (unsigned)avg_ms;
	unsigned avg_ms_r   = 1000 * (avg_ms - avg_ms_l);

	PINF("Test result");
	PINF("   IRQs received %u", TESTS);
	PINF("   IRQ tics      %u", tics);
	PINF("   avg IRQ tics  %u.%03u", avg_tics_l, avg_tics_r);
	PINF("   avg IRQ ms    %u.%03u", avg_ms_l, avg_ms_r);

	while(1);
}

