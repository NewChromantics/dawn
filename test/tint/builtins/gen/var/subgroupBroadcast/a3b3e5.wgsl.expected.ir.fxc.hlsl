SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
vector<float16_t, 3> subgroupBroadcast_a3b3e5() {
  vector<float16_t, 3> arg_0 = (float16_t(1.0h)).xxx;
  vector<float16_t, 3> res = WaveReadLaneAt(arg_0, 1);
  return res;
}

void fragment_main() {
  prevent_dce.Store<vector<float16_t, 3> >(0u, subgroupBroadcast_a3b3e5());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store<vector<float16_t, 3> >(0u, subgroupBroadcast_a3b3e5());
}

FXC validation failure:
C:\src\dawn\Shader@0x000001E0919EF510(3,8-16): error X3000: syntax error: unexpected token 'float16_t'

