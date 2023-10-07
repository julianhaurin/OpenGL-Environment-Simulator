// OpenGL Environment Simulator - Julian Haurin

// Sources:
// learnopengl.com
// cherno
// others on github

// Notes:
// lots of printing good for debugging - remove in release version
// add override keyword bum
// 
// research better error handling practices? printOpenGLErrors() more
// fix order of member init lists EVERYWHERE, make everything const
// std::cerr instead of std::cout?
// organize shader folder
// refactor shadow class and model rendering function

#define STB_IMAGE_IMPLEMENTATION
#include "Renderer.h"

int main() 
{

	Renderer superDuperAwesomeRenderer = Renderer();
	superDuperAwesomeRenderer.Run();
	return EXIT_SUCCESS;

}
