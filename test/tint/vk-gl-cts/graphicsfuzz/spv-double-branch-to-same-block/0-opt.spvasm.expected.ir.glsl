SKIP: FAILED

#version 310 es
precision highp float;
precision highp int;


struct buf0 {
  vec2 injectionSwitch;
};

struct main_out {
  vec4 x_GLF_color_1;
};

int data[10] = int[10](0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
int temp[10] = int[10](0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
layout(binding = 0, std140)
uniform tint_symbol_4_1_ubo {
  buf0 tint_symbol_3;
} v;
vec4 tint_symbol = vec4(0.0f);
vec4 x_GLF_color = vec4(0.0f);
bool continue_execution = true;
layout(location = 0) out vec4 tint_symbol_1_loc0_Output;
void merge_i1_i1_i1_(inout int f, inout int mid, inout int to) {
  int k = 0;
  int i = 0;
  int j = 0;
  int i_1 = 0;
  k = f;
  i = f;
  j = (mid + 1);
  {
    while(true) {
      if (((i <= mid) & (j <= to))) {
      } else {
        break;
      }
      if ((data[i] < data[j])) {
        int x_280 = k;
        k = (k + 1);
        int x_282 = i;
        i = (i + 1);
        temp[x_280] = data[x_282];
      } else {
        int x_287 = k;
        k = (k + 1);
        int x_289 = j;
        j = (j + 1);
        temp[x_287] = data[x_289];
      }
      {
      }
      continue;
    }
  }
  {
    while(true) {
      if (((i < 10) & (i <= mid))) {
      } else {
        break;
      }
      int x_305 = k;
      k = (k + 1);
      int x_307 = i;
      i = (i + 1);
      temp[x_305] = data[x_307];
      {
      }
      continue;
    }
  }
  i_1 = f;
  {
    while(true) {
      if ((i_1 <= to)) {
      } else {
        break;
      }
      int x_321 = i_1;
      data[x_321] = temp[i_1];
      {
        i_1 = (i_1 + 1);
      }
      continue;
    }
  }
}
void mergeSort_() {
  int low = 0;
  int high = 0;
  int m = 0;
  int i_2 = 0;
  int f_1 = 0;
  int mid_1 = 0;
  int to_1 = 0;
  int param = 0;
  int param_1 = 0;
  int param_2 = 0;
  low = 0;
  high = 9;
  m = 1;
  {
    while(true) {
      if ((m <= high)) {
      } else {
        break;
      }
      i_2 = low;
      {
        while(true) {
          if ((i_2 < high)) {
          } else {
            break;
          }
          f_1 = i_2;
          mid_1 = ((i_2 + m) - 1);
          to_1 = min(((i_2 + (2 * m)) - 1), high);
          param = f_1;
          param_1 = mid_1;
          param_2 = to_1;
          merge_i1_i1_i1_(param, param_1, param_2);
          {
            i_2 = (i_2 + (2 * m));
          }
          continue;
        }
      }
      {
        m = (2 * m);
      }
      continue;
    }
  }
}
int tint_f32_to_i32(float value) {
  return (((value <= 2147483520.0f)) ? ((((value >= -2147483648.0f)) ? (int(value)) : ((-2147483647 - 1)))) : (2147483647));
}
void main_1() {
  int i_3 = 0;
  int j_1 = 0;
  float grey = 0.0f;
  i_3 = tint_f32_to_i32(v.tint_symbol_3.injectionSwitch.x);
  {
    while(true) {
      int x_93 = i_3;
      switch(x_93) {
        case 9:
        {
          int x_123 = i_3;
          data[x_123] = -5;
          break;
        }
        case 8:
        {
          int x_121 = i_3;
          data[x_121] = -4;
          break;
        }
        case 7:
        {
          int x_119 = i_3;
          data[x_119] = -3;
          break;
        }
        case 6:
        {
          int x_117 = i_3;
          data[x_117] = -2;
          break;
        }
        case 5:
        {
          int x_115 = i_3;
          data[x_115] = -1;
          break;
        }
        case 4:
        {
          int x_113 = i_3;
          data[x_113] = 0;
          break;
        }
        case 3:
        {
          int x_111 = i_3;
          data[x_111] = 1;
          break;
        }
        case 2:
        {
          int x_109 = i_3;
          data[x_109] = 2;
          break;
        }
        case 1:
        {
          int x_107 = i_3;
          data[x_107] = 3;
          break;
        }
        case 0:
        {
          int x_105 = i_3;
          data[x_105] = 4;
          break;
        }
        default:
        {
          break;
        }
      }
      i_3 = (i_3 + 1);
      {
        int x_127 = i_3;
        if (!((x_127 < 10))) { break; }
      }
      continue;
    }
  }
  j_1 = 0;
  {
    while(true) {
      if ((j_1 < 10)) {
      } else {
        break;
      }
      int x_136 = j_1;
      temp[x_136] = data[j_1];
      {
        j_1 = (j_1 + 1);
      }
      continue;
    }
  }
  mergeSort_();
  if ((tint_f32_to_i32(tint_symbol.y) < 30)) {
    grey = (0.5f + (float(data[0]) / 10.0f));
  } else {
    if ((tint_f32_to_i32(tint_symbol.y) < 60)) {
      grey = (0.5f + (float(data[1]) / 10.0f));
    } else {
      if ((tint_f32_to_i32(tint_symbol.y) < 90)) {
        grey = (0.5f + (float(data[2]) / 10.0f));
      } else {
        if ((tint_f32_to_i32(tint_symbol.y) < 120)) {
          grey = (0.5f + (float(data[3]) / 10.0f));
        } else {
          if ((tint_f32_to_i32(tint_symbol.y) < 150)) {
            continue_execution = false;
          } else {
            if ((tint_f32_to_i32(tint_symbol.y) < 180)) {
              grey = (0.5f + (float(data[5]) / 10.0f));
            } else {
              if ((tint_f32_to_i32(tint_symbol.y) < 210)) {
                grey = (0.5f + (float(data[6]) / 10.0f));
              } else {
                if ((tint_f32_to_i32(tint_symbol.y) < 240)) {
                  grey = (0.5f + (float(data[7]) / 10.0f));
                } else {
                  if ((tint_f32_to_i32(tint_symbol.y) < 270)) {
                    grey = (0.5f + (float(data[8]) / 10.0f));
                  } else {
                    continue_execution = false;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  vec3 x_248 = vec3(grey);
  x_GLF_color = vec4(x_248[0u], x_248[1u], x_248[2u], 1.0f);
}
main_out tint_symbol_1_inner(vec4 tint_symbol_2) {
  tint_symbol = tint_symbol_2;
  main_1();
  main_out v_1 = main_out(x_GLF_color);
  if (!(continue_execution)) {
    discard;
  }
  return v_1;
}
void main() {
  tint_symbol_1_loc0_Output = tint_symbol_1_inner(gl_FragCoord).x_GLF_color_1;
}
error: Error parsing GLSL shader:
ERROR: 0:34: '&' :  wrong operand types: no operation '&' exists that takes a left-hand operand of type ' temp bool' and a right operand of type ' temp bool' (or there is no acceptable conversion)
ERROR: 0:34: '' : compilation terminated 
ERROR: 2 compilation errors.  No code generated.




tint executable returned error: exit status 1
