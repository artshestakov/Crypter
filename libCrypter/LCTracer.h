#pragma once
//-----------------------------------------------------------------------------
#define TRACE() LCTracer Tracer(__FUNCTION__)
//-----------------------------------------------------------------------------
class LCTracer
{
public:
	LCTracer(const char *function_name);
	~LCTracer();

private:
	clock_t BeginTime;
	const char *FunctionName;
};
//-----------------------------------------------------------------------------