#include <stdio.h>
#include <micros.h>
#include <micros/micros_console.h>
#include <stdint.h>
#include "renderer/srender.h"
#include "wavefront/obj.h"
#include <math.h>
#include "lib/bmp.h"
#include "lib/keyboard.h"
#include <float.h>

//COLOR in 32 bit is 0xAARRGGBB

#define PI 3.14159265

//string.h is supposed to be bugged, so I implement my own strcmp
int compareString(char* a, char*b)
{
    if(a == NULL || b == NULL) return -2;
    int i = 0;
    while((a[i] != NULL) && (b[i] != NULL))
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
    
    printf("Looking for mode %dx%d\n", horizontal, vertical);

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
        printf("Error: Mode not found.\n");
        return -1;
    }

    ssurface* buffer = create_surface(vmd.width, vmd.height, 4);

    //for zbuffer treat each pixel value as float
    ssurface* zbuffer = create_surface(vmd.width, vmd.height, 4);
    ssurface* zbuffer_empty = create_surface(vmd.width, vmd.height, 4);
    
    for(int i = 0; i < zbuffer_empty->width*zbuffer_empty->height; i++)
    {
        *(float*)(zbuffer_empty->data+i*4) = -FLT_MAX;
    }

    //uint8_t* gpuBuffer = calloc(vmd.width*vmd.height*4, sizeof(uint8_t));
    color white = {0xFF, 0xFF, 0xFF, 0x00};

    ssurface* head_tex = load_image("A:/RES/HEAD.BMP");

    obj_model* mdl = load_obj_model("A:/RES/HEAD.MDL");

    micros_console_set_video_mode(vmd.mode_number);

    vec3f light_dir = {0,0,1};

    float angle = 90.f;

    while(1)
    {
        processInput();

        //Exit
        if(getKeyDown(key_esc)) break;


        memset(buffer->data, 0, buffer->width*buffer->height*buffer->bpp);
        memcpy(zbuffer->data, zbuffer_empty->data, zbuffer->width*zbuffer->height*zbuffer->bpp);

        light_dir.x = light_dir.z*cos(angle*PI/180.0);
        light_dir = normalize_vec3f(light_dir);
        
        angle += 1.f;

        if(angle >= 360.0f)
        {
            angle = 0.0f;
        }

        for(int i = 0; i < mdl->indices.count; i+=3)
        {
            //if(i + 3 > mdl->indices.count) break;
            vec3f screen_coords[3];
            vec3f world_coords[3]; 
            vec2i uv_coords[3]; 
            for (int j = 0; j < 3; j++)
            { 
                vec3f v = *(vec3f*)mdl->vertices.data[((vec3i*)mdl->indices.data[i+j])->x - 1];
                uv_coords[j].u = (int)(((vec2f*)mdl->uv.data[((vec3i*)mdl->indices.data[i+j])->y - 1])->u * head_tex->width);
                uv_coords[j].v = (int)(((vec2f*)mdl->uv.data[((vec3i*)mdl->indices.data[i+j])->y - 1])->v * head_tex->height);

                screen_coords[j].x = (int)((v.x+1.)*buffer->width/2.+.5);
                screen_coords[j].y = (int)((v.y+1.)*buffer->height/2.+.5);
                screen_coords[j].z = v.z;
                world_coords[j] = v;
            }

            //calculate lightning
            //??
            vec3f n = cross_product_vec3f(sub_vec3f(world_coords[2],world_coords[0]),sub_vec3f(world_coords[1],world_coords[0])); 
            
            n = normalize_vec3f(n);
            //Dot product
            float intensity = dot_product_vec3f(n,light_dir); 

            //Draw if we can see face.
            if(intensity > 0)
            {
                color c = {intensity * 0xFF,intensity * 0xFF,intensity * 0xFF, 0xFF};
                triangle(screen_coords, uv_coords, buffer, zbuffer, head_tex, &c);
                //triangle2D(screen_coords, buffer, &c);
            }// for(int j = 0; j < 3; j++)
            // {
            //     vec3* v0 = (vec3*)mdl->vertices.data[*(int*)mdl->indices.data[i+j] - 1];
            //     vec3* v1 = (vec3*)mdl->vertices.data[*(int*)mdl->indices.data[i+((j+1)%3)] - 1];
            //     int x0 = (v0->x +1.)*buffer->width/2.; 
            //     int y0 = (v0->y +1.)*buffer->height/2.; 
            //     int x1 = (v1->x +1.)*buffer->width/2.; 
            //     int y1 = (v1->y +1.)*buffer->height/2.;
            //     line(x0, y0, x1, y1, buffer, &white); 
            // }
        }
        micros_console_copy_from_buffer(buffer->data, vmd.width*vmd.height*4);
    }

    micros_console_set_video_mode(0x03);

    return 0;
}