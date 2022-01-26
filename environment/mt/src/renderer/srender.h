#ifndef SRENDER_H
#define SRENDER_H

#include <stdint.h>
#include "ssurface.h"
#include "../lib/geometry.h"
#include "../lib/vector.h"

typedef struct _color
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} color;

typedef struct _vert
{
    vec4 vertex;
    vec2i uv;
    vec3 normal;
} vert;

enum varying_type
{
    VT_VEC3 = 0,
    VT_VEC2I = 1,
    VT_VEC4 = 2
};

//A and B are for filled triangle rasterization only, figure out better way to achieve what I need.
typedef struct _varying
{
    uint8_t type;
    void* data;
    void* fragmentData;
    void* A;
    void* B;
} varying;

typedef struct _shader shader;

struct _shader
{
    vector uniforms;
    vector varyings;
    void (*vertex)(shader*, vec4, vec3, vec2f, int);
    color (*fragment)(shader*);
};


enum render_type
{
    SGL_TRIANGLES,
    SGL_WIRE,
    SGL_FAST
};

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


/*!
    Draw vertices on screen in given mode using given shader program.
    \param mode render_type enum specifying rendering mode (wireframe, triangles, quads).
    \param vertices vector containing vertices used for drawing.
    \param indices vector containing indices used for drawing.
    \param shader_program pointer to shader program.
*/
void sgl_draw(uint8_t mode, vector* vertices, vector* indices, vector* normals, vector* uvs, shader* shader_program, ssurface* buffer, ssurface* zbuffer);


void triangle(vec4* pts, vec2i* uvs, ssurface* surf, ssurface* zbuffer, ssurface* tex, color* c);
void triangle_shader(ssurface* surf, ssurface* zbuffer, shader* shader_program);
void triangle_bc(vec3* pts, vec2f* uvs, ssurface* zbuffer, ssurface* surf, ssurface* tex, color* c);

shader* create_shader(vert (*vert_ptr)(shader*, vec4, vec3, vec2f, int), color (*frag_ptr)(shader*));

//color math
color add_color(color left, color right);
color mul_color_f(color c, float f);

color blend_colors(color a, color b);

#endif