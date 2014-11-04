#include "portable_time.h"
#include <windows.h>
#include "stdafx.h"
double PortableGetTime()
{
    return GetTickCount() * 1.0e-3;
}
