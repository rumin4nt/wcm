//
//  wcm.c
//  wcm
//
//  Created by vs on 3/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "wcm.h"

static int window_width;
static int window_height;

#include <stdlib.h>

#ifdef __APPLE__
#include "wcm_tablet_cocoa.h"
#else

#include "wcm_tablet_ez.h"
#endif

#
/*
 typedef void(*tablet_down_func)(double x, double y, double p);
 typedef void(*tablet_up_func)(double x, double y, double p);
 typedef void(*tablet_drag_func)(double x, double y, double p);
 typedef void(*tablet_proximity_func)(int v);

 */

static tablet_down_func      tdown   = NULL;
static tablet_up_func	tup     = NULL;
static tablet_drag_func      tdrag   = NULL;
static tablet_motion_func    tmotion = NULL;
static tablet_proximity_func tprox   = NULL;

int wcm_init(int w, int h)
{
	window_width  = w;
	window_height = h;

#ifdef __APPLE__
	wcm_tablet_cocoa_init();
	return 0;
#else

	return wcm_ez_init();
#endif

	return 1;
}

void wcm_deinit(void)
{

#ifdef __APPLE__
	wcm_tablet_cocoa_deinit();

#else

	wcm_ez_deinit();

#endif
}

void wcm_resize(int w, int h)
{
	window_width  = w;
	window_height = h;
}

void wcm_set_scaling_factor(double scale_factor)
{
	//todo
}

void wcm_set_tablet_motion_func(tablet_motion_func fn)
{
	tmotion = fn;
}
void wcm_set_tablet_down_func(tablet_down_func fn)
{
	tdown = fn;
}
void wcm_set_tablet_up_func(tablet_up_func fn)
{
	tup = fn;
}
void wcm_set_tablet_drag_func(tablet_drag_func fn)
{
	tdrag = fn;
}
void wcm_set_tablet_proximity_func(tablet_proximity_func fn)
{
	tprox = fn;
}

void wcm_recv_tablet_proximity(int v)
{
	if (tprox)
	{
		tprox(v);
	}
}

void wcm_recv_tablet_motion(double x, double y, int b, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth)
{
	if (tmotion)
	{
		tmotion(x, y, b, p, r, tx, ty, tangential, altitude, azimuth);
	}
	else
	{

		printf("no motion cn\n");
	}
}

void wcm_recv_tablet_drag(double x, double y, int b, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth)
{
	if (tdrag)
	{
		tdrag(x, y, b, p, r, tx, ty, tangential, altitude, azimuth);
	}
}

void wcm_recv_tablet_down(double x, double y, int b, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth)
{
	if (tdown)
	{
		tdown(x, y, b, p, r, tx, ty, tangential, altitude, azimuth);
	}
}

void wcm_recv_tablet_up(double x, double y, int b, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth)
{
	if (tup)
	{
		tup(x, y, b, p, r, tx, ty, tangential, altitude, azimuth);
	}
}
