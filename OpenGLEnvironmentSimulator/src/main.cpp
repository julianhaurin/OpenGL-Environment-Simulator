// OpenGL Environment Simulator - Julian Haurin - 8/17/23

// Notes:
// version might not be right in shaders
// lots of printing good for debugging - remove in release version
// add override keyword bum
// 
// research better error handling practices?
// get printOpenGLErrors() to util file and use elsewhere
// in compute shader class rn
// fix order of member init lists EVERYWHERE
// had to use pointer for ocean class in renderer - change?
// std::cerr instead of std::cout?
// commented out a bunch of code in render function of ocean class - still works but idk lol

#include "Renderer.h"

int main() 
{

	Renderer Monke = Renderer();
	Monke.Run();
	return EXIT_SUCCESS;

}
