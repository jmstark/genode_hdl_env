/*
 * \brief   Dummy producing load on emulated IRQs
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
#include <os/config.h>

using namespace Genode;

enum { THREADS_LOCAL = 4 };
static unsigned threads_started(0);

struct Test_thread : Thread<1*1024>
{
	Lock * lock;
	Lock * ready;
	Irq_connection irq;

	Test_thread(Lock * l1, Lock * l2, unsigned i) :
		lock(l1), ready(l2), irq(i)
	{
		Thread::start();
	}

	void entry()
	{
		/* sync startup with other threads */
		lock->lock();

		/* ensure resources are initialized for access */
		irq.wait_for_irq(Dataspace_capability());
		threads_started++;
		if (threads_started == THREADS_LOCAL) ready->unlock();
		else if (threads_started > THREADS_LOCAL) PERR("To much threads");
		else lock->unlock();

		/* produce load on emulated IRQs */
		while (1) irq.wait_for_irq(Dataspace_capability());
	}
};

int main(int argc, char **argv)
{
	/* get the IRQs we shall produce load on */
	unsigned irq_base = 0;
	try {
		Xml_node args = config()->xml_node().sub_node("args");
		args.attribute("irq_base").value(&irq_base);
	} catch (...) { PDBG("Could not obtain args"); }

	/* start dummy threads that produce load on emulated IRQs */
	static Lock start(Lock::LOCKED);
	static Lock ready(Lock::LOCKED);
	Test_thread  t1(&start, &ready, irq_base + 0);
	Test_thread  t2(&start, &ready, irq_base + 1);
	Test_thread  t3(&start, &ready, irq_base + 2);
	Test_thread  t4(&start, &ready, irq_base + 3);
	start.unlock();
	ready.lock();

	/* go to sleep */
	PINF("Started, idle on IRQ %u..%u", irq_base, irq_base + 3);
	sleep_forever();
}

