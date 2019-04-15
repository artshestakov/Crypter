#include "StdAfx.h"
#include "LCTracer.h"
//-----------------------------------------------------------------------------
LCTracer::LCTracer(const char *function_name) :
	BeginTime(clock()),
	FunctionName(function_name)
{
	printf("[TRACE] Start\t\"%s\"\r\n", FunctionName);
}
//-----------------------------------------------------------------------------
LCTracer::~LCTracer()
{
	clock_t EndTime = clock();
	long Msec = EndTime - BeginTime;
	printf("[TRACE] Finish\t\"%s\" with %ld msec\r\n", FunctionName, Msec);
}
//-----------------------------------------------------------------------------
