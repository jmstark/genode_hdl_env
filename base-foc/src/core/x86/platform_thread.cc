/*
 * \brief  Fiasco.OC thread facility (x86 specifics)
 * \author Stefan Kalkowski
 * \date   2011-09-08
 */

/*
 * Copyright (C) 2011 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <cpu/cpu_state.h>
#include <cpu/consts.h>

/* core includes */
#include <platform_thread.h>


bool Genode::Platform_thread::_in_syscall(Fiasco::l4_umword_t flags)
{
	using namespace Genode;

	return flags & X86::IOPL;
}