SKIP: INVALID


RWByteAddressBuffer prevent_dce : register(u0);
float subgroupShuffle_4752bd() {
  float res = WaveReadLaneAt(1.0f, 1u);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(subgroupShuffle_4752bd()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(subgroupShuffle_4752bd()));
}

FXC validation failure:
<scrubbed_path>(4,15-38): error X3004: undeclared identifier 'WaveReadLaneAt'


tint executable returned error: exit status 1
