SKIP: FAILED


static matrix<float16_t, 4, 3> u = matrix<float16_t, 4, 3>(vector<float16_t, 3>(float16_t(1.0h), float16_t(2.0h), float16_t(3.0h)), vector<float16_t, 3>(float16_t(4.0h), float16_t(5.0h), float16_t(6.0h)), vector<float16_t, 3>(float16_t(7.0h), float16_t(8.0h), float16_t(9.0h)), vector<float16_t, 3>(float16_t(10.0h), float16_t(11.0h), float16_t(12.0h)));
void f() {
  float4x3 v = float4x3(u);
}

[numthreads(1, 1, 1)]
void unused_entry_point() {
}

FXC validation failure:
C:\src\dawn\Shader@0x000001FF8F95A3B0(2,15-23): error X3000: syntax error: unexpected token 'float16_t'

