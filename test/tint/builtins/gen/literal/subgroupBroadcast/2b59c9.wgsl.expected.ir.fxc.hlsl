SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
int3 subgroupBroadcast_2b59c9() {
  int3 res = WaveReadLaneAt((1).xxx, 1);
  return res;
}

void fragment_main() {
  prevent_dce.Store3(0u, asuint(subgroupBroadcast_2b59c9()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store3(0u, asuint(subgroupBroadcast_2b59c9()));
}

FXC validation failure:
C:\src\dawn\Shader@0x00000272CE18EE90(4,14-39): error X3004: undeclared identifier 'WaveReadLaneAt'

