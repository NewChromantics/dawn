SKIP: FAILED


static int2 u = (1).xx;
void f() {
  vector<float16_t, 2> v = vector<float16_t, 2>(u);
}

[numthreads(1, 1, 1)]
void unused_entry_point() {
}

FXC validation failure:
C:\src\dawn\Shader@0x00000173900C0E60(4,10-18): error X3000: syntax error: unexpected token 'float16_t'

