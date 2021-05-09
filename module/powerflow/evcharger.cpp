/* evcharger.cpp

This model implements a QSTS reduced-order model of the Caldera EVSE model from INEL. 

 */

#include "powerflow.h"

CLASS* evcharger::oclass = NULL;
CLASS* evcharger::pclass = NULL;
evcharger *evcharger::defaults = NULL;

EXPORT_CREATE(evcharger)
EXPORT_INIT(evcharger)
EXPORT_SYNC(evcharger)

evcharger::evcharger(MODULE *mod)
{
	if(oclass == NULL)
	{
		pclass = evcharger::oclass;
		
		oclass = gl_register_class(mod,"evcharger",sizeof(evcharger),PC_BOTTOMUP|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
		if (oclass==NULL)
			throw "unable to register class evcharger";
		else
			oclass->trl = TRL_PROVEN;

		if(gl_publish_variable(oclass,

			PT_enumeration, "charger_type", get_charger_type_offset(),
				PT_DESCRIPTION, "charger type (e.g., A, B, C, ...)",
				PT_KEYWORD, "A", (enumeration)EVCT_A,
				PT_KEYWORD, "B", (enumeration)EVCT_B,
				PT_KEYWORD, "C", (enumeration)EVCT_C,
				PT_KEYWORD, "D", (enumeration)EVCT_D,
				PT_KEYWORD, "E", (enumeration)EVCT_E,
				PT_KEYWORD, "F", (enumeration)EVCT_F,
				PT_KEYWORD, "UNKNOWN", (enumeration)EVCT_UNKNOWN,
				PT_DEFAULT, "UNKNOWN",

			PT_enumeration, "charger_state", get_charger_state_offset(),
				PT_DESCRIPTION, "charger_state (e.g., OFF, ON, ...)",
				PT_KEYWORD, "OFF", (enumeration)EVCS_OFF,
				PT_KEYWORD, "ON", (enumeration)EVCS_ON,
				PT_KEYWORD, "OVERCURRENT", (enumeration)EVCS_OVERCURRENT,
				PT_KEYWORD, "POWERCYCLE", (enumeration)EVCS_POWERCYCLE,
				PT_KEYWORD, "RECOVERY", (enumeration)EVCS_RECOVERY,
				PT_DEFAULT, "OFF",

			PT_enumeration, "charger_mode", get_charger_mode_offset(),
				PT_DESCRIPTION, "charger_mode (e.v., POWER, CURRENT)",
				PT_KEYWORD, "POWER", (enumeration)EVCM_POWER,
				PT_KEYWORD, "CURRENT", (enumeration)EVCM_CURRENT,
				PT_DEFAULT, "POWER",

			PT_complex, "nominal_charge_power[W]", get_nominal_charge_power_offset(),
				PT_DESCRIPTION, "charger power when in constant power mode",
				PT_DEFAULT, "0.0 W",

			PT_complex, "nominal_charge_current[A]", get_nominal_charge_current_offset(),
				PT_DESCRIPTION, "charger current when in constant current mode",
				PT_DEFAULT, "0.0 A",	

         	NULL) < 1) GL_THROW("unable to publish properties in %s",__FILE__);
    }
}

int evcharger::isa(char *classname)
{
	return strcmp(classname,"evcharger")==0;
}

int evcharger::create(void)
{        
	// TODO
	parent_load = NULL;
	parent_triplex_load = NULL;
    return 1;
}

// Initialize, return 1 on success
int evcharger::init(OBJECT *pobj)
{
	// check parent object existence
	gld_object *parent = get_object(pobj);
	if ( parent == NULL )
	{
		error("parent must be specified");
		return 0;
	}

	// check parent object type
	if ( parent->isa("load","powerflow") )
	{
		parent_load = OBJECTDATA(pobj,load);
	}
	else if ( parent->isa("triplex_load","powerflow") )
	{
		parent_triplex_load = OBJECTDATA(pobj,triplex_load);
	}
	else
	{
		error("parent is not a powerflow load or triplex_load object");
		return 0;
	}

	// check charger type
	if ( charger_type == EVCT_UNKNOWN )
	{
		verbose("charger_type is not specified, no voltage dynamics modeled");
	}

	// check charger mode
	if ( charger_mode == EVCM_POWER && nominal_charge_power <= 0 )
	{
		error("nominal charge power must be positive when operating in constant power mode");
		return 0;
	}
	if ( charger_mode == EVCM_CURRENT && nominal_charge_current <= 0 )
	{
		error("nominal charge current must be positive when operating in constant current mode");
		return 0;
	}
	return 1;
}

TIMESTAMP evcharger::presync(TIMESTAMP t0)
{
	TIMESTAMP t1 = TS_NEVER;
	
	// TODO

	return t1;
}

TIMESTAMP evcharger::sync(TIMESTAMP t0)
{
	TIMESTAMP t1 = TS_NEVER;
	
	if ( charger_state == EVCS_ON )
	{
		if ( charger_mode == EVCM_CURRENT )
		{
			add_current(nominal_charge_current);
		}
		else if ( charger_mode == EVCM_POWER )
		{
			add_power(nominal_charge_power);
		}
		else
		{
			exception("charger mode %d is not valid",(int)charger_mode);
		}
	}
	else if ( charger_state != EVCS_OFF )
	{
		exception("charger state %d is not valid",(int)charger_state);
	}
	return t1;
}

TIMESTAMP evcharger::postsync(TIMESTAMP t0)
{
	TIMESTAMP t1 = TS_NEVER;
	
	// TODO

	return t1;
}

void evcharger::add_power(complex &S)
{
	if ( parent_load != NULL )
	{
		parent_load->add_power(nominal_charge_power);
	}
	else if ( parent_triplex_load != NULL )
	{
		parent_triplex_load->add_power(nominal_charge_power);
	}
	else
	{
		exception("parent object type error");
	}
}

void evcharger::add_current(complex &I)
{
	if ( parent_load != NULL )
	{
		parent_load->add_power(nominal_charge_current);
	}
	else if ( parent_triplex_load != NULL )
	{
		parent_triplex_load->add_current(nominal_charge_current);
	}
	else
	{
		exception("parent object type error");
	}
}

