/*
 * \brief  Declare interface of the expected singleton wishbone-slave
 * \author Martin Stein
 * \date   2012-07-15
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <stdint.h>

namespace Wishbone_slave
{
	/**
	 * In/out of fixed width
	 */
	extern uint8_t & rst_i();
	extern uint8_t & clk_i();
	extern uint8_t & stb_i();
	extern uint8_t & cyc_i();
	extern uint8_t & we_i();
	extern uint8_t & ack_o();
	extern uint8_t & err_o();
	extern uint8_t & rty_o();

	/**
	 * Input of design specific width
	 */
	extern void sel_i(uint8_t const);
	extern void adr_i(uint32_t const);
	extern void dat_i(uint32_t const);

	/**
	 * Output of design specific width
	 */
	extern void dat_o(uint32_t * const);

	/**
	 * Do an evaluation cycle
	 */
	extern void evaluate_design();
}

