#ifndef SRENDER_H
#define SRENDER_H

#include <stdint.h>
#include "ssurface.h"
#include "../lib/geometry.h"

typedef struct _color
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} color;

/*!
    Draw a line on surface using int coordinates.
    \param x0 X coordinate of first point.
    \param y0 Y coordinate of first point.
    \param x1 X coordinate of second point.
    \param y1 Y coordinate of second point.
    \param surf Pointer to ssurface we want to draw on.
    \param c Pointer to color we want to draw with.
    \todo consider changing x and y params of each point to vertex (vec3)
*/
void line_int(int x0, int y0, int x1, int y1, ssurface* surf, color* c);

/*!
    Draw a line on surface using vec2i points.
    \param p0 first point (integer).
    \param p1 second point (integer).
    \param surf Pointer to ssurface we want to draw on.
    \param c Pointer to color we want to draw with.
    \todo consider changing x and y params of each point to vertex (vec3)
*/
void line_vec2i(vec2i p0, vec2i p1, ssurface* surf, color* c);


/*!
    Set viewport coordinates and size
    \param x x axis coordinate of viewport (screen)
    \param y y axis coordinate of viewport (screen)
    \param w width of viewport (screen)
    \param h height of viewport (screen)
*/
void viewport(int x, int y, int w, int h);


void triangle(vec4* pts, vec2i* uvs, ssurface* surf, ssurface* zbuffer, ssurface* tex, color* c);
void triangle_bc(vec3* pts, vec2f* uvs, ssurface* zbuffer, ssurface* surf, ssurface* tex, color* c);

#endif