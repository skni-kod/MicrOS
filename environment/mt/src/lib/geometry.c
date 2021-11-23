#include "geometry.h"
#include <math.h>

vec2i c_vec2i(int x, int y)
{
    vec2i temp = {x,y};
    return temp;
}

vec3f c_vec3f(float x, float y, float z)
{
    vec3f temp = {x,y,z};
    return temp;
}

vec3f barycentric(vec3f A, vec3f B, vec3f C, vec3f P)
{
    vec3f result = {-1.f, 1.f, 1.f};
    vec3f s[2];
    for (int i=2; i--; )
    {
        s[i].x = C.elems[i]-A.elems[i];
        s[i].y = B.elems[i]-A.elems[i];
        s[i].z = A.elems[i]-P.elems[i];
    }
    vec3f u = cross_product_vec3f(s[0], s[1]);
    if (fabs(u.elems[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
    {
        result.x = 1.f-(u.x+u.y)/u.z;
        result.y = u.y/u.z;
        result.z = u.x/u.z;
    }
    return result; // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

vec2i add_vec2i(vec2i a, vec2i b)
{
    vec2i result = {a.x + b.x, a.y + b.y};
    return result;
}

vec2i sub_vec2i(vec2i a, vec2i b)
{
    vec2i result = {a.x - b.x, a.y - b.y};
    return result;
}

vec2i mul_vec2i_f(vec2i a, float b)
{
    vec2i result = {a.x * b, a.y * b};
    return result;
}

vec3f add_vec3f(vec3f a, vec3f b)
{
    vec3f result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

vec3f sub_vec3f(vec3f a, vec3f b)
{
    vec3f result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

vec3f cross_product_vec3f(vec3f a, vec3f b)
{
    vec3f result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}

float dot_product_vec3f(vec3f a, vec3f b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

vec3f mul_vec3f_f(vec3f a, float b)
{
    vec3f result = {a.x * b, a.y * b, a.z * b};
    return result;
}

vec3f normalize_vec3f(vec3f a)
{
    return mul_vec3f_f(a,(1.f/sqrt(a.x*a.x + a.y*a.y + a.z*a.z)));
}

void swap_vec2i(vec2i* a, vec2i*b)
{
    vec2i tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_vec3f(vec3f* a, vec3f*b)
{
    vec3f tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}