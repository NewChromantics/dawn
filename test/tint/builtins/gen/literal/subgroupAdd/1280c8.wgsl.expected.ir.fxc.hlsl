SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
uint2 subgroupAdd_1280c8() {
  uint2 res = WaveActiveSum((1u).xx);
  return res;
}

void fragment_main() {
  prevent_dce.Store2(0u, subgroupAdd_1280c8());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store2(0u, subgroupAdd_1280c8());
}

FXC validation failure:
C:\src\dawn\Shader@0x000001DA4802CB10(4,15-36): error X3004: undeclared identifier 'WaveActiveSum'

