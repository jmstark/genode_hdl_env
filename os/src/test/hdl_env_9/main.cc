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
#include <io_mem_session/connection.h>
#include <irq_session/connection.h>

using namespace Genode;

struct Rom : Io_mem_connection
{
	typedef Io_mem_connection Io_mem;
	enum { SIZE = 0x1000 };
	unsigned base;

	Rom(addr_t const rom_base) :
		Io_mem(rom_base, SIZE),
		base((unsigned)env()->rm_session()->attach(Io_mem::dataspace())) { }

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

enum { ROM_0_BASE = 0x71000000 };

static Rom rom(ROM_0_BASE);
static Irq_connection irq1(101);
static Irq_connection irq2(102);
static unsigned test_round[2];

void test(bool const thread_id)
{
	unsigned rom_low  = thread_id ? 0xdf8 : 0x5f8;
	unsigned rom_high = rom_low + 0x10,
	         v1 = 0, v2 = 0;
	bool     protect = 1;
	while (1)
	{
		/* walk through our whole exclusive ROM area */
		for(unsigned o = rom_low; o < rom_high; o += 4)
		{
			/* wait for one IRQ */
			irq1.wait_for_irq(Dataspace_capability());

			/* modify a ROM value */
			v1 = rom.read(o);
			v2 = v1 + 1;
			rom.write(o, v2);

			/* wait for another IRQ */
			irq2.wait_for_irq(Dataspace_capability());

			/* check write result against access policy */
			protect = (o & (0b11 << 9)) != (0b11 << 9);
			if (protect) { if (rom.read(o) != v1) PDBG("Protect failed %x", o); }
			else if (rom.read(o) != v2)           PDBG("Write failed %x", o);
		}
		/* acknowledge end of the test round */
		test_round[thread_id]++;
		PINF("Thread %u round %u rom %x..%x finished", thread_id, test_round[thread_id], rom_low, rom_high);
	}
}

struct Test_thread : Thread<1*1024>
{
	Test_thread() { Thread::start(); }
	void entry() { test(0); }
};

int main(int argc, char **argv)
{
	PERR("Started");

	/* start tests in parallel */
	test_round[0] = 0;
	test_round[1] = 0;
	static Test_thread t;
	test(1);
}

