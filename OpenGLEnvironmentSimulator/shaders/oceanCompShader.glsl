
// Ocean computer shader

#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in; // invocations per work group
layout(std430, binding = 0) buffer ocean_data 
{
	// same as ocean vertex data in Ocean.h
	float pos[3]; // current position: x, y, z
	float opos[3]; // original position (for offset calc): ox, oy, oz
	float norm[3]; // normal: nx, ny, nz
	float htilde[3]; // htilde: a, b, c
	float htilde0[3]; // discrete fourier transform: _a, _b, _c

};
// layout(rgba32f, binding = 0) uniform image2D screen; // type of data container

void main()
{


}