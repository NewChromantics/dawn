SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
uint subgroupBroadcastFirst_61f177() {
  uint res = WaveReadLaneFirst(1u);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, subgroupBroadcastFirst_61f177());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, subgroupBroadcastFirst_61f177());
}

FXC validation failure:
C:\src\dawn\Shader@0x0000023C69F02F90(4,14-34): error X3004: undeclared identifier 'WaveReadLaneFirst'

