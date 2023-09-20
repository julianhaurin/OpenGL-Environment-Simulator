// OpenGL Environment Simulator - Julian Haurin - 8/17/23

// Notes:
// version might not be right in shaders
// lots of printing good for debugging - remove in release version
// add override keyword bum
// 
// research better error handling practices?
// get printOpenGLErrors() to util file and use elsewhere
// in compute shader class rn
// 

#include "Renderer.h"

int main() 
{

	Renderer Monke = Renderer();
	Monke.Run();
	return EXIT_SUCCESS;

}
