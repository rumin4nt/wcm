//
//  wcm_ez.h
//  wcm
//
//  Created by vs on 3/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef wcm_ez_h
#define wcm_ez_h

//	thin wrapper for eztab - want to remove and replace with native
//	x11 / wintab / cocoa
//	code support as eztab *only* supports pressure but just
//	trying to get the ball rolling here

int wcm_ez_init(void);
void wcm_ez_deinit(void);

#endif /* wcm_ez_h */
