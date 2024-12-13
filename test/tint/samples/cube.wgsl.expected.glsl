//
// vtx_main
//
#version 310 es


struct Uniforms {
  mat4 modelViewProjectionMatrix;
};

struct VertexOutput {
  vec4 vtxFragColor;
  vec4 Position;
};

struct VertexInput {
  vec4 cur_position;
  vec4 color;
};

layout(binding = 0, std140)
uniform v_uniforms_block_ubo {
  Uniforms inner;
} v;
layout(location = 0) in vec4 vtx_main_loc0_Input;
layout(location = 1) in vec4 vtx_main_loc1_Input;
layout(location = 0) out vec4 tint_interstage_location0;
VertexOutput vtx_main_inner(VertexInput tint_symbol) {
  VertexOutput tint_symbol_1 = VertexOutput(vec4(0.0f), vec4(0.0f));
  tint_symbol_1.Position = (v.inner.modelViewProjectionMatrix * tint_symbol.cur_position);
  tint_symbol_1.vtxFragColor = tint_symbol.color;
  return tint_symbol_1;
}
void main() {
  VertexOutput v_1 = vtx_main_inner(VertexInput(vtx_main_loc0_Input, vtx_main_loc1_Input));
  tint_interstage_location0 = v_1.vtxFragColor;
  gl_Position = v_1.Position;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
//
// frag_main
//
#version 310 es
precision highp float;
precision highp int;

layout(location = 0) in vec4 tint_interstage_location0;
layout(location = 0) out vec4 frag_main_loc0_Output;
vec4 frag_main_inner(vec4 fragColor) {
  return fragColor;
}
void main() {
  frag_main_loc0_Output = frag_main_inner(tint_interstage_location0);
}
