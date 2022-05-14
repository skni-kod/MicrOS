#include <stdio.h>
#include <stdint.h>
#include <micros.h>

#include "util/util.h"
#define PL_MPEG_IMPLEMENTATION
#include "mpeg/mpeg.h"
#include "image/image.h"

static uint8_t bpp;
static image* fbuffer;

void app_on_video(plm_t *player, plm_frame_t *frame, void *user);

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        printf("Usage: player filename\n");
        return 0;
    }

    video_mode_descriptor vmd;
    vmd.width = 800;
    vmd.height = 600;
    vmd.grayscale = 0;
    vmd.colors = 0x100000000;
    vmd.text = 0;
    vmd.planar = 0;
    bpp = 4;
    if(micros_gdi_find_video_mode(&vmd) != 0)
    {
        printf("32bit color mode not found. Looking for 24 bit.\n");
        vmd.colors = 0x1000000;
        if(micros_gdi_find_video_mode(&vmd) != 0)
        {
            printf("ERROR: No True Color mode found.\n");
            return -1;
        }
        bpp = 3;
    }

    printf("Loading file %s...\n", argv[2]);

    uint8_t* data = NULL;
    uint32_t fileSize = 0;
    fileSize = loadFile(argv[2], &data);
    if(fileSize == 0)
    {
        return -2;
    }

    fbuffer = create_image(800,600,bpp);

    plm_t* plm = plm_create_with_memory(data, fileSize, 0);
    //plm_t* plm = plm_create_with_filename(argv[2]);

    plm_set_video_decode_callback(plm, app_on_video, NULL);

    //Update & draw

    double dt = 0;

    micros_console_set_video_mode(vmd.mode_number);


    clock_t beginTime;
    while(1)
    {
        beginTime = clock();
        //CLEAR SURFACE
        //memset(fbuffer->data, 0, fbuffer->width*fbuffer->height*bpp);

        //LOGIC & DRAW
        if(plm->has_ended) break;
        plm_decode(plm, dt);

        //BLIT SURFACE
        micros_console_copy_from_buffer(fbuffer->data, vmd.width*vmd.height*bpp);
        dt = (clock() - beginTime)/1000.0;
    }
    //END OF LOGIC

    //Not required, since application is ending.
    free(data);
    plm_destroy(plm);

    micros_console_set_video_mode(3);

    return 0;
}

void app_on_video(plm_t *player, plm_frame_t *frame, void *user)
{
    //plm_frame_to_rgb(frame,rgbdata, frame->width*3);

    int offset = ((fbuffer->height - frame->height)/2)*fbuffer->width*bpp;

    if(bpp > 3)
    {
        plm_frame_to_bgra(frame, fbuffer->data+offset, frame->width*4);
    }
    else
    {
        plm_frame_to_bgr(frame, fbuffer->data+offset, frame->width*3);
    }

    // if(frame->height < fbuffer->height)
    // {
    //     bufferPos = ((fbuffer->height - frame->height)/2)*fbuffer->width*bpp;
    // }

    // for(int i = 0; i < frame->width*frame->height; i++)
    // {
    //     fbuffer->data[bufferPos++] = rgbdata[rgbPos+2];
    //     fbuffer->data[bufferPos++] = rgbdata[rgbPos+1];
    //     fbuffer->data[bufferPos++] = rgbdata[rgbPos];
    //     if(bpp > 3) fbuffer->data[bufferPos++] = 0xFF;
    //     rgbPos += 3;
    // }
}