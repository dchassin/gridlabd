// File: stochastic.cpp
// Copyright: 2020, Regents of the Leland Stanford Junior University
// Author: DP Chassin (dchassin@slac.stanford.edu)

#include "gldcore.h"

stochastic::stochastic(const char* str)
{
	name = strdup(name);
	refresh = 0;
	samples = 0;
	inputs = new std::list(objprop);
	outputs = new std::list(objprop);
}