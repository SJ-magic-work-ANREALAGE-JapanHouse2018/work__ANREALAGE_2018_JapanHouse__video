#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform int b_Rev = 0;

uniform sampler2DRect texture0;
uniform sampler2DRect texture1;
uniform sampler2DRect texture_mask;

void main(){
	/********************
	********************/
	vec2 pos = gl_TexCoord[0].xy;
	
	vec4 color0 = texture2DRect(texture0, pos);
	vec4 color1 = texture2DRect(texture1, pos);
	vec4 mask = texture2DRect(texture_mask, pos);
	
	vec4 color;
	if(b_Rev == 0){
		color.r = (mask.r) * color0.r + (1 - mask.r) * color1.r;
		color.g = (mask.g) * color0.g + (1 - mask.g) * color1.g;
		color.b = (mask.b) * color0.b + (1 - mask.b) * color1.b;
	}else{
		color.r = (mask.r) * color1.r + (1 - mask.r) * color0.r;
		color.g = (mask.g) * color1.g + (1 - mask.g) * color0.g;
		color.b = (mask.b) * color1.b + (1 - mask.b) * color0.b;
	}
	color.a = 1.0;
	
	gl_FragColor = color;
}
