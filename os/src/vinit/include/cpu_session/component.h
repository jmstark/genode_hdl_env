/*
 * \brief  Session component of an eavesdropping CPU service
 * \author Martin Stein
 * \date   2012-06-11
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__CPU_SESSION__COMPONENT_H_
#define _INCLUDE__CPU_SESSION__COMPONENT_H_

/* Genode includes */
#include <base/env.h>
#include <base/allocator_guard.h>
#include <cpu_session/client.h>

/* local includes */
#include <cpu_client.h>
#include <spy_session_args.h>

namespace Init
{
	using namespace Genode;

	/**
	 * Session component of an eavesdropping CPU service
	 */
	class Cpu_session_component : public Rpc_object<Cpu_session>
	{
		Spy_session_args<128*1024> _args;     /* args adjustment */
		Cpu_session_client         _backend;  /* backend session */
		Allocator_guard            _md_alloc; /* metadata allocator */

		public:

			/**
			 * Constructor
			 *
			 * \param  args  session arguments
			 * \param  md    metadata allocator
			 */
			Cpu_session_component(const char * args, Allocator * md_alloc) :
				_args(args),
				_backend(env()->parent()->session<Cpu_session>(_args.backend_args)),
				_md_alloc(md_alloc, _args.spy_ram_quota) { }

			/*****************
			 ** Cpu_session **
			 *****************/

			Thread_capability create_thread(Name const &name, addr_t utcb)
			{
				/* let parent create thread */
				Thread_capability thread_cap;
				thread_cap = _backend.create_thread(name, utcb);

				/* if creation failed do not remember this thread */
				if(!thread_cap.valid()) return thread_cap;

				/* remember the new thread and its attributes */
				new (&_md_alloc) Cpu_client(thread_cap, &_backend);
				return thread_cap;
			}

			Ram_dataspace_capability utcb(Thread_capability thread) {
				return _backend.utcb(thread); }

			void kill_thread(Thread_capability thread) {
				_backend.kill_thread(thread); }

			int set_pager(Thread_capability thread, Pager_capability  pager) {
				return _backend.set_pager(thread, pager); }

			int start(Thread_capability thread, addr_t ip, addr_t sp) {
				return _backend.start(thread, ip, sp); }

			void pause(Thread_capability thread) {
				return _backend.pause(thread); }

			void resume(Thread_capability thread) {
				return _backend.resume(thread); }

			void cancel_blocking(Thread_capability thread) {
				return _backend.cancel_blocking(thread); }

			Thread_state state(Thread_capability thread) {
				return _backend.state(thread); }

			void state(Thread_capability thread, Thread_state state) {
				_backend.state(thread, state); }

			void exception_handler(Thread_capability thread,
			                       Signal_context_capability handler) {
				return _backend.exception_handler(thread, handler); }

			void single_step(Thread_capability thread, bool enable) {
				return _backend.single_step(thread, enable); }

			unsigned int num_cpus() const { return _backend.num_cpus(); }

			void affinity(Thread_capability t, unsigned int cpu) {
				_backend.affinity(t, cpu); }
	};
}

#endif /* _INCLUDE__CPU_SESSION__COMPONENT_H_ */

