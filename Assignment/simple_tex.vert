#version 400

layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn; // vertex normal
layout (location = 2) in vec2 vt; // per-vertex texture co-ords

uniform mat4 P; // projection matrix
uniform mat4 V; // view matrix
uniform mat4 M; // model matrix

out vec3 normal;
out vec2 texture_coordinates;

void main() {
	texture_coordinates = vt;
	normal = vn;
	gl_Position = P * V * M * vec4 (vp, 1.0);
}
