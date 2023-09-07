
#include "ShaderProgram.h"

// initializes vertex and fragment data source code (tesselation shader coming soon?)
ShaderProgram::ShaderProgram(const char* in_vertexShaderPath, const char* in_fragmentShaderPath)
	: m_vertShaderCode(""), m_fragShaderCode("")
{
	retrieveShaderSourceCode(in_vertexShaderPath, in_fragmentShaderPath);
	InitializeShaders();
}

GLint ShaderProgram::getID() {
	return m_ID;
}

// compiles and links vertex and fragment shaders into shader program object and checks for errors
void ShaderProgram::InitializeShaders() {

	const char* vertShaderCode = m_vertShaderCode.c_str();
	const char* fragShaderCode = m_fragShaderCode.c_str();

	uint32_t vertShaderID;
	uint32_t fragShaderID;

	// compiles vertex shader
	vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderID, 1, &vertShaderCode, NULL);
	glCompileShader(vertShaderID);
	if (checkCompilationSuccess(vertShaderID, "vertex") == true) {
		std::cout << "[J] - Shader program (ID: " << m_ID << ") successfully compiled vertex shader! \n";
	}

	// compiles fragment shader
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderID, 1, &fragShaderCode, NULL);
	glCompileShader(fragShaderID);
	if (checkCompilationSuccess(fragShaderID, "fragment") == true) {
		std::cout << "[J] - Shader program (ID: " << m_ID << ") successfully compiled fragment shader! \n";
	}
	
	// initializes and links shader program
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertShaderID);
	glAttachShader(m_ID, fragShaderID);
	glLinkProgram(m_ID);
	if (checkLinkingSuccess(m_ID) == true) {
		std::cout << "[J] - Shader program (ID: " << m_ID << ") successfully linked shaders! \n";
	}

	// deallocate resources
	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	std::cout << "\n";

}

// reads source code from provided files and updates member variables
void ShaderProgram::retrieveShaderSourceCode(const char* in_vertexShaderPath, const char* in_fragmentShaderPath) {

	// creates file streams and reads shader source code into strings
	std::ifstream vertFileStream;
	std::ifstream fragFileStream;

	vertFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// reads file data into source code member variables
	try {
		std::stringstream vertStrStream;
		std::stringstream fragStrStream;

		vertFileStream.open(in_vertexShaderPath);
		fragFileStream.open(in_fragmentShaderPath);

		vertStrStream << vertFileStream.rdbuf();
		fragStrStream << fragFileStream.rdbuf();

		vertFileStream.close();
		fragFileStream.close();

		m_vertShaderCode = vertStrStream.str(); // updates vertex shader code
		m_fragShaderCode = fragStrStream.str(); // updates fragment shader code


	} // prints any errors 
	catch (std::ifstream::failure& err) {
		std::cout << "[J] - ERROR: Shader program (ID: " << m_ID 
			<< ") failed to successfully read shader files: " << err.what() << std::endl;

	}
}

// checks for shader linking errors
bool ShaderProgram::checkLinkingSuccess(const int32_t in_programID) const {

	GLint success;
	char infoLog[1024];

	glGetProgramiv(in_programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(in_programID, 1024, NULL, infoLog);
		std::cout << "[J] - ERROR: Failed to successfully link shader program: " << infoLog << std::endl;
	}
	
	return success;
}

