SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
uint2 subgroupExclusiveAdd_48acea() {
  uint2 res = WavePrefixSum((1u).xx);
  return res;
}

void fragment_main() {
  prevent_dce.Store2(0u, subgroupExclusiveAdd_48acea());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store2(0u, subgroupExclusiveAdd_48acea());
}

FXC validation failure:
C:\src\dawn\Shader@0x0000014E1D0F3A40(4,15-36): error X3004: undeclared identifier 'WavePrefixSum'

