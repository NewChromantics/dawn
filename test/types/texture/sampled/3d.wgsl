[[group(0), binding(0)]] var t_f : texture_3d<f32>;
[[group(0), binding(1)]] var t_i : texture_3d<i32>;
[[group(0), binding(2)]] var t_u : texture_3d<u32>;

[[stage(compute)]]
fn main() {
  ignore(t_f);
  ignore(t_i);
  ignore(t_u);
}
