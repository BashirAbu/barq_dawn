#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, r32f) uniform highp image2D arg_0;
void textureStore_3d1ebe() {
  uvec2 arg_1 = uvec2(1u);
  vec4 arg_2 = vec4(1.0f);
  vec4 v = arg_2;
  imageStore(arg_0, ivec2(arg_1), v);
}
void main() {
  textureStore_3d1ebe();
}
#version 310 es

layout(binding = 0, r32f) uniform highp image2D arg_0;
void textureStore_3d1ebe() {
  uvec2 arg_1 = uvec2(1u);
  vec4 arg_2 = vec4(1.0f);
  vec4 v = arg_2;
  imageStore(arg_0, ivec2(arg_1), v);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  textureStore_3d1ebe();
}
