#version 310 es


struct S {
  int before;
  uint tint_pad;
  uint tint_pad_1;
  uint tint_pad_2;
  mat4 m;
  uint tint_pad_3;
  uint tint_pad_4;
  uint tint_pad_5;
  uint tint_pad_6;
  uint tint_pad_7;
  uint tint_pad_8;
  uint tint_pad_9;
  uint tint_pad_10;
  uint tint_pad_11;
  uint tint_pad_12;
  uint tint_pad_13;
  uint tint_pad_14;
  int after;
  uint tint_pad_15;
  uint tint_pad_16;
  uint tint_pad_17;
  uint tint_pad_18;
  uint tint_pad_19;
  uint tint_pad_20;
  uint tint_pad_21;
  uint tint_pad_22;
  uint tint_pad_23;
  uint tint_pad_24;
  uint tint_pad_25;
  uint tint_pad_26;
  uint tint_pad_27;
  uint tint_pad_28;
  uint tint_pad_29;
};

layout(binding = 0, std140)
uniform tint_symbol_1_1_ubo {
  S tint_symbol[4];
} v;
shared S w[4];
void f_inner(uint tint_local_index) {
  {
    uint v_1 = 0u;
    v_1 = tint_local_index;
    while(true) {
      uint v_2 = v_1;
      if ((v_2 >= 4u)) {
        break;
      }
      w[v_2] = S(0, 0u, 0u, 0u, mat4(vec4(0.0f), vec4(0.0f), vec4(0.0f), vec4(0.0f)), 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      {
        v_1 = (v_2 + 1u);
      }
      continue;
    }
  }
  barrier();
  w = v.tint_symbol;
  w[1] = v.tint_symbol[2];
  w[3].m = v.tint_symbol[2].m;
  w[1].m[0] = v.tint_symbol[0].m[1].ywxz;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  f_inner(gl_LocalInvocationIndex);
}
