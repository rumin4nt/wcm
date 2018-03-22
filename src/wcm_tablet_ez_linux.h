//
//  wcm_tablet_ez_linux.h
//  wcm
//
//  Created by vs on 3/22/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef wcm_tablet_ez_linux_h
#define wcm_tablet_ez_linux_h

#ifdef __linux__

int  wcm_tablet_ez_linux_init(void);
int  wcm_tablet_ez_linux_update(void);
void wcm_tablet_ez_linux_deinit(void);
void wcm_ez_query(double* x, double* y, double* pressure);

#endif

#endif /* wcm_tablet_ez_linux_h */
