SKIP: FAILED


RWByteAddressBuffer U : register(u0);
void f() {
  U.Store<vector<float16_t, 3> >(0u, vector<float16_t, 3>(float16_t(1.0h), float16_t(2.0h), float16_t(3.0h)));
  U.Store<float16_t>(0u, float16_t(1.0h));
  U.Store<float16_t>(2u, float16_t(2.0h));
  U.Store<float16_t>(4u, float16_t(3.0h));
}

[numthreads(1, 1, 1)]
void unused_entry_point() {
}

FXC validation failure:
C:\src\dawn\Shader@0x000001B6193EF610(4,3-9): error X3018: invalid subscript 'Store'

