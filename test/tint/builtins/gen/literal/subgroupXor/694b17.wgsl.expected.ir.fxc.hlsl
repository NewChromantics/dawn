SKIP: FAILED


RWByteAddressBuffer prevent_dce : register(u0);
int subgroupXor_694b17() {
  int res = asint(WaveActiveBitXor(asuint(1)));
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(subgroupXor_694b17()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(subgroupXor_694b17()));
}

FXC validation failure:
C:\src\dawn\Shader@0x00000193D889EEB0(4,19-45): error X3004: undeclared identifier 'WaveActiveBitXor'

