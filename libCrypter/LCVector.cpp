#include "StdAfx.h"
#include "LCVector.h"
//-----------------------------------------------------------------------------
#define VECTOR_INIT_CAPACITY 4
//-----------------------------------------------------------------------------
void VectorInit(LCVector *Vector)
{
	Vector->Capacity = VECTOR_INIT_CAPACITY;
	Vector->Total = 0;
	Vector->Items = (void**)malloc(sizeof(void *) * Vector->Capacity);
}
//-----------------------------------------------------------------------------
int VectorTotal(LCVector *Vector)
{
	return Vector->Total;
}
//-----------------------------------------------------------------------------
static void VectorResize(LCVector *Vector, int Capacity)
{
	void **Items = (void**)realloc(Vector->Items, sizeof(void *) * Capacity);
	if (Items)
	{
		Vector->Items = Items;
		Vector->Capacity = Capacity;
	}
}
//-----------------------------------------------------------------------------
void VectorAdd(LCVector *Vector, void *Item)
{
	if (Vector->Capacity == Vector->Total)
	{
		VectorResize(Vector, Vector->Capacity * 2);
	}

	Vector->Items[Vector->Total++] = Item;
}
//-----------------------------------------------------------------------------
void VectorSet(LCVector *Vector, int Index, void *Item)
{
	if (Index >= 0 && Index < Vector->Total)
	{
		Vector->Items[Index] = Item;
	}
}
//-----------------------------------------------------------------------------
void *VectorGet(LCVector *Vector, int Index)
{
	if (Index >= 0 && Index < Vector->Total)
	{
		return Vector->Items[Index];
	}

	return NULL;
}
//-----------------------------------------------------------------------------
void VectorDelete(LCVector *Vector, int Index)
{
	if (Index < 0 || Index >= Vector->Total)
	{
		return;
	}

	Vector->Items[Index] = NULL;

	for (int i = Index; i < Vector->Total - 1; i++)
	{
		Vector->Items[i] = Vector->Items[i + 1];
		Vector->Items[i + 1] = NULL;
	}

	Vector->Total--;

	if (Vector->Total > 0 && Vector->Total == Vector->Capacity / 4)
	{
		VectorResize(Vector, Vector->Capacity / 2);
	}
}
//-----------------------------------------------------------------------------
void VectorFree(LCVector *Vector)
{
	free(Vector->Items);
}
//-----------------------------------------------------------------------------
