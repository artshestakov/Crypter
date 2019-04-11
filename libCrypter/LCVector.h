#pragma once
//-----------------------------------------------------------------------------
struct LCVector
{
	void **Items;
	int Capacity;
	int Total;
};
//-----------------------------------------------------------------------------
void VectorInit(LCVector *Vector);
int VectorTotal(LCVector *Vector);
static void VectorResize(LCVector *Vector, int Capacity);
void VectorAdd(LCVector *Vector, void *Item);
void VectorSet(LCVector *Vector, int Index, void *Item);
void *VectorGet(LCVector *Vector, int Index);
void VectorDelete(LCVector *Vector, int);
void VectorFree(LCVector *Vector);
//-----------------------------------------------------------------------------
