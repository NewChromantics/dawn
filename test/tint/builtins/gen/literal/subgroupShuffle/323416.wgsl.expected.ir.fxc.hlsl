SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
int2 subgroupShuffle_323416() {
  int2 res = WaveReadLaneAt((1).xx, 1u);
  return res;
}

void fragment_main() {
  prevent_dce.Store2(0u, asuint(subgroupShuffle_323416()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store2(0u, asuint(subgroupShuffle_323416()));
}

FXC validation failure:
C:\src\dawn\Shader@0x0000024C78702300(4,14-39): error X3004: undeclared identifier 'WaveReadLaneAt'

