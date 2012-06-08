/*
 * \brief  Filter session args of an eavesdropping service
 * \author Martin Stein
 * \date   2012-11-26
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__SPY_SESSION_ARGS_H_
#define _INCLUDE__SPY_SESSION_ARGS_H_

/* Genode includes */
#include <util/arg_string.h>

namespace Init
{
	using namespace Genode;

	/**
	 * Filter session args of an eavesdropping service
	 *
	 * \param  BACKEND_RAM_QUOTA  RAM amount donated to the backend session
	 */
	template <size_t BACKEND_RAM_QUOTA>
	struct Spy_session_args
	{
		enum { MAX_ARGS_LEN = 256 };

		size_t spy_ram_quota;              /* RAM donation for spy session */
		char   backend_args[MAX_ARGS_LEN]; /* args to the backend session */

		/**
		 * Constructor
		 */
		Spy_session_args(const char * args) :
			spy_ram_quota(Arg_string::find_arg(args, "ram_quota").long_value(0))
		{
			strncpy(backend_args, args, sizeof(backend_args));
			if (spy_ram_quota > BACKEND_RAM_QUOTA) {
				spy_ram_quota -= BACKEND_RAM_QUOTA;
				char buf[64];
				snprintf(buf, sizeof(buf), "%zd", BACKEND_RAM_QUOTA);
				Arg_string::set_arg(backend_args, sizeof(backend_args),
				                    "ram_quota", buf);
			} else {
				spy_ram_quota = 0;
				PDBG("Insufficient RAM quota");
			}
		}
	};
}

#endif /* _INCLUDE__SPY_SESSION_ARGS_H_ */

