#version 310 es

void v() {
  int i = 0;
  i = (i + 1);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
}
