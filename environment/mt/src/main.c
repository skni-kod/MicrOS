#include <stdio.h>
#include <micros.h>
#include <micros/micros_console.h>
#include <stdint.h>
#include "renderer/srender.h"
#include "renderer/shaders.h"
#include "wavefront/obj.h"
#include <math.h>
#include "lib/bmp.h"
#include "lib/keyboard.h"
#include <float.h>

//COLOR in 32 bit is 0xAARRGGBB

int depth_buffer_res = 255;
int bytespp = 4;

//string.h is supposed to be bugged, so I implement my own strcmp
int compareString(char* a, char*b)
{
    if(a == NULL || b == NULL) return -2;
    int i = 0;
    while((a[i] != 0) && (b[i] != 0))
    {
        if(a[i] < b[i]) return -1;
        if(a[i] > b[i]) return 1;
        i++;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    initInput();
    uint16_t horizontal = 640;
    uint16_t vertical = 480;
    uint8_t render_mode = SGL_TRIANGLES;
    if(argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            if(argv[i] == NULL) continue;
            if(!compareString(argv[i], "-h"))
            {
                char buffer[32];
                i++;
                int n = atoi(argv[i]);
                if ( strlen(argv[i]) == strlen(itoa(n, buffer, 10))) 
                {
                    horizontal = (uint16_t)n;
                }
            }
            if(!compareString(argv[i], "-v"))
            {
                char buffer[32];
                i++;
                int n = atoi(argv[i]);
                if ( strlen(argv[i]) == strlen(itoa(n, buffer, 10))) 
                {
                    vertical = (uint16_t)n;
                }
            }
        }
    }
    
    printf("Looking for mode %dx%dx32b\n", horizontal, vertical);

    srand(time(NULL));
    video_mode_descriptor vmd;
    vmd.width = horizontal;
    vmd.height = vertical;
    vmd.grayscale = 0;
    vmd.colors = 0x100000000;
    vmd.text = 0;
    vmd.planar = 0;
    if(micros_gdi_find_video_mode(&vmd) != 0)
    {
        printf("32bit color mode not found. Looking for 24 bit.\n");
        vmd.colors = 0x1000000;
        if(micros_gdi_find_video_mode(&vmd) != 0)
        {
            printf("ERROR: No True Color mode found.\n");
            return -1;
        }
        bytespp = 3;
    }

    ssurface* buffer = create_surface(vmd.width, vmd.height, bytespp);

    //for zbuffer treat each pixel value as float
    ssurface* zbuffer = create_surface(vmd.width, vmd.height, 4);
    ssurface* zbuffer_empty = create_surface(vmd.width, vmd.height, 4);
    
    for(int i = 0; i < zbuffer_empty->width*zbuffer_empty->height; i++)
    {
        *(float*)(zbuffer_empty->data+i*4) = 1.f;
    }

    init_shaders();

    //uint8_t* gpuBuffer = calloc(vmd.width*vmd.height*4, sizeof(uint8_t));
    //color white = {0xFF, 0xFF, 0xFF, 0x00};

    ssurface* head_tex = load_image("A:/RES/HEAD.BMP");

    obj_model* mdl = load_obj_model("A:/RES/HEAD.MDL");

    micros_console_set_video_mode(vmd.mode_number);

    viewport(0,0,vmd.width, vmd.height);

    vec3 light_dir = {{{-0.6f,-0.7f,-1.f}}};

    float angle = 90.f;

    vec3 cam_pos = {{{0.0f, 0.0f, -3.0f}}};
    vec3 cam_target = {{{0.0f,0.0f,0.0f}}};
    vec3 up = {{{0.0f,1.0f,0.0f}}};

    //mat4 view = lookat(cam_pos, cam_target, up);

    mat4 view;

    mat4 proj = perspective(45., buffer->width/(float)buffer->height, .001f, 100.0f);

    mat4 model;

    float hrot_angle = 210.0f, vrot_angle = -180.0f;
    vec3 hrotd = {{{0.f, 1.f,0.f}}};
    vec3 rot_x = {{{1.f,0.f,0.f}}};
    mat4 vrot;
    mat4 hrot;

    viewport(0,0,vmd.width, vmd.height);

    shader* std_shader = get_shader(0);

    color ambient = {39,26,22,255};

    vec3 light_ndir = normalize_vec3(neg_vec3(light_dir));

    while(1)
    {
        processInput();

        //Exit
        if(getKeyDown(key_esc)) break;

        if(getKey(key_d)) cam_pos.x += .5;
        if(getKey(key_a)) cam_pos.x -= .5f;

        if(getKey(key_s)) cam_pos.y += .5f;
        if(getKey(key_w)) cam_pos.y -= .5f;

        if(getKey(key_i)) cam_pos.z += .5f;
        if(getKey(key_k)) cam_pos.z -= .5f;

        if(getKey(key_q)) hrot_angle -= 2.f;
        if(getKey(key_e)) hrot_angle += 2.f;

        if(getKey(key_r)) vrot_angle -= 2.f;
        if(getKey(key_f)) vrot_angle += 2.f;

        if(getKey(key_2))
        {
            render_mode = SGL_TRIANGLES;
        }
        
        if(getKey(key_1))
        {
            render_mode = SGL_WIRE;
        }

        if(getKey(key_9))
        {
            std_shader = get_shader(0);
        }

        if(getKey(key_0))
        {
            std_shader = get_shader(1);
        }

        view = translate(cam_pos);

        model = mat4_identity();
        
        vrot = rotate(vrot_angle, rot_x);

        hrot = rotate(hrot_angle, hrotd);
        
        mat4 rot = multiply_mat4(vrot, hrot);

        model = multiply_mat4(model, rot);

        memset(buffer->data, 0, buffer->width*buffer->height*buffer->bpp);
        memcpy(zbuffer->data, zbuffer_empty->data, zbuffer->width*zbuffer->height*zbuffer->bpp);

        if(angle >= 360.0f)
        {
            angle = 0.0f;
        }
        mat4 vp = multiply_mat4(proj, view);
        mat4 mvp = multiply_mat4(vp, model);

        //clear shader uniforms, since it is new frame
        //vector_clear(&std_shader->uniforms);
        //Insert new uniforms per frame (values might've changed)
        vector_place(&std_shader->uniforms, (void*)&mvp, 0);
        vector_place(&std_shader->uniforms, (void*)head_tex, 1);
        vector_place(&std_shader->uniforms, (void*)&ambient, 2);
        vector_place(&std_shader->uniforms, (void*)&light_ndir, 3);
        vector_place(&std_shader->uniforms, (void*)&model, 4);
        vector_place(&std_shader->uniforms, (void*)&cam_pos, 5);
        vector_place(&std_shader->uniforms, (void*)&light_dir, 6);

        //call draw
        sgl_draw(render_mode, &mdl->vertices, &mdl->indices, &mdl->normals, &mdl->uv, std_shader, buffer, zbuffer);

        micros_console_copy_from_buffer(buffer->data, vmd.width*vmd.height*bytespp);
    }

    micros_console_set_video_mode(0x03);

    return 0;
}