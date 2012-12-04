/*
 * \brief   Test emulated PTC
 * \author  Martin Stein
 * \date    2012-12-04
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <io_mem_session/connection.h>
#include <irq_session/connection.h>
#include <util/mmio.h>

using namespace Genode;

//	uint16_t read_h(unsigned const o)
//	{
//		uint16_t v;
//		asm volatile ("ldrh %[v], [%[d]]"
//		              : [v]"=r"(v) : [d]"r"(base + o) : );
//		return v;
//	}
//
//	void write_h(unsigned const o, uint16_t const v)
//	{
//		asm volatile ("strh %[v], [%[d]]"
//		              :: [v]"r"(v), [d]"r"(base + o) : );
//	}

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

int main(int argc, char **argv)
{
	PINF("ptc driver");

	Irq_connection ptc_irq(100);
	Io_mem_connection ptc_io_mem(0x71000000, 0x1000);
	Rm_session * const rm = env()->rm_session();
	addr_t ptc_base = (addr_t)rm->attach(ptc_io_mem.dataspace());

	Ptc ptc(ptc_base);
	ptc.write<Ptc::Cntr>(0x0);
	ptc.write<Ptc::Hrc>(0x12345);
	ptc.write<Ptc::Lrc>(0x12345);
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	ptc.write<Ptc::Ctrl>(Ptc::Ctrl::start_timer());
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	ptc_irq.wait_for_irq();
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	ptc_irq.wait_for_irq();
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());
	PINF("PTC %x %x", ptc.read<Ptc::Cntr>(), ptc.read<Ptc::Ctrl>());

	while(1);
}

