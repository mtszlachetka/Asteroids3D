#version 430 core

in vec2 tex_coord;
out vec4 out_color;

uniform sampler2D img;
uniform bool is_horizontal;

const float weights[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };

void main() {
	vec2 texel_size = 1.0 / textureSize(img, 0);
    vec3 texel_color = texture(img, tex_coord).rgb * weights[0];
    if(is_horizontal) {
        for(int i = 1; i < 5; ++i) {
            texel_color += texture(img, tex_coord + vec2(texel_size.x * i, 0.0)).rgb * weights[i];
            texel_color += texture(img, tex_coord - vec2(texel_size.x * i, 0.0)).rgb * weights[i];
        }
    } else {
        for(int i = 1; i < 5; ++i) {
            texel_color += texture(img, tex_coord + vec2(0.0, texel_size.y * i)).rgb * weights[i];
            texel_color += texture(img, tex_coord - vec2(0.0, texel_size.y * i)).rgb * weights[i];
        }
    }
    out_color = vec4(texel_color, 1.0);
}