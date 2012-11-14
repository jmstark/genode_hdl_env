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
		for (unsigned volatile i = 0; i<500*1000; i++) ;
		watch->start();
		Rom::write_w(o, v);
		unsigned const tics = watch->read();
		for (unsigned volatile i = 0; i<500*1000; i++) ;
		unsigned const o_tics = w_tics;
		w_tics += tics;
		if (w_tics < o_tics) PDBG("timer overflow");
		writes++;
	}
};

/*

Ergebnisse
##########

Test 1
------

	Verlangsamung eines Befehlstyps am Emulator erhöht durchschnittliche
	ms je Befehl des Typs proportional

	Szenario: 1000 tics / ms, 1024 reads, 512 writes, real vea9x4

	write 0x slowed

	[init -> vinit -> test_1]    read tics      6005505
	[init -> vinit -> test_1]    write tics     3002573
	[init -> vinit -> test_1]    avg read tics  5864.750
	[init -> vinit -> test_1]    avg write tics 5864.400
	[init -> vinit -> test_1]    avg read ms    5.863
	[init -> vinit -> test_1]    avg write ms   5.863

	write 1x slowed

	[init -> vinit -> test_1]    read tics      6005577
	[init -> vinit -> test_1]    write tics     5434875
	[init -> vinit -> test_1]    avg read tics  5864.821
	[init -> vinit -> test_1]    avg write tics 10614.990
	[init -> vinit -> test_1]    avg read ms    5.863
	[init -> vinit -> test_1]    avg write ms   10.614

	write 2x slowed

	[init -> vinit -> test_1]    read tics      6005919
	[init -> vinit -> test_1]    write tics     7866236
	[init -> vinit -> test_1]    avg read tics  5865.155
	[init -> vinit -> test_1]    avg write tics 15363.742
	[init -> vinit -> test_1]    avg read ms    5.864
	[init -> vinit -> test_1]    avg write ms   15.362

	write 4x slowed

	[init -> vinit -> test_1]    read tics      6004651
	[init -> vinit -> test_1]    write tics     12730858
	[init -> vinit -> test_1]    avg read tics  5863.916
	[init -> vinit -> test_1]    avg write tics 24864.957
	[init -> vinit -> test_1]    avg read ms    5.862
	[init -> vinit -> test_1]    avg write ms   24.864

Test 2
------

	Durchschnittliche ms je Befehl konvergieren über der Gesamtzahl der Befehle

	Szenario: 1000 tics / ms, real vea9x4

	[init -> vinit -> test_1]    reads          2
	[init -> vinit -> test_1]    writes         1
	[init -> vinit -> test_1]    avg read tics  6061.500
	[init -> vinit -> test_1]    avg write tics 5896.000
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          8
	[init -> vinit -> test_1]    writes         4
	[init -> vinit -> test_1]    avg read tics  5915.750
	[init -> vinit -> test_1]    avg write tics 5876.000
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          22
	[init -> vinit -> test_1]    writes         11
	[init -> vinit -> test_1]    avg read tics  5882.500
	[init -> vinit -> test_1]    avg write tics 5871.636
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          52
	[init -> vinit -> test_1]    writes         26
	[init -> vinit -> test_1]    avg read tics  5872.038
	[init -> vinit -> test_1]    avg write tics 5865.346
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          114
	[init -> vinit -> test_1]    writes         57
	[init -> vinit -> test_1]    avg read tics  5867.675
	[init -> vinit -> test_1]    avg write tics 5864.473
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          240
	[init -> vinit -> test_1]    writes         120
	[init -> vinit -> test_1]    avg read tics  5866.283
	[init -> vinit -> test_1]    avg write tics 5865.274
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          494
	[init -> vinit -> test_1]    writes         247
	[init -> vinit -> test_1]    avg read tics  5865.344
	[init -> vinit -> test_1]    avg write tics 5865.360
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          1004
	[init -> vinit -> test_1]    writes         502
	[init -> vinit -> test_1]    avg read tics  5865.568
	[init -> vinit -> test_1]    avg write tics 5865.199
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          2026
	[init -> vinit -> test_1]    writes         1013
	[init -> vinit -> test_1]    avg read tics  5865.459
	[init -> vinit -> test_1]    avg write tics 5864.547
	[init -> vinit -> test_1] 
	[init -> vinit -> test_1]    reads          4072
	[init -> vinit -> test_1]    writes         2036
	[init -> vinit -> test_1]    avg read tics  5864.840
	[init -> vinit -> test_1]    avg write tics 5864.583

*/

void test(off_t const init_o)
{
	enum { ROM_0_BASE = 0x71000000 };

	static Watch watch(0);
	static Test_rom rom_0(ROM_0_BASE, &watch);
	unsigned v1, v2;
	bool protect;

	for (unsigned o = init_o; o < Rom::SIZE; o += 4) {
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

	PINF("Test results:");
	PINF("   reads          %u", rom_0.reads);
	PINF("   writes         %u", rom_0.writes);
	PINF("   read tics      %u", rom_0.r_tics);
	PINF("   write tics     %u", rom_0.w_tics);
	PINF("   avg read tics  %u.%03u", r_period_l, r_period_r);
	PINF("   avg write tics %u.%03u", w_period_l, w_period_r);
	PINF("   avg read ms    %u.%03u", r_ms_l, r_ms_r);
	PINF("   avg write ms   %u.%03u", w_ms_l, w_ms_r);
}

int main(int argc, char **argv)
{
	printf("--- test device emulation ---\n");

	off_t init_o = Rom::SIZE;
	unsigned minus = 4;
	while (1)
	{
		init_o -= minus;
		if (init_o < 0 ) break;
		test(init_o);
		minus *= 2;
	}
	PINF("Ende");

	while(1);
}

