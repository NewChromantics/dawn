SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
int subgroupBroadcast_9ccdca() {
  int res = WaveReadLaneAt(1, 1);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(subgroupBroadcast_9ccdca()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(subgroupBroadcast_9ccdca()));
}

FXC validation failure:
C:\src\dawn\Shader@0x0000022A53FF9260(4,13-32): error X3004: undeclared identifier 'WaveReadLaneAt'

