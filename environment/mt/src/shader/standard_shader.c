#include "standard_shader.h"
#include <math.h>

void standard_vertex(shader* _shader, vec4 vertex, vec3 norm, vec2f uv, int vertnum)
{
    //Get values from uniforms. In reality some of these should be provided by parameter, but this should also work.
    mat4 mvp = *(mat4*)_shader->uniforms.data[0];
    mat4 model = *(mat4*)_shader->uniforms.data[4];
    //Diffuse / Albedo
    ssurface* tex0 = (ssurface*)_shader->uniforms.data[1];

    //Try to avoid getting many varyings, since after vertex shader all of these need to be interpolated.
    //Also each varying has to be 3 element array (3 vertices)
    
    varying* var_t;
    //This one is ALWAYS required and ALWAYS has to be first.
    //POSITION
    var_t =  0 < _shader->varyings.count ? (varying*)_shader->varyings.data[0] : (varying*)calloc(sizeof(varying), 1);
    var_t->type = VT_VEC4;
    var_t->data = realloc(var_t->data, sizeof(vec4)*(vertnum+1));
    ((vec4*)var_t->data)[vertnum] = multiply_mat4_vec4(mvp, vertex);
    vector_place(&_shader->varyings, (void*)var_t, 0);

    //FRAGPOS
    var_t =  1 < _shader->varyings.count ? (varying*)_shader->varyings.data[1] : (varying*)calloc(sizeof(varying), 1);
    var_t->type = VT_VEC3;
    var_t->data = realloc(var_t->data, sizeof(vec3)*(vertnum+1));
    ((vec3*)var_t->data)[vertnum] = multiply_mat4_vec4(model, vertex).xyz;
    vector_place(&_shader->varyings, (void*)var_t, 1);

    //NORMAL
    var_t =  2 < _shader->varyings.count ? (varying*)_shader->varyings.data[2] : (varying*)calloc(sizeof(varying), 1);
    var_t->type = VT_VEC3;
    var_t->data = realloc(var_t->data, sizeof(vec3)*(vertnum+1));
    ((vec3*)var_t->data)[vertnum] = mul_mat3_vec3(mat3_from_mat4(mat4_transpose(mat4_inverse(model))), norm);
    vector_place(&_shader->varyings, (void*)var_t, 2);

    //UV
    var_t =  3 < _shader->varyings.count ? (varying*)_shader->varyings.data[3] : (varying*)calloc(sizeof(varying), 1);
    var_t->type = VT_VEC2I;
    var_t->data = realloc(var_t->data, sizeof(vec2i)*(vertnum+1));
    ((vec2i*)var_t->data)[vertnum].x = (int)(uv.u * tex0->width);
    ((vec2i*)var_t->data)[vertnum].y = (int)(uv.v * tex0->height);
    vector_place(&_shader->varyings, (void*)var_t, 3);

    //All above values are per vertex, now interpolate them for each fargment pass.
}

color light_col_d = {255,255,255,255};

color standard_fragment(shader* _shader)
{
    //Blinn-Phong model will be used, but for start Phong model should be good

    // calculate ambient lightning:
    // Get ambient lightning color from uniforms, also for now we're going treat all objects as if their own color was white.
    color ambient = *(color*)_shader->uniforms.data[2];

    vec2i uv = *(vec2i*)((varying*)_shader->varyings.data[3])->fragmentData;
    vec3 n = normalize_vec3(*(vec3*)((varying*)_shader->varyings.data[2])->fragmentData);

    vec3 frag_pos = *(vec3*)((varying*)_shader->varyings.data[1])->fragmentData;

    vec3 light_dir = *(vec3*)_shader->uniforms.data[3];

    float diffuse = MAX(dot_product_vec3(n, light_dir), 0.0f);

    color diff_c = mul_color_f(light_col_d, diffuse);

    //Diffuse / Albedo
    ssurface* tex0 = (ssurface*)_shader->uniforms.data[1];

    color tex = *(color*)(tex0->data + (uv.y * tex0->width * tex0->bpp) + (uv.x * tex0->bpp));

    //ambient = blend_colors(ambient, tex);
    color c = add_color(diff_c, ambient);

    vec3 cam_pos = *(vec3*)_shader->uniforms.data[5];
    vec3 view_dir = normalize_vec3(sub_vec3(cam_pos, frag_pos));
    //vec3 reflect_dir = reflect_vec3(inv_vec3(light_ndir), n);
    vec3 halfway = normalize_vec3(add_vec3(light_dir, view_dir));

    float spec = pow(MAX(dot_product_vec3(n, halfway), 0.0f), 32.0f);

    color spec_c = mul_color_f(light_col_d, spec);

    c = add_color(c, spec_c);

    //Since we're treating whole object as white we can simply return ambient color as color of fragment(pixel).
    return blend_colors(c, tex);
}