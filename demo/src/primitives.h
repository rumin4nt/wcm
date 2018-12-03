//
//  primitives.h
//  wash-demo
//
//  Created by vs on 3/20/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef primitives_h
#define primitives_h

void d_color_clear(double r, double g, double b, double a);
void d_clear(void);
void d_setup(int width, int height);
void d_setup_view_ortho(int w, int h);

void d_set_dpiscale(double v);

void drw_push(void);
void d_pop(void);
void d_translate(double x, double y, double z);
void d_color(double r, double g, double b, double a);

void drw_line(double ax, double ay, double bx, double by);
void d_circle(float);
void d_ellipse(float, float);

#endif /* primitives_h */
