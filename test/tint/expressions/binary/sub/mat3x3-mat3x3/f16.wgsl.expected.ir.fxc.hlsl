SKIP: FAILED


[numthreads(1, 1, 1)]
void f() {
  matrix<float16_t, 3, 3> a = matrix<float16_t, 3, 3>(vector<float16_t, 3>(float16_t(1.0h), float16_t(2.0h), float16_t(3.0h)), vector<float16_t, 3>(float16_t(4.0h), float16_t(5.0h), float16_t(6.0h)), vector<float16_t, 3>(float16_t(7.0h), float16_t(8.0h), float16_t(9.0h)));
  matrix<float16_t, 3, 3> b = matrix<float16_t, 3, 3>(vector<float16_t, 3>(float16_t(-1.0h), float16_t(-2.0h), float16_t(-3.0h)), vector<float16_t, 3>(float16_t(-4.0h), float16_t(-5.0h), float16_t(-6.0h)), vector<float16_t, 3>(float16_t(-7.0h), float16_t(-8.0h), float16_t(-9.0h)));
  matrix<float16_t, 3, 3> r = (a - b);
}

FXC validation failure:
C:\src\dawn\Shader@0x0000029F5A330150(4,10-18): error X3000: syntax error: unexpected token 'float16_t'
C:\src\dawn\Shader@0x0000029F5A330150(5,10-18): error X3000: syntax error: unexpected token 'float16_t'
C:\src\dawn\Shader@0x0000029F5A330150(6,10-18): error X3000: syntax error: unexpected token 'float16_t'

