SKIP: INVALID


RWByteAddressBuffer prevent_dce : register(u0);
int subgroupMax_6c913e() {
  int res = WaveActiveMax(1);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(subgroupMax_6c913e()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(subgroupMax_6c913e()));
}

FXC validation failure:
<scrubbed_path>(4,13-28): error X3004: undeclared identifier 'WaveActiveMax'


tint executable returned error: exit status 1
