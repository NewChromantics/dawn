SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
float3 subgroupBroadcastFirst_5c6962() {
  float3 res = WaveReadLaneFirst((1.0f).xxx);
  return res;
}

void fragment_main() {
  prevent_dce.Store3(0u, asuint(subgroupBroadcastFirst_5c6962()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store3(0u, asuint(subgroupBroadcastFirst_5c6962()));
}

FXC validation failure:
C:\src\dawn\Shader@0x0000021F906B0630(4,16-44): error X3004: undeclared identifier 'WaveReadLaneFirst'

