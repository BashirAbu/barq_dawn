#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, rgba8_snorm) uniform highp writeonly image2D arg_0;
void textureStore_40c455() {
  imageStore(arg_0, ivec2(uvec2(1u)), vec4(1.0f));
}
void main() {
  textureStore_40c455();
}
#version 310 es

layout(binding = 0, rgba8_snorm) uniform highp writeonly image2D arg_0;
void textureStore_40c455() {
  imageStore(arg_0, ivec2(uvec2(1u)), vec4(1.0f));
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  textureStore_40c455();
}
