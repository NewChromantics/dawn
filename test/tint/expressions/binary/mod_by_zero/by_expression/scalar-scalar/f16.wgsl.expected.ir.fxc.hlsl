SKIP: FAILED


[numthreads(1, 1, 1)]
void f() {
  float16_t a = float16_t(1.0h);
  float16_t b = float16_t(0.0h);
  float16_t v = a;
  float16_t v_1 = (b + b);
  float16_t v_2 = (v / v_1);
  float16_t v_3 = floor(v_2);
  float16_t r = ((v - (((v_2 < float16_t(0.0h))) ? (ceil(v_2)) : (v_3))) * v_1);
}

FXC validation failure:
C:\src\dawn\Shader@0x000002816ADD92D0(4,3-11): error X3000: unrecognized identifier 'float16_t'
C:\src\dawn\Shader@0x000002816ADD92D0(4,13): error X3000: unrecognized identifier 'a'

