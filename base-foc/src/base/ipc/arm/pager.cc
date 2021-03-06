/*
 * \brief  Fiasco.OC pager framework
 * \author Stefan Kalkowski
 * \date   2011-09-08
 *
 * ARM specific parts, when handling cpu-exceptions.
 */

/*
 * Copyright (C) 2011-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/ipc_pager.h>


void Genode::Ipc_pager::copy_regs(Thread_state *state)
{
	state->ip = _regs.pc;
	state->sp = _regs.sp;
	state->r0  = _regs.r[0];
	state->r1  = _regs.r[1];
	state->r2  = _regs.r[2];
	state->r3  = _regs.r[3];
	state->r4  = _regs.r[4];
	state->r5  = _regs.r[5];
	state->r6  = _regs.r[6];
	state->r7  = _regs.r[7];
	state->r8  = _regs.r[8];
	state->r9  = _regs.r[9];
	state->r10 = _regs.r[10];
	state->r11 = _regs.r[11];
	state->r12 = _regs.r[12];
	state->lr = _regs.ulr;
	state->cpsr = _regs.cpsr;
}
