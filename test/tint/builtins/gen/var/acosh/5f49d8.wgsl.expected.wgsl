enable f16;

fn acosh_5f49d8() {
  var arg_0 = vec2<f16>(1.0h);
  var res : vec2<f16> = acosh(arg_0);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  acosh_5f49d8();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  acosh_5f49d8();
}

@compute @workgroup_size(1)
fn compute_main() {
  acosh_5f49d8();
}
