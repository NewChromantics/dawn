SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
float2 subgroupBroadcast_cd7aa1() {
  float2 arg_0 = (1.0f).xx;
  float2 res = WaveReadLaneAt(arg_0, 1);
  return res;
}

void fragment_main() {
  prevent_dce.Store2(0u, asuint(subgroupBroadcast_cd7aa1()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store2(0u, asuint(subgroupBroadcast_cd7aa1()));
}

FXC validation failure:
C:\src\dawn\Shader@0x000001E1B582F350(5,16-39): error X3004: undeclared identifier 'WaveReadLaneAt'

