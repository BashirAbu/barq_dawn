enable f16;

fn distance_892a5d() {
  var res : f16 = distance(vec2<f16>(1.0h), vec2<f16>(1.0h));
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  distance_892a5d();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  distance_892a5d();
}

@compute @workgroup_size(1)
fn compute_main() {
  distance_892a5d();
}
