#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/ewMath/vec3.h>
#include <ew/procGen.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Square aspect ratio for now. We will account for this with projection later.
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	cubeTransform[0].position.x = -0.5f;
	cubeTransform[1].position.x = -0.5f;

	cubeTransform[0].position.y = -0.5f;
	cubeTransform[1].position.y = 0.5f;

	cubeTransform[2].position.x = 0.5f;
	cubeTransform[3].position.x = 0.5f;

	cubeTransform[2].position.y = -0.5f;
	cubeTransform[3].position.y = 0.5f;
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		

		//TODO: Set model matrix uniform
		shader.use();

		shader.setMat4("_Model", cubeTransform[0].getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", cubeTransform[1].getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", cubeTransform[2].getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", cubeTransform[3].getModelMatrix());
		cubeMesh.draw();

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");

			for (size_t i = 0; i < 4; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) 
				{
					ImGui::DragFloat3("Position", &cubeTransform[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransform[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransform[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}

			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

