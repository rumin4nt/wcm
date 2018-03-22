//
//  wcm_tablet_ez_windows.h
//  wcm
//
//  Created by vs on 3/22/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef wcm_tablet_ez_windows_h
#define wcm_tablet_ez_windows_h

#ifdef WIN32

int  wcm_tablet_ez_windows_init(void);
int  wcm_tablet_ez_windows_update(void);
void wcm_tablet_ez_windows_deinit(void);
void wcm_ez_query(double* x, double* y, double* pressure);

#endif

#endif /* wcm_tablet_ez_windows_h */
