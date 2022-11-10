struct frexp_result_vec3_f16 {
  vector<float16_t, 3> sig;
  int3 exp;
};
frexp_result_vec3_f16 tint_frexp(vector<float16_t, 3> param_0) {
  vector<float16_t, 3> exp;
  vector<float16_t, 3> sig = frexp(param_0, exp);
  frexp_result_vec3_f16 result = {sig, int3(exp)};
  return result;
}

void frexp_ae4a66() {
  frexp_result_vec3_f16 res = tint_frexp((float16_t(1.0h)).xxx);
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  frexp_ae4a66();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  frexp_ae4a66();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  frexp_ae4a66();
  return;
}
