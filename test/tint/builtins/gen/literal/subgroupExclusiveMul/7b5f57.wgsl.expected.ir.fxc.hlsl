SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
float4 subgroupExclusiveMul_7b5f57() {
  float4 res = WavePrefixProduct((1.0f).xxxx);
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(subgroupExclusiveMul_7b5f57()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(subgroupExclusiveMul_7b5f57()));
}

FXC validation failure:
C:\src\dawn\Shader@0x0000024D6287FE00(4,16-45): error X3004: undeclared identifier 'WavePrefixProduct'

