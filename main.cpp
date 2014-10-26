#include <iostream>
#include <GL/glew.h>
#include <SDL.h>

using namespace std;

const GLchar* strVertexShader(
	"#version 330\n"
	"in vec2 position;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(position.xy, 0.0f, 1.0f);\n"
	"}\n"
	);

const GLchar* strFragmentShader(
	"#version 330\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n"
	);

int main(int argc, char* args[])
{


	while (getchar() != '\n') {}//Prevents application from closing without user input

	return 0;
}
