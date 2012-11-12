/*
 * \brief  Raw CPU state
 * \author Martin Stein
 * \date   2012-06-12
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__CORTEX_A9__BASE__CPU_CONTEXT_H_
#define _INCLUDE__CORTEX_A9__BASE__CPU_CONTEXT_H_

/* core includes */
#include <cpu/cpu_state.h>

namespace Genode
{
	struct Thread_state_base : Cpu_state
	{
		void fetch(Native_thread_id const thread_id)
		{
			r0 = Kernel::read_register(thread_id, 0);
			r1 = Kernel::read_register(thread_id, 1);
			r2 = Kernel::read_register(thread_id, 2);
			r3 = Kernel::read_register(thread_id, 3);
			r4 = Kernel::read_register(thread_id, 4);
			r5 = Kernel::read_register(thread_id, 5);
			r6 = Kernel::read_register(thread_id, 6);
			r7 = Kernel::read_register(thread_id, 7);
			r8 = Kernel::read_register(thread_id, 8);
			r9 = Kernel::read_register(thread_id, 9);
			r10 = Kernel::read_register(thread_id, 10);
			r11 = Kernel::read_register(thread_id, 11);
			r12 = Kernel::read_register(thread_id, 12);
			sp = Kernel::read_register(thread_id, 13);
			lr = Kernel::read_register(thread_id, 14);
			ip = Kernel::read_register(thread_id, 15);
		}

		void override(Native_thread_id const thread_id)
		{
			Kernel::write_register(thread_id, 0, r0);
			Kernel::write_register(thread_id, 1, r1);
			Kernel::write_register(thread_id, 2, r2);
			Kernel::write_register(thread_id, 3, r3);
			Kernel::write_register(thread_id, 4, r4);
			Kernel::write_register(thread_id, 5, r5);
			Kernel::write_register(thread_id, 6, r6);
			Kernel::write_register(thread_id, 7, r7);
			Kernel::write_register(thread_id, 8, r8);
			Kernel::write_register(thread_id, 9, r9);
			Kernel::write_register(thread_id, 10, r10);
			Kernel::write_register(thread_id, 11, r11);
			Kernel::write_register(thread_id, 12, r12);
			Kernel::write_register(thread_id, 13, sp);
			Kernel::write_register(thread_id, 14, lr);
			Kernel::write_register(thread_id, 15, ip);
		}

		/**
		 * Read a general purpose register
		 *
		 * \param   id  ID of the targeted register
		 * \param   r   Holds register value if this returns 1
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
		 * \param   id  ID of the targeted register
		 * \param   r   Has been written to register if this returns 1
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

#endif /* _INCLUDE__CORTEX_A9__BASE__CPU_CONTEXT_H_ */

