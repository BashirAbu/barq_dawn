enable subgroups;

@group(0) @binding(0) var<storage, read_write> prevent_dce : vec3<f32>;

fn quadSwapY_d1ab4d() -> vec3<f32> {
  var arg_0 = vec3<f32>(1.0f);
  var res : vec3<f32> = quadSwapY(arg_0);
  return res;
}

@fragment
fn fragment_main() {
  prevent_dce = quadSwapY_d1ab4d();
}

@compute @workgroup_size(1)
fn compute_main() {
  prevent_dce = quadSwapY_d1ab4d();
}
