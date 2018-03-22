//
//  wcm_ez.c
//  wcm
//
//  Created by vs on 3/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "wcm_tablet_ez.h"

#ifndef __APPLE__

#ifdef WIN32
#include <wcm_tablet_ez_windows.h>
#endif

#ifdef __linux__
#include <wcm_tablet_ez_linux.h>
#endif

int wcm_ez_init(void)
{
#ifdef __linux__
	wcm_tablet_ez_linux_init();
#endif
#ifdef WIN32
	wcm_tablet_ez_windows_init();
#endif
}

void wcm_ez_deinit(void)
{
#ifdef __linux__
	wcm_tablet_ez_linux_deinit();
#endif
#ifdef WIN32
	wcm_tablet_ez_windows_deinit();

#endif
}

void wcm_ez_update(void)
{
#ifdef __linux__
	wcm_tablet_ez_linux_update();
#endif
#ifdef WIN32
	wcm_tablet_ez_windows_update();
#endif
}

#endif
