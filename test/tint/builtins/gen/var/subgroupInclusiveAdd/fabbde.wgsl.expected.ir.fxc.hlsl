SKIP: INVALID


RWByteAddressBuffer prevent_dce : register(u0);
int2 subgroupInclusiveAdd_fabbde() {
  int2 arg_0 = (int(1)).xx;
  int2 v = arg_0;
  int2 res = (WavePrefixSum(v) + v);
  return res;
}

void fragment_main() {
  prevent_dce.Store2(0u, asuint(subgroupInclusiveAdd_fabbde()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store2(0u, asuint(subgroupInclusiveAdd_fabbde()));
}

FXC validation failure:
<scrubbed_path>(6,15-30): error X3004: undeclared identifier 'WavePrefixSum'


tint executable returned error: exit status 1
