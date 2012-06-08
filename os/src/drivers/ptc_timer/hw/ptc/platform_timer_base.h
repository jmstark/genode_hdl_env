/*
 * \brief  Basic driver behind platform timer
 * \author Stefan Kalkowski
 * \date   2012-10-25
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _HW__PTC__PLATFORM_TIMER_BASE_H_
#define _HW__PTC__PLATFORM_TIMER_BASE_H_

/* Genode includes */
#include <io_mem_session/connection.h>
#include <util/mmio.h>
#include <irq_session/connection.h>
#include <drivers/board_base.h>

namespace Genode
{
	struct Ptc : Mmio
	{
		struct Cntr : Register<0x0, 32> { };
		struct Hrc  : Register<0x4, 32> { };
		struct Lrc  : Register<0x8, 32> { };
		struct Ctrl : Register<0xc, 32>
		{
			struct En     : Bitfield<0, 1> { };
			struct Eclk   : Bitfield<1, 1> { };
			struct Nec    : Bitfield<2, 1> { };
			struct Oe     : Bitfield<3, 1> { };
			struct Single : Bitfield<4, 1> { };
			struct Inte   : Bitfield<5, 1> { };
			struct Int    : Bitfield<6, 1> { };
			struct Cntrst : Bitfield<7, 1> { };
			struct Capte  : Bitfield<8, 1> { };

			static access_t start_timer() {
				return En::bits(1) |
					   Eclk::bits(0) |
					   Nec::bits(0) |
					   Oe::bits(0) |
					   Single::bits(0) |
					   Inte::bits(1) |
					   Int::bits(0) |
					   Cntrst::bits(0) |
					   Capte::bits(0); }
		};

		Ptc(addr_t base) : Mmio(base) { }
	};
}

/**
 * Basic driver behind platform timer
 */
class Platform_timer_base : public Genode::Io_mem_connection,
                            public Genode::Ptc
{
	private:

		enum {
			MMIO_BASE   = 0x71000000,
			MMIO_SIZE   = 0x1000,
			TICS_PER_MS = 100
		};

		/**
		 * Timer tics per microsecond
		 */
		static float tics_per_us() {
			return (float)TICS_PER_MS / 1000.0; }

		/**
		 * Microseconds per timer tic
		 */
		static float us_per_tic() { return 1.0 / tics_per_us(); }

	public:

		enum { IRQ = 100 };

		/**
		 * Count down 'value', raise IRQ output, wrap counter and continue
		 */
		void run_and_wrap(unsigned long value)
		{
			Ptc::write<Ptc::Cntr>(max_value() - value);
			Ptc::write<Ptc::Hrc>(max_value());
			Ptc::write<Ptc::Lrc>(max_value());
			Ptc::write<Ptc::Ctrl>(Ptc::Ctrl::start_timer());
		}

		/**
		 * Maximum timeout value
		 */
		unsigned long max_value() const { return 0xffffffff; }

		/**
		 * Translate timer tics to microseconds
		 */
		unsigned long tics_to_us(unsigned long const tics) const
		{
			float const us = tics * us_per_tic();
			return (unsigned long)us;
		}

		/**
		 * Translate microseconds to timer tics
		 */
		unsigned long us_to_tics(unsigned long const us)
		{
			float const tics = us * tics_per_us();
			return (unsigned long)tics;
		}

		/**
		 * Sample the timer counter and according wrapped status
		 */
		unsigned long value(bool & wrapped) const
		{
			unsigned long v = read<Cntr>();
			wrapped = (bool)read<Ptc::Ctrl::Int>();
			return wrapped ? max_value() - read<Cntr>() : v - max_value();
		}


		/**
		 * Constructor
		 */
		Platform_timer_base()
		: Io_mem_connection(MMIO_BASE, MMIO_SIZE),
		  Ptc((Genode::addr_t)Genode::env()->rm_session()->attach(dataspace()))
		{ PDBG("."); }
};

#endif /* _HW__PTC__PLATFORM_TIMER_BASE_H_ */

