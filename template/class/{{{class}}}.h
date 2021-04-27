/** $Id: {{{class}}}.h 4738 2014-07-03 00:55:39Z dchassin $

 General purpose {{{class}}} objects

 **/

#ifndef _{{{CLASS}}}_H
#define _{{{CLASS}}}_H

#include "gridlabd.h"

class {{{class}}} : public gld_object 
{

private:

	// TODO: add private data

public:

	// TODO: add published data

private:

	// TODO: add private methods

public:
	
	// required methods
	{{{class}}}(MODULE *module);
	int create(void);
	int init(OBJECT *parent);

public:

	// TODO: optional methods
	TIMESTAMP precommit(TIMESTAMP t1, TIMESTAMP t2);
	TIMESTAMP presync(TIMESTAMP t1, TIMESTAMP t2);
	TIMESTAMP sync(TIMESTAMP t1, TIMESTAMP t2);
	TIMESTAMP postsync(TIMESTAMP t1, TIMESTAMP t2);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);
	TIMESTAMP finalize(TIMESTAMP t1, TIMESTAMP t2);

	int postnotify(PROPERTY *prop, const char *value);
	inline int prenotify(PROPERTY *prop, const char *value) { return 1; };

public:
	static CLASS *oclass;
	static {{{class}}} *defaults;
};

#endif // _{{{CLASS}}}_H
