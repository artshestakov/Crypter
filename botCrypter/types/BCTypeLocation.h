#pragma once
#ifndef _BCTYPELOCATION_H_INCLUDED
#define _BCTYPELOCATION_H_INCLUDED
//-----------------------------------------------------------------------------
class BCTypeLocation
{
public:
    BCTypeLocation();
    BCTypeLocation(QJsonObject JsonObject);
    ~BCTypeLocation();

    float Longitude;
    float Latitude;
};
//-----------------------------------------------------------------------------
#endif
