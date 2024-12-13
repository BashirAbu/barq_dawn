//
// fragment_main
//
#version 460
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  uvec4 inner;
} v;
uniform highp usamplerCubeArray arg_1_arg_2;
uvec4 textureGather_f2c6e3() {
  vec4 v_1 = vec4(vec3(1.0f), float(1));
  uvec4 res = textureGather(arg_1_arg_2, v_1, int(1));
  return res;
}
void main() {
  v.inner = textureGather_f2c6e3();
}
//
// compute_main
//
#version 460

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uvec4 inner;
} v;
uniform highp usamplerCubeArray arg_1_arg_2;
uvec4 textureGather_f2c6e3() {
  vec4 v_1 = vec4(vec3(1.0f), float(1));
  uvec4 res = textureGather(arg_1_arg_2, v_1, int(1));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureGather_f2c6e3();
}
//
// vertex_main
//
#version 460


struct VertexOutput {
  vec4 pos;
  uvec4 prevent_dce;
};

uniform highp usamplerCubeArray arg_1_arg_2;
layout(location = 0) flat out uvec4 tint_interstage_location0;
uvec4 textureGather_f2c6e3() {
  vec4 v = vec4(vec3(1.0f), float(1));
  uvec4 res = textureGather(arg_1_arg_2, v, int(1));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), uvec4(0u));
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = textureGather_f2c6e3();
  return tint_symbol;
}
void main() {
  VertexOutput v_1 = vertex_main_inner();
  gl_Position = v_1.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v_1.prevent_dce;
  gl_PointSize = 1.0f;
}
