SKIP: FAILED


[numthreads(1, 1, 1)]
void f() {
  float16_t a = float16_t(1.0h);
  float16_t b = a;
}

FXC validation failure:
C:\src\dawn\Shader@0x000001E096A1ABC0(4,3-11): error X3000: unrecognized identifier 'float16_t'
C:\src\dawn\Shader@0x000001E096A1ABC0(4,13): error X3000: unrecognized identifier 'a'

