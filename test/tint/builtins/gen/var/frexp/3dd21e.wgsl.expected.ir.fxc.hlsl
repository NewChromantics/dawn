SKIP: FAILED

struct frexp_result_vec4_f16 {
  vector<float16_t, 4> fract;
  int4 exp;
};

struct VertexOutput {
  float4 pos;
};

struct vertex_main_outputs {
  float4 VertexOutput_pos : SV_Position;
};


void frexp_3dd21e() {
  vector<float16_t, 4> arg_0 = (float16_t(1.0h)).xxxx;
  frexp_result_vec4_f16 res = frexp(arg_0);
}

void fragment_main() {
  frexp_3dd21e();
}

[numthreads(1, 1, 1)]
void compute_main() {
  frexp_3dd21e();
}

VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = (VertexOutput)0;
  tint_symbol.pos = (0.0f).xxxx;
  frexp_3dd21e();
  VertexOutput v = tint_symbol;
  return v;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_1 = vertex_main_inner();
  vertex_main_outputs v_2 = {v_1.pos};
  return v_2;
}

FXC validation failure:
C:\src\dawn\Shader@0x000001F1EC0F0A80(2,10-18): error X3000: syntax error: unexpected token 'float16_t'

