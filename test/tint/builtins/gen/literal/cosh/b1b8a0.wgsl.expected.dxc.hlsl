void cosh_b1b8a0() {
  vector<float16_t, 3> res = cosh((float16_t(1.0h)).xxx);
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  cosh_b1b8a0();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  cosh_b1b8a0();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  cosh_b1b8a0();
  return;
}
