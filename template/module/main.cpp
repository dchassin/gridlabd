// File: main.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#define DLMAIN

#include "gridlabd.h"

#include "{{{module}}}.h"

// TODO: define module globals here

EXPORT CLASS *init(CALLBACKS *fntable, MODULE *module, int argc, char *argv[])
{
	if (set_callback(fntable)==NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	INIT_MMF({{{module}}});

	// TODO: add the first class here
	// {{{class1}}} *first = new {{{class1}}}(module);

	// TODO: add more classes here
	// new {{{class2}}}(module);

	// TODO: register module globals here
	// gl_global_create("{{{global_name}}}",PT_{{{global_type}}},&{{{global_name}}},NULL);

	// always return the first class registered */
	return NULL; // {{{class1}}}::oclass;
}


EXPORT int do_kill(void*)
{
	// TODO: actions when module is unloaded
	return 0;
}

EXPORT int check()
{
	// TODO: actions when --check command line option is used
	return 0;
}
