SKIP: FAILED

#version 310 es
precision highp float;
precision highp int;

void select_e381c3() {
  bool arg_2 = true;
  bool v = arg_2;
  int v_1 = ((v.x) ? (ivec4(1).x) : (ivec4(1).x));
  int v_2 = ((v.y) ? (ivec4(1).y) : (ivec4(1).y));
  int v_3 = ((v.z) ? (ivec4(1).z) : (ivec4(1).z));
  ivec4 res = ivec4(v_1, v_2, v_3, ((v.w) ? (ivec4(1).w) : (ivec4(1).w)));
}
void main() {
  select_e381c3();
}
error: Error parsing GLSL shader:
ERROR: 0:8: 'scalar swizzle' : not supported with this profile: es
ERROR: 0:8: '' : compilation terminated 
ERROR: 2 compilation errors.  No code generated.



#version 310 es

void select_e381c3() {
  bool arg_2 = true;
  bool v = arg_2;
  int v_1 = ((v.x) ? (ivec4(1).x) : (ivec4(1).x));
  int v_2 = ((v.y) ? (ivec4(1).y) : (ivec4(1).y));
  int v_3 = ((v.z) ? (ivec4(1).z) : (ivec4(1).z));
  ivec4 res = ivec4(v_1, v_2, v_3, ((v.w) ? (ivec4(1).w) : (ivec4(1).w)));
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  select_e381c3();
}
error: Error parsing GLSL shader:
ERROR: 0:6: 'scalar swizzle' : not supported with this profile: es
ERROR: 0:6: '' : compilation terminated 
ERROR: 2 compilation errors.  No code generated.



#version 310 es


struct VertexOutput {
  vec4 pos;
};

void select_e381c3() {
  bool arg_2 = true;
  bool v = arg_2;
  int v_1 = ((v.x) ? (ivec4(1).x) : (ivec4(1).x));
  int v_2 = ((v.y) ? (ivec4(1).y) : (ivec4(1).y));
  int v_3 = ((v.z) ? (ivec4(1).z) : (ivec4(1).z));
  ivec4 res = ivec4(v_1, v_2, v_3, ((v.w) ? (ivec4(1).w) : (ivec4(1).w)));
}
VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = VertexOutput(vec4(0.0f));
  tint_symbol.pos = vec4(0.0f);
  select_e381c3();
  return tint_symbol;
}
void main() {
  gl_Position = vertex_main_inner().pos;
  gl_Position[1u] = -(gl_Position.y);
  gl_Position[2u] = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
error: Error parsing GLSL shader:
ERROR: 0:11: 'scalar swizzle' : not supported with this profile: es
ERROR: 0:11: '' : compilation terminated 
ERROR: 2 compilation errors.  No code generated.




tint executable returned error: exit status 1
