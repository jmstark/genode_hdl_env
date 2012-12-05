/*
 * \brief  Drive HDL clock signal periodically
 * \author Martin Stein
 * \date   2012-12-05
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__VERILATOR_ENV__DRIVEN_CLOCK_H_
#define _INCLUDE__VERILATOR_ENV__DRIVEN_CLOCK_H_

/* Genode includes */
#include <base/lock.h>
#include <base/thread.h>
#include <timer_session/connection.h>

/* verilator_env includes */
#include <verilator_env/clock.h>

namespace Genode
{
	/**
	 * Drive HDL clock signal periodically
	 */
	class Driven_clock_base : public Thread<1024>
	{
		Lock * const _lock;
		unsigned const _interval_ms;
		unsigned const _interval_cnt;
		Timer::Connection _timer;

		protected:

			Clock _clk;
			unsigned _cnt;

		public:

			/**
			 * Constructor
			 *
			 * \param raw         raw HDL clock line
			 * \param up          if the clock is up-edge or down-edge triggered
			 * \param freq_ms     clock frequency per ms
			 * \param interval_ms delay between clock updates
			 * \param lock        clock access lock
			 */
			Driven_clock_base(uint8_t * const raw, bool up,
			                  unsigned const freq_ms,
			                  unsigned const interval_ms, Lock * const lock) :
				_lock(lock), _interval_ms(interval_ms),
				_interval_cnt(_interval_ms * freq_ms), _clk(raw, up), _cnt(0)
			{
				Thread::start();
			}

			/**
			 * Destructor
			 */
			virtual ~Driven_clock_base() { }


			/***********
			 ** Clock **
			 ***********/

			virtual void cycle() = 0;


			/************
			 ** Thread **
			 ************/

			void entry()
			{
				while(1)
				{
					_timer.msleep(_interval_ms);
					Lock::Guard guard(*_lock);
					while (_cnt < _interval_cnt) cycle();
					_cnt = 0;
				}
			}
	};

	/**
	 * Drive HDL clock signal periodically
	 */
	class Driven_clock : private Driven_clock_base
	{
		public:

			/**
			 * Constructor
			 *
			 * \param raw         raw HDL clock line
			 * \param up          if the clock is up-edge or down-edge triggered
			 * \param freq_ms     clock frequency per ms
			 * \param interval_ms delay between clock updates
			 * \param lock        clock access lock
			 */
			Driven_clock(uint8_t * const raw, bool up, unsigned const freq_ms,
			             unsigned const interval_ms, Lock * const lock) :
				Driven_clock_base(raw, up, freq_ms, interval_ms, lock) { }


			/***********************
			 ** Driven_clock_base **
			 ***********************/

			void cycle()
			{
				_cnt++;
				_clk.cycle();
			}
	};
}

#endif /* _INCLUDE__VERILATOR_ENV__DRIVEN_CLOCK_H_ */

