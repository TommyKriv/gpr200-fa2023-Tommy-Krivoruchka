#version 450
//defaultLit.vert
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

//This entire block will be passed to our fragment shader.
out Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	vs_out.UV = vUV;
	vs_out.WorldPosition = vec3(_Model * vec4(vPos, 1.0));
	vs_out.WorldNormal =  vec3(_Model * vec4(vNormal, 1.0));
	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}
