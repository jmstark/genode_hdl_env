/*
 * \brief  Linker script for Genode programs
 * \author Christian Helmuth
 * \date   2006-04-12
 */

/*
 * Copyright (C) 2006-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

ENTRY(_start)

PHDRS
{
	ro PT_LOAD;
	rw PT_LOAD;
}

SECTIONS
{
	.text : {
		/* begin of program image (link address) */
		_prog_img_beg = .;

		*(.init)
		*(.text .text.* .gnu.linkonce.t.*)
		*(.fini)
		*(.rodata .rodata.* .gnu.linkonce.r.*)

		. = ALIGN(0x08);

		_ctors_start = .;
		KEEP (*(.ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.init_array)) /* list of constructors specific for ARM eabi */
		_ctors_end = .;
		_dtors_start = .;
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
		_dtors_end = .;
	} : ro = 0x90909090

	/* Linux: exception section for uaccess mechanism */
	__ex_table : { *(__ex_table) }

	.eh_frame_hdr : { *(.eh_frame_hdr) }

	. = ALIGN(0x1000);

	_prog_img_data = .;

	.data : {
		/*
		 * Leave space for parent capability parameters at start of data
		 * section. The protection domain creator is reponsible for storing
		 * sane values here.
		 */
		_parent_cap = .;
		_parent_cap_thread_id = .;
		LONG(0xffffffff);
		_parent_cap_local_name = .;
		LONG(0xffffffff);
		LONG(0xffffffff);
		LONG(0xffffffff);
		LONG(0xffffffff);
		LONG(0xffffffff);
		LONG(0xffffffff);
		LONG(0xffffffff);

		/*
		 * Platform-specific entry for Fiasco.OC.
		 *
		 * PIC-code compiled for Fiasco.OC, needs some PIC-compatible
		 * way to enter the kernel, the fixed address of the kernel
		 * entry code address needs to be found here.
		 */
		__l4sys_invoke_indirect = .;
		LONG(0xeacff000);

		*(.data .data.* .gnu.linkonce.d.*)
	} : rw

	/* exception frames for C++ */
	.eh_frame : {
		__eh_frame_start__ = .;
		KEEP (*(.eh_frame))
		LONG(0)
	} : rw

	.gcc_except_table : {
		KEEP(*(.gcc_except_table))
		KEEP(*(.gcc_except_table.*))
	}

	.dynamic :  { *(.dynamic) }

	/* .ARM.exidx is sorted, so has to go in its own output section */
	__exidx_start = .;
	.ARM.exidx : {
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	}
	__exidx_end = .;

	.ARM.extab : {
		*(.ARM.extab*)
	} : rw

	.bss : {
		*(.bss .bss.* .gnu.linkonce.b.* COMMON)
	}

	/* end of program image -- must be after last section */
	_prog_img_end = .;

	/DISCARD/ : {
		*(.note)
		*(.note.ABI-tag)
		*(.comment)
	}
}
