SKIP: FAILED

struct VertexOutput {
  float4 pos;
  int prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation int VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


RWByteAddressBuffer prevent_dce : register(u0);
int transpose_d6faec() {
  matrix<float16_t, 3, 2> res = matrix<float16_t, 3, 2>((float16_t(1.0h)).xx, (float16_t(1.0h)).xx, (float16_t(1.0h)).xx);
  return (((res[0].x == float16_t(0.0h))) ? (1) : (0));
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(transpose_d6faec()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(transpose_d6faec()));
}

VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = (VertexOutput)0;
  tint_symbol.pos = (0.0f).xxxx;
  tint_symbol.prevent_dce = transpose_d6faec();
  VertexOutput v = tint_symbol;
  return v;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_1 = vertex_main_inner();
  VertexOutput v_2 = v_1;
  VertexOutput v_3 = v_1;
  vertex_main_outputs v_4 = {v_3.prevent_dce, v_2.pos};
  return v_4;
}

FXC validation failure:
C:\src\dawn\Shader@0x00000267E68F00A0(14,10-18): error X3000: syntax error: unexpected token 'float16_t'
C:\src\dawn\Shader@0x00000267E68F00A0(15,13-15): error X3004: undeclared identifier 'res'

