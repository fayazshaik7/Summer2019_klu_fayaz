// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <tchar.h>
#ifndef _stdfax_h
	#define _stdfax_h
void PRINT(struct cell **sheet);
struct cell **createSheet();

#endif

// TODO: reference additional headers your program requires here
