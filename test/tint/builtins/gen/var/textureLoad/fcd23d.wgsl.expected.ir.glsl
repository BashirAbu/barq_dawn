SKIP: FAILED


@group(0) @binding(0) var<storage, read_write> prevent_dce : f32;

@group(1) @binding(0) var arg_0 : texture_depth_multisampled_2d;

fn textureLoad_fcd23d() -> f32 {
  var arg_1 = vec2<u32>(1u);
  var arg_2 = 1i;
  var res : f32 = textureLoad(arg_0, arg_1, arg_2);
  return res;
}

struct VertexOutput {
  @builtin(position)
  pos : vec4<f32>,
  @location(0) @interpolate(flat)
  prevent_dce : f32,
}

@fragment
fn fragment_main() {
  prevent_dce = textureLoad_fcd23d();
}

Failed to generate: :19:15 error: glsl.texelFetch: no matching call to 'glsl.texelFetch(texture_depth_multisampled_2d, vec2<i32>, i32)'

4 candidate functions:
 • 'glsl.texelFetch(texture: texture_2d<T>  ✗ , location: vec2<i32>  ✓ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_multisampled_2d<T>  ✗ , location: vec2<i32>  ✓ , sample_index: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_2d_array<T>  ✗ , location: vec3<i32>  ✗ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_3d<T>  ✗ , location: vec3<i32>  ✗ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'

    %11:f32 = glsl.texelFetch %8, %9, %10
              ^^^^^^^^^^^^^^^

:11:3 note: in block
  $B2: {
  ^^^

note: # Disassembly
tint_symbol_1 = struct @align(4), @block {
  tint_symbol:f32 @offset(0)
}

$B1: {  # root
  %1:ptr<storage, tint_symbol_1, read_write> = var @binding_point(0, 0)
  %arg_0:ptr<handle, texture_depth_multisampled_2d, read> = var
}

%textureLoad_fcd23d = func():f32 {
  $B2: {
    %arg_1:ptr<function, vec2<u32>, read_write> = var, vec2<u32>(1u)
    %arg_2:ptr<function, i32, read_write> = var, 1i
    %6:vec2<u32> = load %arg_1
    %7:i32 = load %arg_2
    %8:texture_depth_multisampled_2d = load %arg_0
    %9:vec2<i32> = convert %6
    %10:i32 = convert %7
    %11:f32 = glsl.texelFetch %8, %9, %10
    %res:ptr<function, f32, read_write> = var, %11
    %13:f32 = load %res
    ret %13
  }
}
%fragment_main = @fragment func():void {
  $B3: {
    %15:f32 = call %textureLoad_fcd23d
    %16:ptr<storage, f32, read_write> = access %1, 0u
    store %16, %15
    ret
  }
}


@group(0) @binding(0) var<storage, read_write> prevent_dce : f32;

@group(1) @binding(0) var arg_0 : texture_depth_multisampled_2d;

fn textureLoad_fcd23d() -> f32 {
  var arg_1 = vec2<u32>(1u);
  var arg_2 = 1i;
  var res : f32 = textureLoad(arg_0, arg_1, arg_2);
  return res;
}

struct VertexOutput {
  @builtin(position)
  pos : vec4<f32>,
  @location(0) @interpolate(flat)
  prevent_dce : f32,
}

@compute @workgroup_size(1)
fn compute_main() {
  prevent_dce = textureLoad_fcd23d();
}

Failed to generate: :19:15 error: glsl.texelFetch: no matching call to 'glsl.texelFetch(texture_depth_multisampled_2d, vec2<i32>, i32)'

4 candidate functions:
 • 'glsl.texelFetch(texture: texture_2d<T>  ✗ , location: vec2<i32>  ✓ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_multisampled_2d<T>  ✗ , location: vec2<i32>  ✓ , sample_index: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_2d_array<T>  ✗ , location: vec3<i32>  ✗ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_3d<T>  ✗ , location: vec3<i32>  ✗ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'

    %11:f32 = glsl.texelFetch %8, %9, %10
              ^^^^^^^^^^^^^^^

:11:3 note: in block
  $B2: {
  ^^^

note: # Disassembly
tint_symbol_1 = struct @align(4), @block {
  tint_symbol:f32 @offset(0)
}

$B1: {  # root
  %1:ptr<storage, tint_symbol_1, read_write> = var @binding_point(0, 0)
  %arg_0:ptr<handle, texture_depth_multisampled_2d, read> = var
}

%textureLoad_fcd23d = func():f32 {
  $B2: {
    %arg_1:ptr<function, vec2<u32>, read_write> = var, vec2<u32>(1u)
    %arg_2:ptr<function, i32, read_write> = var, 1i
    %6:vec2<u32> = load %arg_1
    %7:i32 = load %arg_2
    %8:texture_depth_multisampled_2d = load %arg_0
    %9:vec2<i32> = convert %6
    %10:i32 = convert %7
    %11:f32 = glsl.texelFetch %8, %9, %10
    %res:ptr<function, f32, read_write> = var, %11
    %13:f32 = load %res
    ret %13
  }
}
%compute_main = @compute @workgroup_size(1, 1, 1) func():void {
  $B3: {
    %15:f32 = call %textureLoad_fcd23d
    %16:ptr<storage, f32, read_write> = access %1, 0u
    store %16, %15
    ret
  }
}


@group(1) @binding(0) var arg_0 : texture_depth_multisampled_2d;

fn textureLoad_fcd23d() -> f32 {
  var arg_1 = vec2<u32>(1u);
  var arg_2 = 1i;
  var res : f32 = textureLoad(arg_0, arg_1, arg_2);
  return res;
}

struct VertexOutput {
  @builtin(position)
  pos : vec4<f32>,
  @location(0) @interpolate(flat)
  prevent_dce : f32,
}

@vertex
fn vertex_main() -> VertexOutput {
  var tint_symbol : VertexOutput;
  tint_symbol.pos = vec4<f32>();
  tint_symbol.prevent_dce = textureLoad_fcd23d();
  return tint_symbol;
}

Failed to generate: :19:15 error: glsl.texelFetch: no matching call to 'glsl.texelFetch(texture_depth_multisampled_2d, vec2<i32>, i32)'

4 candidate functions:
 • 'glsl.texelFetch(texture: texture_2d<T>  ✗ , location: vec2<i32>  ✓ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_multisampled_2d<T>  ✗ , location: vec2<i32>  ✓ , sample_index: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_2d_array<T>  ✗ , location: vec3<i32>  ✗ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'
 • 'glsl.texelFetch(texture: texture_3d<T>  ✗ , location: vec3<i32>  ✗ , level: i32  ✓ ) -> vec4<T>' where:
      ✗  'T' is 'f32', 'i32' or 'u32'

    %10:f32 = glsl.texelFetch %7, %8, %9
              ^^^^^^^^^^^^^^^

:11:3 note: in block
  $B2: {
  ^^^

note: # Disassembly
VertexOutput = struct @align(16) {
  pos:vec4<f32> @offset(0), @builtin(position)
  prevent_dce:f32 @offset(16), @location(0), @interpolate(flat)
}

$B1: {  # root
  %arg_0:ptr<handle, texture_depth_multisampled_2d, read> = var
}

%textureLoad_fcd23d = func():f32 {
  $B2: {
    %arg_1:ptr<function, vec2<u32>, read_write> = var, vec2<u32>(1u)
    %arg_2:ptr<function, i32, read_write> = var, 1i
    %5:vec2<u32> = load %arg_1
    %6:i32 = load %arg_2
    %7:texture_depth_multisampled_2d = load %arg_0
    %8:vec2<i32> = convert %5
    %9:i32 = convert %6
    %10:f32 = glsl.texelFetch %7, %8, %9
    %res:ptr<function, f32, read_write> = var, %10
    %12:f32 = load %res
    ret %12
  }
}
%vertex_main = @vertex func():VertexOutput {
  $B3: {
    %tint_symbol:ptr<function, VertexOutput, read_write> = var
    %15:ptr<function, vec4<f32>, read_write> = access %tint_symbol, 0u
    store %15, vec4<f32>(0.0f)
    %16:ptr<function, f32, read_write> = access %tint_symbol, 1u
    %17:f32 = call %textureLoad_fcd23d
    store %16, %17
    %18:VertexOutput = load %tint_symbol
    ret %18
  }
}


tint executable returned error: exit status 1
