#ifndef GEOMETRY_H
#define GEOMETRY_H

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define PI 3.14159265

typedef struct _vec2i
{
    union
    {
        struct
        {
            int x;
            int y;
        };
        struct
        {
            int u;
            int v;
        };
        struct
        {
            int r;
            int g;
        };
        int elems[2];      
    };
    
} vec2i;

typedef struct _vec2f
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        struct
        {
            float u;
            float v;
        };
        struct
        {
            float r;
            float g;
        }; 
        float elems[2];      
    };
    
} vec2f;

typedef struct _vec3
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        struct
        {
            float u;
            float v;
            float w;
        };
        struct
        {
            float r;
            float g;
            float b;
        }; 
        float elems[3];      
    };
} vec3;

typedef struct _vec3i
{
    union
    {
        struct
        {
            int x;
            int y;
            int z;
        };
        struct
        {
            int u;
            int v;
            int w;
        };
        struct
        {
            int r;
            int g;
            int b;
        }; 
        int elems[3];      
    };
} vec3i;

typedef struct _vec4
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float elems[4];
    };
    
} vec4;

typedef union mat4
{
    float elems[4][4];
    float m[16];
} mat4;

/*!
    Create identity matrix 4x4.
    \return 4x4 identity matrix.
*/
mat4 mat4_identity();

/*!
    Create transposed matrix 4x4 from m.
    \param m 4x4 matrix to be transposed.
    \return 4x4 transposed matrix.
*/
mat4 mat4_transpose(mat4 m);

/*!
    Create inverse of matrix m.
    \param m 4x4 matrix to be inversed.
    \return 4x4 inversed matrix.
*/
mat4 mat4_inverse(mat4 m);

/*!
    Multiply matrices.
    \param a matrix.
    \param b matrix.
    \return result of operation a * b.
*/
mat4 multiply_mat4(mat4 left, mat4 right);

/*!
    Multiply matrix by scalar.
    \param left matrix.
    \param right scalar.
    \return result of operation a * b.
*/
mat4 multiply_mat4f(mat4 left, float right);

/*!
    Multiply matrix by vector.
    \param a matrix.
    \param b vector.
    \return result of operation a * b.
*/
vec4 multiply_mat4_vec4(mat4 left, vec4 right);

/*!
    Create vec2i from coordinates
    \param x X coordinate
    \param y Y coordinate
    \return vec2i with x,y coordinates
*/
vec2i c_vec2i(int x, int y);

/*!
    Create vec3 from coordinates
    \param x X coordinate
    \param y Y coordinate
    \param z Z coordinate
    \return vec3 with x,y,z coordinates
*/
vec3 c_vec3(float x, float y, float z);

//Some math functions. Refer to function documentation for more information.

/*!
    Add two vec2i together
    \param a first term.
    \param b second term.
    \return vec2i equal to sum of a and b.
*/
vec2i add_vec2i(vec2i a, vec2i b);

/*!
    Subtract two vec2i (a-b)
    \param a first term.
    \param b second term.
    \return vec2i equal to subtraction of a and b vectors.
*/
vec2i sub_vec2i(vec2i a, vec2i b);

/*!
    Multiply vec2i by float
    \param a vec2i object.
    \param b float value.
    \return vec2i equal to a*b.
*/
vec2i mul_vec2i_f(vec2i a, float b);

/*!
    Add two vec2i together
    \param a first term.
    \param b second term.
    \return vec2i equal to sum of a and b.
*/
vec3 add_vec3(vec3 a, vec3 b);

/*!
    Subtract two vec3 (a-b)
    \param a first term.
    \param b second term.
    \return vec3 equal to subtraction of a and b vectors.
*/
vec3 sub_vec3(vec3 a, vec3 b);

/*!
    Cross product of vectors a and b
    \param a first term.
    \param b second term.
    \return vec3 equal to cross product of a and b.
*/
vec3 cross_product_vec3(vec3 a, vec3 b);

/*!
    dot product of vectors a and b
    \param a first term.
    \param b second term.
    \return vec3 equal to dot product of a and b.
*/
float dot_product_vec3(vec3 a, vec3 b);

/*!
    Multiply vec3 by float
    \param a vec3 object.
    \param b float value.
    \return vec3 equal to a*b.
*/
vec3 mul_vec3_f(vec3 a, float b);

/*!
    Normalizes vector (vec3)
    \param a Vector to be normalized.
    \return normalized vector a.
*/
vec3 normalize_vec3(vec3 a);

/*!
    Calculate barycentric cooridnates of point P in triangle space defined by pts.
    \param A A vertex of triangle.
    \param B B vertex of triangle.
    \param C C vertex of triangle.
    \param P point we're calculating barycentric coordinates for.
    \return vec3 containing barycentric coordinates of point P.
*/
vec3 barycentric(vec3 A, vec3 B, vec3 C, vec3 P);

//Helper functions, swapping etc

/*!
    Swaps two vec2i elements.
    \param a first vec2i
    \param b second vec2i
*/
void swap_vec2i(vec2i* a, vec2i*b);

/*!
    Swaps two vec3 elements.
    \param a first vec3
    \param b second vec3
*/
void swap_vec3(vec3* a, vec3*b);

//common graphics stuff

/*!
    Creates perspective projection matrix.
    \param fov Field of view.
    \param aspect_ratio Aspect ratio of current viewport.
    \param near Near field of projection.
    \param far Far field of projection.
    \return Matrix containing perspective projection.
*/
mat4 perspective(float fov, float aspect_ratio, float near, float far);

//TODO Add ortho

/*!
    sets translation in transformation matrix.
    \param translation translation vector.
    \return transformation matrix after translation.
*/
mat4 translate(vec3 translation);
#endif