SKIP: INVALID


RWByteAddressBuffer prevent_dce : register(u0);
float4 subgroupInclusiveMul_69326e() {
  float4 res = (WavePrefixProduct((1.0f).xxxx) * (1.0f).xxxx);
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(subgroupInclusiveMul_69326e()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(subgroupInclusiveMul_69326e()));
}

FXC validation failure:
<scrubbed_path>(4,17-46): error X3004: undeclared identifier 'WavePrefixProduct'


tint executable returned error: exit status 1
