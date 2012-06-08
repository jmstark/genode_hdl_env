/*
 * \brief  Do cycles at a HDL clock line
 * \author Martin Stein
 * \date   2012-12-05
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__VERILATOR_ENV__CLOCK_H_
#define _INCLUDE__VERILATOR_ENV__CLOCK_H_

void evaluate_hdl();

namespace Genode
{
	/**
	 * Do cycles at a HDL clock line
	 */
	class Clock
	{
		uint8_t * const _raw;
		bool const _up;

		public:

			/**
			 * Constructor
			 *
			 * \param raw  raw HDL clock line
			 * \param up   if the clock is up-edge or down-edge triggered
			 */
			Clock(uint8_t * const raw, bool const up) : _raw(raw), _up(up) { }

			/**
			 * Do a clock cycle
			 */
			void cycle()
			{
				*_raw = !_up;
				evaluate_hdl();
				*_raw = _up;
				evaluate_hdl();
			}
	};
}

#endif /* _INCLUDE__VERILATOR_ENV__CLOCK_H_ */

