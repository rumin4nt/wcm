//
//  wcm_tablet_ez_windows.c
//  wcm
//
//  Created by vs on 3/22/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "wcm_tablet_ez_windows.h"

#ifdef WIN32
#include <windows.h>
#define EASYTAB_IMPLEMENTATION
#include "../contrib/easytab/easytab.h"

HWND Window;

int wcm_tablet_ez_windows_init(void)
{

	if (EasyTab_Load(Window) != EASYTAB_OK) // Load
	{
		OutputDebugStringA("Tablet init failed\n");
		return false;
	}
}

int wcm_tablet_ez_windows_update(void)
{
	if (EasyTab_HandleEvent(Window, Message, LParam, WParam) == EASYTAB_OK) // Event
	{
		return true; // Tablet event handled
	}

	switch (Message)
	{
		...
	}
}

int wcm_tablet_ez_windows_deinit(void)
{
	EasyTab_Unload();
}

void wcm_ez_query(double* x, double* y, double* pressure)
{
	*x	= EasyTab->PosX;
	*y	= EasyTab->PosY;
	*pressure = EasyTab->Pressure;
}

#endif
