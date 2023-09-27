// OpenGL Environment Simulator - Julian Haurin

// Sources:
// learnopengl.com
// cherno
// others on github

// Notes:
// version might not be right in shaders
// lots of printing good for debugging - remove in release version
// add override keyword bum
// 
// research better error handling practices?
// get printOpenGLErrors() to util file and use elsewhere
// in compute shader class rn
// fix order of member init lists EVERYWHERE, make everything const
// had to use pointer for ocean class in renderer - change?
// std::cerr instead of std::cout?
// commented out a bunch of code in render function of ocean class - still works but idk lol
// print out version to make sure its the right one (4.3 for rn)

#include "Renderer.h"

int main() 
{

	Renderer superDuperAwesomeRenderer = Renderer();
	superDuperAwesomeRenderer.Run();
	return EXIT_SUCCESS;

}
