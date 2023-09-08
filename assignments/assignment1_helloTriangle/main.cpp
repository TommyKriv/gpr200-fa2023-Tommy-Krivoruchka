#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[21] = {
	//x   //y  //z   //r  //g  //b  //a
	-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, //Bottom left
	 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, //Bottom right
	 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0  //Top center
};

const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
	layout(location = 1) in vec4 vColor;
	out vec4 Color;
	uniform float _Time;
	void main(){
		Color = vColor;
		vec3 offset = vec3(0,sin(vPos.x + _Time),0)*0.5;
		gl_Position = vec4(vPos + offset,1.0);
	}
)";

const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	in vec4 Color;
	uniform float _Time;
	void main(){
		FragColor = Color * abs(sin(_Time));
	}
)";

unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	//Create a new vertex shader object
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Supply the shader object with source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compile the shader object
	glCompileShader(vertexShader);

	//Create a new fragment shader object
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Supply the shader object with source code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//Compile the shader object
	glCompileShader(fragmentShader);

	unsigned int shaderProgram = glCreateProgram();
	//Attach each stage
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);

	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
		return NULL;
	}
	else
		return shaderProgram;
}

unsigned int createVAO(float* vertexData, int numVertices)
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	return vao;
}

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Creating vao
	unsigned int vao = createVAO(vertices, 3); 

	//Creating shader
	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	while (!glfwWindowShouldClose(window)) {
		//The current time in seconds this frame
		float time = (float)glfwGetTime();
		//Get the location of the uniform by name
		int timeLocation = glGetUniformLocation(shader, "_Time");
		//Set the value of the variable at the location
		glUniform1f(timeLocation, time);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
