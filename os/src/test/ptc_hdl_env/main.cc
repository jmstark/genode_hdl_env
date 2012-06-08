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

#include <timer_session/client.h>
#include <base/connection.h>

namespace Timer {

	class Connection : public Genode::Connection<Session>, public Session_client
	{
		private:

			Lock                      _lock;
			Signal_receiver           _sig_rec;
			Signal_context            _default_sigh_ctx;
			Signal_context_capability _default_sigh_cap;
			Signal_context_capability _custom_sigh_cap;

		public:

			Connection()
			:
				Genode::Connection<Session>(session("ram_quota=8K, type=ptc")),
				Session_client(cap()),
				_default_sigh_cap(_sig_rec.manage(&_default_sigh_ctx))
			{
				/* register default signal handler */
				Session_client::sigh(_default_sigh_cap);
			}

			~Connection() { _sig_rec.dissolve(&_default_sigh_ctx); }

			/*
			 * Intercept 'sigh' to keep track of customized signal handlers
			 */
			void sigh(Signal_context_capability sigh)
			{
				_custom_sigh_cap = sigh;
				Session_client::sigh(_custom_sigh_cap);
			}

			void usleep(unsigned us)
			{
				/* serialize sleep calls issued by different threads */
				Lock::Guard guard(_lock);

				/* temporarily install to the default signal handler */
				if (_custom_sigh_cap.valid())
					Session_client::sigh(_default_sigh_cap);

				/* trigger timeout at default signal handler */
				trigger_once(us);
				_sig_rec.wait_for_signal();

				/* revert custom signal handler if registered */
				if (_custom_sigh_cap.valid())
					Session_client::sigh(_custom_sigh_cap);
			}

			void msleep(unsigned ms)
			{
				usleep(1000*ms);
			}
	};
}

///* Genode includes */
//#include <io_mem_session/connection.h>
//#include <irq_session/connection.h>
//#include <util/mmio.h>
//
//using namespace Genode;
//
//struct Ptc : Mmio
//{
//	struct Cntr : Register<0x0, 32> { };
//	struct Hrc  : Register<0x4, 32> { };
//	struct Lrc  : Register<0x8, 32> { };
//	struct Ctrl : Register<0xc, 32>
//	{
//		struct En     : Bitfield<0, 1> { };
//		struct Eclk   : Bitfield<1, 1> { };
//		struct Nec    : Bitfield<2, 1> { };
//		struct Oe     : Bitfield<3, 1> { };
//		struct Single : Bitfield<4, 1> { };
//		struct Inte   : Bitfield<5, 1> { };
//		struct Int    : Bitfield<6, 1> { };
//		struct Cntrst : Bitfield<7, 1> { };
//		struct Capte  : Bitfield<8, 1> { };
//
//		static access_t start_timer() {
//			return En::bits(1) |
//			       Eclk::bits(0) |
//			       Nec::bits(0) |
//			       Oe::bits(0) |
//			       Single::bits(0) |
//			       Inte::bits(1) |
//			       Int::bits(0) |
//			       Cntrst::bits(0) |
//			       Capte::bits(0); }
//	};
//
//	Ptc(addr_t base) : Mmio(base) { }
//};

int main(int argc, char **argv)
{
//	PINF("ptc driver");
//
//	Irq_connection ptc_irq(100);
//	Io_mem_connection ptc_io_mem(0x71000000, 0x1000);
//	Rm_session * const rm = env()->rm_session();
//	addr_t ptc_base = (addr_t)rm->attach(ptc_io_mem.dataspace());
//
//	Ptc ptc(ptc_base);
//	ptc.write<Ptc::Cntr>(0xffffffff - 0x800);
//	ptc.write<Ptc::Hrc>(0xffffffff);
//	ptc.write<Ptc::Lrc>(0xffffffff);
//	ptc.write<Ptc::Ctrl>(Ptc::Ctrl::start_timer());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	ptc_irq.wait_for_irq();
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//	PINF("TIC %x", ptc.read<Ptc::Cntr>());
//
//	ptc.write<Ptc::Ctrl>(Ptc::Ctrl::start_timer());
//	ptc_irq.wait_for_irq();
//	PINF("TIC");
//	ptc.write<Ptc::Ctrl>(Ptc::Ctrl::start_timer());
//	ptc_irq.wait_for_irq();
//	PINF("TIC");
//	ptc.write<Ptc::Ctrl>(Ptc::Ctrl::start_timer());
//	ptc_irq.wait_for_irq();
//	PINF("TIC");

	static Timer::Connection timer;
	timer.msleep(2000);
	PINF("Hallo %p", &timer);
	while(1);
}

