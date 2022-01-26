#include "shaders.h"

//Include any shader that should be created here
#include "../shader/standard_shader.h"
#include "../shader/toon_shader.h"

//Vector containing all shaders
vector shaders;

//Remember to register ID of shader
//Reconstuction of shader vector is not allowed, known IDs will change
void init_shaders()
{
    vector_init(&shaders);
    //0
    vector_add(&shaders, (void*)create_shader(standard_vertex, standard_fragment)); // 0
    vector_add(&shaders, (void*)create_shader(toon_vertex, toon_fragment)); // 1
}

shader* get_shader(int id)
{
    return id >= shaders.size ? NULL : shaders.data[id];
}