#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>
#include <patchwork/model.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	patchwork::Model torus("assets/torus.dae");
	patchwork::Model chandelier("assets/GrappleYChandelier.dae");
	patchwork::Model flower("assets/Flowa.dae");
	patchwork::Model plate("assets/plate.dae");

	struct Light {
		ew::Vec3 position; //World space
		ew::Vec3 color; //RGB
	};

	struct Material {
		float ambientK; //Ambient coefficient (0-1)
		float diffuseK; //Diffuse coefficient (0-1)
		float specular; //Specular coefficient (0-1)
		float shininess; //Shininess
	};

	bool blinn = true;

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader unlit("assets/unlit.vert", "assets/unlit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg", GL_REPEAT, GL_LINEAR);

	Material material1;
	material1.ambientK = 0.1f;
	material1.diffuseK = 0.7f;
	material1.shininess = 16;
	material1.specular = 0.5f;

	ew::Transform torusTransform;
	torusTransform.position = ew::Vec3(0, -1.0, 0);

	ew::Transform chandTransform;
	chandTransform.position = ew::Vec3(3.2, 0.5, -3);
	chandTransform.rotation = ew::Vec3(-60, 90, 0);
	chandTransform.scale = ew::Vec3(0.2, 0.2, 0.2);

	ew::Transform flowerTransform;
	flowerTransform.position = ew::Vec3(-3.2, -0.1, -3);
	flowerTransform.rotation = ew::Vec3(-124, 70, -78);
	flowerTransform.scale = ew::Vec3(0.6, 0.6, 0.6);

	ew::Transform plateTransform;
	plateTransform.position = ew::Vec3(0.2, 3, -3);
	plateTransform.rotation = ew::Vec3(-58, 47, -50);
	plateTransform.scale = ew::Vec3(2, 2, 0.16);

	//Create cube
	Light lights[3];

	lights[0].position = ew::Vec3(3, 3, -3);
	lights[0].color = ew::Vec3(1, 1, 1);
	ew::Transform lightTrans[3];
	lightTrans[0].position = lights[0].position;

	lights[1].position = ew::Vec3(-3, 3, -3);
	lights[1].color = ew::Vec3(1, 1, 0);
	lightTrans[1].position = lights[1].position;

	lights[2].position = ew::Vec3(2, 3, 2);
	lights[2].color = ew::Vec3(0, 1, 1);
	lightTrans[2].position = lights[2].position;

	resetCamera(camera, cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setVec3("_ViewLocation", camera.position);

		ew::Mat4 model = ew::Mat4(1.0f);

		shader.setMat4("_Model", torusTransform.getModelMatrix());
		torus.Draw(shader);

		shader.setMat4("_Model", chandTransform.getModelMatrix());
		chandelier.Draw(shader);

		shader.setMat4("_Model", flowerTransform.getModelMatrix());
		flower.Draw(shader);

		shader.setMat4("_Model", plateTransform.getModelMatrix());
		plate.Draw(shader);

		shader.setVec3("_Lights[0].position", lights[0].position);
		shader.setVec3("_Lights[0].color", lights[0].color);
		shader.setVec3("_Lights[1].position", lights[1].position);
		shader.setVec3("_Lights[1].color", lights[1].color);
		shader.setVec3("_Lights[2].position", lights[2].position);
		shader.setVec3("_Lights[2].color", lights[2].color);
		shader.setFloat("_Material.ambientK", material1.ambientK);
		shader.setFloat("_Material.diffuseK", material1.diffuseK);
		shader.setFloat("_Material.shininess", material1.shininess);
		shader.setFloat("_Material.specular", material1.specular);
		if (blinn)
			shader.setInt("blinn", 1);
		else
			shader.setInt("blinn", 0);

		//TODO: Render point lights
		unlit.use();
		unlit.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		for (int i = 0; i < 3; i++)
		{
			unlit.setMat4("_Model", lightTrans[i].getModelMatrix());
			unlit.setVec3("_Color", lights[i].color);
		}


		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Blinn", &blinn);
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}

			ImGui::ColorEdit3("BG color", &bgColor.x);
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
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}