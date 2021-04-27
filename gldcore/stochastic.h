// File: stochastic.h 
// Copyright: 2020, Regents of the Leland Stanford Junior University
// Author: DP Chassin (dchassin@slac.stanford.edu)

#ifndef _STOCHASTIC_H
#define _STOCHASTIC_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif // ! defined _GLDCORE_H && ! defined _GRIDLABD_H

class stochastic
{
private:
	const char *name;
	int refresh;
	int samples;
	std::list<objprop> *inputs;
	std::list<objprop> *outputs;
public:
	stochastic(const char *name);
	~stochastic(void);
};

#endif // _STOCHASTIC_H
