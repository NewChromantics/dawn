struct frexp_result_f32 {
  float fract;
  int exp;
};


[numthreads(1, 1, 1)]
void main() {
  float tint_symbol = 1.25f;
  float v = 0.0f;
  float v_1 = frexp(tint_symbol, v);
  float v_2 = float(sign(tint_symbol));
  v = (v_2 * v);
  frexp_result_f32 v_3 = {v_1, int(v)};
  frexp_result_f32 res = v_3;
  float fract = res.fract;
  frexp_result_f32 v_4 = v_3;
  int exp = v_4.exp;
}

