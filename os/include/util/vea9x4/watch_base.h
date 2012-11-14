/*
 * \brief  Stop-watch base
 * \author Martin Stein
 * \date   2012-11-10
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__UTIL__VEA9X4__WATCH_BASE_H_
#define _INCLUDE__UTIL__VEA9X4__WATCH_BASE_H_

/* Genode includes */
#include <drivers/board.h>
#include <io_mem_session/connection.h>
#include <irq_session/connection.h>
#include <drivers/timer/sp804_base.h>

namespace Genode
{
	struct Watch_base : Io_mem_connection,
	                    Irq_connection,
	                    Sp804_base<Board::SP804_0_1_CLOCK>
	{
		typedef Io_mem_connection Io_mem;
		typedef Irq_connection Irq;
		typedef Sp804_base<Board::SP804_0_1_CLOCK> Timer;

		Watch_base() :
			Io_mem(Board::SP804_0_1_MMIO_BASE,
			       Board::SP804_0_1_MMIO_SIZE),
			Irq(Board::SP804_0_1_IRQ),
			Timer((addr_t)env()->rm_session()->attach(Io_mem::dataspace()))
		{ }
	};
}

#endif /* _INCLUDE__UTIL__VEA9X4__WATCH_BASE_H_ */

