#ifndef GEOMETRY_H
#define GEOMETRY_H

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

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

typedef struct _vec3f
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
} vec3f;

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

/*!
    Create vec2i from coordinates
    \param x X coordinate
    \param y Y coordinate
    \return vec2i with x,y coordinates
*/
vec2i c_vec2i(int x, int y);

/*!
    Create vec3f from coordinates
    \param x X coordinate
    \param y Y coordinate
    \param z Z coordinate
    \return vec3f with x,y,z coordinates
*/
vec3f c_vec3f(float x, float y, float z);

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
vec3f add_vec3f(vec3f a, vec3f b);

/*!
    Subtract two vec3f (a-b)
    \param a first term.
    \param b second term.
    \return vec3f equal to subtraction of a and b vectors.
*/
vec3f sub_vec3f(vec3f a, vec3f b);

/*!
    Cross product of vectors a and b
    \param a first term.
    \param b second term.
    \return vec3f equal to cross product of a and b.
*/
vec3f cross_product_vec3f(vec3f a, vec3f b);

/*!
    dot product of vectors a and b
    \param a first term.
    \param b second term.
    \return vec3f equal to dot product of a and b.
*/
float dot_product_vec3f(vec3f a, vec3f b);

/*!
    Multiply vec3f by float
    \param a vec3f object.
    \param b float value.
    \return vec3f equal to a*b.
*/
vec3f mul_vec3f_f(vec3f a, float b);

/*!
    Normalizes vector (vec3f)
    \param a Vector to be normalized.
    \return normalized vector a.
*/
vec3f normalize_vec3f(vec3f a);

/*!
    Calculate barycentric cooridnates of point P in triangle space defined by pts.
    \param A A vertex of triangle.
    \param B B vertex of triangle.
    \param C C vertex of triangle.
    \param P point we're calculating barycentric coordinates for.
    \return vec3f containing barycentric coordinates of point P.
*/
vec3f barycentric(vec3f A, vec3f B, vec3f C, vec3f P);

//Helper functions, swapping etc

/*!
    Swaps two vec2i elements.
    \param a first vec2i
    \param b second vec2i
*/
void swap_vec2i(vec2i* a, vec2i*b);

/*!
    Swaps two vec3f elements.
    \param a first vec3f
    \param b second vec3f
*/
void swap_vec3f(vec3f* a, vec3f*b);

#endif