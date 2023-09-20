
// Ocean computer shader

#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in; // invocations per work group
//layout(std430, binding = 0) buffer oceanData 
//{
	// same as ocean vertex data in Ocean.h
//	GLfloat x, y, z; // current position
//	GLfloat ox, oy, oz; // original position (for offset calc)
//	GLfloat nx, ny, nz; // normal
//	GLfloat a, b, c; // htilde
//	GLfloat _a, _b, _c; // discrete fourier transform //

//}
// layout(rgba32f, binding = 0) uniform image2D screen; // type of data container

void main()
{


}