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
#include <drivers/timer/sp804_base.h>

namespace Genode
{
	struct Watch_base : Io_mem_connection,
	                    Sp804_base<Board::SP804_CLOCK>
	{
		typedef Io_mem_connection Io_mem;
		typedef Sp804_base<Board::SP804_CLOCK> Timer;

		static addr_t io_mem_base(unsigned const id)
		{
			switch (id)
			{
			case 0: return Board::SP804_0_1_MMIO_BASE;
			case 1: return Board::SP804_2_3_MMIO_BASE;
			}
			PDBG("Invalid watch ID");
			return 0;
		}

		Watch_base(unsigned const id) :
			Io_mem(io_mem_base(id), Board::SP804_MMIO_SIZE),
			Timer((addr_t)env()->rm_session()->attach(Io_mem::dataspace()))
		{ }
	};
}

#endif /* _INCLUDE__UTIL__VEA9X4__WATCH_BASE_H_ */

