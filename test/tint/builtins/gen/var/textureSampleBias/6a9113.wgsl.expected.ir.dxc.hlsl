
RWByteAddressBuffer prevent_dce : register(u0);
Texture2D<float4> arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);
float4 textureSampleBias_6a9113() {
  float2 arg_2 = (1.0f).xx;
  float arg_3 = 1.0f;
  float2 v = arg_2;
  float4 res = arg_0.SampleBias(arg_1, v, clamp(arg_3, -16.0f, 15.9899997711181640625f));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureSampleBias_6a9113()));
}

