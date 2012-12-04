/*
 * \brief   Implementation of the wishbone-slave interface
 * \author  Martin Stein
 * \date    2012-07-25
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* local includes */
#include "Vmonitor.h"

/* Wishbone-lib includes */
#include <wishbone_slave.h>

static Vmonitor * design()
{
	static Vmonitor _design;
	return &_design;
}

namespace Wishbone_slave
{
	uint8_t & rst_i() { return design()->sys_rst; };
	uint8_t & clk_i() { return design()->sys_clk; };
	uint8_t & stb_i() { return design()->wb_stb_i; };
	uint8_t & cyc_i() { return design()->wb_cyc_i; };
	uint8_t & we_i()  { return design()->wb_we_i; };
	uint8_t & ack_o() { return design()->wb_ack_o; };
	uint8_t & err_o() { static uint8_t dummy = 0; return dummy; };
	uint8_t & rty_o() { static uint8_t dummy = 0; return dummy; };

	void sel_i(uint8_t const v)    { design()->wb_sel_i = v; };
	void adr_i(uint32_t const v)   { design()->wb_adr_i = v; };
	void dat_i(uint32_t const v)   { design()->wb_dat_i = v; };

	void dat_o(uint32_t * const v) { *v = design()->wb_dat_o; };

	void evaluate_design() {
		design()->write_lock = 1;
		design()->eval();
	};
}

