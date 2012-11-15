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
wird der Timer aktiviert. Der Client nimmt dann die Zeit vom Timer sobald
er deblockiert. Dies gilt jedoch nur für den Programmfluss der der Messung
dient. Die parallelen Dummy Threads erzeugen Last indem sie auf ihren jeweils
eigenen IRQ zugreifen, welcher zwar auch über den Signal-Mechanismus, aber
nicht zeitversetzt getriggert wird.

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       1
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      4248
[init -> vinit -> test]    avg IRQ tics  42.479
[init -> vinit -> test]    avg IRQ ms    0.042

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       2
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      9042
[init -> vinit -> test]    avg IRQ tics  45.209
[init -> vinit -> test]    avg IRQ ms    0.045

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       4
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      19066
[init -> vinit -> test]    avg IRQ tics  47.665
[init -> vinit -> test]    avg IRQ ms    0.047

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       8
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      39789
[init -> vinit -> test]    avg IRQ tics  49.736
[init -> vinit -> test]    avg IRQ ms    0.049

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       12
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      60877
[init -> vinit -> test]    avg IRQ tics  50.730
[init -> vinit -> test]    avg IRQ ms    0.050

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       16
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      81663
[init -> vinit -> test]    avg IRQ tics  51.039
[init -> vinit -> test]    avg IRQ ms    0.051

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       20
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      101970
[init -> vinit -> test]    avg IRQ tics  50.985
[init -> vinit -> test]    avg IRQ ms    0.050

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       24
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      123592
[init -> vinit -> test]    avg IRQ tics  51.496
[init -> vinit -> test]    avg IRQ ms    0.051

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       28
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      145518
[init -> vinit -> test]    avg IRQ tics  51.970
[init -> vinit -> test]    avg IRQ ms    0.051

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       32
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      166402
[init -> vinit -> test]    avg IRQ tics  52.000
[init -> vinit -> test]    avg IRQ ms    0.052

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       36
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      190003
[init -> vinit -> test]    avg IRQ tics  52.778
[init -> vinit -> test]    avg IRQ ms    0.052

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       40
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      209949
[init -> vinit -> test]    avg IRQ tics  52.487
[init -> vinit -> test]    avg IRQ ms    0.052

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       44
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      230948
[init -> vinit -> test]    avg IRQ tics  52.488
[init -> vinit -> test]    avg IRQ ms    0.052

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       48
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      251656
[init -> vinit -> test]    avg IRQ tics  52.428
[init -> vinit -> test]    avg IRQ ms    0.052

[init -> vinit -> test] Test result
[init -> vinit -> test]    tasks         1
[init -> vinit -> test]    threads       52
[init -> vinit -> test]    IRQs received 100
[init -> vinit -> test]    IRQ tics      274855
[init -> vinit -> test]    avg IRQ tics  52.856
[init -> vinit -> test]    avg IRQ ms    0.052

*/

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

enum { MY_THREADS = 52, ALL_THREADS = 52, EMULATED_PROGRAMS = 1, ROM_0_BASE = 0x71000000 };
static unsigned threads_started;

struct Test_thread : Thread<1*1024>
{
	Lock * lock;
	Lock * ready;
	Irq_connection irq;
	unsigned id;

	Test_thread(Lock * l1, Lock * l2, unsigned i) :
		lock(l1), ready(l2), irq(i), id(i-100)
	{
		Thread::start();
	}

	void entry()
	{
		/* sync startup with all initialization */
		lock->lock();
		irq.wait_for_irq(Dataspace_capability());
		threads_started++;
		PINF("Thread %u", id);
		if (threads_started == MY_THREADS) ready->unlock();
		else if (threads_started > MY_THREADS) PERR("To much threads");
		else lock->unlock();

		/* produce load on resource */
		while (1) irq.wait_for_irq(Dataspace_capability());
	}
};

static Watch watch(0);
static unsigned tics = 0;

int main(int argc, char **argv)
{
	printf("--- test device emulation ---\n");

	static Lock start(Lock::LOCKED);
	static Lock ready(Lock::LOCKED);
	threads_started = 1;
	if (threads_started == MY_THREADS) ready.unlock();
	Test_thread  t2(&start, &ready, 102);
	Test_thread  t3(&start, &ready, 103);
	Test_thread  t4(&start, &ready, 104);
	Test_thread  t5(&start, &ready, 105);
	Test_thread  t6(&start, &ready, 106);
	Test_thread  t7(&start, &ready, 107);
	Test_thread  t8(&start, &ready, 108);
	Test_thread  t9(&start, &ready, 109);
	Test_thread  t10(&start, &ready, 110);
	Test_thread  t11(&start, &ready, 111);
	Test_thread  t12(&start, &ready, 112);
	Test_thread  t13(&start, &ready, 113);
	Test_thread  t14(&start, &ready, 114);
	Test_thread  t15(&start, &ready, 115);
	Test_thread  t16(&start, &ready, 116);
	Test_thread  t17(&start, &ready, 117);
	Test_thread  t18(&start, &ready, 118);
	Test_thread  t19(&start, &ready, 119);
	Test_thread  t20(&start, &ready, 120);
	Test_thread  t21(&start, &ready, 121);
	Test_thread  t22(&start, &ready, 122);
	Test_thread  t23(&start, &ready, 123);
	Test_thread  t24(&start, &ready, 124);
	Test_thread  t25(&start, &ready, 125);
	Test_thread  t26(&start, &ready, 126);
	Test_thread  t27(&start, &ready, 127);
	Test_thread  t28(&start, &ready, 128);
	Test_thread  t29(&start, &ready, 129);
	Test_thread  t30(&start, &ready, 130);
	Test_thread  t31(&start, &ready, 131);
	Test_thread  t32(&start, &ready, 132);
	Test_thread  t33(&start, &ready, 133);
	Test_thread  t34(&start, &ready, 134);
	Test_thread  t35(&start, &ready, 135);
	Test_thread  t36(&start, &ready, 136);
	Test_thread  t37(&start, &ready, 137);
	Test_thread  t38(&start, &ready, 138);
	Test_thread  t39(&start, &ready, 139);
	Test_thread  t40(&start, &ready, 140);
	Test_thread  t41(&start, &ready, 141);
	Test_thread  t42(&start, &ready, 142);
	Test_thread  t43(&start, &ready, 143);
	Test_thread  t44(&start, &ready, 144);
	Test_thread  t45(&start, &ready, 145);
	Test_thread  t46(&start, &ready, 146);
	Test_thread  t47(&start, &ready, 147);
	Test_thread  t48(&start, &ready, 148);
	Test_thread  t49(&start, &ready, 149);
	Test_thread  t50(&start, &ready, 150);
	Test_thread  t51(&start, &ready, 151);
	Test_thread  t52(&start, &ready, 152);
	start.unlock();
	ready.lock();

	static Irq_connection irq(101);
	irq.wait_for_irq(watch.Io_mem_connection::dataspace());
	PINF("Measurement started");

	enum { TESTS = 100 };

	for(unsigned i = 0; i< TESTS; i++)
	{
		irq.wait_for_irq(watch.Io_mem_connection::dataspace());
		tics += watch.read();
	}

	float avg_tics      = ((float)tics / TESTS) / ALL_THREADS;
	float avg_ms        = Watch::tics_to_ms(avg_tics);
	unsigned avg_tics_l = (unsigned)avg_tics;
	unsigned avg_tics_r = 1000 * (avg_tics - avg_tics_l);
	unsigned avg_ms_l   = (unsigned)avg_ms;
	unsigned avg_ms_r   = 1000 * (avg_ms - avg_ms_l);

	PINF("Test result");
	PINF("   tasks         %u", EMULATED_PROGRAMS);
	PINF("   threads       %u", ALL_THREADS);
	PINF("   IRQs received %u", TESTS);
	PINF("   IRQ tics      %u", tics);
	PINF("   avg IRQ tics  %u.%03u", avg_tics_l, avg_tics_r);
	PINF("   avg IRQ ms    %u.%03u", avg_ms_l, avg_ms_r);

	while(1);
}

