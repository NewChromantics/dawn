SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
int2 subgroupBroadcast_3e6879() {
  int2 res = WaveReadLaneAt((1).xx, 1u);
  return res;
}

void fragment_main() {
  prevent_dce.Store2(0u, asuint(subgroupBroadcast_3e6879()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store2(0u, asuint(subgroupBroadcast_3e6879()));
}

FXC validation failure:
C:\src\dawn\Shader@0x000001DBC9E792D0(4,14-39): error X3004: undeclared identifier 'WaveReadLaneAt'

