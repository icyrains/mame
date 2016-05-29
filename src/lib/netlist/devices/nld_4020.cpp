// license:GPL-2.0+
// copyright-holders:Couriersud
/*
 * nld_4020.c
 *
 */

#include "nld_4020.h"
#include "nld_cmos.h"

namespace netlist
{
	namespace devices
	{

	NETLIB_OBJECT(CD4020_sub)
	{
		NETLIB_CONSTRUCTOR(CD4020_sub)
		NETLIB_FAMILY("CD4XXX")
		, m_cnt(0)
		{
			enregister("IP", m_IP);

			enregister("Q1", m_Q[0]);
			enregister("Q4", m_Q[3]);
			enregister("Q5", m_Q[4]);
			enregister("Q6", m_Q[5]);
			enregister("Q7", m_Q[6]);
			enregister("Q8", m_Q[7]);
			enregister("Q9", m_Q[8]);
			enregister("Q10", m_Q[9]);
			enregister("Q11", m_Q[10]);
			enregister("Q12", m_Q[11]);
			enregister("Q13", m_Q[12]);
			enregister("Q14", m_Q[13]);

			save(NLNAME(m_cnt));
		}

		NETLIB_RESETI()
		{
			m_IP.set_state(logic_t::STATE_INP_HL);
			m_cnt = 0;
		}

		NETLIB_UPDATEI();

	public:
		ATTR_HOT void update_outputs(const UINT16 cnt);

		logic_input_t m_IP;
		logic_output_t m_Q[14];

		UINT16 m_cnt;
	};

	NETLIB_OBJECT(CD4020)
	{
		NETLIB_CONSTRUCTOR(CD4020)
		NETLIB_FAMILY("CD4XXX")
		, m_sub(*this, "sub")
		, m_supply(*this, "supply")
		{

			enregister("RESET", m_RESET);
			register_subalias("IP", m_sub.m_IP);
			register_subalias("Q1", m_sub.m_Q[0]);
			register_subalias("Q4", m_sub.m_Q[3]);
			register_subalias("Q5", m_sub.m_Q[4]);
			register_subalias("Q6", m_sub.m_Q[5]);
			register_subalias("Q7", m_sub.m_Q[6]);
			register_subalias("Q8", m_sub.m_Q[7]);
			register_subalias("Q9", m_sub.m_Q[8]);
			register_subalias("Q10", m_sub.m_Q[9]);
			register_subalias("Q11", m_sub.m_Q[10]);
			register_subalias("Q12", m_sub.m_Q[11]);
			register_subalias("Q13", m_sub.m_Q[12]);
			register_subalias("Q14", m_sub.m_Q[13]);
			register_subalias("VDD", m_supply.m_vdd);
			register_subalias("VSS", m_supply.m_vss);
		}
		NETLIB_RESETI() { }
		NETLIB_UPDATEI();

	private:
		NETLIB_SUB(CD4020_sub) m_sub;
		NETLIB_SUB(vdd_vss) m_supply;
		logic_input_t m_RESET;
	};


	NETLIB_UPDATE(CD4020_sub)
	{
		UINT8 cnt = m_cnt;
		cnt = ( cnt + 1) & 0x3fff;
		update_outputs(cnt);
		m_cnt = cnt;
	}

	NETLIB_UPDATE(CD4020)
	{
		if (INPLOGIC(m_RESET))
		{
			m_sub.m_cnt = 0;
			m_sub.m_IP.inactivate();
			/* static */ const netlist_time reset_time = netlist_time::from_nsec(140);
			OUTLOGIC(m_sub.m_Q[0], 0, reset_time);
			for (int i=3; i<14; i++)
				OUTLOGIC(m_sub.m_Q[i], 0, reset_time);
		}
		else
			m_sub.m_IP.activate_hl();
	}

	inline NETLIB_FUNC_VOID(CD4020_sub, update_outputs, (const UINT16 cnt))
	{
		/* static */ const netlist_time out_delayQn[14] = {
				NLTIME_FROM_NS(180), NLTIME_FROM_NS(280),
				NLTIME_FROM_NS(380), NLTIME_FROM_NS(480),
				NLTIME_FROM_NS(580), NLTIME_FROM_NS(680),
				NLTIME_FROM_NS(780), NLTIME_FROM_NS(880),
				NLTIME_FROM_NS(980), NLTIME_FROM_NS(1080),
				NLTIME_FROM_NS(1180), NLTIME_FROM_NS(1280),
				NLTIME_FROM_NS(1380), NLTIME_FROM_NS(1480),
		};

		OUTLOGIC(m_Q[0], 0, out_delayQn[0]);
		for (int i=3; i<14; i++)
			OUTLOGIC(m_Q[i], (cnt >> i) & 1, out_delayQn[i]);
	}

	NETLIB_DEVICE_IMPL(CD4020)

	} //namespace devices
} // namespace netlist
