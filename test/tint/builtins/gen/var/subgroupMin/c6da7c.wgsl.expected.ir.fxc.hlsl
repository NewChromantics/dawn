SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
vector<float16_t, 3> subgroupMin_c6da7c() {
  vector<float16_t, 3> arg_0 = (float16_t(1.0h)).xxx;
  vector<float16_t, 3> res = WaveActiveMin(arg_0);
  return res;
}

void fragment_main() {
  prevent_dce.Store<vector<float16_t, 3> >(0u, subgroupMin_c6da7c());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store<vector<float16_t, 3> >(0u, subgroupMin_c6da7c());
}

FXC validation failure:
C:\src\dawn\Shader@0x000002A81ECFECD0(3,8-16): error X3000: syntax error: unexpected token 'float16_t'

