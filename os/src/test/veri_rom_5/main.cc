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

enum { THREADS = 32 };
static unsigned threads_started;

struct Test_thread : Thread<2*1024>
{
	Lock * lock;
	Lock * ready;
	unsigned high_o;
	unsigned low_o;

	Test_thread(Lock * l1, Lock * l2, unsigned low, unsigned high) : lock(l1), ready(l2), high_o(high), low_o(low)
	{
		Thread::start();
		PINF("Thread started");
	}

	void entry()
	{
		/* sync startup */
		lock->lock();
		threads_started++;
		if (threads_started == THREADS) ready->unlock();
		else if (threads_started > THREADS) PERR("To much threads");
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

	static Lock start(Lock::LOCKED);
	static Lock ready(Lock::LOCKED);
	threads_started = 1;
	if (threads_started == THREADS) ready.unlock();
	Test_thread  t1(&start, &ready, 0b000000<<6, 0b000001<<6);
	Test_thread  t2(&start, &ready, 0b000001<<6, 0b000010<<6);
	Test_thread  t3(&start, &ready, 0b000010<<6, 0b000011<<6);
	Test_thread  t4(&start, &ready, 0b000011<<6, 0b000100<<6);
	Test_thread  t5(&start, &ready, 0b000100<<6, 0b000101<<6);
	Test_thread  t6(&start, &ready, 0b000101<<6, 0b000110<<6);
	Test_thread  t7(&start, &ready, 0b000110<<6, 0b000111<<6);
	Test_thread  t8(&start, &ready, 0b000111<<6, 0b001000<<6);
	Test_thread  t9(&start, &ready, 0b001000<<6, 0b001001<<6);
	Test_thread t10(&start, &ready, 0b001001<<6, 0b001010<<6);
	Test_thread t11(&start, &ready, 0b001010<<6, 0b001011<<6);
	Test_thread t12(&start, &ready, 0b001011<<6, 0b001100<<6);
	Test_thread t13(&start, &ready, 0b001100<<6, 0b001101<<6);
	Test_thread t14(&start, &ready, 0b001101<<6, 0b001110<<6);
	Test_thread t15(&start, &ready, 0b001110<<6, 0b001111<<6);
	Test_thread t16(&start, &ready, 0b001111<<6, 0b010000<<6);
	Test_thread t17(&start, &ready, 0b010000<<6, 0b010001<<6);
	Test_thread t18(&start, &ready, 0b010001<<6, 0b010010<<6);
	Test_thread t19(&start, &ready, 0b010010<<6, 0b010011<<6);
	Test_thread t20(&start, &ready, 0b010011<<6, 0b010100<<6);
	Test_thread t21(&start, &ready, 0b010100<<6, 0b010101<<6);
	Test_thread t22(&start, &ready, 0b010101<<6, 0b010110<<6);
	Test_thread t23(&start, &ready, 0b010110<<6, 0b010111<<6);
	Test_thread t24(&start, &ready, 0b010111<<6, 0b011000<<6);
	Test_thread t25(&start, &ready, 0b011000<<6, 0b011001<<6);
	Test_thread t26(&start, &ready, 0b011001<<6, 0b011010<<6);
	Test_thread t27(&start, &ready, 0b011010<<6, 0b011011<<6);
	Test_thread t28(&start, &ready, 0b011011<<6, 0b011100<<6);
	Test_thread t29(&start, &ready, 0b011100<<6, 0b011101<<6);
	Test_thread t30(&start, &ready, 0b011101<<6, 0b011110<<6);
	Test_thread t31(&start, &ready, 0b011110<<6, 0b011111<<6);
	start.unlock();
	ready.lock();

	for (unsigned o = 0b01000<<8; o < 0b10000<<8; o += 4) {
		protect = (o & (0b11 << 9)) != (0b11 << 9);
		v1 = rom_0.read(o);
		v2 = v1 + 1;
		rom_0.write(o, v2);
		if (protect) { if (rom_0.read(o) != v1) PDBG("Protect failed %x", o); }
		else if (rom_0.read(o) != v2) PDBG("Write failed %x", o);
	}

	float r_period = ((float)rom_0.r_tics / rom_0.reads) / THREADS,
	      w_period = ((float)rom_0.w_tics / rom_0.writes) / THREADS;
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
	PINF("   threads        %u", THREADS);
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

	sleep_forever();
}

/*

Ergebnisse
##########

Test 3
------

Gesamte und design-bedingte Dauer von Zugriffen auf emuliertem IOMEM
mit variierender Nutzerzahl an einem Emulator

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

          hdl      restliche emulation
read      0.13 %   5.82 ms
write     0.14 %   5.82 ms


Test 5a
-------

Mit 5 weiteren Threads die nur idle-CPU-Last produzieren.

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          128
[init -> vinit -> test]    writes         64
[init -> vinit -> test]    read tics      66942367
[init -> vinit -> test]    write tics     33454320
[init -> vinit -> test]    avg read tics  522987.250
[init -> vinit -> test]    avg write tics 522723.750
[init -> vinit -> test]    avg read ms    522.986
[init -> vinit -> test]    avg write ms   522.723
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      951
[init -> vinit -> monitor]    write tics     519
[init -> vinit -> monitor]    avg read tics  1.857
[init -> vinit -> monitor]    avg write tics 2.027
[init -> vinit -> monitor]    avg read ms    0.001
[init -> vinit -> monitor]    avg write ms   0.002

Mit 4 weiteren Threads die nur idle-CPU-Last produzieren.

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          128
[init -> vinit -> test]    writes         64
[init -> vinit -> test]    read tics      53706886
[init -> vinit -> test]    write tics     26836590
[init -> vinit -> test]    avg read tics  419585.062
[init -> vinit -> test]    avg write tics 419321.718
[init -> vinit -> test]    avg read ms    419.584
[init -> vinit -> test]    avg write ms   419.321
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      965
[init -> vinit -> monitor]    write tics     512
[init -> vinit -> monitor]    avg read tics  1.884
[init -> vinit -> monitor]    avg write tics 2.000
[init -> vinit -> monitor]    avg read ms    0.001
[init -> vinit -> monitor]    avg write ms   0.002

Mit 3 weiteren Threads die nur idle-CPU-Last produzieren.

t -> vinit -> test] Driver test results:
[init -> vinit -> test]    reads          128
[init -> vinit -> test]    writes         64
[init -> vinit -> test]    read tics      40468115
[init -> vinit -> test]    write tics     20219039
[init -> vinit -> test]    avg read tics  316157.156
[init -> vinit -> test]    avg write tics 315922.500
[init -> vinit -> test]    avg read ms    316.157
[init -> vinit -> test]    avg write ms   315.921
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      943
[init -> vinit -> monitor]    write tics     511
[init -> vinit -> monitor]    avg read tics  1.841
[init -> vinit -> monitor]    avg write tics 1.996
[init -> vinit -> monitor]    avg read ms    0.001
[init -> vinit -> monitor]    avg write ms   0.001


Test 5b
-------

Alle Threads greifen auf den selben IOMEM zu (real vea9x4)

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    threads        1
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      6176309
[init -> vinit -> test]    write tics     3088691
[init -> vinit -> test]    avg read tics  6031.551
[init -> vinit -> test]    avg write tics 6032.599
[init -> vinit -> test]    avg read ms    6.031
[init -> vinit -> test]    avg write ms   6.032
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      7881
[init -> vinit -> monitor]    write tics     4109
[init -> vinit -> monitor]    avg read tics  15.392
[init -> vinit -> monitor]    avg write tics 16.050
[init -> vinit -> monitor]    avg read ms    0.015
[init -> vinit -> monitor]    avg write ms   0.016

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    threads        2
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      12471424
[init -> vinit -> test]    write tics     6235266
[init -> vinit -> test]    avg read tics  6089.562
[init -> vinit -> test]    avg write tics 6089.126
[init -> vinit -> test]    avg read ms    6.089
[init -> vinit -> test]    avg write ms   6.089
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      14867
[init -> vinit -> monitor]    write tics     10542
[init -> vinit -> monitor]    avg read tics  14.518
[init -> vinit -> monitor]    avg write tics 20.589
[init -> vinit -> monitor]    avg read ms    0.014
[init -> vinit -> monitor]    avg write ms   0.020

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    threads        3
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      18713347
[init -> vinit -> test]    write tics     9356573
[init -> vinit -> test]    avg read tics  6091.584
[init -> vinit -> test]    avg write tics 6091.519
[init -> vinit -> test]    avg read ms    6.091
[init -> vinit -> test]    avg write ms   6.091
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      21711
[init -> vinit -> monitor]    write tics     17459
[init -> vinit -> monitor]    avg read tics  14.134
[init -> vinit -> monitor]    avg write tics 22.733
[init -> vinit -> monitor]    avg read ms    0.014
[init -> vinit -> monitor]    avg write ms   0.022

[init -> vinit -> test]    threads        4
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      24873746
[init -> vinit -> test]    write tics     12437056
[init -> vinit -> test]    avg read tics  6072.691
[init -> vinit -> test]    avg write tics 6072.781
[init -> vinit -> test]    avg read ms    6.072
[init -> vinit -> test]    avg write ms   6.072
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      28006
[init -> vinit -> monitor]    write tics     23447
[init -> vinit -> monitor]    avg read tics  13.674
[init -> vinit -> monitor]    avg write tics 22.897
[init -> vinit -> monitor]    avg read ms    0.013
[init -> vinit -> monitor]    avg write ms   0.022

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    threads        5
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      31171319
[init -> vinit -> test]    write tics     15586095
[init -> vinit -> test]    avg read tics  6088.148
[init -> vinit -> test]    avg write tics 6088.318
[init -> vinit -> test]    avg read ms    6.087
[init -> vinit -> test]    avg write ms   6.087
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      34722
[init -> vinit -> monitor]    write tics     30772
[init -> vinit -> monitor]    avg read tics  13.563
[init -> vinit -> monitor]    avg write tics 24.040
[init -> vinit -> monitor]    avg read ms    0.013
[init -> vinit -> monitor]    avg write ms   0.024

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    threads        8
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      49974679
[init -> vinit -> test]    write tics     24986961
[init -> vinit -> test]    avg read tics  6100.424
[init -> vinit -> test]    avg write tics 6100.332
[init -> vinit -> test]    avg read ms    6.099
[init -> vinit -> test]    avg write ms   6.099
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      51396
[init -> vinit -> monitor]    write tics     47187
[init -> vinit -> monitor]    avg read tics  12.547
[init -> vinit -> monitor]    avg write tics 23.040
[init -> vinit -> monitor]    avg read ms    0.012
[init -> vinit -> monitor]    avg write ms   0.023

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    threads        16
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      100018174
[init -> vinit -> test]    write tics     50008260
[init -> vinit -> test]    avg read tics  6104.625
[init -> vinit -> test]    avg write tics 6104.523
[init -> vinit -> test]    avg read ms    6.104
[init -> vinit -> test]    avg write ms   6.104
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      101030
[init -> vinit -> monitor]    write tics     96911
[init -> vinit -> monitor]    avg read tics  12.332
[init -> vinit -> monitor]    avg write tics 23.659
[init -> vinit -> monitor]    avg read ms    0.012
[init -> vinit -> monitor]    avg write ms   0.023

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    threads        32
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      50373314
[init -> vinit -> test]    write tics     25187111
[init -> vinit -> test]    avg read tics  6149.085
[init -> vinit -> test]    avg write tics 6149.197
[init -> vinit -> test]    avg read ms    6.149
[init -> vinit -> test]    avg write ms   6.149
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      51151
[init -> vinit -> monitor]    write tics     46792
[init -> vinit -> monitor]    avg read tics  3.122
[init -> vinit -> monitor]    avg write tics 5.711
[init -> vinit -> monitor]    avg read ms    0.003
[init -> vinit -> monitor]    avg write ms   0.005

[init -> vinit -> test] Driver test results:
[init -> vinit -> test]    threads        32
[init -> vinit -> test]    reads          1024
[init -> vinit -> test]    writes         512
[init -> vinit -> test]    read tics      201904272
[init -> vinit -> test]    write tics     100953155
[init -> vinit -> test]    avg read tics  6161.629
[init -> vinit -> test]    avg write tics 6161.691
[init -> vinit -> test]    avg read ms    6.160
[init -> vinit -> test]    avg write ms   6.160
[init -> vinit -> monitor] Emulator test results:
[init -> vinit -> monitor]    read tics      199090 (*2 due to wrong factor)
[init -> vinit -> monitor]    write tics     195142 (*2 due to wrong factor)
[init -> vinit -> monitor]    avg read tics  12.151 (*2 due to wrong factor)
[init -> vinit -> monitor]    avg write tics 23.821 (*2 due to wrong factor)
[init -> vinit -> monitor]    avg read ms    0.012 (*2 due to wrong factor)
[init -> vinit -> monitor]    avg write ms   0.023 (*2 due to wrong factor)

*/

