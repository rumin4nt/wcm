//
//  wcm.c
//  wcm
//
//  Created by vs on 3/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "wcm.h"

#ifdef __APPLE__
	#include "platform/macos/r_input_tablet_cocoa.h"
#else
	#include "platform/common/wcm_ez.h"
#endif

int wcm_init(int window_width, int window_height)
{
	
#ifdef __APPLE__
	r_input_tablet_cocoa_init();
	return 0;
#else
	return wcm_ez_init();
#endif
	
	return 1;

}

void wcm_deinit(void)
{

#ifdef __APPLE__
	r_input_tablet_cocoa_deinit();

#else

	wcm_ez_deinit();

#endif
}
