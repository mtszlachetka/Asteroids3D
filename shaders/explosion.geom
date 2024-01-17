#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 tex_coord;
    vec3 light_dir_TS;
    vec3 view_dir_TS;
    vec3 light_space_pos;
} gs_in[];

out vec2 tex_coord; 
out vec3 view_dir_TS_;
out vec3 light_dir_TS_;
out vec3 light_space_pos_;

uniform float time_for_explosion;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 10.0;
    if (time_for_explosion <= 0.0)
        return position;
    vec3 direction = normal * (magnitude * time_for_explosion + abs(sin(position.x * position.y * position.z)));
    return position + vec4(direction, 0.0);
} 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

void main() {    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    tex_coord = gs_in[0].tex_coord;
    view_dir_TS_ = gs_in[0].view_dir_TS;
    light_dir_TS_ = gs_in[0].light_dir_TS;
    light_space_pos_ = gs_in[0].light_space_pos;
    EmitVertex();
    
    gl_Position = explode(gl_in[1].gl_Position, normal);
    tex_coord = gs_in[1].tex_coord;
    view_dir_TS_ = gs_in[1].view_dir_TS;
    light_dir_TS_ = gs_in[1].light_dir_TS;
    light_space_pos_ = gs_in[1].light_space_pos;
    EmitVertex();
    
    gl_Position = explode(gl_in[2].gl_Position, normal);
    tex_coord = gs_in[2].tex_coord;
    view_dir_TS_ = gs_in[2].view_dir_TS;
    light_dir_TS_ = gs_in[2].light_dir_TS;
    light_space_pos_ = gs_in[2].light_space_pos;
    EmitVertex();
    
    EndPrimitive();
}
