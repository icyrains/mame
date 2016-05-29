// license:GPL-2.0+
// copyright-holders:Couriersud
/*
 * nld_74153.c
 *
 */

#include "nld_74153.h"

namespace netlist
{
	namespace devices
	{

/* FIXME: timing is not 100% accurate, Strobe and Select inputs have a
 *        slightly longer timing.
 *        Convert this to sub-devices at some time.
 */

	NETLIB_OBJECT(74153sub)
	{
		NETLIB_CONSTRUCTOR(74153sub)
		, m_chan(0)
		{
			enregister("C0", m_C[0]);
			enregister("C1", m_C[1]);
			enregister("C2", m_C[2]);
			enregister("C3", m_C[3]);
			enregister("G", m_G);

			enregister("AY", m_Y); //FIXME: Change netlists

			save(NLNAME(m_chan));
		}

		NETLIB_RESETI();
		NETLIB_UPDATEI();

	public:
		logic_input_t m_C[4];
		logic_input_t m_G;

		logic_output_t m_Y;

		int m_chan;
	};

	NETLIB_OBJECT(74153)
	{
		NETLIB_CONSTRUCTOR(74153)
		, m_sub(*this, "sub")
		{

			register_subalias("C0", m_sub.m_C[0]);
			register_subalias("C1",  m_sub.m_C[1]);
			register_subalias("C2",  m_sub.m_C[2]);
			register_subalias("C3",  m_sub.m_C[3]);
			enregister("A", m_A);
			enregister("B", m_B);
			register_subalias("G",  m_sub.m_G);

			register_subalias("AY",  m_sub.m_Y); //FIXME: Change netlists
		}
		NETLIB_RESETI() { }
		NETLIB_UPDATEI();
	public:
		NETLIB_SUB(74153sub) m_sub;
		logic_input_t m_A;
		logic_input_t m_B;
	};

	NETLIB_OBJECT(74153_dip)
	{
		NETLIB_CONSTRUCTOR(74153_dip)
		, m_1(*this, "1")
		, m_2(*this, "2")
		{

			register_subalias("1", m_1.m_G);
			enregister("2", m_B);    // m_2.m_B
			register_subalias("3", m_1.m_C[3]);
			register_subalias("4", m_1.m_C[2]);
			register_subalias("5", m_1.m_C[1]);
			register_subalias("6", m_1.m_C[0]);
			register_subalias("7", m_1.m_Y);

			register_subalias("9", m_2.m_Y);
			register_subalias("10", m_2.m_C[0]);
			register_subalias("11", m_2.m_C[1]);
			register_subalias("12", m_2.m_C[2]);
			register_subalias("13", m_2.m_C[3]);

			enregister("14", m_A);   // m_2.m_B
			register_subalias("15", m_2.m_G);

		}
		//NETLIB_RESETI();
		NETLIB_UPDATEI();

	protected:
		NETLIB_SUB(74153sub) m_1;
		NETLIB_SUB(74153sub) m_2;
		logic_input_t m_A;
		logic_input_t m_B;
	};


	NETLIB_RESET(74153sub)
	{
		m_chan = 0;
	}

	NETLIB_UPDATE(74153sub)
	{
		const netlist_time delay[2] = { NLTIME_FROM_NS(23), NLTIME_FROM_NS(18) };
		if (!INPLOGIC(m_G))
		{
			UINT8 t = INPLOGIC(m_C[m_chan]);
			OUTLOGIC(m_Y, t, delay[t] );
		}
		else
		{
			OUTLOGIC(m_Y, 0, delay[0]);
		}
	}


	NETLIB_UPDATE(74153)
	{
		m_sub.m_chan = (INPLOGIC(m_A) | (INPLOGIC(m_B)<<1));
		m_sub.do_update();
	}


	NETLIB_UPDATE(74153_dip)
	{
		m_2.m_chan = m_1.m_chan = (INPLOGIC(m_A) | (INPLOGIC(m_B)<<1));
		m_1.do_update();
		m_2.do_update();
	}

	NETLIB_DEVICE_IMPL(74153)
	NETLIB_DEVICE_IMPL(74153_dip)

	} //namespace devices
} // namespace netlist
