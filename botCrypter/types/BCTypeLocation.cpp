#include "StdAfx.h"
#include "BCTypeLocation.h"
//-----------------------------------------------------------------------------
BCTypeLocation::BCTypeLocation()
{

}
//-----------------------------------------------------------------------------
BCTypeLocation::BCTypeLocation(QJsonObject JsonObject)
{
    Longitude = JsonObject.value("longitude").toDouble();
    Latitude = JsonObject.value("latitude").toDouble();
}
//-----------------------------------------------------------------------------
BCTypeLocation::~BCTypeLocation()
{

}
//-----------------------------------------------------------------------------
