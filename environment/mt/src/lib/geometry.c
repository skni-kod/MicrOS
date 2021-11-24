#include "geometry.h"
#include <math.h>
#include <assert.h>

vec2i c_vec2i(int x, int y)
{
    vec2i temp = {x,y};
    return temp;
}

vec3 c_vec3(float x, float y, float z)
{
    vec3 temp = {x,y,z};
    return temp;
}

vec3 barycentric(vec3 A, vec3 B, vec3 C, vec3 P)
{
    vec3 result = {-1.f, 1.f, 1.f};
    vec3 s[2];
    for (int i=2; i--; )
    {
        s[i].x = C.elems[i]-A.elems[i];
        s[i].y = B.elems[i]-A.elems[i];
        s[i].z = A.elems[i]-P.elems[i];
    }
    vec3 u = cross_product_vec3(s[0], s[1]);
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

vec3 add_vec3(vec3 a, vec3 b)
{
    vec3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

vec3 sub_vec3(vec3 a, vec3 b)
{
    vec3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

vec3 cross_product_vec3(vec3 a, vec3 b)
{
    vec3 result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}

float dot_product_vec3(vec3 a, vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

vec3 mul_vec3_f(vec3 a, float b)
{
    vec3 result = {a.x * b, a.y * b, a.z * b};
    return result;
}

vec3 normalize_vec3(vec3 a)
{
    return mul_vec3_f(a,(1.f/sqrt(a.x*a.x + a.y*a.y + a.z*a.z)));
}

void swap_vec2i(vec2i* a, vec2i*b)
{
    vec2i tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_vec3(vec3* a, vec3*b)
{
    vec3 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

//Matrix stuff
mat4 mat4_identity()
{
    mat4 result = {1.,0.,0.,0.,
                   0.,1.,0.,0.,
                   0.,0.,1.,0.,
                   0.,0.,0.,1};
    return result;
}

mat4 mat4_transpose(mat4 m)
{
    mat4 result = m;
    for(int columns = 0; columns < 4; ++columns)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            result.elems[rows][columns] = m.elems[columns][rows];
        }
    }
    return result;
}

mat4 mat4_inverse(mat4 m)
{
    //lol
    assert(0);
}

mat4 multiply_mat4(mat4 left, mat4 right)
{
    mat4 result;
    for(int columns = 0; columns < 4; ++columns)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            float sum = 0;
            int current_mat;
            for(current_mat = 0; current_mat < 4; ++current_mat)
            {
                sum += left.elems[current_mat][rows] * right.elems[columns][current_mat];
            }
        }
    }
    return result;
}

mat4 multiply_mat4f(mat4 left, float right)
{
    mat4 result;
    for(int columns = 0; columns < 4; ++columns)
    {
        for(int rows = 0; rows < 4; ++rows)
        {
            result.elems[columns][rows] = left.elems[columns][rows] * right;
        }
    }
    return result;
}

vec4 multiply_mat4_vec4(mat4 left, vec4 right)
{
    vec4 result;
    int columns, rows;
    for(rows = 0; rows < 4; ++rows)
    {
        float sum = 0;
        for(columns = 0; columns < 4; ++columns)
        {
            sum += left.elems[columns][rows] * right.elems[columns];
        }
        result.elems[rows] = sum;
    }
    return result;
}

mat4 perspective(float fov, float aspect_ratio, float near, float far)
{
    mat4 result = {};

    float cotangent = 1.0f / tan(fov * (PI / 360.0f));

    result.elems[0][0] = cotangent / aspect_ratio;
    result.elems[1][1] = cotangent;
    result.elems[2][3] = -1.0f;
    result.elems[2][2] = (near + far) / (near - far);
    result.elems[3][2] = (2.0f * near * far) / (near - far);
    result.elems[3][3] = 0.0f;

    return result;
}

mat4 translate(vec3 translation)
{
    mat4 result = mat4_identity();

    result.elems[3][0] = translation.x;
    result.elems[3][1] = translation.y;
    result.elems[3][2] = translation.z;

    return result;
}