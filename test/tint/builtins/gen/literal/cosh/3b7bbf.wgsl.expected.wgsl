enable f16;

fn cosh_3b7bbf() {
  var res : vec4<f16> = cosh(vec4<f16>(1.0h));
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  cosh_3b7bbf();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  cosh_3b7bbf();
}

@compute @workgroup_size(1)
fn compute_main() {
  cosh_3b7bbf();
}
