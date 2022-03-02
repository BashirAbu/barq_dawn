#version 310 es

uvec3 x_2 = uvec3(0u, 0u, 0u);
layout(binding = 0, std430) buffer S_1 {
  uint field0[];
} x_5;
layout(binding = 1, std430) buffer S_2 {
  uint field0[];
} x_6;
void main_1() {
  uint x_20 = x_2.x;
  uint x_22 = x_5.field0[x_20];
  x_6.field0[x_20] = uint(-(int(x_22)));
  return;
}

void tint_symbol(uvec3 x_2_param) {
  x_2 = x_2_param;
  main_1();
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  tint_symbol(gl_GlobalInvocationID);
  return;
}
