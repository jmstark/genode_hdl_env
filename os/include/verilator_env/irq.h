/*
 * \brief   Provide HDL interrupt lines
 * \author  Martin Stein
 * \date    2013-12-05
 */

#ifndef _INCLUDE__VERILATOR_ENV__IRQ_H_
#define _INCLUDE__VERILATOR_ENV__IRQ_H_

/* Genode includes */
#include <base/signal.h>
#include <base/printf.h>

/* verilator_env includes */
#include <verilator_env/driven_clock.h>

namespace Genode
{
	/**
	 * HDL interrupt
	 */
	class Irq
	{
		uint8_t * _raw;
		Signal_context_capability _signal;
		Lock _lock;

		public:

			/**
			 * Constructor
			 *
			 * \param raw  raw HDL interrupt line
			 */
			Irq(uint8_t * const raw) : _raw(raw) { }

			/**
			 * Set the IRQ-handler signal
			 */
			void signal(Signal_context_capability signal)
			{
				Lock::Guard guard(_lock);
				_signal = signal;
			}

			/**
			 * Trigger the IRQ-handler signal if HDL interrupt is active
			 */
			void check()
			{
				Lock::Guard guard(_lock);
				if (*_raw && _signal.valid()) {
					Signal_transmitter t(_signal);
					t.submit();
				}
			}


			/***************
			 ** Accessors **
			 ***************/

			bool raw() { return *_raw; }
	};

	/**
	 * Enable users to listen to items out of a HDL interrupt group
	 */
	class Irq_listener
	{
		Irq * const _irqs;
		unsigned const _irqs_size;

		public:

			/**
			 * Constructor
			 *
			 * \param irqs        base of the HDL interrupt array
			 * \param irqs_size   size of the HDL interrupt array
			 */
			Irq_listener(Irq * const irqs, unsigned const irqs_size = 1) :
				_irqs(irqs), _irqs_size(irqs_size) { }


			/**********************************
			 ** Emulation::Session_component **
			 **********************************/

			bool irq_handler(unsigned i, Signal_context_capability signal)
			{
				if (i < _irqs_size) {
					_irqs[i].signal(signal);
					return _irqs[i].raw();
				}
				PDBG("Unknown IRQ %u", i);
				return 0;
			}
	};

	/**
	 * A driven clock that might trigger multiple HDL interrupt lines
	 */
	class Irq_clock : private Driven_clock_base
	{
		Irq * const _irqs;
		unsigned const _irqs_size;

		public:

			/**
			 * Constructor
			 *
			 * \param raw         raw HDL clock line
			 * \param up          if the clock is up-edge or down-edge triggered
			 * \param freq_ms     clock frequency per ms
			 * \param interval_ms delay between clock updates
			 * \param lock        clock access lock
			 * \param irqs        base of the HDL interrupt array
			 * \param irqs_size   size of the HDL interrupt array
			 */
			Irq_clock(uint8_t * const raw, bool up, unsigned const freq_ms,
			          unsigned const interval_ms, Lock * const lock,
			          Irq * const irqs, unsigned const irqs_size = 1) :
				Driven_clock_base(raw, up, freq_ms, interval_ms, lock),
				_irqs(irqs), _irqs_size(irqs_size) { }


			/***********************
			 ** Driven_clock_base **
			 ***********************/

			void cycle()
			{
				_cnt++;
				_clk.cycle();
				for (unsigned i = 0; i < _irqs_size; i++) _irqs[i].check();
			}
	};
}

#endif /* _INCLUDE__VERILATOR_ENV__IRQ_H_ */

