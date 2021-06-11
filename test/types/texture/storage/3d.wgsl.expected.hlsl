Texture3D<float4> t_rgba8unorm_r : register(t0, space0);
Texture3D<float4> t_rgba8snorm_r : register(t1, space0);
Texture3D<uint4> t_rgba8uint_r : register(t2, space0);
Texture3D<int4> t_rgba8sint_r : register(t3, space0);
Texture3D<uint4> t_rgba16uint_r : register(t4, space0);
Texture3D<int4> t_rgba16sint_r : register(t5, space0);
Texture3D<float4> t_rgba16float_r : register(t6, space0);
Texture3D<uint4> t_r32uint_r : register(t7, space0);
Texture3D<int4> t_r32sint_r : register(t8, space0);
Texture3D<float4> t_r32float_r : register(t9, space0);
Texture3D<uint4> t_rg32uint_r : register(t10, space0);
Texture3D<int4> t_rg32sint_r : register(t11, space0);
Texture3D<float4> t_rg32float_r : register(t12, space0);
Texture3D<uint4> t_rgba32uint_r : register(t13, space0);
Texture3D<int4> t_rgba32sint_r : register(t14, space0);
Texture3D<float4> t_rgba32float_r : register(t15, space0);
RWTexture3D<float4> t_rgba8unorm_w : register(u50, space0);
RWTexture3D<float4> t_rgba8snorm_w : register(u51, space0);
RWTexture3D<uint4> t_rgba8uint_w : register(u52, space0);
RWTexture3D<int4> t_rgba8sint_w : register(u53, space0);
RWTexture3D<uint4> t_rgba16uint_w : register(u54, space0);
RWTexture3D<int4> t_rgba16sint_w : register(u55, space0);
RWTexture3D<float4> t_rgba16float_w : register(u56, space0);
RWTexture3D<uint4> t_r32uint_w : register(u57, space0);
RWTexture3D<int4> t_r32sint_w : register(u58, space0);
RWTexture3D<float4> t_r32float_w : register(u59, space0);
RWTexture3D<uint4> t_rg32uint_w : register(u60, space0);
RWTexture3D<int4> t_rg32sint_w : register(u61, space0);
RWTexture3D<float4> t_rg32float_w : register(u62, space0);
RWTexture3D<uint4> t_rgba32uint_w : register(u63, space0);
RWTexture3D<int4> t_rgba32sint_w : register(u64, space0);
RWTexture3D<float4> t_rgba32float_w : register(u65, space0);

[numthreads(1, 1, 1)]
void main() {
  t_rgba8unorm_r;
  t_rgba8snorm_r;
  t_rgba8uint_r;
  t_rgba8sint_r;
  t_rgba16uint_r;
  t_rgba16sint_r;
  t_rgba16float_r;
  t_r32uint_r;
  t_r32sint_r;
  t_r32float_r;
  t_rg32uint_r;
  t_rg32sint_r;
  t_rg32float_r;
  t_rgba32uint_r;
  t_rgba32sint_r;
  t_rgba32float_r;
  t_rgba8unorm_w;
  t_rgba8snorm_w;
  t_rgba8uint_w;
  t_rgba8sint_w;
  t_rgba16uint_w;
  t_rgba16sint_w;
  t_rgba16float_w;
  t_r32uint_w;
  t_r32sint_w;
  t_r32float_w;
  t_rg32uint_w;
  t_rg32sint_w;
  t_rg32float_w;
  t_rgba32uint_w;
  t_rgba32sint_w;
  t_rgba32float_w;
  return;
}

