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
layout(binding = 0, rg32i) uniform highp readonly iimage2D arg_0;
ivec4 textureLoad_53378a() {
  uvec2 v_1 = (uvec2(imageSize(arg_0)) - uvec2(1u));
  ivec4 res = imageLoad(arg_0, ivec2(min(uvec2(ivec2(1)), v_1)));
  return res;
}
void main() {
  v.inner = textureLoad_53378a();
}
//
// compute_main
//
#version 460

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  ivec4 inner;
} v;
layout(binding = 0, rg32i) uniform highp readonly iimage2D arg_0;
ivec4 textureLoad_53378a() {
  uvec2 v_1 = (uvec2(imageSize(arg_0)) - uvec2(1u));
  ivec4 res = imageLoad(arg_0, ivec2(min(uvec2(ivec2(1)), v_1)));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_53378a();
}
//
// vertex_main
//
#version 460


struct VertexOutput {
  vec4 pos;
  ivec4 prevent_dce;
};

layout(binding = 0, rg32i) uniform highp readonly iimage2D arg_0;
layout(location = 0) flat out ivec4 tint_interstage_location0;
ivec4 textureLoad_53378a() {
  uvec2 v = (uvec2(imageSize(arg_0)) - uvec2(1u));
  ivec4 res = imageLoad(arg_0, ivec2(min(uvec2(ivec2(1)), v)));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), ivec4(0));
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = textureLoad_53378a();
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
