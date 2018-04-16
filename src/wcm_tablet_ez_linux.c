//
//  wcm_tablet_ez_linux.c
//  wcm
//
//  Created by vs on 3/22/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "wcm_tablet_ez_linux.h"

#ifdef __linux__

#include <stdio.h>

#define EASYTAB_IMPLEMENTATION
#include "../contrib/easytab/easytab.h"
#include <X11/Xlib.h>
#include <stdbool.h>

Display* Disp;
Window   Win;

int wcm_tablet_ez_linux_init(void)
{
	Disp = XOpenDisplay(NULL);
	if ( !Disp )
	{
		printf("Couldn't connect to x server?\n");
		return false;
	}
	if (EasyTab_Load(Disp, Win) != EASYTAB_OK) // Load
	{
		printf("Tablet init failed\n");
		return false;
	}
	return true;
}

int wcm_tablet_ez_linux_update(void)
{
	while (XPending(Disp)) // Event loop
	{
		XEvent Event;
		XNextEvent(Disp, &Event);

		if (EasyTab_HandleEvent(&Event) == EASYTAB_OK) // Event
		{
			printf("m?");
			continue; // Tablet event handled
		}
		//if ( Event->type == EasyTab->MotionType)
		//{

		//}
		printf("Event: %d\n", Event.type);
		/*switch (Event.type)
		{
			case EasyTab->MotionType:
				printf("m");
				break;
			default:
				printf("no");
				break;
		}*/

	}
}

void wcm_tablet_ez_linux_deinit(void)
{
	EasyTab_Unload(Disp);
}

void wcm_tablet_ez_linux_query(double* x, double* y, double* pressure)
{

	*x	= EasyTab->PosX;
	*y	= EasyTab->PosY;
	*pressure = EasyTab->Pressure;
}

#endif
