// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <gdiplus.h>
#include <string>

using namespace Gdiplus;
using namespace std;

#include <vector>



// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// Classes

class person
{
public:

	int weight = 70;
	int from = 0;
	int to = 0;
};

class elevator_c
{
public:
	POINT poz = { 610, 500 };
	int lifting_cap = 600;
	int current_weight = 0;
	int velocity = 33;
};

class level
{
public:
	POINT poz;
};
