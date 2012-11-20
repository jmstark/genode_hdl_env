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

	unsigned raw_read(unsigned const o)
	{
		unsigned v;
		asm volatile ("ldr %[v], [%[d]]"
		              : [v]"=r"(v) : [d]"r"(base + o) : );
		return v;
	}

	void raw_write(unsigned const o, unsigned const v)
	{
		asm volatile ("str %[v], [%[d]]"
		              :: [v]"r"(v), [d]"r"(base + o) : );
	}

	unsigned raw_read_sync(unsigned const o)
	{
		Lock::Guard guard(lock);
		return raw_read(o);
	}

	void raw_write_sync(unsigned const o, unsigned const v)
	{
		Lock::Guard guard(lock);
		raw_write(o, v);
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

	unsigned read(unsigned const o)
	{
		watch->start();
		unsigned const v = raw_read(o);
		unsigned const tics = watch->read();
		unsigned const o_tics = r_tics;
		r_tics += tics;
		if (r_tics < o_tics) PDBG("timer overflow");
		reads++;
		return v;
	}

	void write(unsigned const o, unsigned const v)
	{
		watch->start();
		raw_write(o, v);
		unsigned const tics = watch->read();
		unsigned const o_tics = w_tics;
		w_tics += tics;
		if (w_tics < o_tics) PDBG("timer overflow");
		writes++;
	}

	unsigned read_sync(unsigned const o)
	{
		Lock::Guard guard(lock);
		return read(o);
	}

	void write_sync(unsigned const o, unsigned const v)
	{
		Lock::Guard guard(lock);
		write(o, v);
	}
};


enum { ROM_0_BASE = 0x71000000 };

static Watch watch(0);
static Test_rom rom_0(ROM_0_BASE, &watch);

enum { MY_THREADS = 2, ALL_THREADS = 20, EMULATED_PROGRAMS = 10 };
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
		rom_0.raw_write(low_o, rom_0.raw_read(low_o) + 1); /* ensure that emulator creation is done */
		threads_started++;
		if (threads_started == MY_THREADS) ready->unlock();
		else if (threads_started > MY_THREADS) PERR("To much threads");
		else lock->unlock();

		while (1)
			for (unsigned o = low_o; o < high_o; o += 4)
				rom_0.raw_write(o, rom_0.raw_read(o) + 1);
	}
};


int main(int argc, char **argv)
{
	printf("--- test device emulation ---\n");

	unsigned v1, v2;
	bool protect;

	for (unsigned volatile i = 0; i< 100*1000*1000; i++) ; /* await start of other task */

	static Lock start(Lock::LOCKED);
	static Lock ready(Lock::LOCKED);
	threads_started = 1;
	if (threads_started == MY_THREADS) ready.unlock();
	Test_thread  t1(&start, &ready, 0b000000<<6, 0b000001<<6);
//	Test_thread  t2(&start, &ready, 0b000001<<6, 0b000010<<6);
//	Test_thread  t3(&start, &ready, 0b000010<<6, 0b000011<<6);
//	Test_thread  t4(&start, &ready, 0b000011<<6, 0b000100<<6);
//	Test_thread  t5(&start, &ready, 0b000100<<6, 0b000101<<6);
//	Test_thread  t6(&start, &ready, 0b000101<<6, 0b000110<<6);
//	Test_thread  t7(&start, &ready, 0b000110<<6, 0b000111<<6);
//	Test_thread  t8(&start, &ready, 0b000111<<6, 0b001000<<6);
//	Test_thread  t9(&start, &ready, 0b001000<<6, 0b001001<<6);
//	Test_thread t10(&start, &ready, 0b001001<<6, 0b001010<<6);
//	Test_thread t11(&start, &ready, 0b001010<<6, 0b001011<<6);
//	Test_thread t12(&start, &ready, 0b001011<<6, 0b001100<<6);
//	Test_thread t13(&start, &ready, 0b001100<<6, 0b001101<<6);
//	Test_thread t14(&start, &ready, 0b001101<<6, 0b001110<<6);
//	Test_thread t15(&start, &ready, 0b001110<<6, 0b001111<<6);
//	Test_thread t16(&start, &ready, 0b001111<<6, 0b010000<<6);
//	Test_thread t17(&start, &ready, 0b010000<<6, 0b010001<<6);
//	Test_thread t18(&start, &ready, 0b010001<<6, 0b010010<<6);
//	Test_thread t19(&start, &ready, 0b010010<<6, 0b010011<<6);
//	Test_thread t20(&start, &ready, 0b010011<<6, 0b010100<<6);
//	Test_thread t21(&start, &ready, 0b010100<<6, 0b010101<<6);
//	Test_thread t22(&start, &ready, 0b010101<<6, 0b010110<<6);
//	Test_thread t23(&start, &ready, 0b010110<<6, 0b010111<<6);
//	Test_thread t24(&start, &ready, 0b010111<<6, 0b011000<<6);
//	Test_thread t25(&start, &ready, 0b011000<<6, 0b011001<<6);
//	Test_thread t26(&start, &ready, 0b011001<<6, 0b011010<<6);
//	Test_thread t27(&start, &ready, 0b011010<<6, 0b011011<<6);
//	Test_thread t28(&start, &ready, 0b011011<<6, 0b011100<<6);
//	Test_thread t29(&start, &ready, 0b011100<<6, 0b011101<<6);
//	Test_thread t30(&start, &ready, 0b011101<<6, 0b011110<<6);
//	Test_thread t31(&start, &ready, 0b011110<<6, 0b011111<<6);
	start.unlock();
	ready.lock();

	PINF("Measurement started");

	for (unsigned o = 0b01000<<5; o < 0b10000<<5; o += 4) {
		protect = (o & (0b11 << 9)) != (0b11 << 9);
		v1 = rom_0.read(o);
		v2 = v1 + 1;
		rom_0.write(o, v2);
		if (protect) { if (rom_0.read(o) != v1) PDBG("Protect failed %x", o); }
		else if (rom_0.read(o) != v2) PDBG("Write failed %x", o);
	}

	float r_period = ((float)rom_0.r_tics / rom_0.reads) / ALL_THREADS,
	      w_period = ((float)rom_0.w_tics / rom_0.writes) / ALL_THREADS;
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
	PINF("   tasks          %u", EMULATED_PROGRAMS);
	PINF("   threads        %u", ALL_THREADS);
	PINF("   reads          %u", rom_0.reads);
	PINF("   writes         %u", rom_0.writes);
	PINF("   read tics      %u", rom_0.r_tics);
	PINF("   write tics     %u", rom_0.w_tics);
	PINF("   avg read tics  %u.%03u", r_period_l, r_period_r);
	PINF("   avg write tics %u.%03u", w_period_l, w_period_r);
	PINF("   avg read ms    %u.%03u", r_ms_l, r_ms_r);
	PINF("   avg write ms   %u.%03u", w_ms_l, w_ms_r);

//	Irq_connection trigger_end(123);
//	trigger_end.wait_for_irq();

	sleep_forever();
}

/*

Ergebnisse
##########

Test 3
------

Gesamte und design-bedingte Dauer von Zugriffen auf emuliertem IOMEM
mit variierender Anzahl der Treiber.

Szenario: real vea9x4

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          2
[init -> vinit -> test_1]    threads        4
[init -> vinit -> test_1]    reads          1024
[init -> vinit -> test_1]    writes         512
[init -> vinit -> test_1]    read tics      37043047
[init -> vinit -> test_1]    write tics     18521921
[init -> vinit -> test_1]    avg read tics  9043.712
[init -> vinit -> test_1]    avg write tics 9043.906
[init -> vinit -> test_1]    avg read ms    9.043
[init -> vinit -> test_1]    avg write ms   9.043

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          3
[init -> vinit -> test_1]    threads        6
[init -> vinit -> test_1]    reads          1024
[init -> vinit -> test_1]    writes         512
[init -> vinit -> test_1]    read tics      77447930
[init -> vinit -> test_1]    write tics     38722550
[init -> vinit -> test_1]    avg read tics  12605.457
[init -> vinit -> test_1]    avg write tics 12604.997
[init -> vinit -> test_1]    avg read ms    12.604
[init -> vinit -> test_1]    avg write ms   12.604

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          4
[init -> vinit -> test_1]    threads        8
[init -> vinit -> test_1]    reads          1024
[init -> vinit -> test_1]    writes         512
[init -> vinit -> test_1]    read tics      139823270
[init -> vinit -> test_1]    write tics     69912706
[init -> vinit -> test_1]    avg read tics  17068.269
[init -> vinit -> test_1]    avg write tics 17068.531
[init -> vinit -> test_1]    avg read ms    17.068
[init -> vinit -> test_1]    avg write ms   17.068

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          5
[init -> vinit -> test_1]    threads        10
[init -> vinit -> test_1]    reads          1024
[init -> vinit -> test_1]    writes         512
[init -> vinit -> test_1]    read tics      215614290
[init -> vinit -> test_1]    write tics     107805508
[init -> vinit -> test_1]    avg read tics  21056.082
[init -> vinit -> test_1]    avg write tics 21055.761
[init -> vinit -> test_1]    avg read ms    21.055
[init -> vinit -> test_1]    avg write ms   21.055

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          6
[init -> vinit -> test_1]    threads        12
[init -> vinit -> test_1]    reads          512
[init -> vinit -> test_1]    writes         256
[init -> vinit -> test_1]    read tics      162080596
[init -> vinit -> test_1]    write tics     81040319
[init -> vinit -> test_1]    avg read tics  26380.304
[init -> vinit -> test_1]    avg write tics 26380.312
[init -> vinit -> test_1]    avg read ms    26.379
[init -> vinit -> test_1]    avg write ms   26.379

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          7
[init -> vinit -> test_1]    threads        14
[init -> vinit -> test_1]    reads          512
[init -> vinit -> test_1]    writes         256
[init -> vinit -> test_1]    read tics      233990151
[init -> vinit -> test_1]    write tics     116998471
[init -> vinit -> test_1]    avg read tics  32643.714
[init -> vinit -> test_1]    avg write tics 32644.662
[init -> vinit -> test_1]    avg read ms    32.643
[init -> vinit -> test_1]    avg write ms   32.644

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          8
[init -> vinit -> test_1]    threads        16
[init -> vinit -> test_1]    reads          256
[init -> vinit -> test_1]    writes         128
[init -> vinit -> test_1]    read tics      155300981
[init -> vinit -> test_1]    write tics     77648442
[init -> vinit -> test_1]    avg read tics  37915.277
[init -> vinit -> test_1]    avg write tics 37914.277
[init -> vinit -> test_1]    avg read ms    37.915
[init -> vinit -> test_1]    avg write ms   37.914

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          9
[init -> vinit -> test_1]    threads        18
[init -> vinit -> test_1]    reads          128
[init -> vinit -> test_1]    writes         64
[init -> vinit -> test_1]    read tics      104570035
[init -> vinit -> test_1]    write tics     52286960
[init -> vinit -> test_1]    avg read tics  45386.296
[init -> vinit -> test_1]    avg write tics 45387.984
[init -> vinit -> test_1]    avg read ms    45.386
[init -> vinit -> test_1]    avg write ms   45.387

[init -> vinit -> test_1] Driver test results:
[init -> vinit -> test_1]    tasks          10
[init -> vinit -> test_1]    threads        20
[init -> vinit -> test_1]    reads          64
[init -> vinit -> test_1]    writes         32
[init -> vinit -> test_1]    read tics      68634072
[init -> vinit -> test_1]    write tics     34314925
[init -> vinit -> test_1]    avg read tics  53620.367
[init -> vinit -> test_1]    avg write tics 53617.070
[init -> vinit -> test_1]    avg read ms    53.619
[init -> vinit -> test_1]    avg write ms   53.617

*/

