//
//  wcm_ez.c
//  wcm
//
//  Created by vs on 3/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "wcm_ez.h"

#ifndef __APPLE__
#define EASYTAB_IMPLEMENTATION
#include "../../../contrib/easytab/easytab.h"
#endif



int wcm_ez_init(void)
{
	
#ifdef __APPLE__
	return 1;
#else
	
	return 0;
#endif
}

void wcm_ez_deinit(void)
{
	
}


