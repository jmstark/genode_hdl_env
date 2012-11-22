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
#include <base/sleep.h>
#include <irq_session/connection.h>
#include <util/watch.h>

using namespace Genode;

enum {
	TASKS = 10,
	THREADS_LOCAL = 4,
	THREADS_GLOBAL = TASKS * THREADS_LOCAL,
};

static unsigned threads_started;
static Lock start(Lock::LOCKED);
static Lock ready(Lock::LOCKED);

struct Test_thread : Thread<1*1024>
{
	Lock * lock;
	Lock * ready;
	Irq_connection irq;
	unsigned id;

	/**
	 * Constructor
	 */
	Test_thread(Lock * l1, Lock * l2, unsigned i) :
		lock(l1), ready(l2), irq(i), id(i-100)
	{
		Thread::start();
	}

	/************
	 ** Thread **
	 ************/

	void entry()
	{
		/* sync startup with all initialization */
		lock->lock();
		irq.wait_for_irq(Dataspace_capability());
		threads_started++;
		if (threads_started == THREADS_LOCAL) ready->unlock();
		else if (threads_started > THREADS_LOCAL) PERR("To much threads");
		else lock->unlock();

		/* produce load on resource */
		while (1) irq.wait_for_irq(Dataspace_capability());
	}
};

static Watch watch(0);
static unsigned tics = 0;

int main(int argc, char **argv)
{
	/* init our dummy threads */
	threads_started = 1;
	if (threads_started == THREADS_LOCAL) ready.unlock();
	Test_thread  t2(&start, &ready, 102);
	Test_thread  t3(&start, &ready, 103);
	Test_thread  t4(&start, &ready, 104);

	/* start local dummy threads */
	start.unlock();

	/* wait for the local dummies to start producing load on vinit */
	ready.lock();

	/* wait for the dummy tasks to start producing load on vinit */
	for (unsigned volatile i = 0; i<400*1000*1000; i++) ;

	/* ensure that all stuff around our IRQ emulation is initialized */
	static Irq_connection irq(101);
	irq.wait_for_irq(watch.Io_mem_connection::dataspace());

	/* do a bunch of measurements */
	PINF("Measurement started");
	enum { TESTS = 500 };
	for(unsigned i = 0; i< TESTS; i++) {
		irq.wait_for_irq(watch.Io_mem_connection::dataspace());
		tics += watch.read();
	}

	/* interpret results */
	float avg_tics      = ((float)tics / TESTS) / THREADS_GLOBAL;
	float avg_ms        = Watch::tics_to_ms(avg_tics);
	unsigned avg_tics_l = (unsigned)avg_tics;
	unsigned avg_tics_r = 1000 * (avg_tics - avg_tics_l);
	unsigned avg_ms_l   = (unsigned)avg_ms;
	unsigned avg_ms_r   = 1000 * (avg_ms - avg_ms_l);

	/* print out results */
	PINF("Test result");
	PINF("   tasks         %u", TASKS);
	PINF("   threads       %u", THREADS_GLOBAL);
	PINF("   IRQs received %u", TESTS);
	PINF("   IRQ tics      %u", tics);
	PINF("   avg IRQ tics  %u.%03u", avg_tics_l, avg_tics_r);
	PINF("   avg IRQ ms    %u.%03u", avg_ms_l, avg_ms_r);
	sleep_forever();
}

