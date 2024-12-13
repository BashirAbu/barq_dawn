//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  uvec4 inner;
} v;
uniform highp usampler2DMS arg_0;
uvec4 textureLoad_49f76f() {
  ivec2 v_1 = ivec2(min(uvec2(1u), (uvec2(textureSize(arg_0)) - uvec2(1u))));
  uvec4 res = texelFetch(arg_0, v_1, int(1u));
  return res;
}
void main() {
  v.inner = textureLoad_49f76f();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uvec4 inner;
} v;
uniform highp usampler2DMS arg_0;
uvec4 textureLoad_49f76f() {
  ivec2 v_1 = ivec2(min(uvec2(1u), (uvec2(textureSize(arg_0)) - uvec2(1u))));
  uvec4 res = texelFetch(arg_0, v_1, int(1u));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_49f76f();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  uvec4 prevent_dce;
};

uniform highp usampler2DMS arg_0;
layout(location = 0) flat out uvec4 tint_interstage_location0;
uvec4 textureLoad_49f76f() {
  ivec2 v = ivec2(min(uvec2(1u), (uvec2(textureSize(arg_0)) - uvec2(1u))));
  uvec4 res = texelFetch(arg_0, v, int(1u));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), uvec4(0u));
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = textureLoad_49f76f();
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
