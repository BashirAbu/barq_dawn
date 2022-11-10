#version 310 es
#extension GL_AMD_gpu_shader_half_float : require

void clamp_b195eb() {
  f16vec3 arg_0 = f16vec3(1.0hf);
  f16vec3 arg_1 = f16vec3(1.0hf);
  f16vec3 arg_2 = f16vec3(1.0hf);
  f16vec3 res = clamp(arg_0, arg_1, arg_2);
}

vec4 vertex_main() {
  clamp_b195eb();
  return vec4(0.0f);
}

void main() {
  gl_PointSize = 1.0;
  vec4 inner_result = vertex_main();
  gl_Position = inner_result;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  return;
}
#version 310 es
#extension GL_AMD_gpu_shader_half_float : require
precision mediump float;

void clamp_b195eb() {
  f16vec3 arg_0 = f16vec3(1.0hf);
  f16vec3 arg_1 = f16vec3(1.0hf);
  f16vec3 arg_2 = f16vec3(1.0hf);
  f16vec3 res = clamp(arg_0, arg_1, arg_2);
}

void fragment_main() {
  clamp_b195eb();
}

void main() {
  fragment_main();
  return;
}
#version 310 es
#extension GL_AMD_gpu_shader_half_float : require

void clamp_b195eb() {
  f16vec3 arg_0 = f16vec3(1.0hf);
  f16vec3 arg_1 = f16vec3(1.0hf);
  f16vec3 arg_2 = f16vec3(1.0hf);
  f16vec3 res = clamp(arg_0, arg_1, arg_2);
}

void compute_main() {
  clamp_b195eb();
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  compute_main();
  return;
}
