#include "obj.h"
#include <stdio.h>
#include <math.h>

obj_model* load_obj_model(char* filename)
{
    obj_model* mdl = (obj_model*)malloc(sizeof(obj_model));
    
    FILE* in = fopen(filename, "r");
    if(in == NULL)
    {
        printf("ERROR: File %s couldn't be loaded.\n", filename);
        return NULL;
    }
    vector_init(&mdl->vertices);
    vector_init(&mdl->indices);
    vector_init(&mdl->uv);
    //char buff[256];

    //Read own model format (scanf isn't working)

    uint32_t block_header;

    while(!feof(in))
    {
        //Read block header to determine what kind of data is ahead
        fread((char*)&block_header, sizeof(uint32_t), 1, in);
        switch(block_header)
        {
            //VERT
            case 0x56455254:
            {
                uint32_t vcount;
                fread((char*)&vcount, sizeof(uint32_t), 1, in);
                for(uint32_t i = 0; i < vcount; i++)
                {
                    vec3* vert = (vec3*)malloc(sizeof(vec3));
                    fread((char*)vert, sizeof(vec3), 1, in);
                    //vert->y*= -1;
                    //vert->z *= -1;
                    vector_add(&mdl->vertices, (void*)vert);
                }
                break;
            }
            //CXET
            case 0x54455843:
            {
                uint32_t ucount;
                fread((char*)&ucount, sizeof(uint32_t), 1, in);
                for(uint32_t i = 0; i < ucount; i++)
                {
                    vec2f* uv = (vec2f*)malloc(sizeof(vec2f));
                    fread((char*)uv, sizeof(vec2f), 1, in);
                    uv->v = (float)fabs(1 - uv->v);
                    vector_add(&mdl->uv, (void*)uv);
                }
                break;
            }
            //INDS
            case 0x494E4453:
            {
                uint32_t icount;
                fread((char*)&icount, sizeof(uint32_t), 1, in);
                for(uint32_t i = 0; i < icount; i++)
                {
                    vec3i* index = (vec3i*)malloc(sizeof(vec3i));
                    fread((char*)index, sizeof(vec3i), 1, in);
                    vector_add(&mdl->indices, (void*)index);
                }
                break;
            }
        }
    }
    fclose(in);

    return mdl;
}