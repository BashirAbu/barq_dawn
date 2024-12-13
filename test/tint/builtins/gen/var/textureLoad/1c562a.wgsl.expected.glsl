//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;


struct TintTextureUniformData {
  uint tint_builtin_value_0;
};

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  uvec4 inner;
} v;
layout(binding = 0, std140)
uniform f_tint_symbol_ubo {
  TintTextureUniformData inner;
} v_1;
uniform highp usampler3D arg_0;
uvec4 textureLoad_1c562a() {
  uvec3 arg_1 = uvec3(1u);
  uint arg_2 = 1u;
  uvec3 v_2 = arg_1;
  uint v_3 = min(arg_2, (v_1.inner.tint_builtin_value_0 - 1u));
  ivec3 v_4 = ivec3(min(v_2, (uvec3(textureSize(arg_0, int(v_3))) - uvec3(1u))));
  uvec4 res = texelFetch(arg_0, v_4, int(v_3));
  return res;
}
void main() {
  v.inner = textureLoad_1c562a();
}
//
// compute_main
//
#version 310 es


struct TintTextureUniformData {
  uint tint_builtin_value_0;
};

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uvec4 inner;
} v;
layout(binding = 0, std140)
uniform tint_symbol_1_ubo {
  TintTextureUniformData inner;
} v_1;
uniform highp usampler3D arg_0;
uvec4 textureLoad_1c562a() {
  uvec3 arg_1 = uvec3(1u);
  uint arg_2 = 1u;
  uvec3 v_2 = arg_1;
  uint v_3 = min(arg_2, (v_1.inner.tint_builtin_value_0 - 1u));
  ivec3 v_4 = ivec3(min(v_2, (uvec3(textureSize(arg_0, int(v_3))) - uvec3(1u))));
  uvec4 res = texelFetch(arg_0, v_4, int(v_3));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_1c562a();
}
//
// vertex_main
//
#version 310 es


struct TintTextureUniformData {
  uint tint_builtin_value_0;
};

struct VertexOutput {
  vec4 pos;
  uvec4 prevent_dce;
};

layout(binding = 0, std140)
uniform v_tint_symbol_1_ubo {
  TintTextureUniformData inner;
} v;
uniform highp usampler3D arg_0;
layout(location = 0) flat out uvec4 tint_interstage_location0;
uvec4 textureLoad_1c562a() {
  uvec3 arg_1 = uvec3(1u);
  uint arg_2 = 1u;
  uvec3 v_1 = arg_1;
  uint v_2 = min(arg_2, (v.inner.tint_builtin_value_0 - 1u));
  ivec3 v_3 = ivec3(min(v_1, (uvec3(textureSize(arg_0, int(v_2))) - uvec3(1u))));
  uvec4 res = texelFetch(arg_0, v_3, int(v_2));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f), uvec4(0u));
  tint_symbol.pos = vec4(0.0f);
  tint_symbol.prevent_dce = textureLoad_1c562a();
  return tint_symbol;
}
void main() {
  VertexOutput v_4 = vertex_main_inner();
  gl_Position = v_4.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v_4.prevent_dce;
  gl_PointSize = 1.0f;
}
