SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
vector<float16_t, 2> subgroupShuffle_8c3fd2() {
  vector<float16_t, 2> arg_0 = (float16_t(1.0h)).xx;
  int arg_1 = 1;
  vector<float16_t, 2> res = WaveReadLaneAt(arg_0, arg_1);
  return res;
}

void fragment_main() {
  prevent_dce.Store<vector<float16_t, 2> >(0u, subgroupShuffle_8c3fd2());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store<vector<float16_t, 2> >(0u, subgroupShuffle_8c3fd2());
}

FXC validation failure:
C:\src\dawn\Shader@0x0000015FC5730360(3,8-16): error X3000: syntax error: unexpected token 'float16_t'

