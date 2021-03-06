//
//  wcm.h
//  wcm
//
//  Created by vs on 3/21/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef wcm_h
#define wcm_h

typedef void (*tablet_motion_func)(double x, double y, int button, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth);

//	possible values: x y pressure rotation tiltx tilty altitude azimuth?
typedef void (*tablet_down_func)(double x, double y, int button, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth);
typedef void (*tablet_up_func)(double x, double y, int button, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth);
typedef void (*tablet_drag_func)(double x, double y, int button, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth);
typedef void (*tablet_proximity_func)(int v);

int  wcm_init(int window_width, int window_height);
void wcm_deinit(void);
void wcm_update(void);

void wcm_resize(int window_width, int window_height);
void wcm_set_scaling_factor(double scale_factor);

void wcm_set_tablet_motion_func(tablet_motion_func fn);
void wcm_set_tablet_down_func(tablet_down_func fn);
void wcm_set_tablet_up_func(tablet_up_func fn);
void wcm_set_tablet_drag_func(tablet_drag_func fn);
void wcm_set_tablet_proximity_func(tablet_proximity_func fn);

void wcm_recv_tablet_proximity(int v);
void wcm_recv_tablet_motion(double x, double y, int button, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth);
void wcm_recv_tablet_drag(double x, double y, int button, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth);
void wcm_recv_tablet_down(double x, double y, int button, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth);
void wcm_recv_tablet_up(double x, double y, int button, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth);

#ifdef __APPLE__
void wcm_set_event_coalescing(int val);
#endif

//	todo: implement buttons?  NAH haha

#endif /* wcm_h */
