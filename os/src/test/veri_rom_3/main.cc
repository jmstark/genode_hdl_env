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

struct Rom : Io_mem_connection
{
	typedef Io_mem_connection Io_mem;

	enum { SIZE = 0x1000 };

	unsigned base;

	Rom(addr_t const rom_base) : Io_mem(rom_base, Rom::SIZE),
	                             base((unsigned)env()->rm_session()->attach(Io_mem::dataspace()))
	{ }

	unsigned read_w(unsigned const o)
	{
		unsigned v;
		asm volatile ("ldr %[v], [%[d]]"
		              : [v]"=r"(v) : [d]"r"(base + o) : );
		return v;
	}

	void write_w(unsigned const o, unsigned const v)
	{
		asm volatile ("str %[v], [%[d]]"
		              :: [v]"r"(v), [d]"r"(base + o) : );
	}
};

struct Test_rom : Rom
{
	Watch * const watch;

	unsigned r_tics, w_tics, reads, writes;

	Test_rom(addr_t const rom_base, Watch * const w) :
		Rom(rom_base), watch(w), r_tics(0), w_tics(0),
		reads(0), writes(0)
	{ }

	unsigned read_w(unsigned const o)
	{
		watch->start();
		unsigned const v = Rom::read_w(o);
		unsigned const tics = watch->read();
		unsigned const o_tics = r_tics;
		r_tics += tics;
		if (r_tics < o_tics) PDBG("timer overflow");
		reads++;
		return v;
	}

	void write_w(unsigned const o, unsigned const v)
	{
		watch->start();
		Rom::write_w(o, v);
		unsigned const tics = watch->read();
		unsigned const o_tics = w_tics;
		w_tics += tics;
		if (w_tics < o_tics) PDBG("timer overflow");
		writes++;
	}
};

/*

Ergebnisse
##########

Test 3
------

Design-spezifischer Anteil an der Emulationszeit

Szenario: real vea9x4

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          512
[init -> vinit -> test]    writes         256
[init -> vinit -> test]    read tics      2985481
[init -> vinit -> test]    write tics     1492472
[init -> vinit -> test]    avg read tics  5831.017
[init -> vinit -> test]    avg write tics 5829.968
[init -> vinit -> test]    avg read ms    5.830
[init -> vinit -> test]    avg write ms   5.829
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      4024
[init -> vinit -> monitor]    write tics     2121
[init -> vinit -> monitor]    avg read tics  7.859
[init -> vinit -> monitor]    avg write tics 8.285
[init -> vinit -> monitor]    avg read ms    0.007
[init -> vinit -> monitor]    avg write ms   0.008

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          512
[init -> vinit -> test]    writes         256
[init -> vinit -> test]    read tics      2985352
[init -> vinit -> test]    write tics     1492407
[init -> vinit -> test]    avg read tics  5830.765
[init -> vinit -> test]    avg write tics 5829.714
[init -> vinit -> test]    avg read ms    5.829
[init -> vinit -> test]    avg write ms   5.829
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      4037
[init -> vinit -> monitor]    write tics     2114
[init -> vinit -> monitor]    avg read tics  7.884
[init -> vinit -> monitor]    avg write tics 8.257
[init -> vinit -> monitor]    avg read ms    0.007
[init -> vinit -> monitor]    avg write ms   0.00

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      5970005
[init -> vinit -> test]    write tics     2984752
[init -> vinit -> test]    avg read tics  5830.083
[init -> vinit -> test]    avg write tics 5829.593
[init -> vinit -> test]    avg read ms    5.829
[init -> vinit -> test]    avg write ms   5.829
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      8061
[init -> vinit -> monitor]    write tics     4216
[init -> vinit -> monitor]    avg read tics  7.872
[init -> vinit -> monitor]    avg write tics 8.234
[init -> vinit -> monitor]    avg read ms    0.007
[init -> vinit -> monitor]    avg write ms   0.008

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      5970435
[init -> vinit -> test]    write tics     2985068
[init -> vinit -> test]    avg read tics  5830.502
[init -> vinit -> test]    avg write tics 5830.210
[init -> vinit -> test]    avg read ms    5.829
[init -> vinit -> test]    avg write ms   5.829
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      8098
[init -> vinit -> monitor]    write tics     4183
[init -> vinit -> monitor]    avg read tics  7.908
[init -> vinit -> monitor]    avg write tics 8.169
[init -> vinit -> monitor]    avg read ms    0.007
[init -> vinit -> monitor]    avg write ms   0.008

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          2048
[init -> vinit -> test]    writes         1024
[init -> vinit -> test]    read tics      11940782
[init -> vinit -> test]    write tics     5969104
[init -> vinit -> test]    avg read tics  5830.459
[init -> vinit -> test]    avg write tics 5829.203
[init -> vinit -> test]    avg read ms    5.829
[init -> vinit -> test]    avg write ms   5.829
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      16182
[init -> vinit -> monitor]    write tics     8401
[init -> vinit -> monitor]    avg read tics  7.901
[init -> vinit -> monitor]    avg write tics 8.204
[init -> vinit -> monitor]    avg read ms    0.007
[init -> vinit -> monitor]    avg write ms   0.008

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          2048
[init -> vinit -> test]    writes         1024
[init -> vinit -> test]    read tics      11939612
[init -> vinit -> test]    write tics     5970368
[init -> vinit -> test]    avg read tics  5829.888
[init -> vinit -> test]    avg write tics 5830.437
[init -> vinit -> test]    avg read ms    5.829
[init -> vinit -> test]    avg write ms   5.829
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      16107
[init -> vinit -> monitor]    write tics     8380
[init -> vinit -> monitor]    avg read tics  7.864
[init -> vinit -> monitor]    avg write tics 8.183
[init -> vinit -> monitor]    avg read ms    0.007
[init -> vinit -> monitor]    avg write ms   0.008

6 tests   hdl      restliche emulation
read      0.13 %   5.82 ms
write     0.14 %   5.82 ms

*/

int main(int argc, char **argv)
{
	printf("--- test device emulation ---\n");

	enum { ROM_0_BASE = 0x71000000 };

	static Watch watch(0);
	static Test_rom rom_0(ROM_0_BASE, &watch);
	unsigned v1, v2;
	bool protect;

	for (unsigned o = 0b11<<10; o < Rom::SIZE; o += 4) {
		protect = (o & (0b11 << 9)) != (0b11 << 9);
		v1 = rom_0.read_w(o);
		v2 = v1 + 1;
		rom_0.write_w(o, v2);
		if (protect) { if (rom_0.read_w(o) != v1) PDBG("Protect failed %x", o); }
		else if (rom_0.read_w(o) != v2) PDBG("Write failed %x", o);
	}

	float r_period = (float)rom_0.r_tics / rom_0.reads,
	      w_period = (float)rom_0.w_tics / rom_0.writes;
	unsigned r_period_l = (unsigned)r_period;
	unsigned w_period_l = (unsigned)w_period;
	unsigned r_period_r = 1000 * (r_period - r_period_l);
	unsigned w_period_r = 1000 * (w_period - w_period_l);
	float r_ms = Watch::tics_to_ms(r_period),
	      w_ms = Watch::tics_to_ms(w_period);
	unsigned r_ms_l = (unsigned)r_ms;
	unsigned w_ms_l = (unsigned)w_ms;
	unsigned r_ms_r = 1000 * (r_ms - r_ms_l);
	unsigned w_ms_r = 1000 * (w_ms - w_ms_l);

	PINF("Driver test results:");
	PINF("   reads          %u", rom_0.reads);
	PINF("   writes         %u", rom_0.writes);
	PINF("   read tics      %u", rom_0.r_tics);
	PINF("   write tics     %u", rom_0.w_tics);
	PINF("   avg read tics  %u.%03u", r_period_l, r_period_r);
	PINF("   avg write tics %u.%03u", w_period_l, w_period_r);
	PINF("   avg read ms    %u.%03u", r_ms_l, r_ms_r);
	PINF("   avg write ms   %u.%03u", w_ms_l, w_ms_r);

	Irq_connection trigger_end(123);
	trigger_end.wait_for_irq();

	while(1);
}

