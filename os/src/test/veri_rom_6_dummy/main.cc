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
#include <base/sleep.h>

using namespace Genode;

struct Rom : Io_mem_connection
{
	typedef Io_mem_connection Io_mem;

	enum { SIZE = 0x1000 };

	unsigned base;
	static Lock lock;

	Rom(addr_t const rom_base) : Io_mem(rom_base, SIZE),
	                             base((unsigned)env()->rm_session()->attach(Io_mem::dataspace()))
	{ }

	unsigned read(unsigned const o)
	{
		unsigned v;
		asm volatile ("ldr %[v], [%[d]]"
		              : [v]"=r"(v) : [d]"r"(base + o) : );
		return v;
	}

	void write(unsigned const o, unsigned const v)
	{
		asm volatile ("str %[v], [%[d]]"
		              :: [v]"r"(v), [d]"r"(base + o) : );
	}
};


enum { ROM_0_BASE = 0x72000000, THREADS = 2 };

static Rom rom_0(ROM_0_BASE);
static unsigned threads_started;

struct Test_thread : Thread<2*1024>
{
	Lock * lock;
	Lock * ready;
	unsigned high_o;
	unsigned low_o;

	Test_thread(Lock * l1, Lock * l2, unsigned low, unsigned high) : lock(l1), ready(l2), high_o(high), low_o(low) {
		Thread::start(); }

	void entry()
	{
		/* sync startup */
		lock->lock();
		rom_0.write(low_o, rom_0.read(low_o) + 1); /* ensure that emulator creation is done */
		threads_started++;
		if (threads_started == THREADS) ready->unlock();
		else if (threads_started > THREADS) PERR("To much threads");
		else lock->unlock();

		while (1)
			for (unsigned o = low_o; o < high_o; o += 4)
				rom_0.write(o, rom_0.read(o) + 1);
	}
};


int main(int argc, char **argv)
{
	printf("--- test device emulation ---\n");
	static Lock start(Lock::LOCKED);
	static Lock ready(Lock::LOCKED);
	threads_started = 0;
	if (threads_started == THREADS) ready.unlock();
	Test_thread  t1(&start, &ready, 0b00<<11, 0b01<<11);
	Test_thread  t2(&start, &ready, 0b01<<11, 0b10<<11);
	start.unlock();
	ready.lock();
	PINF("Dummies started");
	sleep_forever();
}

