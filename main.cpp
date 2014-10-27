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
	"uniform vec3 uniColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = vec4(uniColor, 1.0f);\n"
	"}\n"
	);

int main(int argc, char* args[])
{

	// Initialise SDL and exit the program if false

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return 1;
	}
	std::cout << "SDL Initialised!" << std::endl;


	// Create a new window - Window Title, X and Y position on screen, Width and Height dimensions, and make it an SDL window
	SDL_Window* window;
	window = SDL_CreateWindow("Window Title", 100, 100, 600, 600, SDL_WINDOW_OPENGL);

	// Make sure we tell it we're using OpenGL 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Turn our window into an OpenGL window
	SDL_GLContext context = SDL_GL_CreateContext(window);
	std::cout << "SDL context initialised!" << std::endl;

	// We use glewExperimental to call OpenGL 3 functions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return 1;

	// Create our square - two different triangles
	float vertices[] = {
		-0.5f, -0.5f, // bottom left
		0.5f, -0.5f, // bottom right
		-0.5f, 0.5f, // top left
		
		-0.5f, 0.5f, // top left
		0.5f, 0.5f, // top right
		0.5f, -0.5f // bottom right

	};

	
	///Create a vertex buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &strVertexShader, NULL);
	glCompileShader(vertexShader);

	// Compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &strFragmentShader, NULL);
	glCompileShader(fragmentShader);


	// Check for errors in our vertex shader - will print '0' if there's something wrong, or '1' if it's fine
	GLint vertexStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexStatus);
	std::cout << "Vertex shader: " << vertexStatus << std::endl;

	// Check for errors in the fragment shader the same way we check the vertex shader
	GLint fragStatus;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragStatus);
	std::cout << "Frag shader: " << fragStatus << std::endl;

	// Attach our two shaders to a new shader program (i.e. say we want to use those shaders together)
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	
	// Say that the variable "outputColour" is the final colour - not actually necessary in this case.
	glBindFragDataLocation(shaderProgram, 0, "outputColor");

	// "Link" the program - up to this point, the two shaders can still be modified. This finialises them.
	glLinkProgram(shaderProgram);
	// All proceeding shader functions will use this shaderProgram
	glUseProgram(shaderProgram);

	std::cout << "Shader program created!" << std::endl;

	/// Create the Vertex Array Object
	// A VAO essentially 'points' to your VBO - if you have multiple VBOs, you use a VAO to manage them
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Find where the 'position' variable is in our vertex shader
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	// Tell the shader what we're sending into it - we're sending 2 things at a time, which are GL_FLOATs
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Find where our uniform is in the fragment shader
	GLint uniColor = glGetUniformLocation(shaderProgram, "uniColor");
	// Alter the values of the uniform - RGB - to make a green triangle
	glUniform3f(uniColor, 0.0f, 1.0f, 0.0f);

	// Draw everything our VAO points to - we're drawing triangles, GL_TRIANGLE, and we're using six vertices for our square
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Updates the window
	SDL_GL_SwapWindow(window);


	while (getchar() != '\n') {}//Prevents application from closing without user input

	return 0;
}
