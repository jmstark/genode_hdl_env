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
#include <io_mem_session/connection.h>
#include <timer_session/connection.h>

using namespace Genode;

struct Monitor : Io_mem_connection
{
	typedef Io_mem_connection Io_mem;

	enum { SIZE = 0x1000 };

	unsigned base;

	Monitor(addr_t const base) :
		Io_mem(base, SIZE),
		base((unsigned)env()->rm_session()->attach(Io_mem::dataspace())) { }

	uint32_t read_w(unsigned const o)
	{
		uint32_t v;
		asm volatile ("ldr %[v], [%[d]]"
		              : [v]"=r"(v) : [d]"r"(base + o) : );
		return v;
	}

	void write_w(unsigned const o, uint32_t const v) {
		asm volatile ("str %[v], [%[d]]"
		              :: [v]"r"(v), [d]"r"(base + o) : ); }

	uint16_t read_h(unsigned const o)
	{
		uint16_t v;
		asm volatile ("ldrh %[v], [%[d]]"
		              : [v]"=r"(v) : [d]"r"(base + o) : );
		return v;
	}

	void write_h(unsigned const o, uint16_t const v) {
		asm volatile ("strh %[v], [%[d]]"
		              :: [v]"r"(v), [d]"r"(base + o) : ); }

	uint8_t read_b(unsigned const o)
	{
		uint8_t v;
		asm volatile ("ldrb %[v], [%[d]]"
		              : [v]"=r"(v) : [d]"r"(base + o) : );
		return v;
	}

	void write_b(unsigned const o, uint8_t const v) {
		asm volatile ("strb %[v], [%[d]]"
		              :: [v]"r"(v), [d]"r"(base + o) : ); }
};

int main(int argc, char **argv)
{
	enum { MONITOR_BASE = 0x71000000 };
	static Timer::Connection timer;
	static Monitor monitor(MONITOR_BASE);
	unsigned base = 0;
	for (unsigned i = 0; i < 10; i++) {
		printf("--- monitor test ---\n");
		for (unsigned o = base + 0x400; o < 512*4; o += 0x40) {
			bool protect = (o & (0b11 << 9)) != (0b11 << 9);
			unsigned v1 = monitor.read_w(o);
			unsigned v2 = v1 + 1;
			monitor.write_w(o, v2);
			unsigned v3 = monitor.read_w(o);
			if (protect) {
				if ( v3 != v1) { PDBG("Protect 0x%x failed", MONITOR_BASE + o); }
				else { printf("Protect  0x%x: 0x%08x\n", MONITOR_BASE + o, v1); }
			} else {
				if (v3 != v2) printf("Override 0x%x failed", MONITOR_BASE + o);
				else printf("Override 0x%x: 0x%08x 0x%08x\n", MONITOR_BASE + o, v1, v3);
			}
			base += 4;
		}
		timer.msleep(1000);
	}
	printf("--- monitor test finished ---\n");
	sleep_forever();
}

