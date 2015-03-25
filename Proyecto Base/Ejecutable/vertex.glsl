#version 140

in vec3 aVertexPosition;
void main() {
	gl_Position = vec4( aVertexPosition.x, aVertexPosition.y, aVertexPosition.z, 1.0 );
}