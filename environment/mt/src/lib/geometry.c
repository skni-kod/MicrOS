#include "geometry.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

vec2i c_vec2i(int x, int y)
{
    vec2i temp = {{{x,y}}};
    return temp;
}

vec3 c_vec3(float x, float y, float z)
{
    vec3 temp = {{{x,y,z}}};
    return temp;
}

vec3 barycentric(vec3 A, vec3 B, vec3 C, vec3 P)
{
    vec3 result = {{{-1.f, 1.f, 1.f}}};
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
    vec2i result = {{{a.x + b.x, a.y + b.y}}};
    return result;
}

vec2i sub_vec2i(vec2i a, vec2i b)
{
    vec2i result = {{{a.x - b.x, a.y - b.y}}};
    return result;
}

vec2i mul_vec2i_f(vec2i a, float b)
{
    vec2i result = {{{a.x * b, a.y * b}}};
    return result;
}

vec3 add_vec3(vec3 a, vec3 b)
{
    vec3 result = {{{a.x + b.x, a.y + b.y, a.z + b.z}}};
    return result;
}

vec3 sub_vec3(vec3 a, vec3 b)
{
    vec3 result = {{{a.x - b.x, a.y - b.y, a.z - b.z}}};
    return result;
}

vec3 div_vec3(vec3 vector, float scalar)
{
    return mul_vec3_f(vector, 1/scalar);
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
    vec3 result = {{{a.x * b, a.y * b, a.z * b}}};
    return result;
}

vec3 normalize_vec3(vec3 a)
{
    return mul_vec3_f(a,(1.f/sqrt(a.x*a.x + a.y*a.y + a.z*a.z)));
}

vec4 add_vec4(vec4 a, vec4 b)
{
    vec4 result = {{{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}}};
    return result;
}

vec4 sub_vec4(vec4 a, vec4 b)
{
    vec4 result = {{{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w}}};
    return result;
}

vec4 div_vec4_f(vec4 a, float b)
{
    return mul_vec4_f(a, 1/b);
}

vec4 mul_vec4_f(vec4 a, float b)
{
    vec4 result = {{{a.x * b, a.y * b, a.z * b, a.w * b}}};
    return result;
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

void swap_vec4(vec4* a, vec4*b)
{
    vec4 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

vec3 neg_vec3(vec3 a)
{
    vec3 result = {{{-a.x, -a.y, -a.z}}};
    return result;
}

//Matrix stuff
mat4 mat4_identity()
{
    mat4 result = {0};

    result.elems[0][0] = 1.f;
    result.elems[1][1] = 1.f;
    result.elems[2][2] = 1.f;
    result.elems[3][3] = 1.f;
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

static float invf(int i,int j,const float *m)
{
    int o = 2+(j-i);
    i += 4+o;
    j += 4-o;
    
    #define e(a,b) m[ ((j+b)%4)*4 + ((i+a)%4) ]

    float inv =
     + e(+1,-1)*e(+0,+0)*e(-1,+1)
     + e(+1,+1)*e(+0,-1)*e(-1,+0)
     + e(-1,-1)*e(+1,+0)*e(+0,+1)
     - e(-1,-1)*e(+0,+0)*e(+1,+1)
     - e(-1,+1)*e(+0,-1)*e(+1,+0)
     - e(+1,-1)*e(-1,+0)*e(+0,+1);
    
    return (o%2)?inv : -inv;
    #undef e 
}

mat4 mat4_inverse(mat4 m)
{
    mat4 result = {};
    float inv[16];
    
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            inv[j*4+i] = invf(i,j,m.linear_elems);
    
    double D = 0;
    
    for(int k=0;k<4;k++) D += m.linear_elems[k] * inv[k*4];
    
    D = 1.0 / D;
    
    for (int i = 0; i < 16; i++)
        result.linear_elems[i] = inv[i] * D;

    return result;
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
            result.elems[columns][rows] = sum;
        }
    }
    return result;
}

mat4 multiply_mat4f(mat4 left, float right)
{
    mat4 result;
    for(int i = 0; i < 16; i++)
        result.linear_elems[i] = left.linear_elems[i] * right;
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

mat4 lookat(vec3 eye, vec3 center, vec3 up)
{
    mat4  result;

    vec3 F = normalize_vec3(sub_vec3(center, eye));
    vec3 S = normalize_vec3(cross_product_vec3(F, up));
    vec3 U = cross_product_vec3(S, F);

    result.elems[0][0] = S.x;
    result.elems[0][1] = U.x;
    result.elems[0][2] = -F.x;
    result.elems[0][3] = 0.0f;

    result.elems[1][0] = S.y;
    result.elems[1][1] = U.y;
    result.elems[1][2] = -F.y;
    result.elems[1][3] = 0.0f;

    result.elems[2][0] = S.z;
    result.elems[2][1] = U.z;
    result.elems[2][2] = -F.z;
    result.elems[2][3] = 0.0f;

    result.elems[3][0] = -dot_product_vec3(S, eye);
    result.elems[3][1] = -dot_product_vec3(U, eye);
    result.elems[3][2] = dot_product_vec3(F, eye);
    result.elems[3][3] = 1.0f;

    return (result);
}

mat4 translate(vec3 translation)
{
    mat4 result = mat4_identity();

    result.elems[3][0] = translation.x;
    result.elems[3][1] = translation.y;
    result.elems[3][2] = translation.z;

    return result;
}

mat4 scale(vec3 scale)
{
    mat4 result = mat4_identity();

    result.elems[0][0] = scale.x;
    result.elems[1][1] = scale.y;
    result.elems[2][2] = scale.z;

    return result;
}

mat4 rotate(float angle, vec3 axis)
{
    mat4 result = mat4_identity();

    axis = normalize_vec3(axis);


    float sinangle = sin(angle*(PI/180.f));
    float cosangle = cos(angle*(PI/180.f));
    float cosvalue = 1.0f - cosangle;

    result.elems[0][0] = (axis.x * axis.x * cosvalue) + cosangle;
    result.elems[0][1] = (axis.x * axis.y * cosvalue) + (axis.z * sinangle);
    result.elems[0][2] = (axis.x * axis.z * cosvalue) - (axis.y * sinangle);

    result.elems[1][0] = (axis.y * axis.x * cosvalue) - (axis.z * sinangle);
    result.elems[1][1] = (axis.y * axis.y * cosvalue) + cosangle;
    result.elems[1][2] = (axis.y * axis.z * cosvalue) + (axis.x * sinangle);

    result.elems[2][0] = (axis.z * axis.x * cosvalue) + (axis.y * sinangle);
    result.elems[2][1] = (axis.z * axis.y * cosvalue) - (axis.x * sinangle);
    result.elems[2][2] = (axis.z * axis.z * cosvalue) + cosangle;

    return (result);
}

vec3 reflect_vec3(vec3 I, vec3 N)
{
    return sub_vec3(I, mul_vec3_f(mul_vec3_f(N, dot_product_vec3(N,I)), -2));
}

vec3 mul_mat3_vec3(mat3 m, vec3 v)
{
    vec3 result;
    int columns, rows;
    for(rows = 0; rows < 3; ++rows)
    {
        float sum = 0;
        for(columns = 0; columns < 3; ++columns)
        {
            sum += m.elems[columns][rows] * v.elems[columns];
        }
        result.elems[rows] = sum;
    }
    return result;
}

mat3 mat3_from_mat4(mat4 m)
{
    mat3 result;

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            result.elems[i][j] = m.elems[i][j];
        }
    }

    return result;
}