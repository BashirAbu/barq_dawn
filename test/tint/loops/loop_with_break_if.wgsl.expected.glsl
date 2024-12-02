#version 310 es

int f() {
  int i = 0;
  {
    uvec2 tint_loop_idx = uvec2(0u);
    while(true) {
      if (all(equal(tint_loop_idx, uvec2(4294967295u)))) {
        break;
      }
      if ((i > 4)) {
        return i;
      }
      {
        uint tint_low_inc = (tint_loop_idx.x + 1u);
        tint_loop_idx.x = tint_low_inc;
        uint tint_carry = uint((tint_low_inc == 0u));
        tint_loop_idx.y = (tint_loop_idx.y + tint_carry);
        i = (i + 1);
        if ((i == 4)) { break; }
      }
      continue;
    }
  }
  return i;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
}
