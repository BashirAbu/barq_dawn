//
// fragment_main
//
#version 460
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  ivec4 inner;
} v;
layout(binding = 0, rg32i) uniform highp readonly iimage2DArray arg_0;
ivec4 textureLoad_7dab57() {
  uvec2 arg_1 = uvec2(1u);
  int arg_2 = 1;
  uvec2 v_1 = arg_1;
  int v_2 = arg_2;
  uint v_3 = (uint(imageSize(arg_0).z) - 1u);
  uint v_4 = min(uint(v_2), v_3);
  ivec2 v_5 = ivec2(min(v_1, (uvec2(imageSize(arg_0).xy) - uvec2(1u))));
  ivec4 res = imageLoad(arg_0, ivec3(v_5, int(v_4)));
  return res;
}
void main() {
  v.inner = textureLoad_7dab57();
}
//
// compute_main
//
#version 460

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  ivec4 inner;
} v;
layout(binding = 0, rg32i) uniform highp readonly iimage2DArray arg_0;
ivec4 textureLoad_7dab57() {
  uvec2 arg_1 = uvec2(1u);
  int arg_2 = 1;
  uvec2 v_1 = arg_1;
  int v_2 = arg_2;
  uint v_3 = (uint(imageSize(arg_0).z) - 1u);
  uint v_4 = min(uint(v_2), v_3);
  ivec2 v_5 = ivec2(min(v_1, (uvec2(imageSize(arg_0).xy) - uvec2(1u))));
  ivec4 res = imageLoad(arg_0, ivec3(v_5, int(v_4)));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_7dab57();
}
//
// vertex_main
//
#version 460


struct VertexOutput {
  vec4 pos;
  ivec4 prevent_dce;
};

layout(binding = 0, rg32i) uniform highp readonly iimage2DArray arg_0;
layout(location = 0) flat out ivec4 tint_interstage_location0;
ivec4 textureLoad_7dab57() {
  uvec2 arg_1 = uvec2(1u);
  int arg_2 = 1;
  uvec2 v = arg_1;
  int v_1 = arg_2;
  uint v_2 = (uint(imageSize(arg_0).z) - 1u);
  uint v_3 = min(uint(v_1), v_2);
  ivec2 v_4 = ivec2(min(v, (uvec2(imageSize(arg_0).xy) - uvec2(1u))));
  ivec4 res = imageLoad(arg_0, ivec3(v_4, int(v_3)));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), ivec4(0));
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = textureLoad_7dab57();
  return tint_symbol;
}
void main() {
  VertexOutput v_5 = vertex_main_inner();
  gl_Position = v_5.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v_5.prevent_dce;
  gl_PointSize = 1.0f;
}
