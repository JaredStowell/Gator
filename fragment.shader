#version 410
out vec4 color;

in vec3 normal;

void main() {
  vec4 A = vec4(.2,.2,.2, 1);
  vec3 L = normalize(vec3(200.0,20.0,0.0));
  vec3 N = normalize(normal);
  double D = clamp(dot(N, L), 0.0, 1.0);
  color = clamp(vec4(vec3(1,.5,.5)*D, 1) + A, 0.0, 1.0);
}
