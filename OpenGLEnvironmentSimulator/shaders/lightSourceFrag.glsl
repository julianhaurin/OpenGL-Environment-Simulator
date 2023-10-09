
// Fragment shader for light source

#version 430

// output frag color
out vec4 out_FragColor;

void main()
{
	
	const vec3 color = vec3(0.9f, 0.9f, 0.9f);
	out_FragColor = vec4(color, 1.0f);

}