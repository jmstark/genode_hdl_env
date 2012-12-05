/*
 * \brief  Wishbone slave protocol
 * \author Martin Stein
 * \date   2012-12-05
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__VERILATOR_ENV__WISHBONE_SLAVE_H_
#define _INCLUDE__VERILATOR_ENV__WISHBONE_SLAVE_H_

/* Genode includes */
#include <rm_session/rm_session.h>
#include <base/lock.h>

/* Verilator includes */
#include <verilated.h>

void evaluate_hdl();

namespace Genode
{
	/**
	 * Wishbone slave protocol
	 *
	 * \param RAW      type that provides the raw wishbone interface
	 * \param TIMEOUT  maximum cycles a transfer may take (0 for endless)
	 *
	 * FIXME would be nice if this class wouldn't be a template
	 */
	template <typename RAW, unsigned TIMEOUT>
	class Wishbone_slave : public RAW
	{
		typedef Rm_session::Access_format Access;

		/**
		 * Do a reset cycle at a wishbone slave
		 */
		void _reset()
		{
			/* apply low reset signal */
			RAW::rst_i() = 0;
			evaluate_hdl();

			/* raise reset and simulate reaction of the master */
			RAW::rst_i() = 1;
			RAW::cyc_i() = 0;
			RAW::stb_i() = 0;
			evaluate_hdl();

			/* end reset cycle */
			RAW::rst_i() = 0;
		}

		/**
		 * Give the slave an additional cycle for transfer
		 */
		void _transfer_pending(unsigned cycles)
		{
			assert(!RAW::err_o() && !RAW::rty_o());
			if (TIMEOUT) {
				assert(cycles < TIMEOUT);
				cycles++;
			}
			RAW::cycle();
		}

		/**
		 * End a transfer
		 *
		 * FIXME might be unnecessary
		 */
		void _transfer_end()
		{
			RAW::cyc_i() = 0;
			RAW::stb_i() = 0;
			RAW::cycle();
		}

		/**
		 * Start a transfer
		 */
		void _transfer_start(addr_t const addr, Access const a, bool const writes)
		{
			switch(a) {
			case Rm_session::LSB8:  RAW::sel_i(0b1);    break;
			case Rm_session::LSB16: RAW::sel_i(0b11);   break;
			case Rm_session::LSB32: RAW::sel_i(0b1111); break;
			default: assert(0);
			}
			RAW::adr_i(addr);
			RAW::stb_i() = 1;
			RAW::cyc_i() = 1;
			RAW::we_i() = writes;
			RAW::cycle();
		}

		public:

			/**********************************
			 ** Emulation::Session_component **
			 **********************************/

			void initialize() { _reset(); }

			umword_t read_mmio(addr_t const addr, Access const a)
			{
				_transfer_start(addr, a, 0);

				/* wait for feedback from the slave  */
				unsigned cycles = 0;
				while (1) {
					if (RAW::ack_o()) {
						uint32_t result;
						RAW::dat_o(&result);
						_transfer_end();
						return result;
					}
					_transfer_pending(cycles);
				}
			}

			void write_mmio(addr_t const addr, Access const a,
			                umword_t const value)
			{
				RAW::dat_i(value);
				_transfer_start(addr, a, 1);

				/* wait for feedback from the slave */
				unsigned cycles = 0;
				while (1) {
					if (RAW::ack_o()) {
						_transfer_end();
						return;
					}
					_transfer_pending(cycles);
				}
			}
	};

	/**
	 * Wishbone protocol with sync access to the raw interface
	 */
	template <typename RAW, unsigned TIMEOUT>
	class Sync_wishbone_slave : Wishbone_slave<RAW, TIMEOUT>
	{
		typedef Wishbone_slave<RAW, TIMEOUT> Async;
		typedef Rm_session::Access_format Access;

		Lock * const _lock;

		public:

			/**
			 * Constructor
			 *
			 * \param lock  raw interface access lock
			 */
			Sync_wishbone_slave(Lock * const lock) :
				 _lock(lock) { }


			/**********************************
			 ** Emulation::Session_component **
			 **********************************/

			void initialize()
			{
				Lock::Guard guard(*_lock);
				Async::initialize();
			}

			umword_t read_mmio(addr_t const addr, Access const a)
			{
				Lock::Guard guard(*_lock);
				return Async::read_mmio(addr, a);
			}

			void write_mmio(addr_t const addr, Access const a,
			                umword_t const value)
			{
				Lock::Guard guard(*_lock);
				Async::write_mmio(addr, a, value);
			}
	};
}

#endif /* _INCLUDE__VERILATOR_ENV__WISHBONE_SLAVE_H_ */

