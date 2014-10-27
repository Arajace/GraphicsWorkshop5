Graphics Workshop 5     
=========    
   
    
This workshop will act as a recap session for the basics rendering process to ensure you have a thorough working knowledge before moving further on. We'll go through triangle rendering line by line and break each aspect down. If you already feel comfortable with this, you can try the extra tasks (PDF available on Blackboard): https://github.com/shearer12345/graphics/blob/master/lincolnWorkshop05.mdpp

To start, open Git Shell and clone this repository:
```c
git clone https://github.com/jblakeLincoln/GraphicsWorkshop5.git
```

Then naviage to it with ``cd GraphicsWorkshop5`` and then run premake4 to create the solution (eg. ``./premake4.exe vs2012`` on Windows to create a solution you can open in Visual Studio 2012 and 2013).

Shaders
----

Open the solution and navigate to **main.cpp**. Here you'll see
* vertex shader
* fragment shader
* an empty main

The basics for our vertex shader are:
* **in vec2 position** is a variable (vec2 - two numbers) that we send data to through a vertex buffer (indicated by the word **in**).
* **vec4(position.xy, 0.0f, 1.0f)** is assigned to **gl_Position**, which is GLSL's way of assigning the current vertex position. We're going send in the **X** and **Y** values, and then always use 0 for **Z** and 1 for **W**.
* Each vertex we send through the vertex buffer object (later on) will run through this code, and be placed on screen in different places depend on what we send through the **position** variable.

And the fragment shader:
* **out vec4 outputColour** is vec4 variable (four numbers), but this time we use the keyword **out**
* Later on we'll tell our code tell our code that whatever this variable equals is the colour to draw on screen for that fragment
* So **vec4(1.0f, 1.0f, 1.0f, 1.0f)** means our output colour will be white (RGBA values from 0 to 1)

Creating the window
---
We're using a library called [SDL] for our window creation. So we need to start by *initialising* SDL.

```c
if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
{
    std::cout << "SDL initialisation failed!" << std::endl;
    return 1;
}
else
{
    std::cout << "SDL initialised!" << std::endl;
}
```

When we call the **SDL_Init** function in the if statement, it runs and returns a value indicating whether everything was created successfully. If something went wrong, the application will "return 1" and exit - but that shouldn't happen here!

Now we need to create the window by creating a new SDL Window object by typing:
```c
SDL_Window *window;
```
(The asterisk means it's a **pointer** - you should go over these in your other C++ modules - don't worry about it for now).

We then initialise the window by calling the window creation function with five arguments - the window name, its X and Y position on screen, its width and height, and the kind of window it will be.
```
window = SDL_CreateWindow("Window Title", 100, 100, 600, 600, SDL_WINDOW_OPENGL);
```
We've created a window that will open at the 100,100 position on screen (from the top left), will be 600 by 600 pixels large, and will be used for OpenGL rendering. If you run the application now, the window will appear.

Creating the OpenGL context
---
We've made a blank window, but it's not ready to use yet. To render OpenGL graphics, we need an **OpenGL context** - the thing that lets us see OpenGL stuff in the window we've made.

We need to specify the version of OpenGL we're using.
```
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
```
We specify our ```SDL_GL_CONTEXT_MAJOR_VERSION``` is 3, meaning we're using OpenGL 3.x - modern OpenGL. We further specify the minor version as 3 to indicate we're using OpenGL 3.3.

Now we can add the context to our window.
```c
SDL_GLContext context = SDL_GL_CreateContext(window);
std::cout << "Created OpenGL context!" << std::endl;
```
The window will still look the same if you run it now, but it's ready for OpenGL. Finally, we have to initialise **GLEW**, the OpenGL Extension Wrangler Library - this just works as a way to let us call OpenGL functions. Like when we initialised SDL (the very first code snippet), we check for any errors when this initialisation is performed. If something is wrong, the window will close. The ```glewExperimental``` line specifies that we're calling modern OpenGL functions.
```c
glewExperimental = GL_TRUE;
if (glewInit() != GLEW_OK)
{
    return 1;
}
```
Vertex Buffer Object
---
A ***vertex buffer*** is used to upload vertices to the vertex shader. In this case, we will use it to send three points of a triangle to our shader. To send the data, we need a ***vertex buffer object***.

Firstly, we need to define the vertices for the triangle we will render. We will specify everything in a single array, where each pair of values refer to an **x** and **y** position on screen.
```c
float vertices[] = {
		-0.5f, -0.5f, // bottom left
		0.5f, -0.5f, // bottom right
		0.0f, 0.5f // top
	};
```
The default OpenGL ***screen space*** is from -1.0 to +1.0, so we specify our vertices within those bounds.

---
![alt text][ScreenSpace1] ![alt text][ScreenSpace2]  
##### Images from https://open.gl/ - a great learning resource that you should check out
---
Now we will create the vertex buffer object to upload the vertices to the shader. We use the type ```GLuint```, a regular unsigned number (can only be positive) which will specify the ID of the buffer.
```c
GLuint vbo;
glGenBuffers(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```
The ***glGenBuffers*** function takes in the number of buffers we are creating (in this case, just 1), and the variable we will be using for the ID of our buffer (the GLuint ```vbo```). It creates a buffer and assigns the ID to the variable ```vbo```.

***glBindBuffer*** is used to specify which buffer we want to be 'active'. The first argument is the type of buffer it will be - a ```GL_ARRAY_BUFFER```, which means it will be used for vertices - and the ID of the buffer (the variable ```vbo```).

The ***glBufferData*** function is used to send data to the currently active buffer (which we bound on the previous line). We specify that the buffer is for vertex data (```GL_ARRAY_BUFFER```). We then state the *actual* size of the data we're going to send through using ***sizeof*** on our array - which will be 24 bytes, since a ```float``` is 4 bytes in this case and we are sending 6 of them (our three pairs). The third parameter is the array that we will be sending through (```vertices```), and the final parameter specifies the frequency at which we intend to change the buffer data (for now we'll use ```GL_STATIC_DRAW``` - you can Google the alternatives).

Creating the shader program
---
We've written our shader programs at the top of this file: ```GLchar* strVertexShader``` and ```strFragmentShader```. Now we need to tell the application to use them.

We create the ***IDs*** for the shaders, then specify the data, and then compile them.

```c
GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &strVertexShader, NULL);
glCompileShader(vertexShader);
```
The vertex ID is assigned to a ```GLuint```, just like the ***vbo*** was. We assign the ID using the ***glCreateShader*** function with the argument of the type of shader it will be - for the vertex shader, ```GL_VERTEX_SHADER```.

The ***glShaderSource*** function takes in the parameters:
* the *shader ID* (the variable ```vertexShader```)
* the number of shaders we are creating (just 1 here)
* the shader code (our ```GLchar``` at the top, ```strVertexShader```). 
*  --The fourth value can be used to specify the length of the shader, but we can leave it as ```NULL``` right now.

The same thing is necessary for the fragment shader (except this time it's a different type, ```GL_FRAGMENT_SHADER```).
```c
GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragShader, 1, &strFragmentShader, NULL);
glCompileShader(fragShader);
```
Finally, we need to state that our vertex shader and fragment shader are working together by creating a ***shader program***. We create an ID for the shader program (just like the VBO and shaders before), then attach our shaders to the program. 
```c
GLuint shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragShader);
```
Take a look at the fragment shader at the top of the file. We have an ***out*** variable called outputColour. It needs to be specified that this will be the final output colour of the fragment shader of our shader program.
```c
glBindFragDataLocation(shaderProgram, 0, "outputColour"); // The second parameter can be left as 0 for now
```
Finally, we need to **link** and **use** the shader program. ```glLinkProgram``` will link the vertex and fragment shaders to the shader program (because of the ```glAttachShader``` calls we made). ```glUseProgram``` specifies the shader program that all following operations will use.

```
glLinkProgram(shaderProgram);
glUseProgram(shaderProgram);
```

Vertex Array Object
---
To finish the process of specifying our vertices, we need to create a ***vertex array object***. This acts as a way to point to any previously bound vertex buffer objects - although we're just using one right now, this comes in handy when you're uploading multiple pieces of data to the shader.

As with the VBO, we have to generate and bind the buffer.
```c
GLuint vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);
```
We also need to specify details about the vertex data that we're using - it needs to be explicitly stated that we're sending 2 pieces of data at a time (the **X** and **Y** positions of the triangle vertices).

Look at the vertex shader at the top - the variable ```position``` has the keyword **in**, meaning we'll send data through the buffer. First we need to find the position of this variable in the shader and enable its use.
```c
GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
glEnableVertexAttribArray(posAttrib);
```
Now to state how we're sending that variable data:
```c
glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
```

The first four parameters are:

* The location in the shader we're referring to (which we assigned to **posAttrib**).
* The number of components we'll be sending through at a time - two, the X and Y of the triangle.
* The type of variable we'll be using, a float.
* Whether to normalise the values (making the values between 0 and 1) - which we don't want to do, so we specify GL_FALSE

The final parameters are the **stride** (byte offset) and **pointer** (offset of the first component). These can both be 0 for now, but can be useful in situations where you want to send more data.

Rendering
---
At this point everything is in place - we just need to tell everything to render.

We can do this with the following two lines:

```c
glDrawArrays(GL_TRIANGLES, 0, 3);
SDL_GL_SwapWindow(window);
```

**glDrawArrays** takes three arguments: 
* the type of primitive we are drawing (triangles in this case)
* the starting index of the arrays we're using (0, since we want to use all our data)
* the number of vertices we're drawing (3 - the bottom left, bottom right, and top middle points of the triangle)

The **SDL_GL_SwapWindow** function prompts a window to update. We pass it the parameter **window**, which is the variable we made at the start.

Make sure you still have the ```while (getchar() != '\n') {} ``` line to prevent the window from closing. Run your application now and you should hopefully see a triangle!  

---
![alt text][FinishedTriangle]
---

Looping
---
Typically you will want to have a loop in which you can alter what you're going to render. We'll make a loop and perform drawing functionality inside of it.

```c
SDL_Event event;
bool looping = true;
while (looping) //This means the same thing as "looping == true"
{

}
```

(at this point, delete the line ```while (getchar() != '\n') {} ```)  

The **SDL_Event** variable will be used to check if the window has been closed by the user. Inside the loop we're going to enter the following code to draw and check the window state.

```glClear(GL_COLOR_BUFFER_BIT)``` will be used to clear the window at the start of the loop. Now, move your ```glDrawArrays``` and ```SDL_GL_SwapWindow``` calls inside the loop (after ```glClear```).

If you run your code now your triangle should still appear. You can't see it, but it's updating frame by frame - before it was just updating once as a static image. If you press the close button of your window, nothing happens, so we need to check for when the user does that.

```
SDL_PollEvent(&event);
if (event.type == SDL_QUIT)
{
    looping = false;
}
```
The **SDL_PollEvent** function is used to update our event variable with the latest action the user has performed. After we've got that information, we use an if statement to check if the type of event the user triggered was **SDL_QUIT** - an event that occurs when you press the window's 'X' button. If they do press it, the variable of the while loop ('looping') will be set to false: the loop will end and the program will terminate.

So your final loop will look like this:
```c
SDL_Event event;
bool looping = true;
while (looping)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	SDL_GL_SwapWindow(window);
	SDL_PollEvent(&event);

	if (event.type == SDL_QUIT)
	{
		looping = false;
	}
}
```
Done!
---
Now that we're done, you should understand:
* What a vertex buffer object is for
* How to send vertex data to the shader
* The basics of a shader and creating a shader program
* Creating a rendering loop to draw to screen

Extras
---
Refer to the other workshop tasks to modify your code to do the following
* Change the background colour
* Change the triangle colour in the shader
* Change the triangle colour in your program's **main** by using **uniforms**
* Change the vertices to change the position of the triangle
* Add more vertices and change the draw code to draw a square

[SDL]:https://www.libsdl.org/
[ScreenSpace1]:http://i.imgur.com/sut5WoQ.png "Default OpenGL screen space"
[ScreenSpace2]:http://i.imgur.com/5dDESGZ.png "Where our triangle points lie relative in screen space"
[FinishedTriangle]:http://i.imgur.com/kzy1bpm.png "The triangle output"
