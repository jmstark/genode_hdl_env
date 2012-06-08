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
#include <verilator_env/clock.h>
#include <verilator_env/wishbone_slave.h>
#include <emulation_session_component.h>

using namespace Genode;

/**
 * Prepare for the use of verilator_env tools
 */

static Vmonitor hdl;
static Lock hdl_lock;

void evaluate_hdl() { if (!Verilated::gotFinish()) hdl.eval(); }

/**
 * Use verilator_env tools to ease connection between
 * HDL and emulator interface.
 */

static Clock clk(&hdl.sys_clk, 1);

struct Raw_wishbone_slave
{
	void cycle() { clk.cycle(); };

	uint8_t & rst_i() { return hdl.sys_rst; };
	uint8_t & cyc_i() { return hdl.wb_cyc_i; };
	uint8_t & we_i()  { return hdl.wb_we_i; };
	uint8_t & stb_i() { return hdl.wb_stb_i; };
	uint8_t & ack_o() { return hdl.wb_ack_o; };
	uint8_t & err_o() { static uint8_t dummy = 0; return dummy; };
	uint8_t & rty_o() { static uint8_t dummy = 0; return dummy; };

	void sel_i(uint8_t const v)    { hdl.wb_sel_i = *((uint8_t  *)&v); };
	void adr_i(uint32_t const v)   { hdl.wb_adr_i = *((uint32_t *)&v); };
	void dat_i(uint32_t const v)   { hdl.wb_dat_i = *((uint32_t *)&v); };
	void dat_o(uint32_t * const v) { *v = hdl.wb_dat_o; };
};

static Wishbone_slave<Raw_wishbone_slave, 10> wbs;

/**
 * Connect emulator interface and HDL design
 */
         Emulation::Session_component::Session_component() { wbs.initialize(); }
void     Emulation::Session_component::write_mmio(addr_t const addr, Access const a, umword_t const v) { wbs.write_mmio(addr, a, v); }
umword_t Emulation::Session_component::read_mmio(addr_t const addr, Access const a) { return wbs.read_mmio(addr, a); }
bool     Emulation::Session_component::irq_handler(unsigned i, Signal_context_capability s) { return 0; }

