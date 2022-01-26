#include "srender.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

#include <time.h>

typedef struct uint24
{
    unsigned int data : 24;
} __attribute__((packed)) uint24_t;

int viewport_x = 0;
int viewport_y = 0;
int viewport_w = 0;
int viewport_h = 0;

/*!
    Helper function to swap a and b numbers.
    \param a Pointer to first integer to be swapped.
    \param b Pointer to second integer to be swapped.
*/
void swap(int* a, int*b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void line_int(int x0, int y0, int x1, int y1, ssurface* surf, color* c)
{
    // Function uses alot of lines to determine additional things, but after compilation with O3 flag (O2 should work too)
    // It is going to be way faster, since we're operating only on integers and are not introducing any divisions.
    // Multiplications by 2 are fast since these can be changed to SHL instruction.
    //Line drawing uses Bresenham's line algorithm
    bool steep = false; 
    //Check if line is steep
    if (abs(x0-x1)<abs(y0-y1))
    {
        //Swap coordinates to fix line drawing
        swap(&x0, &y0); 
        swap(&x1, &y1); 
        steep = true; 
    } 
    //Check if line is going to be drawn left-to-right
    if (x0>x1)
    { 
        // make it left−to−right 
        swap(&x0, &x1); 
        swap(&y0, &y1); 
    }
    
    //calculate deltas
    int dx = x1-x0; 
    int dy = y1-y0; 
    //calculate error and delta error
    int derror2 = abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 

    for(int x = x0; x<=x1; x++)
    {
        //draw depending on if line is steep or not (swapping cooridnates)
        if(steep)
        {
            *(uint32_t*)(surf->data + ((x * surf->width * surf->bpp) +  (y * surf->bpp))) = *(uint32_t*)c; 
        }
        else
        {
            *(uint32_t*)(surf->data + ((y * surf->width * surf->bpp) +  (x * surf->bpp))) = *(uint32_t*)c; 
        }

        //increase error by delta error
        error2 += derror2;
        if(error2 > dx)
        {
            //increase or decrease y by 1, depending on direction the line is supposed to go
            y += (y1 > y0 ? 1 : -1);
            //decrease error by 2 times delta x
            error2 -= dx*2;
        }
    }
}

void line_vec2i(vec2i p0, vec2i p1, ssurface* surf, color* c)
{
    // Function uses alot of lines to determine additional things, but after compilation with O3 flag (O2 should work too)
    // It is going to be way faster, since we're operating only on integers and are not introducing any divisions.
    // Multiplications by 2 are fast since these can be changed to SHL instruction.
    //Line drawing uses Bresenham's line algorithm
    bool steep = false; 
    //Check if line is steep
    if (abs(p0.x-p1.x)<abs(p0.y-p1.y))
    {
        //Swap coordinates to fix line drawing
        swap(&p0.x, &p0.y); 
        swap(&p1.x, &p1.y); 
        steep = true; 
    } 
    //Check if line is going to be drawn left-to-right
    if (p0.x>p1.x)
    { 
        // make it left−to−right 
        swap(&p0.x, &p0.x); 
        swap(&p0.y, &p1.y); 
    }
    
    //calculate deltas
    int dx = p1.x-p0.x; 
    int dy = p1.y-p0.y; 
    //calculate error and delta error
    int derror2 = abs(dy)*2; 
    int error2 = 0; 
    int y = p0.y; 

    for(int x = p0.x; x<=p1.x; x++)
    {
        //draw depending on if line is steep or not (swapping cooridnates)
        if(steep)
        {
            *(uint32_t*)(surf->data + ((x * surf->width * surf->bpp) +  (y * surf->bpp))) = *(uint32_t*)c; 
        }
        else
        {
            *(uint32_t*)(surf->data + ((y * surf->width * surf->bpp) +  (x * surf->bpp))) = *(uint32_t*)c; 
        }

        //increase error by delta error
        error2 += derror2;
        if(error2 > dx)
        {
            //increase or decrease y by 1, depending on direction the line is supposed to go
            y += (p1.y > p0.y ? 1 : -1);
            //decrease error by 2 times delta x
            error2 -= dx*2;
        }
    }
}

color blend_colors(color a, color b)
{
    color result = {(a.b * b.b)/255, (a.g * b.g)/255, (a.r * b.r)/255, (a.a * b.a)/255};
    return result;
}

static int is_vertex_visible(vec4 v)
{
    return fabs(v.x) <= v.w && fabs(v.y) <= v.w && fabs(v.z) <= v.w;
}

static int is_back_facing(vec3 ndc_coords[3])
{
    vec3 a = ndc_coords[0];
    vec3 b = ndc_coords[1];
    vec3 c = ndc_coords[2];
    float signed_area = a.x * b.y - a.y * b.x +
                        b.x * c.y - b.y * c.x +
                        c.x * a.y - c.y * a.x;
    return signed_area <= 0;
}

//Todo speed up rasterization
//TODO Divide drawing routines to vertex and fragment elements.
// Fragment drawing will rasterize triangle, vertex drawing will only calculate position, so we can determine if given pixel is visible.
// This way it should make rendering way faster, since vertex operations are way faster.
void triangle(vec4* pts, vec2i* uvs, ssurface* surf, ssurface* zbuffer, ssurface* tex, color* c)
{
    //Check if triangle is at all visible (implement better culling algorithm)

    int t1_vis = is_vertex_visible(pts[0]);
    int t2_vis = is_vertex_visible(pts[1]);
    int t3_vis = is_vertex_visible(pts[2]);
    if(!t1_vis && !t2_vis && !t3_vis) return;
    
    vec3 t[3];
    t[0] = div_vec3(pts[0].xyz, pts[0].w);
    t[1] = div_vec3(pts[1].xyz, pts[1].w);
    t[2] = div_vec3(pts[2].xyz, pts[2].w);

    if(is_back_facing(t)) return;

    t[0].x = (int)((t[0].x+1.)*(viewport_w/2.f) + viewport_x);
    t[0].y = (int)((t[0].y+1.)*(viewport_h/2.f) + viewport_y);
    t[0].z = (t[0].z + 1.f) * 0.5f;
    t[0].z = CLAMP(t[0].z, 0.0f, 1.0f); 
    
    t[1].x = (int)((t[1].x+1.)*(viewport_w/2.f) + viewport_x);
    t[1].y = (int)((t[1].y+1.)*(viewport_h/2.f) + viewport_y);
    t[1].z = (t[1].z + 1.f) * 0.5f;
    t[1].z = CLAMP(t[1].z, 0.0f, 1.0f); 
    
    t[2].x = (int)((t[2].x+1.)*(viewport_w/2.f) + viewport_x);
    t[2].y = (int)((t[2].y+1.)*(viewport_h/2.f) + viewport_y);
    t[2].z = (t[2].z + 1.f) * 0.5f;  
    t[2].z = CLAMP(t[2].z, 0.0f, 1.0f); 

    //if (t0.y==t1.y && t0.y==t2.y)

    if (t[0].y>t[1].y) swap_vec3(&t[0], &t[1]); 
    if (t[0].y>t[2].y) swap_vec3(&t[0], &t[2]); 
    if (t[1].y>t[2].y) swap_vec3(&t[1], &t[2]); 

    int total_height = t[2].y-t[0].y; 
    for (int i=0; i<total_height; i++)
    { 
        bool second_half = i > t[1].y - t[0].y || t[1].y == t[0].y; 
        int segment_height = second_half ? t[2].y - t[1].y : t[1].y - t[0].y; 
        float alpha = (float)i / total_height; 
        float beta  = (float)(i - (second_half ? t[1].y - t[0].y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
        
        // Since we're doing everything in C we need to implement functions that will work in place of operators.
        // Normally in C++ I'd suggest making operator overloads for each vec type.

        //Calculate proper positions of vertices for drawing.
        vec3 A =               add_vec3(t[0], mul_vec3_f(sub_vec3(t[2],t[0]),alpha)); 
        vec3 B = second_half ? add_vec3(t[1], mul_vec3_f(sub_vec3(t[2], t[1]), beta)) : add_vec3(t[0], mul_vec3_f(sub_vec3(t[1], t[0]), beta)); 
        
        vec2i uvA =               add_vec2i(uvs[0], mul_vec2i_f(sub_vec2i(uvs[2],uvs[0]), alpha));
        vec2i uvB = second_half ? add_vec2i(uvs[1], mul_vec2i_f(sub_vec2i(uvs[2],uvs[1]), beta)) : add_vec2i(uvs[0], mul_vec2i_f(sub_vec2i(uvs[1], uvs[0]), beta));

        if (A.x>B.x) swap_vec3(&A, &B); 
        for (int j=(int)A.x; j<=(int)B.x; j++)
        { 
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            vec3  p = add_vec3(A, mul_vec3_f(sub_vec3(B,A),phi));
            vec2i uvp =     add_vec2i(uvA, mul_vec2i_f(sub_vec2i(uvB, uvA), phi));
            //part of culling, this shouldn't be here, but works.
            if(p.x < viewport_x || p.x > viewport_w || p.y < viewport_y || p.y > viewport_h) continue;
            int index = (int)((p.x*zbuffer->bpp)+(p.y*zbuffer->width*zbuffer->bpp));
            if (p.z <= *(float*)&zbuffer->data[index])
            {
                *(float*)&zbuffer->data[index] = p.z;
                //vec4 col = shd->fragment();
                color col = blend_colors(*(color*)(tex->data + (uvp.y * tex->width * tex->bpp) + (uvp.x * tex->bpp)), *c);
                *(uint32_t*)(surf->data + (((int)p.y * surf->width * surf->bpp) +  ((int)p.x * surf->bpp))) = *(uint32_t*)&col;
            }
        } 
    }
}

//WIP
float EdgeFunc( vec3 a, vec3 b, vec3 c )
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void triangle_bc(vec3* pts, vec2f* uvs, ssurface* zbuffer, ssurface* surf, ssurface* tex, color* c)
{
    //Create boundging box of screen size.
    vec2i bboxmin = {{{0, 0}}}; 
    vec2i bboxmax = {{{surf->width - 1, surf->height - 1}}};
    //create clamping vector
    vec2i clamp = {{{surf->width-1,  surf->height-1}}};
    //clamp values to bounding box
    
    bboxmax.x = MIN(MAX(pts[0].x, MAX(pts[1].x, pts[2].x)), clamp.x);
    bboxmin.x = MAX(MIN(pts[0].x, MIN(pts[1].x, pts[2].x)), 0);
    bboxmax.y = MIN(MAX(pts[0].y, MAX(pts[1].y, pts[2].y)), clamp.y);
    bboxmin.y = MAX(MIN(pts[0].y, MIN(pts[1].y, pts[2].y)), 0);
    
    // for (int i=0; i<3; i++)
    // { 
    //     for (int j=0; j<2; j++)
    //     { 
    //         bboxmin.elems[j] = MAX(float, 0.f, MIN(float, bboxmin.elems[j], pts[i].elems[j])); 
    //         bboxmax.elems[j] = MIN(float, clamp.elems[j], MAX(float, bboxmax.elems[j], pts[i].elems[j])); 
    //     } 
    // } 
    // //now our triangle is clamped to be inside screen.

    // calculate barycentric coordinates of pixel in triangle.
    // This allows code to find if pixel is inside our bounding box of triangle.
    vec3 p;

    for (p.x=bboxmin.x; p.x<=bboxmax.x; p.x++)
    {
        for (p.y=bboxmin.y; p.y<=bboxmax.y; p.y++)
        {
            vec3 bc_screen = barycentric(pts[0], pts[1], pts[2], p); //This might be slow
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            p.z = 0;
            
            for (int i=0; i<3; i++) p.z += pts[i].elems[2] * bc_screen.elems[i];

            int index = (int)((p.x*zbuffer->bpp)+(p.y*zbuffer->width*zbuffer->bpp));
            if (*(float*)&zbuffer->data[index] < p.z)
            {
                *(float*)&zbuffer->data[index] = p.z;
                
                *(uint32_t*)(surf->data + (((int)p.y * surf->width * surf->bpp) +  ((int)p.x * surf->bpp))) = *(uint32_t*)c;
            }
        }
    }
}

void triangle_shader(ssurface* surf, ssurface* zbuffer, shader* shader_program)
{  
    //All this math will need to be calculated based on varyings
    //Position varying is supposed to always be first, if not, then it most likely is going to crash ¯\_(ツ)_/¯

    //get position from varyings
    vec4* vert = (vec4*)((varying*)shader_program->varyings.data[0])->data;
    //Check if triangle is at all visible (implement better culling algorithm)
    int t1_vis = is_vertex_visible(vert[0]);
    int t2_vis = is_vertex_visible(vert[1]);
    int t3_vis = is_vertex_visible(vert[2]);
    if(!t1_vis && !t2_vis && !t3_vis) return;
    
    //convert from clipping space to device coordinates
    vec3 t[3];
    t[0] = div_vec3(vert[0].xyz, vert[0].w);
    t[1] = div_vec3(vert[1].xyz, vert[1].w);
    t[2] = div_vec3(vert[2].xyz, vert[2].w);

    //if face is backfacing there's no point in drawing it.
    if(is_back_facing(t)) return;

    //convert device coordinates to screen coordinates (pixels)
    t[0].x = (int)((t[0].x+1.)*(viewport_w/2.f) + viewport_x);
    t[0].y = (int)((t[0].y+1.)*(viewport_h/2.f) + viewport_y);
    t[0].z = (t[0].z + 1.f) * 0.5f;
    t[0].z = CLAMP(t[0].z, 0.0f, 1.0f); 
    
    t[1].x = (int)((t[1].x+1.)*(viewport_w/2.f) + viewport_x);
    t[1].y = (int)((t[1].y+1.)*(viewport_h/2.f) + viewport_y);
    t[1].z = (t[1].z + 1.f) * 0.5f;
    t[1].z = CLAMP(t[1].z, 0.0f, 1.0f); 
    
    t[2].x = (int)((t[2].x+1.)*(viewport_w/2.f) + viewport_x);
    t[2].y = (int)((t[2].y+1.)*(viewport_h/2.f) + viewport_y);
    t[2].z = (t[2].z + 1.f) * 0.5f;  
    t[2].z = CLAMP(t[2].z, 0.0f, 1.0f); 

    //Now, since we're having more than one varying we need to swap them all...
    //Or have misplaced normals and UVs, that would be nice...
    
    //Simple bubble sorting by Y coordinate on screen.
    if (t[0].y>t[1].y) 
    {
        swap_vec3(&t[0], &t[1]);
        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                //If new varying data type is specified this switch has to be updated!
                case VT_VEC2I:
                    swap_vec2i(&((vec2i*)var->data)[0], &((vec2i*)var->data)[1]);
                    break;
                case VT_VEC3:
                    swap_vec3(&((vec3*)var->data)[0], &((vec3*)var->data)[1]);
                    break;
                case VT_VEC4:
                    swap_vec4(&((vec4*)var->data)[0], &((vec4*)var->data)[1]);
                    break;
            }
        }
    }
    
    if (t[0].y>t[2].y)
    {
        swap_vec3(&t[0], &t[2]);
        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                //If new varying data type is specified this switch has to be updated!
                case VT_VEC2I:
                    swap_vec2i(&((vec2i*)var->data)[0], &((vec2i*)var->data)[2]);
                    break;
                case VT_VEC3:
                    swap_vec3(&((vec3*)var->data)[0], &((vec3*)var->data)[2]);
                    break;
                case VT_VEC4:
                    swap_vec4(&((vec4*)var->data)[0], &((vec4*)var->data)[2]);
                    break;
            }
        }
    }
    
    if (t[1].y>t[2].y)
    {
        swap_vec3(&t[1], &t[2]);
        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                //If new varying data type is specified this switch has to be updated!
                case VT_VEC2I:
                    swap_vec2i(&((vec2i*)var->data)[1], &((vec2i*)var->data)[2]);
                    break;
                case VT_VEC3:
                    swap_vec3(&((vec3*)var->data)[1], &((vec3*)var->data)[2]);
                    break;
                case VT_VEC4:
                    swap_vec4(&((vec4*)var->data)[1], &((vec4*)var->data)[2]);
                    break;
            }
        }
    }

    //Now each vertex and corresponding varyings are sorted, time to rasterize.

    int total_height = t[2].y-t[0].y; 
    for (int i=0; i<total_height; i++)
    { 
        //determine if we're past first arm of triangle
        bool second_half = i > t[1].y - t[0].y || t[1].y == t[0].y; 

        int segment_height = second_half ? t[2].y - t[1].y : t[1].y - t[0].y; 
        

        float alpha = (float)i / total_height; 
        float beta  = (float)(i - (second_half ? t[1].y - t[0].y : 0)) / segment_height; 
        // Since we're doing everything in C we need to implement functions that will work in place of operators.
        // Normally in C++ I'd suggest making operator overloads for each vec type.

        //Calculate edge points positions for each row
        vec3 A =               add_vec3(t[0], mul_vec3_f(sub_vec3(t[2],t[0]),alpha)); 
        vec3 B = second_half ? add_vec3(t[1], mul_vec3_f(sub_vec3(t[2], t[1]), beta)) : add_vec3(t[0], mul_vec3_f(sub_vec3(t[1], t[0]), beta)); 
        
        // calculate edge points varying values for later line drawing
        // for each line this part is constant, so we need to calculate it just once per line.
        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                case VT_VEC2I:
                    // figure out how to store this value for each pass... it cannot be in shader varying, unless I assume I always will be drawing using filled triangles.
                    // rasterization for wiremesh triangles is different, so values will be interpolated in other way too...
                    // anyway for now let's skip other drawing modes.
                    // Also reallocing, so I don't have to free it each time.
                    var->A = realloc(var->A, sizeof(vec2i));
                    var->B = realloc(var->B, sizeof(vec2i));
                    *(vec2i*)var->A = add_vec2i(((vec2i*)var->data)[0], mul_vec2i_f(sub_vec2i(((vec2i*)var->data)[2],((vec2i*)var->data)[0]), alpha));
                    *(vec2i*)var->B = second_half ? add_vec2i(((vec2i*)var->data)[1], mul_vec2i_f(sub_vec2i(((vec2i*)var->data)[2], ((vec2i*)var->data)[1]), beta)) : add_vec2i(((vec2i*)var->data)[0], mul_vec2i_f(sub_vec2i(((vec2i*)var->data)[1], ((vec2i*)var->data)[0]), beta));
                    break;
                case VT_VEC3:
                    var->A = realloc(var->A, sizeof(vec3));
                    var->B = realloc(var->B, sizeof(vec3));
                    *(vec3*)var->A = add_vec3(((vec3*)var->data)[0], mul_vec3_f(sub_vec3(((vec3*)var->data)[2],((vec3*)var->data)[0]), alpha));
                    *(vec3*)var->B = second_half ? add_vec3(((vec3*)var->data)[1], mul_vec3_f(sub_vec3(((vec3*)var->data)[2], ((vec3*)var->data)[1]), beta)) : add_vec3(((vec3*)var->data)[0], mul_vec3_f(sub_vec3(((vec3*)var->data)[1], ((vec3*)var->data)[0]), beta));
                    break;
                case VT_VEC4:
                    var->A = realloc(var->A, sizeof(vec4));
                    var->B = realloc(var->B, sizeof(vec4));
                    *(vec4*)var->A = add_vec4(((vec4*)var->data)[0], mul_vec4_f(sub_vec4(((vec4*)var->data)[2],((vec4*)var->data)[0]), alpha));
                    *(vec4*)var->B = second_half ? add_vec4(((vec4*)var->data)[1], mul_vec4_f(sub_vec4(((vec4*)var->data)[2], ((vec4*)var->data)[1]), beta)) : add_vec4(((vec4*)var->data)[0], mul_vec4_f(sub_vec4(((vec4*)var->data)[1], ((vec4*)var->data)[0]), beta));
                    break;
            }
        }

        // shouldn't I swap all varyings too?
        // meh, in worst case scenario - it'll look weird
        if (A.x>B.x) 
        {
            swap_vec3(&A, &B);

            for(int i = 1; i < shader_program->varyings.count; i++)
            {
                varying* var = (varying*)shader_program->varyings.data[i];
                switch(var->type)
                {
                    case VT_VEC2I:
                        swap_vec2i((vec2i*)var->A, (vec2i*)var->B);
                        break;
                    case VT_VEC3:
                        swap_vec3((vec3*)var->A, (vec3*)var->B);
                        break;
                    case VT_VEC4:
                        swap_vec4((vec4*)var->A, (vec4*)var->B);
                        break;
                }
            }
        }

        for (int j=(int)A.x; j<=(int)B.x; j++)
        { 
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            //final calculation of pixel position in screen space
            vec3  p = add_vec3(A, mul_vec3_f(sub_vec3(B,A),phi));
            //If pixel outside viewport discard.
            if(p.x < viewport_x || p.x > viewport_w || p.y < viewport_y || p.y > viewport_h) continue;
            
            //calculate index of zbuffer.
            int index = (int)((p.x*zbuffer->bpp)+(p.y*zbuffer->width*zbuffer->bpp));
            if (p.z <= *(float*)&zbuffer->data[index])
            {
                //Now slowest part of drawing: calculating interpolated values for varyings
                //it is calculated per pixel, so it has to be kinda fast

                // varyings are things shared between vertex and fragment shaders.
                // problem is that varying values should be interpolated, since vertex shader is running per triangle (so for each 3 vertices)
                // that means results from vertex shaders will give 3 points just like input.
                // Renderer then is supposed to interpolate these values depending on position of fragment (pixel) within triangle.
                // vector holds only pointers to data, no way to specify which varying is what type.
                // Solution: provide structure containing pointer to varying data AND some kind of specifier which type it is (enum?)

                for(int i = 1; i < shader_program->varyings.count; i++)
                {
                    varying* var = (varying*)shader_program->varyings.data[i];
                    switch(var->type)
                    {
                        case VT_VEC2I:
                            var->fragmentData = realloc(var->fragmentData, sizeof(vec2i));
                            *(vec2i*)var->fragmentData = add_vec2i(*(vec2i*)var->A, mul_vec2i_f(sub_vec2i(*(vec2i*)var->B, *(vec2i*)var->A), phi));
                            break;
                        case VT_VEC3:
                            var->fragmentData = realloc(var->fragmentData, sizeof(vec3));
                            *(vec3*)var->fragmentData = add_vec3(*(vec3*)var->A, mul_vec3_f(sub_vec3(*(vec3*)var->B, *(vec3*)var->A), phi));
                            break;
                        case VT_VEC4:
                            var->fragmentData = realloc(var->fragmentData, sizeof(vec4));
                            *(vec4*)var->fragmentData = add_vec4(*(vec4*)var->A, mul_vec4_f(sub_vec4(*(vec4*)var->B, *(vec4*)var->A), phi));
                            break;
                    }
                }
                
                *(float*)&zbuffer->data[index] = p.z;
                color col = shader_program->fragment(shader_program);
                //I know, I know... Some black magic with pointers, it is C, so no asking about that.
                if(surf->bpp == 4)
                {
                    *(uint32_t*)(surf->data + (((int)p.y * surf->width * surf->bpp) +  ((int)p.x * surf->bpp))) = *(uint32_t*)&col;    
                }
                else
                {
                    ((uint24_t*)(surf->data + (((int)p.y * surf->width * surf->bpp) +  ((int)p.x * surf->bpp))))->data = ((*(uint32_t*)&col));
                }
            }
        }
    }
}

void triangle_wire_shader(ssurface* surf, ssurface* zbuffer, shader* shader_program)
{  
    //All this math will need to be calculated based on varyings
    //Position varying is supposed to always be first, if not, then it most likely is going to crash ¯\_(ツ)_/¯

    //get position from varyings
    vec4* vert = (vec4*)((varying*)shader_program->varyings.data[0])->data;
    //Check if triangle is at all visible (implement better culling algorithm)
    int t1_vis = is_vertex_visible(vert[0]);
    int t2_vis = is_vertex_visible(vert[1]);
    int t3_vis = is_vertex_visible(vert[2]);
    if(!t1_vis && !t2_vis && !t3_vis) return;
    
    //convert from clipping space to device coordinates
    vec3 t[3];
    t[0] = div_vec3(vert[0].xyz, vert[0].w);
    t[1] = div_vec3(vert[1].xyz, vert[1].w);
    t[2] = div_vec3(vert[2].xyz, vert[2].w);

    //if face is backfacing there's no point in drawing it.
    //if(is_back_facing(t)) return;

    //convert device coordinates to screen coordinates (pixels)
    t[0].x = (int)((t[0].x+1.)*(viewport_w/2.f) + viewport_x);
    t[0].y = (int)((t[0].y+1.)*(viewport_h/2.f) + viewport_y);
    t[0].z = (t[0].z + 1.f) * 0.5f;
    t[0].z = CLAMP(t[0].z, 0.0f, 1.0f); 
    
    t[1].x = (int)((t[1].x+1.)*(viewport_w/2.f) + viewport_x);
    t[1].y = (int)((t[1].y+1.)*(viewport_h/2.f) + viewport_y);
    t[1].z = (t[1].z + 1.f) * 0.5f;
    t[1].z = CLAMP(t[1].z, 0.0f, 1.0f); 
    
    t[2].x = (int)((t[2].x+1.)*(viewport_w/2.f) + viewport_x);
    t[2].y = (int)((t[2].y+1.)*(viewport_h/2.f) + viewport_y);
    t[2].z = (t[2].z + 1.f) * 0.5f;  
    t[2].z = CLAMP(t[2].z, 0.0f, 1.0f); 

    //Now, since we're having more than one varying we need to swap them all...
    //Or have misplaced normals and UVs, that would be nice...

    //Simple bubble sorting by Y coordinate on screen.
    if (t[0].y>t[1].y) 
    {
        swap_vec3(&t[0], &t[1]);
        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                //If new varying data type is specified this switch has to be updated!
                case VT_VEC2I:
                    swap_vec2i(&((vec2i*)var->data)[0], &((vec2i*)var->data)[1]);
                    break;
                case VT_VEC3:
                    swap_vec3(&((vec3*)var->data)[0], &((vec3*)var->data)[1]);
                    break;
                case VT_VEC4:
                    swap_vec4(&((vec4*)var->data)[0], &((vec4*)var->data)[1]);
                    break;
            }
        }
    }
    
    if (t[0].y>t[2].y)
    {
        swap_vec3(&t[0], &t[2]);
        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                //If new varying data type is specified this switch has to be updated!
                case VT_VEC2I:
                    swap_vec2i(&((vec2i*)var->data)[0], &((vec2i*)var->data)[2]);
                    break;
                case VT_VEC3:
                    swap_vec3(&((vec3*)var->data)[0], &((vec3*)var->data)[2]);
                    break;
                case VT_VEC4:
                    swap_vec4(&((vec4*)var->data)[0], &((vec4*)var->data)[2]);
                    break;
            }
        }
    }
    
    if (t[1].y>t[2].y)
    {
        swap_vec3(&t[1], &t[2]);
        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                //If new varying data type is specified this switch has to be updated!
                case VT_VEC2I:
                    swap_vec2i(&((vec2i*)var->data)[1], &((vec2i*)var->data)[2]);
                    break;
                case VT_VEC3:
                    swap_vec3(&((vec3*)var->data)[1], &((vec3*)var->data)[2]);
                    break;
                case VT_VEC4:
                    swap_vec4(&((vec4*)var->data)[1], &((vec4*)var->data)[2]);
                    break;
            }
        }
    }

    //Now each vertex and corresponding varyings are sorted, time to rasterize.
    int total_height = t[2].y-t[0].y; 
    for (int i=0; i<total_height; i++)
    { 
        //determine if we're past first arm of triangle
        bool second_half = i > t[1].y - t[0].y || t[1].y == t[0].y; 

        int segment_height = second_half ? t[2].y - t[1].y : t[1].y - t[0].y; 
        

        float alpha = (float)i / total_height; 
        float beta  = (float)(i - (second_half ? t[1].y - t[0].y : 0)) / segment_height; 
        // Since we're doing everything in C we need to implement functions that will work in place of operators.
        // Normally in C++ I'd suggest making operator overloads for each vec type.

        //Calculate edge points positions for each row
        vec3 A =               add_vec3(t[0], mul_vec3_f(sub_vec3(t[2],t[0]),alpha)); 
        vec3 B = second_half ? add_vec3(t[1], mul_vec3_f(sub_vec3(t[2], t[1]), beta)) : add_vec3(t[0], mul_vec3_f(sub_vec3(t[1], t[0]), beta)); 
        
        // calculate edge points varying values for later line drawing
        // for each line this part is constant, so we need to calculate it just once per line.
        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                case VT_VEC2I:
                    // figure out how to store this value for each pass... it cannot be in shader varying, unless I assume I always will be drawing using filled triangles.
                    // rasterization for wiremesh triangles is different, so values will be interpolated in other way too...
                    // anyway for now let's skip other drawing modes.
                    // Also reallocing, so I don't have to free it each time.
                    var->A = realloc(var->A, sizeof(vec2i));
                    var->B = realloc(var->B, sizeof(vec2i));
                    *(vec2i*)var->A = add_vec2i(((vec2i*)var->data)[0], mul_vec2i_f(sub_vec2i(((vec2i*)var->data)[2],((vec2i*)var->data)[0]), alpha));
                    *(vec2i*)var->B = second_half ? add_vec2i(((vec2i*)var->data)[1], mul_vec2i_f(sub_vec2i(((vec2i*)var->data)[2], ((vec2i*)var->data)[1]), beta)) : add_vec2i(((vec2i*)var->data)[0], mul_vec2i_f(sub_vec2i(((vec2i*)var->data)[1], ((vec2i*)var->data)[0]), beta));
                    break;
                case VT_VEC3:
                    var->A = realloc(var->A, sizeof(vec3));
                    var->B = realloc(var->B, sizeof(vec3));
                    *(vec3*)var->A = add_vec3(((vec3*)var->data)[0], mul_vec3_f(sub_vec3(((vec3*)var->data)[2],((vec3*)var->data)[0]), alpha));
                    *(vec3*)var->B = second_half ? add_vec3(((vec3*)var->data)[1], mul_vec3_f(sub_vec3(((vec3*)var->data)[2], ((vec3*)var->data)[1]), beta)) : add_vec3(((vec3*)var->data)[0], mul_vec3_f(sub_vec3(((vec3*)var->data)[1], ((vec3*)var->data)[0]), beta));
                    break;
                case VT_VEC4:
                    var->A = realloc(var->A, sizeof(vec4));
                    var->B = realloc(var->B, sizeof(vec4));
                    *(vec4*)var->A = add_vec4(((vec4*)var->data)[0], mul_vec4_f(sub_vec4(((vec4*)var->data)[2],((vec4*)var->data)[0]), alpha));
                    *(vec4*)var->B = second_half ? add_vec4(((vec4*)var->data)[1], mul_vec4_f(sub_vec4(((vec4*)var->data)[2], ((vec4*)var->data)[1]), beta)) : add_vec4(((vec4*)var->data)[0], mul_vec4_f(sub_vec4(((vec4*)var->data)[1], ((vec4*)var->data)[0]), beta));
                    break;
            }
        }

        // shouldn't I swap all varyings too?
        // meh, in worst case scenario - it'll look weird
        if (A.x>B.x) 
        {
            swap_vec3(&A, &B);

            for(int i = 1; i < shader_program->varyings.count; i++)
            {
                varying* var = (varying*)shader_program->varyings.data[i];
                switch(var->type)
                {
                    case VT_VEC2I:
                        swap_vec2i((vec2i*)var->A, (vec2i*)var->B);
                        break;
                    case VT_VEC3:
                        swap_vec3((vec3*)var->A, (vec3*)var->B);
                        break;
                    case VT_VEC4:
                        swap_vec4((vec4*)var->A, (vec4*)var->B);
                        break;
                }
            }
        }

        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                case VT_VEC2I:
                    *(vec2i*)var->fragmentData = *(vec2i*)var->A;
                    break;
                case VT_VEC3:
                    *(vec3*)var->fragmentData = *(vec3*)var->A;
                    break;
                case VT_VEC4:
                    *(vec4*)var->fragmentData = *(vec4*)var->A;
                    break;
            }
        }
        color col1 = shader_program->fragment(shader_program);

        for(int i = 1; i < shader_program->varyings.count; i++)
        {
            varying* var = (varying*)shader_program->varyings.data[i];
            switch(var->type)
            {
                case VT_VEC2I:
                    *(vec2i*)var->fragmentData = *(vec2i*)var->B;
                    break;
                case VT_VEC3:
                    *(vec3*)var->fragmentData = *(vec3*)var->B;
                    break;
                case VT_VEC4:
                    *(vec4*)var->fragmentData = *(vec4*)var->B;
                    break;
            }
        }

        color col2 = shader_program->fragment(shader_program);
        if(surf->bpp == 4)
        {
            if(A.x >= viewport_x || A.x <= viewport_w || A.y >= viewport_y || A.y <= viewport_h)
                *(uint32_t*)(surf->data + (((int)A.y * surf->width * surf->bpp) +  ((int)A.x * surf->bpp))) = *(uint32_t*)&col1;
            if(B.x >= viewport_x || B.x <= viewport_w || B.y >= viewport_y || B.y <= viewport_h)
                *(uint32_t*)(surf->data + (((int)B.y * surf->width * surf->bpp) +  ((int)B.x * surf->bpp))) = *(uint32_t*)&col2;    
        }
        else
        {
            if(B.x >= viewport_x || B.x <= viewport_w || B.y >= viewport_y || B.y <= viewport_h)
                ((uint24_t*)(surf->data + (((int)A.y * surf->width * surf->bpp) +  ((int)A.x * surf->bpp))))->data = ((*(uint32_t*)&col1));
            if(B.x >= viewport_x || B.x <= viewport_w || B.y >= viewport_y || B.y <= viewport_h)
                ((uint24_t*)(surf->data + (((int)B.y * surf->width * surf->bpp) +  ((int)B.x * surf->bpp))))->data = ((*(uint32_t*)&col2));
        }
    }
}

void viewport(int x, int y, int w, int h)
{
    viewport_x = x;
    viewport_y = y;
    viewport_w = w;
    viewport_h = h;
}

static void sgl_draw_triangles(vector* vertices, vector* indices, vector* normals, vector* uvs, shader* shader_program, ssurface* buffer, ssurface* zbuffer)
{
    //Get vertex data based on indices in array
    for(uint32_t i = 0; i < indices->count; i+=3)
    {
        for (int j = 0; j < 3; j++)
        { 
            //create input vertex data that is provided from model mesh
            vec4 vert;
            vert.xyz = *(vec3*)vertices->data[((vec3i*)indices->data[i+j])->x - 1];
            vert.w = 1.0f;
            //Run vertex shader program
            shader_program->vertex(shader_program, vert, *(vec3*)normals->data[((vec3i*)indices->data[i+j])->z - 1], *(vec2f*)uvs->data[((vec3i*)indices->data[i+j])->y - 1], j);
        }
        //Call triangle rasterization with values interpolation.
        triangle_shader(buffer, zbuffer, shader_program);

        //TODO Remember to clear varyings beofre next draw, otherwise memory leak.
    }
}

static void sgl_draw_triangles_wire(vector* vertices, vector* indices, vector* normals, vector* uvs, shader* shader_program, ssurface* buffer, ssurface* zbuffer)
{
    //Get vertex data based on indices in array
    for(uint32_t i = 0; i < indices->count; i+=3)
    {
        for (int j = 0; j < 3; j++)
        { 
            //create input vertex data that is provided from model mesh
            vec4 vert;
            vert.xyz = *(vec3*)vertices->data[((vec3i*)indices->data[i+j])->x - 1];
            vert.w = 1.0f;
            //Run vertex shader program
            shader_program->vertex(shader_program, vert, *(vec3*)normals->data[((vec3i*)indices->data[i+j])->z - 1], *(vec2f*)uvs->data[((vec3i*)indices->data[i+j])->y - 1], j);
        }
        //Call triangle rasterization with values interpolation.
        triangle_wire_shader(buffer, zbuffer, shader_program);

        //TODO Remember to clear varyings beofre next draw, otherwise memory leak.
    }
}

void sgl_draw(uint8_t mode, vector* vertices, vector* indices, vector* normals, vector* uvs, shader* shader_program, ssurface* buffer, ssurface* zbuffer)
{
    switch(mode)
    {
        case SGL_TRIANGLES:
            sgl_draw_triangles(vertices, indices, normals, uvs, shader_program, buffer, zbuffer);
            break;
        case SGL_WIRE:
            sgl_draw_triangles_wire(vertices, indices, normals, uvs, shader_program, buffer, zbuffer);
            break;
    }
}

shader* create_shader(vert (*vert_ptr)(shader*, vec4, vec3, vec2f, int), color (*frag_ptr)(shader*))
{
    shader* shd = (shader*)malloc(sizeof(shader));
    vector_init(&shd->uniforms);
    vector_init(&shd->varyings);
    shd->vertex = vert_ptr;
    shd->fragment = frag_ptr;
    return shd;
}

//color math

color add_color(color left, color right)
{
    color result;
    result.b = CLAMP(left.b + right.b, 0, 255);
    result.g = CLAMP(left.b + right.b, 0, 255);
    result.r = CLAMP(left.b + right.b, 0, 255);
    result.a = CLAMP(left.b + right.b, 0, 255);
    return result;
}

color mul_color_f(color c, float f)
{
    color result;
    result.b = (uint8_t)CLAMP(c.b * f, 0.0f, 255.f);
    result.g = (uint8_t)CLAMP(c.g * f, 0.0f, 255.f);
    result.r = (uint8_t)CLAMP(c.r * f, 0.0f, 255.f);
    result.a = (uint8_t)CLAMP(c.a * f, 0.0f, 255.f);
    return result;
}