#ifndef TOON_SHADER_H
#define TOON_SHADER_H

#include "../renderer/srender.h"

void toon_vertex(shader* _shader, vec4 vert, vec3 norm, vec2f uv, int vertnum);
color toon_fragment(shader* _shader);

#endif