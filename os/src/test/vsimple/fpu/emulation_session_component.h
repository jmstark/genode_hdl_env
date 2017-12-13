/*
 * \brief   Session component of the emulation service
 * \author  Martin Stein
 * \date    2012-05-04
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _FPU__EMULATION_SESSION_COMPONENT_H_
#define _FPU__EMULATION_SESSION_COMPONENT_H_

/* Genode includes */
#include <base/rpc_server.h>
#include <base/printf.h>
#include <base/thread.h>
#include <base/signal.h>
#include <util/register.h>
#include <util/mmio.h>
#include <rm_session/rm_session.h>

/* local includes */
#include <emulation_session/emulation_session.h>

namespace Emulation
{
	/**
	 * Session component of the emulation service
	 */
	class Session_component : public Rpc_object<Session>
	{
		/**
		 * MMIO endianness types
		 */
		enum Endianness { BIG_ENDIAN };

		enum {
			MMIO_ENDIANNESS = BIG_ENDIAN,
			MMIO_1_BASE = 0x0,
			MMIO_2_BASE = 0x1000,
			MMIO_SIZE = 0x2000,
			IRQ_CALC_DONE = 1,
			CALC_THREAD_STACK_SIZE = 4 * 1024,
		};

		/**
		 * Structure of first emulated MMIO region
		 */
		class Mmio_1 : public Mmio
		{
			public:

				/**
				 * Control register
				 */
				struct Ctrl : Register<0x0, 32>
				{
					struct Mode : Bitfield<0, 1> /* calulation mode */
					{
						enum { ADD = 0, SUBTRACT = 1 };
					};
					struct Calc : Bitfield<1, 1> { }; /* calculation active bit */
					struct Clr_irq : Bitfield<2, 1> { }; /* clear interrupt */
				};

				/**
				 * Constructor
				 */
				Mmio_1(addr_t const base) : Mmio(base) { }
		};

		/**
		 * Structure of second emulated MMIO region
		 */
		class Mmio_2 : public Mmio
		{
			public:

				/**
				 * Calculation argument 1
				 */
				struct Arg_1 : Register<0x0, 32> { };

				/**
				 * Calculation argument 2
				 */
				struct Arg_2 : Register<0x4, 32> { };

				/**
				 * Calculation result
				 */
				struct Result : Register<0x8, 32> { };

				/**
				 * Constructor
				 */
				Mmio_2(addr_t const base) : Mmio(base) { }
		};

		/**
		 * IRQ output line
		 */
		class Irq
		{
			bool _state; /* output state */
			Signal_context_capability _handler; /* IRQ edge handler */

			/**
			 * Negate the IRQ output
			 */
			void _edge()
			{
				_state = !_state;
				if (_handler.valid()) {
					Signal_transmitter(_handler).submit();
				}
			}

			public:

				/**
				 * Constructor
				 */
				Irq() : _state(0) { }

				/**
				 * Raise output if it is 0
				 */
				void up() { if (!_state) _edge(); }

				/**
				 * Level down output if it is 1
				 */
				void down() { if (_state) _edge(); }

				/***************
				 ** Accessors **
				 ***************/

				bool state() const { return _state; }

				void handler(Signal_context_capability handler)
				{ _handler = handler; }
		};

		/**
		 * IRQ output line with synchronized access
		 */
		class Synchronized_irq
		{
			Irq _irq; /* asynchronous IRQ output */
			Lock _lock; /* synchronize access to '_irq' */

			public:

				/*******************
				 ** Irq interface **
				 *******************/

				void handler(Signal_context_capability handler)
				{
					Lock::Guard guard(_lock);
					_irq.handler(handler);
				}

				bool state()
				{
					Lock::Guard guard(_lock);
					return _irq.state();
				}

				void up()
				{
					Lock::Guard guard(_lock);
					_irq.up();
				}

				void down()
				{
					Lock::Guard guard(_lock);
					_irq.down();
				}
		};

		/**
		 * Thread that does FPU calculations in parallel with client access
		 */
		class Calculation_thread : public Thread<CALC_THREAD_STACK_SIZE>
		{
			Session_component * const _emu; /* our creator */

			public:

				/**
				 * Construct a valid calculation thread
				 */
				Calculation_thread(Session_component * const emu)
				: _emu(emu) { }


				/**********************
				 ** Thread interface **
				 **********************/

				void entry()
				{
                    return;
				}
		};

		/* MMIO backing store plus 1 word at the end for overhanging writes */
		char _mmio[MMIO_SIZE + sizeof(umword_t)];

		Mmio_1 _mmio_1;
		Mmio_2 _mmio_2;
		Synchronized_irq _calc_done;
		bool _old_calc;
		Signal_context _calc_up;
		Signal_context _ack_calc_up;
		Signal_receiver _receiver;
		Signal_context_capability _calc_up_cap;
		Signal_context_capability _ack_calc_up_cap;
		Calculation_thread _calc_thread;


		public:

			/**
			 * Construct a valid session component
			 */
			Session_component() :
				_mmio_1((addr_t)&_mmio[MMIO_1_BASE]),
				_mmio_2((addr_t)&_mmio[MMIO_2_BASE]),
				_calc_up_cap(_receiver.manage(&_calc_up)),
				_ack_calc_up_cap(_receiver.manage(&_ack_calc_up)),
				_calc_thread(this)
			{
				/* zero-fill MMIO and start the calculation thread */
				for (unsigned i = 0; i < sizeof(_mmio)/sizeof(_mmio[0]); i++) _mmio[i] = 0;
				_calc_thread.start();
			}


			/***********************
			 ** Session interface **
			 ***********************/

			void write_mmio(addr_t const off, Access const a,
			                umword_t const value)
			{
                PINF("WRITE addr %lx, value %lu\n", off, value);
                return;
			}

			umword_t read_mmio(addr_t const off, Access const a)
			{
                PINF("READ addr %lx\n", off);
                return 12345678;
			}

			bool irq_handler(unsigned const irq,
			                 Signal_context_capability irq_edge)
			{
				if (irq == IRQ_CALC_DONE) {
					_calc_done.handler(irq_edge);
					return _calc_done.state();
				}
				else PERR("%s:%d: Invalid IRQ", __FILE__, __LINE__);
				return 0;
			}
	};
}

#endif /* _FPU__EMULATION_SESSION_COMPONENT_H_ */

