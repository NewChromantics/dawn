SKIP: FAILED

#version 310 es
#extension GL_AMD_gpu_shader_half_float: require
precision highp float;
precision highp int;


struct frexp_result_f16 {
  float16_t fract;
  int exp;
};

void frexp_5257dd() {
  float16_t arg_0 = 1.0hf;
  frexp_result_f16 res = frexp(arg_0);
}
void main() {
  frexp_5257dd();
}
error: Error parsing GLSL shader:
ERROR: 0:14: 'frexp' : no matching overloaded function found 
ERROR: 0:14: '=' :  cannot convert from ' const float' to ' temp structure{ global float16_t fract,  global highp int exp}'
ERROR: 0:14: '' : compilation terminated 
ERROR: 3 compilation errors.  No code generated.



#version 310 es
#extension GL_AMD_gpu_shader_half_float: require


struct frexp_result_f16 {
  float16_t fract;
  int exp;
};

void frexp_5257dd() {
  float16_t arg_0 = 1.0hf;
  frexp_result_f16 res = frexp(arg_0);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  frexp_5257dd();
}
error: Error parsing GLSL shader:
ERROR: 0:12: 'frexp' : no matching overloaded function found 
ERROR: 0:12: '=' :  cannot convert from ' const float' to ' temp structure{ global float16_t fract,  global highp int exp}'
ERROR: 0:12: '' : compilation terminated 
ERROR: 3 compilation errors.  No code generated.



#version 310 es
#extension GL_AMD_gpu_shader_half_float: require


struct frexp_result_f16 {
  float16_t fract;
  int exp;
};

struct VertexOutput {
  vec4 pos;
};

void frexp_5257dd() {
  float16_t arg_0 = 1.0hf;
  frexp_result_f16 res = frexp(arg_0);
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f));
  tint_symbol.pos = vec4(0.0f);
  frexp_5257dd();
  return tint_symbol;
}
void main() {
  gl_Position = vertex_main_inner().pos;
  gl_Position[1u] = -(gl_Position.y);
  gl_Position[2u] = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
error: Error parsing GLSL shader:
ERROR: 0:16: 'frexp' : no matching overloaded function found 
ERROR: 0:16: '=' :  cannot convert from ' const float' to ' temp structure{ global float16_t fract,  global highp int exp}'
ERROR: 0:16: '' : compilation terminated 
ERROR: 3 compilation errors.  No code generated.




tint executable returned error: exit status 1
