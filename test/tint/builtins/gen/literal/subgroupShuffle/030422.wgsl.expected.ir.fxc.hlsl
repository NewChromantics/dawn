SKIP: INVALID


RWByteAddressBuffer prevent_dce : register(u0);
float subgroupShuffle_030422() {
  float res = WaveReadLaneAt(1.0f, int(1));
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(subgroupShuffle_030422()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(subgroupShuffle_030422()));
}

FXC validation failure:
<scrubbed_path>(4,15-42): error X3004: undeclared identifier 'WaveReadLaneAt'


tint executable returned error: exit status 1
