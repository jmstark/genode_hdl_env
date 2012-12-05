/*
 * \brief   Integrate HDL design
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

/* verilator_env includes */
#include <verilator_env/wishbone_slave.h>
#include <emulation_session_component.h>

using namespace Genode;

static Vmonitor hdl;

struct Raw_wishbone_slave
{
	uint8_t & rst_i() { return hdl.sys_rst; };
	uint8_t & clk_i() { return hdl.sys_clk; };
	uint8_t & stb_i() { return hdl.wb_stb_i; };
	uint8_t & cyc_i() { return hdl.wb_cyc_i; };
	uint8_t & we_i()  { return hdl.wb_we_i; };
	uint8_t & ack_o() { return hdl.wb_ack_o; };
	uint8_t & err_o() { static uint8_t dummy = 0; return dummy; };
	uint8_t & rty_o() { static uint8_t dummy = 0; return dummy; };

	void sel_i(uint8_t const v)    { hdl.wb_sel_i = v; };
	void adr_i(uint32_t const v)   { hdl.wb_adr_i = v; };
	void dat_i(uint32_t const v)   { hdl.wb_dat_i = v; };
	void dat_o(uint32_t * const v) { *v = hdl.wb_dat_o; };

	void evaluate_design()
	{
		hdl.write_lock = 1;
		hdl.eval();
	}
};

static Wishbone_slave<Raw_wishbone_slave, 10> wbs;

         Emulation::Session_component::Session_component() { wbs.initialize(); }
void     Emulation::Session_component::write_mmio(addr_t const addr, Access const a, umword_t const v) {        wbs.write_mmio(addr, a, v); }
umword_t Emulation::Session_component::read_mmio (addr_t const addr, Access const a)                   { return wbs.read_mmio(addr, a); }

bool Emulation::Session_component::irq_handler(unsigned const, Signal_context_capability)
{
	PDBG("No interrupts provided");
	return 0;
}

