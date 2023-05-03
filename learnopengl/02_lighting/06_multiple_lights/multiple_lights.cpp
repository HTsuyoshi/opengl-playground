#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <shaders.h>
#include <shapes.h>
#include <camera.h>

#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int loadTexture(char const * path);
void setup_shape(std::vector<float> shape, unsigned int* VAO, unsigned int* VBO);

const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 direction;
glm::vec3 lightPos(-1.0f, 0.0f, -1.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = SRC_WIDTH / 2.0, lastY = SRC_HEIGHT / 2.0;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader lightingShader("shaders/color.vs", "shaders/color.fs");
	Shader lightShader("shaders/shader.vs", "shaders/shader.fs");

	glEnable(GL_DEPTH_TEST);

	std::vector<float> pyramid = Shape::pyramid(1.0f, 0.0f, 0.0f, 0.0f);
	std::vector<float> plane = Shape::plane(10.0f, 0.0f, 4.5f, 0.0f);

	unsigned int VBO, pyramidVAO;
	setup_shape(pyramid, &VBO, &pyramidVAO);
	unsigned int planeVAO;
	setup_shape(plane, &VBO, &planeVAO);

	unsigned int texture1 = loadTexture("textures/container2.png");
	unsigned int texture2 = loadTexture("textures/container2_specular.png");

	lightingShader.use();
	lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setFloat("material.shininess", 64.0f);

	lightingShader.setVec3("dirLight.ambient", glm::vec3(0.1f));
	lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.8f));
	lightingShader.setVec3("dirLight.specular", glm::vec3(1.0f));
	lightingShader.setVec3("dirLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));

	glm::vec3 positions[] = {
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(-2.0f, 0.0f, -2.0f),
	};

	lightingShader.setVec3("pointLight[0].position", positions[0]);
	lightingShader.setVec3("pointLight[0].ambient", glm::vec3(0.1f));
	lightingShader.setVec3("pointLight[0].diffuse", glm::vec3(0.8f));
	lightingShader.setVec3("pointLight[0].specular", glm::vec3(1.0f));
	lightingShader.setFloat("pointLight[0].constant", 1.0f);
	lightingShader.setFloat("pointLight[0].linear", 0.09f);
	lightingShader.setFloat("pointLight[0].quadratic", 0.032f);

	lightingShader.setVec3("pointLight[1].position", positions[1]);
	lightingShader.setVec3("pointLight[1].ambient", glm::vec3(0.1f));
	lightingShader.setVec3("pointLight[1].diffuse", glm::vec3(0.8f));
	lightingShader.setVec3("pointLight[1].specular", glm::vec3(1.0f));
	lightingShader.setFloat("pointLight[1].constant", 1.0f);
	lightingShader.setFloat("pointLight[1].linear", 0.09f);
	lightingShader.setFloat("pointLight[1].quadratic", 0.032f);

	lightingShader.setVec3("spotLight.ambient", glm::vec3(0.1f));
	lightingShader.setVec3("spotLight.diffuse", glm::vec3(0.8f));
	lightingShader.setVec3("spotLight.specular", glm::vec3(1.0f));
	lightingShader.setFloat("spotLight.constant", 1.0f);
	lightingShader.setFloat("spotLight.linear", 0.09f);
	lightingShader.setFloat("spotLight.quadratic", 0.032f);
	lightingShader.setVec3("spotLight.position", camera.Position);
	lightingShader.setVec3("spotLight.direction", camera.CameraDirection());
	lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void) io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.4f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SRC_WIDTH / (float)SRC_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(1.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		lightingShader.use();
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.CameraDirection());
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glBindVertexArray(pyramidVAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		glBindVertexArray(planeVAO);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.0f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		for (int i=0; i<2; ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);
			glBindVertexArray(pyramidVAO);
			glDrawArrays(GL_TRIANGLES, 0, 18);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("ImGui");
		ImGui::Text("Settings");
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &pyramidVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void setup_shape(std::vector<float> shape, unsigned int* VAO, unsigned int* VBO)
{
	glGenVertexArrays(1, &*VAO);
	glGenBuffers(1, &*VBO);
	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(float), shape.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RED;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
