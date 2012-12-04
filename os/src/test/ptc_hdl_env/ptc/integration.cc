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
#include "Vptc_top.h"

/* verilator_env includes */
#include <verilator_env/irq.h>
#include <verilator_env/wishbone_slave.h>
#include <emulation_session_component.h>

using namespace Genode;

/**
 * Prepare for the use of verilator_env tools
 */

static Vptc_top hdl;
static Lock hdl_lock;

void evaluate_hdl() { if (!Verilated::gotFinish()) hdl.eval(); }

/**
 * Use verilator_env tools to ease connection between
 * HDL and emulator interface.
 */

static Irq irq(&hdl.wb_inta_o);
static Irq_clock clk(&hdl.wb_clk_i, 1, 100, 10, &hdl_lock, &irq);
static Irq_listener irq_listener(&irq);

struct Raw_wishbone_slave
{
	void cycle() { return clk.cycle(); };

	uint8_t & rst_i() { return hdl.wb_rst_i; };
	uint8_t & cyc_i() { return hdl.wb_cyc_i; };
	uint8_t & we_i()  { return hdl.wb_we_i; };
	uint8_t & stb_i() { return hdl.wb_stb_i; };
	uint8_t & ack_o() { return hdl.wb_ack_o; };
	uint8_t & err_o() { return hdl.wb_err_o; };
	uint8_t & rty_o() { static uint8_t dummy = 0; return dummy; };

	void sel_i(uint8_t const v)    { hdl.wb_sel_i = *((uint8_t  *)&v); };
	void adr_i(uint32_t const v)   { hdl.wb_adr_i = *((uint16_t *)&v); };
	void dat_i(uint32_t const v)   { hdl.wb_dat_i = *((uint32_t *)&v); };
	void dat_o(uint32_t * const v) { *v = hdl.wb_dat_o; };
};

static Sync_wishbone_slave<Raw_wishbone_slave, 10> wbs(&hdl_lock);

/**
 * Connect emulator interface and HDL design
 */
         Emulation::Session_component::Session_component() { wbs.initialize(); }
void     Emulation::Session_component::write_mmio(addr_t const addr, Access const a, umword_t const v) { wbs.write_mmio(addr, a, v); }
umword_t Emulation::Session_component::read_mmio(addr_t const addr, Access const a) { return wbs.read_mmio(addr, a); }
bool     Emulation::Session_component::irq_handler(unsigned i, Signal_context_capability s) { return irq_listener.irq_handler(i, s); }

