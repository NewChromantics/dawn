[[group(0), binding(0)]] var t_f : texture_2d_array<f32>;

[[group(0), binding(1)]] var t_i : texture_2d_array<i32>;

[[group(0), binding(2)]] var t_u : texture_2d_array<u32>;

[[stage(compute)]]
fn main() {
  ignore(t_f);
  ignore(t_i);
  ignore(t_u);
}
