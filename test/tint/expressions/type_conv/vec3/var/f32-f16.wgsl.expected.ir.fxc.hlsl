SKIP: FAILED


static float3 u = (1.0f).xxx;
void f() {
  vector<float16_t, 3> v = vector<float16_t, 3>(u);
}

[numthreads(1, 1, 1)]
void unused_entry_point() {
}

FXC validation failure:
C:\src\dawn\Shader@0x000001FEDC9EAFA0(4,10-18): error X3000: syntax error: unexpected token 'float16_t'

