SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
float3 subgroupMul_93eccd() {
  float3 arg_0 = (1.0f).xxx;
  float3 res = WaveActiveProduct(arg_0);
  return res;
}

void fragment_main() {
  prevent_dce.Store3(0u, asuint(subgroupMul_93eccd()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store3(0u, asuint(subgroupMul_93eccd()));
}

FXC validation failure:
C:\src\dawn\Shader@0x000002045369FF10(5,16-39): error X3004: undeclared identifier 'WaveActiveProduct'

