SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
float3 subgroupExclusiveMul_0a04d5() {
  float3 res = WavePrefixProduct((1.0f).xxx);
  return res;
}

void fragment_main() {
  prevent_dce.Store3(0u, asuint(subgroupExclusiveMul_0a04d5()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store3(0u, asuint(subgroupExclusiveMul_0a04d5()));
}

FXC validation failure:
C:\src\dawn\Shader@0x0000023219CBBE90(4,16-44): error X3004: undeclared identifier 'WavePrefixProduct'

