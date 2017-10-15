#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 normal;

void main() {
  for(int i = 0; i < 3; i++) { // You used triangles, so it's always 3
    gl_Position = gl_in[i].gl_Position;

    normal = normalize(cross(vec3(gl_in[0].gl_Position - gl_in[1].gl_Position), vec3(gl_in[0].gl_Position - gl_in[2].gl_Position)));
    EmitVertex();
  }

  EndPrimitive();
}
