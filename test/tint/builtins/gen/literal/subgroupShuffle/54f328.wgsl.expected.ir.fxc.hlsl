SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
uint subgroupShuffle_54f328() {
  uint res = WaveReadLaneAt(1u, 1);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, subgroupShuffle_54f328());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, subgroupShuffle_54f328());
}

FXC validation failure:
C:\src\dawn\Shader@0x000001ABBCC290B0(4,14-34): error X3004: undeclared identifier 'WaveReadLaneAt'

