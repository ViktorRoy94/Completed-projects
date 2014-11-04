
#include "stdafx.h"
double PortableGetTime()
{
    return GetTickCount() * 1.0e-3;
}
