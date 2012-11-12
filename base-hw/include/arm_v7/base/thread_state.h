/*
 * \brief  Raw state of a thread
 * \author Martin Stein
 * \date   2012-06-11
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__ARM_V7__BASE__THREAD_STATE_H_
#define _INCLUDE__ARM_V7__BASE__THREAD_STATE_H_

/* Genode includes */
#include <cpu/cpu_state.h>

namespace Genode
{
	/**
	 * Raw state of a thread
	 */
	struct Thread_state : public Cpu_state
	{
		/**
		 * Read a general purpose register
		 *
		 * \param id  ID of the targeted register
		 * \param r   Holds register value if this returns 1
		 */
		bool get_gpr(unsigned id, unsigned & r) const
		{
			switch (id) {
			case 0: r = r0; return 1;
			case 1: r = r1; return 1;
			case 2: r = r2; return 1;
			case 3: r = r3; return 1;
			case 4: r = r4; return 1;
			case 5: r = r5; return 1;
			case 6: r = r6; return 1;
			case 7: r = r7; return 1;
			case 8: r = r8; return 1;
			case 9: r = r9; return 1;
			case 10: r = r10; return 1;
			case 11: r = r11; return 1;
			case 12: r = r12; return 1;
			case 13: r = sp; return 1;
			case 14: r = lr; return 1;
			case 15: r = ip; return 1;
			}
			return 0;
		}

		/**
		 * Override a general purpose register
		 *
		 * \param id  ID of the targeted register
		 * \param r   Has been written to register if this returns 1
		 */
		bool set_gpr(unsigned id, unsigned const r)
		{
			switch (id) {
			case 0:  r0  = r; return 1;
			case 1:  r1  = r; return 1;
			case 2:  r2  = r; return 1;
			case 3:  r3  = r; return 1;
			case 4:  r4  = r; return 1;
			case 5:  r5  = r; return 1;
			case 6:  r6  = r; return 1;
			case 7:  r7  = r; return 1;
			case 8:  r8  = r; return 1;
			case 9:  r9  = r; return 1;
			case 10: r10 = r; return 1;
			case 11: r11 = r; return 1;
			case 12: r12 = r; return 1;
			case 13: sp = r; return 1;
			case 14: lr = r; return 1;
			case 15: ip = r; return 1;
			}
			return 0;
		}
	};
}

#endif /* _INCLUDE__ARM_V7__BASE__THREAD_STATE_H_ */

