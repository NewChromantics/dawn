SKIP: FAILED

#version 310 es
precision highp float;
precision highp int;


struct buf0 {
  float one;
};

struct main_out {
  vec4 x_GLF_color_1;
};

layout(binding = 0, std140)
uniform tint_symbol_2_1_ubo {
  buf0 tint_symbol_1;
} v;
vec4 x_GLF_color = vec4(0.0f);
layout(location = 0) out vec4 tint_symbol_loc0_Output;
void main_1() {
  float f = 0.0f;
  float x_28 = v.tint_symbol_1.one;
  f = (4.0f * (2.0f / x_28));
  float x_31 = f;
  float x_33 = f;
  if (((x_31 > 7.90000009536743164062f) & (x_33 < 8.1000003814697265625f))) {
    x_GLF_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
  } else {
    x_GLF_color = vec4(0.0f);
  }
}
main_out tint_symbol_inner() {
  main_1();
  return main_out(x_GLF_color);
}
void main() {
  tint_symbol_loc0_Output = tint_symbol_inner().x_GLF_color_1;
}
error: Error parsing GLSL shader:
ERROR: 0:26: '&' :  wrong operand types: no operation '&' exists that takes a left-hand operand of type ' temp bool' and a right operand of type ' temp bool' (or there is no acceptable conversion)
ERROR: 0:26: '' : compilation terminated 
ERROR: 2 compilation errors.  No code generated.




tint executable returned error: exit status 1
