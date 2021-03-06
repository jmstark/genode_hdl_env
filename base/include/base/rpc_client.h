/*
 * \brief  Support for performing RPC calls
 * \author Norman Feske
 * \date   2011-04-06
 */

/*
 * Copyright (C) 2011-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__RPC_CLIENT_H_
#define _INCLUDE__BASE__RPC_CLIENT_H_

#include <base/ipc.h>

namespace Genode {

	/**
	 * RPC client
	 *
	 * This class template is the base class of the client-side implementation
	 * of the specified 'RPC_INTERFACE'. Usually, it inherits the pure virtual
	 * functions declared in 'RPC_INTERFACE' and has the built-in facility to
	 * perform RPC calls to this particular interface. Hence, the client-side
	 * implementation of each pure virtual interface function comes down to a
	 * simple wrapper in the line of 'return call<Rpc_function>(arguments...)'.
	 */
	template <typename RPC_INTERFACE>
	struct Rpc_client : Capability<RPC_INTERFACE>, RPC_INTERFACE
	{
		typedef RPC_INTERFACE Rpc_interface;

		Rpc_client(Capability<RPC_INTERFACE> const &cap)
		: Capability<RPC_INTERFACE>(cap) { }
	};


	/*********************************************************
	 ** Implementation of 'Capability:call' functions **
	 *********************************************************/

	template <typename RPC_INTERFACE>
	template <typename ATL>
	void Capability<RPC_INTERFACE>::
	_marshal_args(Ipc_client &ipc_client, ATL &args) const
	{
		if (Trait::Rpc_direction<typename ATL::Head>::Type::IN)
			ipc_client << args.get();

		_marshal_args(ipc_client, args._2);
	}


	template <typename RPC_INTERFACE>
	template <typename T>
	void Capability<RPC_INTERFACE>::
	_unmarshal_result(Ipc_client &ipc_client, T &arg,
	                  Meta::Overload_selector<Rpc_arg_out>) const
	{
		ipc_client >> arg;
	}


	template <typename RPC_INTERFACE>
	template <typename T>
	void Capability<RPC_INTERFACE>::
	_unmarshal_result(Ipc_client &ipc_client, T &arg,
	                  Meta::Overload_selector<Rpc_arg_inout>) const
	{
		_unmarshal_result(ipc_client, arg, Meta::Overload_selector<Rpc_arg_out>());
	}


	template <typename RPC_INTERFACE>
	template <typename ATL>
	void Capability<RPC_INTERFACE>::
	_unmarshal_results(Ipc_client &ipc_client, ATL &args) const
	{
		/*
		 * Unmarshal current argument. The overload of
		 * '_unmarshal_result' is selected depending on the RPC
		 * direction.
		 */
		typedef typename Trait::Rpc_direction<typename ATL::Head>::Type Rpc_dir;
		_unmarshal_result(ipc_client, args.get(), Meta::Overload_selector<Rpc_dir>());

		/* unmarshal remaining arguments */
		_unmarshal_results(ipc_client, args._2);
	}


	template <typename RPC_INTERFACE>
	template <typename IF>
	void Capability<RPC_INTERFACE>::
	_call(typename IF::Client_args &args, typename IF::Ret_type &ret) const
	{
		/**
		 * Message buffer for RPC message
		 *
		 * The message buffer gets automatically dimensioned according to the
		 * specified 'IF' RPC function.
		 */
		enum { PROTOCOL_OVERHEAD = 4*sizeof(long),
		       CALL_MSG_SIZE     = Rpc_function_msg_size<IF, RPC_CALL>::Value,
		       REPLY_MSG_SIZE    = Rpc_function_msg_size<IF, RPC_REPLY>::Value };

		Msgbuf<CALL_MSG_SIZE  + PROTOCOL_OVERHEAD>  call_buf;
		Msgbuf<REPLY_MSG_SIZE + PROTOCOL_OVERHEAD> reply_buf;

		Ipc_client ipc_client(*this, &call_buf, &reply_buf);

		/* determine opcode of RPC function */
		typedef typename RPC_INTERFACE::Rpc_functions Rpc_functions;
		Rpc_opcode opcode = static_cast<int>(Meta::Index_of<Rpc_functions, IF>::Value);

		/* marshal opcode and RPC input arguments */
		ipc_client << opcode;
		_marshal_args(ipc_client, args);

		/* perform RPC, unmarshal return value */
		ipc_client << IPC_CALL >> ret;

		/* unmarshal RPC output arguments */
		_unmarshal_results(ipc_client, args);

		/* reflect callee-side exception at the caller */
		_check_for_exceptions(ipc_client.result(),
		                      Meta::Overload_selector<typename IF::Exceptions>());
	}
}

#endif /* _INCLUDE__BASE__RPC_CLIENT_H_ */
