SKIP: FAILED


enable f16;

struct Inner {
  @size(64)
  m : mat3x3<f16>,
}

struct Outer {
  a : array<Inner, 4>,
}

@group(0) @binding(0) var<uniform> a : array<Outer, 4>;

var<private> counter = 0;

fn i() -> i32 {
  counter++;
  return counter;
}

@compute @workgroup_size(1)
fn f() {
  let p_a = &(a);
  let p_a_i = &((*(p_a))[i()]);
  let p_a_i_a = &((*(p_a_i)).a);
  let p_a_i_a_i = &((*(p_a_i_a))[i()]);
  let p_a_i_a_i_m = &((*(p_a_i_a_i)).m);
  let p_a_i_a_i_m_i = &((*(p_a_i_a_i_m))[i()]);
  let l_a : array<Outer, 4> = *(p_a);
  let l_a_i : Outer = *(p_a_i);
  let l_a_i_a : array<Inner, 4> = *(p_a_i_a);
  let l_a_i_a_i : Inner = *(p_a_i_a_i);
  let l_a_i_a_i_m : mat3x3<f16> = *(p_a_i_a_i_m);
  let l_a_i_a_i_m_i : vec3<f16> = *(p_a_i_a_i_m_i);
  let l_a_i_a_i_m_i_i : f16 = (*(p_a_i_a_i_m_i))[i()];
}

Failed to generate: :55:5 error: binary: no matching overload for 'operator * (i32, u32)'

9 candidate operators:
 • 'operator * (T  ✓ , T  ✗ ) -> T' where:
      ✓  'T' is 'f32', 'i32', 'u32' or 'f16'
 • 'operator * (vecN<T>  ✗ , T  ✓ ) -> vecN<T>' where:
      ✓  'T' is 'f32', 'i32', 'u32' or 'f16'
 • 'operator * (T  ✓ , vecN<T>  ✗ ) -> vecN<T>' where:
      ✓  'T' is 'f32', 'i32', 'u32' or 'f16'
 • 'operator * (T  ✗ , matNxM<T>  ✗ ) -> matNxM<T>' where:
      ✗  'T' is 'f32' or 'f16'
 • 'operator * (matNxM<T>  ✗ , T  ✗ ) -> matNxM<T>' where:
      ✗  'T' is 'f32' or 'f16'
 • 'operator * (vecN<T>  ✗ , vecN<T>  ✗ ) -> vecN<T>' where:
      ✗  'T' is 'f32', 'i32', 'u32' or 'f16'
 • 'operator * (matCxR<T>  ✗ , vecC<T>  ✗ ) -> vecR<T>' where:
      ✗  'T' is 'f32' or 'f16'
 • 'operator * (vecR<T>  ✗ , matCxR<T>  ✗ ) -> vecC<T>' where:
      ✗  'T' is 'f32' or 'f16'
 • 'operator * (matKxR<T>  ✗ , matCxK<T>  ✗ ) -> matCxR<T>' where:
      ✗  'T' is 'f32' or 'f16'

    %43:u32 = mul %42, 2u
    ^^^^^^^^^^^^^^^^^^^^^

:24:3 note: in block
  $B3: {
  ^^^

note: # Disassembly
Inner = struct @align(8) {
  m:mat3x3<f16> @offset(0)
}

Outer = struct @align(8) {
  a:array<Inner, 4> @offset(0)
}

$B1: {  # root
  %a:ptr<uniform, array<vec4<u32>, 64>, read> = var @binding_point(0, 0)
  %counter:ptr<private, i32, read_write> = var, 0i
}

%i = func():i32 {
  $B2: {
    %4:i32 = load %counter
    %5:i32 = add %4, 1i
    store %counter, %5
    %6:i32 = load %counter
    ret %6
  }
}
%f = @compute @workgroup_size(1, 1, 1) func():void {
  $B3: {
    %8:i32 = call %i
    %9:u32 = convert %8
    %10:u32 = mul 256u, %9
    %11:i32 = call %i
    %12:u32 = convert %11
    %13:u32 = mul 64u, %12
    %14:i32 = call %i
    %15:u32 = convert %14
    %16:u32 = mul 8u, %15
    %17:array<Outer, 4> = call %18, 0u
    %l_a:array<Outer, 4> = let %17
    %20:Outer = call %21, %10
    %l_a_i:Outer = let %20
    %23:array<Inner, 4> = call %24, %10
    %l_a_i_a:array<Inner, 4> = let %23
    %26:u32 = add %10, %13
    %27:Inner = call %28, %26
    %l_a_i_a_i:Inner = let %27
    %30:u32 = add %10, %13
    %31:mat3x3<f16> = call %32, %30
    %l_a_i_a_i_m:mat3x3<f16> = let %31
    %34:u32 = add %10, %13
    %35:u32 = add %34, %16
    %36:u32 = div %35, 16u
    %37:ptr<uniform, vec4<u32>, read> = access %a, %36
    %38:vec4<u32> = load %37
    %39:vec4<f16> = bitcast %38
    %40:vec3<f16> = swizzle %39, xyz
    %l_a_i_a_i_m_i:vec3<f16> = let %40
    %42:i32 = call %i
    %43:u32 = mul %42, 2u
    %44:u32 = add %10, %13
    %45:u32 = add %44, %16
    %46:u32 = add %45, %43
    %47:u32 = div %46, 16u
    %48:ptr<uniform, vec4<u32>, read> = access %a, %47
    %49:u32 = mod %46, 16u
    %50:u32 = div %49, 4u
    %51:u32 = load_vector_element %48, %50
    %52:u32 = mod %46, 4u
    %53:bool = eq %52, 0u
    %54:u32 = hlsl.ternary 16u, 0u, %53
    %55:u32 = shr %51, %54
    %56:f32 = hlsl.f16tof32 %55
    %57:f16 = convert %56
    %l_a_i_a_i_m_i_i:f16 = let %57
    ret
  }
}
%28 = func(%start_byte_offset:u32):Inner {
  $B4: {
    %60:mat3x3<f16> = call %32, %start_byte_offset
    %61:Inner = construct %60
    ret %61
  }
}
%32 = func(%start_byte_offset_1:u32):mat3x3<f16> {  # %start_byte_offset_1: 'start_byte_offset'
  $B5: {
    %63:u32 = div %start_byte_offset_1, 16u
    %64:ptr<uniform, vec4<u32>, read> = access %a, %63
    %65:vec4<u32> = load %64
    %66:vec4<f16> = bitcast %65
    %67:vec3<f16> = swizzle %66, xyz
    %68:u32 = add 8u, %start_byte_offset_1
    %69:u32 = div %68, 16u
    %70:ptr<uniform, vec4<u32>, read> = access %a, %69
    %71:vec4<u32> = load %70
    %72:vec4<f16> = bitcast %71
    %73:vec3<f16> = swizzle %72, xyz
    %74:u32 = add 16u, %start_byte_offset_1
    %75:u32 = div %74, 16u
    %76:ptr<uniform, vec4<u32>, read> = access %a, %75
    %77:vec4<u32> = load %76
    %78:vec4<f16> = bitcast %77
    %79:vec3<f16> = swizzle %78, xyz
    %80:mat3x3<f16> = construct %67, %73, %79
    ret %80
  }
}
%24 = func(%start_byte_offset_2:u32):array<Inner, 4> {  # %start_byte_offset_2: 'start_byte_offset'
  $B6: {
    %a_1:ptr<function, array<Inner, 4>, read_write> = var, array<Inner, 4>(Inner(mat3x3<f16>(vec3<f16>(0.0h))))  # %a_1: 'a'
    loop [i: $B7, b: $B8, c: $B9] {  # loop_1
      $B7: {  # initializer
        next_iteration 0u  # -> $B8
      }
      $B8 (%idx:u32): {  # body
        %84:bool = gte %idx, 4u
        if %84 [t: $B10] {  # if_1
          $B10: {  # true
            exit_loop  # loop_1
          }
        }
        %85:u32 = mul %idx, 64u
        %86:u32 = add %start_byte_offset_2, %85
        %87:ptr<function, Inner, read_write> = access %a_1, %idx
        %88:Inner = call %28, %86
        store %87, %88
        continue  # -> $B9
      }
      $B9: {  # continuing
        %89:u32 = add %idx, 1u
        next_iteration %89  # -> $B8
      }
    }
    %90:array<Inner, 4> = load %a_1
    ret %90
  }
}
%21 = func(%start_byte_offset_3:u32):Outer {  # %start_byte_offset_3: 'start_byte_offset'
  $B11: {
    %92:array<Inner, 4> = call %24, %start_byte_offset_3
    %93:Outer = construct %92
    ret %93
  }
}
%18 = func(%start_byte_offset_4:u32):array<Outer, 4> {  # %start_byte_offset_4: 'start_byte_offset'
  $B12: {
    %a_2:ptr<function, array<Outer, 4>, read_write> = var, array<Outer, 4>(Outer(array<Inner, 4>(Inner(mat3x3<f16>(vec3<f16>(0.0h))))))  # %a_2: 'a'
    loop [i: $B13, b: $B14, c: $B15] {  # loop_2
      $B13: {  # initializer
        next_iteration 0u  # -> $B14
      }
      $B14 (%idx_1:u32): {  # body
        %97:bool = gte %idx_1, 4u
        if %97 [t: $B16] {  # if_2
          $B16: {  # true
            exit_loop  # loop_2
          }
        }
        %98:u32 = mul %idx_1, 256u
        %99:u32 = add %start_byte_offset_4, %98
        %100:ptr<function, Outer, read_write> = access %a_2, %idx_1
        %101:Outer = call %21, %99
        store %100, %101
        continue  # -> $B15
      }
      $B15: {  # continuing
        %102:u32 = add %idx_1, 1u
        next_iteration %102  # -> $B14
      }
    }
    %103:array<Outer, 4> = load %a_2
    ret %103
  }
}

