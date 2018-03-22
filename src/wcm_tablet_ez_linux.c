//
//  wcm_tablet_ez_linux.c
//  wcm
//
//  Created by vs on 3/22/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "wcm_tablet_ez_linux.h"

#ifdef __linux__

#define EASYTAB_IMPLEMENTATION
#include "../contrib/easytab/easytab.h"

Display* Disp;
Window   Win;

int wcm_tablet_ez_linux_init(void)
{
	if (EasyTab_Load(Disp, Win) != EASYTAB_OK) // Load
	{
		printf("Tablet init failed\n");
	}
}

int wcm_tablet_ez_linux_update(void)
{
	while (XPending(Disp)) // Event loop
	{
		XEvent Event;
		XNextEvent(XlibDisplay, &Event);

		if (EasyTab_HandleEvent(&Event) == EASYTAB_OK) // Event
		{
			continue; // Tablet event handled
		}

		switch (Event.type)
		{
			...
		}
	}
}

void wcm_tablet_ez_linux_deinit(void)
{
	EasyTab_Unload();
}

void wcm_tablet_ez_linux_query(double* x double* y, double* pressure)
{

	*x	= EasyTab->PosX;
	*y	= EasyTab->PosY;
	*pressure = EasyTab->Pressure;
}

#endif
