/*
 * \brief  Fiasco.OC-specific implementation of core's startup Thread API.
 * \author Norman Feske
 * \author Stefan Kalkowski
 * \date   2006-05-03
 */

/*
 * Copyright (C) 2006-2011 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/thread.h>
#include <base/sleep.h>

/* core includes */
#include <cap_session_component.h>
#include <platform.h>
#include <core_env.h>

namespace Fiasco {
#include <l4/sys/debugger.h>
#include <l4/sys/factory.h>
}

using namespace Genode;


void Thread_base::_deinit_platform_thread()
{
	PWRN("%s: not implemented yet!", __func__);
}


void Thread_base::start()
{
	using namespace Fiasco;

	/* create and start platform thread */
	Platform_thread *pt =
		new(platform()->core_mem_alloc()) Platform_thread(_context->name);

	platform_specific()->core_pd()->bind_thread(pt);
	_tid = pt->gate().dst();
	_thread_cap = reinterpret_cap_cast<Cpu_thread>(pt->thread_cap());

	pt->pager(platform_specific()->core_pager());
	pt->start((void *)_thread_start, _context->stack);

	/*
	 * send newly constructed thread, pointer to its Thread_base object,
	 * and its badge
	 */
	Msgbuf<128> snd_msg, rcv_msg;
	Ipc_client cli(_thread_cap, &snd_msg, &rcv_msg);
	cli << (addr_t)this << pt->gate().local_name() << IPC_CALL;
}


void Thread_base::cancel_blocking()
{
	/*
	 * Within core, we never need to unblock threads
	 */
}