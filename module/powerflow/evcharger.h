// evcharger.h

#ifndef _EVCHARGER_H
#define _EVCHARGER_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

#include "gridlabd.h"

typedef enum e_evcharger_type
{
	EVCT_UNKNOWN = 0,
	EVCT_A = 1,
	EVCT_B = 2,
	EVCT_C = 3,
	EVCT_D = 4,
	EVCT_E = 5,
	EVCT_F = 6,
} EVCHARGERTYPE;

typedef enum e_evcharger_state
{
	EVCS_OFF = 0, // (transitions to 1 on arrival or delay timeout)
	EVCS_ON = 1, // aka NORMAL state (transitions to 2 and 3 ok, 0 on departure or full charge)
	EVCS_OVERCURRENT = 2, // (transitions to 3 and 4 ok)
	EVCS_POWERCYCLE = 3, // aka OFF-ON-RECOVERY state (transitions to 1 ok)
	EVCS_RECOVERY = 4, // (transitions to 1 ok)
} EVCHARGERSTATE;

typedef enum e_evcharger_mode
{
	EVCM_POWER = 0, // charging at constant power
	EVCM_CURRENT = 1, // charger at constant current
} EVCHARGERMODE;


class evcharger : public gld_object 
{

private:

	// charger configuration
	GL_ATOMIC(enumeration,charger_type);
	GL_ATOMIC(enumeration,charger_mode);
	GL_ATOMIC(enumeration,charger_state);

	// vehicle state transitions
	GL_ATOMIC(randomvar,weekday_departure_time);
	GL_ATOMIC(randomvar,weekday_arrival_time);
	GL_ATOMIC(randomvar,saturday_departure_time);
	GL_ATOMIC(randomvar,saturday_arrival_time);
	GL_ATOMIC(randomvar,sunday_departure_time);
	GL_ATOMIC(randomvar,sunday_arrival_time);

	// normal operating mode parameters
	GL_ATOMIC(complex,nominal_charge_power); // only used in constant power mode
	GL_ATOMIC(complex,nominal_charge_current); // aka charge_current_at_nominal_voltage, only used in constant current mode
	GL_ATOMIC(double,charging_mode_boundary_voltage); // aka pu_voltage_charging_mode_boundary

	// recovery mode parameters
	GL_ATOMIC(double,overcurrent_max); // aka OVER_CURRENT_STATE_max_PEV_current
	GL_ATOMIC(double,powercycle_offtime); // aka OFF_ON_RECOVERY_STATE_off_time_sec
	GL_ATOMIC(double,powercycle_ontime); // aka OFF_ON_RECOVERY_STATE_recovery_time_sec

	// voltage state transitions
	GL_ATOMIC(double,transition12_vmin); // aka PROB_A1_trans_step_change_in_puV_LB
	GL_ATOMIC(double,transition12_vmax); // aka PROB_A1_trans_step_change_in_puV_UB

	GL_ATOMIC(double,transition13_vmin); // aka PROB_A2_trans_step_change_in_puV_LB
	GL_ATOMIC(double,transition13_vmax); // aka PROB_A3_trans_step_change_in_puV_UB

	GL_ATOMIC(double,transition23_vmin); // aka PROB_B1_trans_step_change_in_puV_LB
	GL_ATOMIC(double,transition23_vmax); // aka PROB_B1_trans_step_change_in_puV_LB

	GL_ATOMIC(double,transition24_time); // aka RECOVERY_STATE_initial_current_offset_as_percent_of_recovery_current
	GL_ATOMIC(double,transition24_offset_current); // aka RECOVERY_STATE_recovery_time_sec

	GL_ATOMIC(double,transition31);

	GL_ATOMIC(double,transition41);

private:

	load *parent_load;
	triplex_load *parent_triplex_load;

public:

	evcharger(MODULE *mod);

	int create(void);
	int isa(char *classname);
	int init(OBJECT *parent);
	
	TIMESTAMP sync(TIMESTAMP t0);
	TIMESTAMP postsync(TIMESTAMP t0);
	TIMESTAMP presync(TIMESTAMP t0);

public:

	void add_power(complex &S);
	void add_current(complex &I);

public:

	static CLASS *oclass;
	static CLASS *pclass;
	static evcharger *defaults;

};

#endif // _EVCHARGER_H
