struct VertexOutput {
  float4 pos;
  float prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation float VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


RWByteAddressBuffer prevent_dce : register(u0);
Texture2D arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);
float textureSampleLevel_f3b2c8() {
  float2 arg_2 = (1.0f).xx;
  uint arg_3 = 1u;
  float2 v = arg_2;
  float res = arg_0.SampleLevel(arg_1, v, float(arg_3), (int(1)).xx).x;
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(textureSampleLevel_f3b2c8()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(textureSampleLevel_f3b2c8()));
}

VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = (VertexOutput)0;
  tint_symbol.pos = (0.0f).xxxx;
  tint_symbol.prevent_dce = textureSampleLevel_f3b2c8();
  VertexOutput v_1 = tint_symbol;
  return v_1;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_2 = vertex_main_inner();
  VertexOutput v_3 = v_2;
  VertexOutput v_4 = v_2;
  vertex_main_outputs v_5 = {v_4.prevent_dce, v_3.pos};
  return v_5;
}

