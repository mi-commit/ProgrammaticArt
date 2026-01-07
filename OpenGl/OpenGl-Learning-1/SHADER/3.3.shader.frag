#version 330 core

out vec4 FragColor;
in	vec3 VertexColor;
in vec2 TexCoord;
in vec3 pos;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform vec4 offset;

void main()
{
	vec4 smily = texture(ourTexture2, vec2(1)-TexCoord*10. + offset.xy) 
	+ texture(ourTexture2, TexCoord * 100) 
	-texture(ourTexture2, TexCoord * 1000)
	+texture(ourTexture2, TexCoord * 9099)*vec4(.9, .89, 1, 1) ;
	FragColor = mix(texture(ourTexture, TexCoord+ offset.xy), smily, .2f) +  .1* vec4(pos,1);
};