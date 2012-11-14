/*
 * \brief  Stop watch
 * \author Martin Stein
 * \date   2012-11-10
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__UTIL__WATCH_H_
#define _INCLUDE__UTIL__WATCH_H_

/* Genode includes */
#include <watch_base.h>

namespace Genode
{
	struct Watch : Watch_base
	{
		void start()
		{
			Timer::run_and_wrap(Timer::max_value());
		}

		unsigned read()
		{
			bool wrapped;
			unsigned const v = Timer::value(wrapped);
			if (!wrapped) return Timer::max_value() - v;
			PDBG("Timer wrapped");
			return Timer::max_value();
		}
	};
}

#endif /* _INCLUDE__UTIL__WATCH_H_ */

