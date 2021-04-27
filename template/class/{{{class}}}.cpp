// File: {{{class}}}.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include "{{{class}}}.h"

EXPORT_CREATE({{{class}}});
EXPORT_INIT({{{class}}});
EXPORT_PRECOMMIT({{{class}}});
EXPORT_COMMIT({{{class}}});
EXPORT_SYNC({{{class}}});
EXPORT_NOTIFY({{{class}}});

CLASS *{{{class}}}::oclass = NULL;
{{{class}}} *{{{class}}}::defaults = NULL;

#define CLASSOPTIONS PC_AUTOLOCK // |PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_OBSERVER

{{{class}}}::{{{class}}}(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"{{{class}}}",sizeof({{{class}}}),CLASSOPTIONS);
		if (oclass==NULL)
			throw "unable to register class {{{class}}}";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			// TODO: add properties 
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int {{{class}}}::create(void) 
{
	// TODO: create object
	return 1; /* return 1 on success, 0 on failure */
}

int {{{class}}}::init(OBJECT *parent)
{
	// TODO: initialize object
	return 1; /* return 2 on deferral, 1 on success, 0 on failure */
}

TIMESTAMP {{{class}}}::precommit(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMETSTAMP t2 = TS_NEVER;
	// TODO: precommit event
	return t2;
}

TIMESTAMP {{{class}}}::presync(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMETSTAMP t2 = TS_NEVER;
	// TODO: presync event
	return t2;
}

TIMESTAMP {{{class}}}::sync(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMETSTAMP t2 = TS_NEVER;
	// TODO: sync event
	return t2;
}

TIMESTAMP {{{class}}}::postcommit(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMETSTAMP t2 = TS_NEVER;
	// TODO: postsync event
	return t2;
}

TIMESTAMP {{{class}}}::commit(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMETSTAMP t2 = TS_NEVER;
	// TODO: commit event
	return t2;
}

TIMESTAMP {{{class}}}::finalize(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMETSTAMP t2 = TS_NEVER;
	// TODO: finalize event
	return t2;
}

int {{{class}}}::prenotify(PROPERTY *prop, const char *value)
{
	// TODO: handle changing value
	return 1;
}

int {{{class}}}::postnotify(PROPERTY *prop, const char *value)
{
	// TODO: handle changed value
	return 1;
}

/** @} **/
