#ifndef STANDARD_SHADER_H
#define STANDARD_SHADER_H

#include "../renderer/srender.h"

void standard_vertex(shader* _shader, vec4 vert, vec3 norm, vec2f uv, int vertnum);
color standard_fragment(shader* _shader);

#endif