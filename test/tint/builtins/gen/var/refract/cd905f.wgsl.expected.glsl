//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  vec2 inner;
} v;
vec2 refract_cd905f() {
  vec2 arg_0 = vec2(1.0f);
  vec2 arg_1 = vec2(1.0f);
  float arg_2 = 1.0f;
  vec2 res = refract(arg_0, arg_1, arg_2);
  return res;
}
void main() {
  v.inner = refract_cd905f();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec2 inner;
} v;
vec2 refract_cd905f() {
  vec2 arg_0 = vec2(1.0f);
  vec2 arg_1 = vec2(1.0f);
  float arg_2 = 1.0f;
  vec2 res = refract(arg_0, arg_1, arg_2);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = refract_cd905f();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  vec2 prevent_dce;
};

layout(location = 0) flat out vec2 tint_interstage_location0;
vec2 refract_cd905f() {
  vec2 arg_0 = vec2(1.0f);
  vec2 arg_1 = vec2(1.0f);
  float arg_2 = 1.0f;
  vec2 res = refract(arg_0, arg_1, arg_2);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), vec2(0.0f));
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = refract_cd905f();
  return tint_symbol;
}
void main() {
  VertexOutput v = vertex_main_inner();
  gl_Position = v.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v.prevent_dce;
  gl_PointSize = 1.0f;
}
