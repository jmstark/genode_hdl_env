/*
 * \brief  Region manager session interface
 * \author Norman Feske
 * \date   2006-05-15
 */

/*
 * Copyright (C) 2006-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__RM_SESSION__RM_SESSION_H_
#define _INCLUDE__RM_SESSION__RM_SESSION_H_

#include <base/exception.h>
#include <base/stdint.h>
#include <base/signal.h>
#include <dataspace/capability.h>
#include <thread/capability.h>
#include <pager/capability.h>
#include <session/session.h>
#include <base/printf.h>

namespace Genode {

	struct Rm_session : Session
	{
		enum Fault_type {
			READY = 0, READ_FAULT = 1, WRITE_FAULT = 2, EXEC_FAULT = 3 };

		enum Access_format { LSB8, LSB16, LSB32 };

		/**
		 * State of region-manager session
		 *
		 * If a client accesses a location outside the regions attached to
		 * the region-manager session, a fault occurs and gets signalled to
		 * the registered fault handler. The fault handler, in turn needs
		 * the information about the fault address and fault type to
		 * resolve the fault. This information is represented by this
		 * structure.
		 */
		struct State
		{
			/**
			 * Type of occurred fault
			 */
			Fault_type type;

			/**
			 * Format of memoryaccess
			 */
			Access_format format;

			/**
			 * Fault address
			 */
			addr_t addr;

			/**
			 * Might be used by a pager to identify a faulter
			 */
			unsigned imprint;

			/**
			 * Value wich is requested to be written, or in return read
			 */
			unsigned value;

			/**
			 * Faulting instruction pointer within the 'core' space
			 */
			unsigned * instr;

			/**
			 * Default constructor
			 */
			State() : type(READY), addr(0), value(0) { }

			/**
			 * Constructor
			 */
			State(Fault_type fault_type, addr_t fault_addr,
			      unsigned const imprint,
			      unsigned const fault_value = 0,
			      unsigned * const fault_instr = 0)
			:
				type(fault_type), addr(fault_addr), value(fault_value),
				instr(fault_instr)
			{ }

			/**
			 * Compare two states for eqality
			 */
			bool operator == (State & s)
			{
				return type == s.type &&
				       addr == s.addr;
			};
		};


		/**
		 * Helper for tranferring the bit representation of a pointer as RPC
		 * argument.
		 */
		class Local_addr
		{
			private:

				void *_ptr;

			public:

				template <typename T>
				Local_addr(T ptr) : _ptr((void *)ptr) { }

				Local_addr() : _ptr(0) { }

				template <typename T>
				operator T () { return (T)_ptr; }
		};


		static const char *service_name() { return "RM"; }


		/*********************
		 ** Exception types **
		 *********************/

		class Attach_failed     : public Exception     { };
		class Invalid_args      : public Attach_failed { };
		class Invalid_dataspace : public Attach_failed { };
		class Region_conflict   : public Attach_failed { };
		class Out_of_metadata   : public Attach_failed { };

		class Invalid_thread    : public Exception { };

		/**
		 * Destructor
		 */
		virtual ~Rm_session() { }

		/**
		 * Map dataspace into local address space
		 *
		 * \param ds               capability of dataspace to map
		 * \param size             size of the locally mapped region
		 *                         default (0) is the whole dataspace
		 * \param offset           start at offset in dataspace (page-aligned)
		 * \param use_local_addr   if set to true, attach the dataspace at
		 *                         the specified 'local_addr'
		 * \param local_addr       local destination address
		 * \param executable       if the mapping should be executable
		 *
		 * \throw Attach_failed    if dataspace or offset is invalid,
		 *                         or on region conflict
		 * \throw Out_of_metadata  if meta-data backing store is exhausted
		 *
		 * \return                 local address of mapped dataspace
		 *
		 */
		virtual Local_addr attach(Dataspace_capability ds,
		                          size_t size = 0, off_t offset = 0,
		                          bool use_local_addr = false,
		                          Local_addr local_addr = (addr_t)0,
		                          bool executable = false) = 0;

		/**
		 * Finish fault, assuming that the faulting instr. has been emulated
		 *
		 * \param  state  Fault attributes and emulation results that are
		 *                needed to identify and continue the faulter.
		 */
		virtual void processed(State state)
		{
			PERR("%s:%d: Not implemented", __FILE__, __LINE__);
			while(1);
		};

		/**
		 * Shortcut for attaching a dataspace at a predefined local address
		 */
		Local_addr attach_at(Dataspace_capability ds, addr_t local_addr,
		                     size_t size = 0, off_t offset = 0) {
			return attach(ds, size, offset, true, local_addr); }

		/**
		 * Shortcut for attaching a dataspace executable at a predefined local address
		 */
		Local_addr attach_executable(Dataspace_capability ds, addr_t local_addr,
		                             size_t size = 0, off_t offset = 0) {
			return attach(ds, size, offset, true, local_addr, true); }

		/**
		 * Remove region from local address space
		 */
		virtual void detach(Local_addr local_addr) = 0;

		/**
		 * Add client to pager
		 *
		 * \param thread  thread that will be paged
		 * \param imprint signature that might be given by the RM session
		 *                client to match faults to faulters through the RM state
		 * \throw         Invalid_thread
		 * \throw         Out_of_metadata
		 * \return        capability to be used for handling page faults
		 *
		 * This method must be called at least once to establish a valid
		 * communication channel between the pager part of the region manager
		 * and the client thread.
		 */
		virtual Pager_capability add_client(Thread_capability thread, unsigned imprint = 0) = 0;

		/**
		 * Register signal handler for region-manager faults
		 */
		virtual void fault_handler(Signal_context_capability handler) = 0;

		/**
		 * Request current state of RM session
		 */
		virtual State state() = 0;

		/**
		 * Return dataspace representation of region-manager session
		 */
		virtual Dataspace_capability dataspace() = 0;


		/*********************
		 ** RPC declaration **
		 *********************/

		GENODE_RPC_THROW(Rpc_attach, Local_addr, attach,
		                 GENODE_TYPE_LIST(Invalid_dataspace, Region_conflict,
		                                  Out_of_metadata, Invalid_args),
		                 Dataspace_capability, size_t, off_t, bool, Local_addr, bool);
		GENODE_RPC(Rpc_processed, void, processed, State);
		GENODE_RPC(Rpc_detach, void, detach, Local_addr);
		GENODE_RPC_THROW(Rpc_add_client, Pager_capability, add_client,
		                 GENODE_TYPE_LIST(Invalid_thread, Out_of_metadata),
		                 Thread_capability, unsigned);
		GENODE_RPC(Rpc_fault_handler, void, fault_handler, Signal_context_capability);
		GENODE_RPC(Rpc_state, State, state);
		GENODE_RPC(Rpc_dataspace, Dataspace_capability, dataspace);

		GENODE_RPC_INTERFACE(Rpc_attach, Rpc_processed, Rpc_detach, Rpc_add_client,
		                     Rpc_fault_handler, Rpc_state, Rpc_dataspace);
	};
}

#endif /* _INCLUDE__RM_SESSION__RM_SESSION_H_ */
