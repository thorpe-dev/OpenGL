#version 300
in vec3 coord3d;
uniform vec4 light0Color;
uniform mat4 mvp;

void main(void) {
  gl_Position = vec4(coord3d, 1.0);
}