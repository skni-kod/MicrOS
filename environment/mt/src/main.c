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

typedef struct uint24
{
    unsigned int data : 24;
} __attribute__((packed)) uint24_t;

//COLOR in 32 bit is 0xAARRGGBB


int depth_buffer_res = 255;
int bytespp = 4;
uint8_t test_mode = 0;

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
            if(!compareString(argv[i], "-t"))
            {
                test_mode = 1;
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

    ssurface* empty_buffer = create_surface(vmd.width, vmd.height, 4);
    color empty_color = {60,60,60,255};
    for(int i = 0; i < empty_buffer->width*empty_buffer->height*bytespp; i+= bytespp)
    {
        if(bytespp == 3)
        {
            ((uint24_t*)&empty_buffer->data[i])->data = ((*(uint32_t*)&empty_color));
        }
        else
        {
            *(uint32_t*)&empty_buffer->data[i] = *(uint32_t*)&empty_color;
        }
    }
    
    for(int i = 0; i < zbuffer_empty->width*zbuffer_empty->height; i++)
    {
        *(float*)(zbuffer_empty->data+i*4) = 1.f;
    }

    init_shaders();

    //uint8_t* gpuBuffer = calloc(vmd.width*vmd.height*4, sizeof(uint8_t));
    //color white = {0xFF, 0xFF, 0xFF, 0x00};

    //load textures
    ssurface* head_tex = load_image("A:/MTDATA/HEAD/HEAD.BMP");
    ssurface* car_tex = load_image("A:/MTDATA/CAR/CAR_TEX.BMP");
    ssurface* fox_tex = load_image("A:/MTDATA/FOX/FOX_TEX.BMP");
    //ssurface* head_tex = load_image("A:/MTDATA/HEAD/HEAD.BMP");

    ssurface* texTbl[] = {head_tex, car_tex, fox_tex};

    //load models
    obj_model* head_mdl = load_obj_model("A:/MTDATA/HEAD/HEAD.MDL");
    obj_model* car_mdl = load_obj_model("A:/MTDATA/CAR/CAR.MDL");
    obj_model* fox_mdl = load_obj_model("A:/MTDATA/FOX/FOX.MDL");

    obj_model* mdlTbl[] = {head_mdl, car_mdl, fox_mdl};

    micros_console_set_video_mode(vmd.mode_number);

    viewport(0,0,vmd.width, vmd.height);

    float angle = 90.f;

    mat4 view;

    mat4 proj = perspective(45., buffer->width/(float)buffer->height, .001f, 100.0f);

    mat4 model;

    float hrot_angle = 90.0f, vrot_angle = -180.0f;
    vec3 hrotd = {{{0.f, 1.f,0.f}}};
    vec3 rot_x = {{{1.f,0.f,0.f}}};
    mat4 vrot;
    mat4 hrot;

    viewport(0,0,vmd.width, vmd.height);

    shader* std_shader = get_shader(0);

    color ambient = {39,26,22,255};

    vec3 light_dir = {{{-0.6f,-0.7f,-1.f}}};
    vec3 light_ndir = normalize_vec3(neg_vec3(light_dir));

    vec3 cam_pos = {{{0.0f, 0.0f, 30.0f}}};
    vec3 cam_target = {{{0.0f,0.0f,0.0f}}};
    vec3 up = {{{0.0f,1.0f,0.0f}}};

    ssurface* texPtr = head_tex;
    obj_model* mdlPtr = head_mdl;

    if(test_mode)
    {
        memcpy(buffer->data, empty_buffer->data, buffer->width*buffer->height*buffer->bpp);
        memcpy(zbuffer->data, zbuffer_empty->data, zbuffer->width*zbuffer->height*zbuffer->bpp);
        micros_console_copy_from_buffer(buffer->data, vmd.width*vmd.height*bytespp);

        //camera positions:
        vec3 cps[] = {{{{0.0f, 0.0f, -30.0f}}}, {{{0.0f, 0.0f, -3.0f}}},};
        //light_dirs
        vec3 lds[] = {{{{-0.6f,-0.7f,-1.f}}}, {{{-0.3f,-0.1f,-0.4f}}}};
        //rotations
        float hrots[] = {90.0f, 210.0f};

        char* names[] = {"HEAD", "CAR", "FOX"};
        char* res[] = {"640x480\n", "1024x768\n", "1280x1024\n"};



        FILE* data = fopen("A:/MTDATA/TIMES.TXT", "w");

        clock_t beginTime;

        for(int n = 0; n < 3; n++)
        {
            switch(n)
            {
                case 0:
                    vmd.width = 640;
                    vmd.height = 480;
                    break;
                case 1:
                    vmd.width = 1024;
                    vmd.height = 768;
                    break;
                case 2:
                    vmd.width = 1280;
                    vmd.height = 1024;
                    break;
            }
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
            
            micros_console_set_video_mode(vmd.mode_number);
            viewport(0,0,vmd.width, vmd.height);
            
            buffer = create_surface(vmd.width, vmd.height, bytespp);

            //for zbuffer treat each pixel value as float
            zbuffer = create_surface(vmd.width, vmd.height, 4);
            zbuffer_empty = create_surface(vmd.width, vmd.height, 4);

            empty_buffer = create_surface(vmd.width, vmd.height, 4);
            for(int i = 0; i < empty_buffer->width*empty_buffer->height*bytespp; i+= bytespp)
            {
                if(bytespp == 3)
                {
                    ((uint24_t*)&empty_buffer->data[i])->data = ((*(uint32_t*)&empty_color));
                }
                else
                {
                    *(uint32_t*)&empty_buffer->data[i] = *(uint32_t*)&empty_color;
                }
            }
            
            for(int i = 0; i < zbuffer_empty->width*zbuffer_empty->height; i++)
            {
                *(float*)(zbuffer_empty->data+i*4) = 1.f;
            }
            
            fwrite(res[n], 1, strlen(res[n]), data);

            char txtBuf[256];

            for(int p = 0; p < 2; p++)
            {
                std_shader = get_shader(p);
                for(int l = 0; l < 3; l++)
                {
                    for(int i = 0; i < 2; i++)
                    {
                        for(int j = 0; j < 2; j++)
                        {
                            for(int k = 0; k < 2; k++)
                            {
                                //choose element for test
                                vec3* cpPtr = &cps[i];
                                vec3* ldPtr = &lds[j];
                                float* hrotPtr = &hrots[k];

                                //clear screen
                                memcpy(buffer->data, empty_buffer->data, buffer->width*buffer->height*buffer->bpp);
                                memcpy(zbuffer->data, zbuffer_empty->data, zbuffer->width*zbuffer->height*zbuffer->bpp);

                                //run test
                                view = translate(*cpPtr);

                                model = mat4_identity();
                
                                vrot = rotate(vrot_angle, rot_x);

                                hrot = rotate(*hrotPtr, hrotd);
                
                                mat4 rot = multiply_mat4(vrot, hrot);

                                model = multiply_mat4(model, rot);

                                mat4 vp = multiply_mat4(proj, view);
                                mat4 mvp = multiply_mat4(vp, model);

                                vec3 ldnDir = normalize_vec3(neg_vec3(*ldPtr));

                                //clear shader uniforms, since it is new frame
                                //vector_clear(&std_shader->uniforms);
                                //Insert new uniforms per frame (values might've changed)
                                vector_place(&std_shader->uniforms, (void*)&mvp, 0);
                                vector_place(&std_shader->uniforms, (void*)texTbl[l], 1);
                                vector_place(&std_shader->uniforms, (void*)&ambient, 2);
                                vector_place(&std_shader->uniforms, (void*)&ldnDir, 3);
                                vector_place(&std_shader->uniforms, (void*)&model, 4);
                                vector_place(&std_shader->uniforms, (void*)cpPtr, 5);
                                vector_place(&std_shader->uniforms, (void*)ldPtr, 6);

                                //measure time
                                beginTime = clock();

                                mdlPtr = mdlTbl[l];
                                sgl_draw(render_mode, &mdlPtr->vertices, &mdlPtr->indices, &mdlPtr->normals, &mdlPtr->uv, std_shader, buffer, zbuffer);
                                micros_console_copy_from_buffer(buffer->data, vmd.width*vmd.height*bytespp);

                                sprintf(txtBuf, "%s-%d%d%d%d - TIME: %d\n", names[l], p, i,j,k, clock() - beginTime);
                                fwrite(txtBuf, 1, strlen(txtBuf), data);
                            }
                        }
                    }
                }
            }
        }
        fclose(data);
        micros_console_set_video_mode(3);
        printf("ALL TESTS DONE!\n");
        return 0;
    }
    else
    {
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

            //view = translate(cam_pos);

            view = lookat(cam_pos, cam_target, up);

            model = mat4_identity();
            
            vrot = rotate(vrot_angle, rot_x);

            hrot = rotate(hrot_angle, hrotd);
            
            mat4 rot = multiply_mat4(vrot, hrot);

            model = multiply_mat4(model, rot);

            memcpy(buffer->data, empty_buffer->data, buffer->width*buffer->height*buffer->bpp);
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
            vector_place(&std_shader->uniforms, (void*)texPtr, 1);
            vector_place(&std_shader->uniforms, (void*)&ambient, 2);
            vector_place(&std_shader->uniforms, (void*)&light_ndir, 3);
            vector_place(&std_shader->uniforms, (void*)&model, 4);
            vector_place(&std_shader->uniforms, (void*)&cam_pos, 5);
            vector_place(&std_shader->uniforms, (void*)&light_dir, 6);

            //call draw
            sgl_draw(render_mode, &mdlPtr->vertices, &mdlPtr->indices, &mdlPtr->normals, &mdlPtr->uv, std_shader, buffer, zbuffer);

            micros_console_copy_from_buffer(buffer->data, vmd.width*vmd.height*bytespp);
        }
    }
    micros_console_set_video_mode(0x03);

    return 0;
}