struct VertexOutput {
  float4 pos;
  float prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation float VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);
float textureSampleLevel_e6ce9e() {
  float2 arg_2 = (1.0f).xx;
  uint arg_3 = 1u;
  int arg_4 = int(1);
  float2 v = arg_2;
  int v_1 = arg_4;
  float3 v_2 = float3(v, float(arg_3));
  float res = arg_0.SampleLevel(arg_1, v_2, float(v_1), (int(1)).xx).x;
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(textureSampleLevel_e6ce9e()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(textureSampleLevel_e6ce9e()));
}

VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = (VertexOutput)0;
  tint_symbol.pos = (0.0f).xxxx;
  tint_symbol.prevent_dce = textureSampleLevel_e6ce9e();
  VertexOutput v_3 = tint_symbol;
  return v_3;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_4 = vertex_main_inner();
  VertexOutput v_5 = v_4;
  VertexOutput v_6 = v_4;
  vertex_main_outputs v_7 = {v_6.prevent_dce, v_5.pos};
  return v_7;
}

