SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
uint3 subgroupBroadcast_34ae44() {
  uint3 arg_0 = (1u).xxx;
  uint3 res = WaveReadLaneAt(arg_0, 1);
  return res;
}

void fragment_main() {
  prevent_dce.Store3(0u, subgroupBroadcast_34ae44());
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store3(0u, subgroupBroadcast_34ae44());
}

FXC validation failure:
C:\src\dawn\Shader@0x000002433D8AEF60(5,15-38): error X3004: undeclared identifier 'WaveReadLaneAt'

