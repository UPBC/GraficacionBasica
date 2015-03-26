#version 140

attribute vec3 aVertexPosition;
attribute vec2 aTextureCoord;

varying vec2 vTextureCoord;


void main() {
	vTextureCoord = aTextureCoord;
	gl_Position = vec4( aVertexPosition.x, aVertexPosition.y, aVertexPosition.z, 1.0 );
}
