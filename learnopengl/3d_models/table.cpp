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

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setup_shape(std::vector<float> shape, unsigned int* VAO, unsigned int* VBO);
unsigned int loadTexture(char const* path);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const unsigned int SRC_WIDTH = 800;
	const unsigned int SRC_HEIGHT = 600;
	GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader tableShader("shaders/shader.vs", "shaders/shader.fs");
	Shader lightShader("shaders/light.vs", "shaders/light.fs");

	glEnable(GL_DEPTH_TEST);

	std::vector<float> const& light = Shape::pyramid(0.4f, 0.0f, 0.0f, 0.0f);
	std::vector<float> const& top = Shape::cube(2.0f, 0.2f, 4.0f, 0.0f, 0.0f, 0.0f);
	std::vector<float> const& leg1 = Shape::cube(0.2f, 0.5f, 0.2f, 0.7f, -0.3f, 1.7f);
	std::vector<float> const& leg2 = Shape::cube(0.2f, 0.5f, 0.2f, -0.7f, -0.3f, 1.7f);
	std::vector<float> const& leg3 = Shape::cube(0.2f, 0.5f, 0.2f, -0.7f, -0.3f, -1.7f);
	std::vector<float> const& leg4 = Shape::cube(0.2f, 0.5f, 0.2f, 0.7f, -0.3f, -1.7f);
	std::vector<float> const& test = Shape::pyramid(1.0f, 0.0f, 1.0f, 0.0f);

	unsigned int VBO, tableVAO, lightVAO, testVAO;
	unsigned int legsVAO[4];
	setup_shape(top, &tableVAO, &VBO);
	setup_shape(leg1, &legsVAO[0], &VBO);
	setup_shape(leg2, &legsVAO[1], &VBO);
	setup_shape(leg3, &legsVAO[2], &VBO);
	setup_shape(leg4, &legsVAO[3], &VBO);
	setup_shape(light, &lightVAO, &VBO);
	setup_shape(test, &testVAO, &VBO);

	unsigned int textureWood = loadTexture("./textures/Table1.jpg");
	tableShader.use();
	tableShader.setInt("wood", 0);

	float speed  = 1.0f,
		  fov    = 22.5f,
		  camX   = 10.0f,
		  camZ   = 10.0f,
		  lightX = 0.0f,
		  lightY = 2.0f,
		  lightZ = 0.0f;
	bool TurnX = false,
		 TurnY = false,
		 TurnZ = false;
	glm::mat4 tableTrans(1.0f);
	glm::vec3 lightPos(lightX, lightY, lightZ),
			  lightColor(1.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void) io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window))
	{
		tableShader.use();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWood);

		processInput(window);

		lightPos = glm::vec3(lightX, lightY, lightZ);
		glm::mat4 view = glm::lookAt(
				glm::vec3(camX, 0.0f, camZ),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
		if (TurnX || TurnY || TurnZ)
			tableTrans = glm::rotate(tableTrans, glm::radians(speed), glm::vec3(TurnX, TurnY, TurnZ));
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), (float) SRC_WIDTH/ (float) SRC_HEIGHT, 0.1f, 100.0f);

		tableShader.setMat4("view", view);
		tableShader.setMat4("model", glm::mat4(1.0f));
		tableShader.setMat4("projection", projection);
		tableShader.setMat4("transform", tableTrans);
		tableShader.setVec3("lightPos", lightPos);
		tableShader.setVec3("lightColor", lightColor);

		glBindVertexArray(tableVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (int i=0; i < 4; ++i) {
			glBindVertexArray(legsVAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(testVAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		lightShader.use();
		glm::mat4 lightModel(1.0f);
		lightModel = glm::translate(lightModel, glm::vec3(lightX, lightY, lightZ));
		lightShader.setMat4("view", view);
		lightShader.setMat4("model", lightModel);
		lightShader.setMat4("projection", projection);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("ImGui");
		ImGui::Text("Settings");
		ImGui::Checkbox("Turn X", &TurnX);
		ImGui::Checkbox("Turn Y", &TurnY);
		ImGui::Checkbox("Turn Z", &TurnZ);
		ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
		ImGui::SliderFloat("light X", &lightX, -5.0f, 5.0f);
		ImGui::SliderFloat("light Y", &lightY, -5.0f, 5.0f);
		ImGui::SliderFloat("light Z", &lightZ, -5.0f, 5.0f);
		ImGui::SliderFloat("FOV", &fov, 0.0f, 90.0f);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &tableVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &legsVAO[0]);
	glDeleteVertexArrays(1, &legsVAO[1]);
	glDeleteVertexArrays(1, &legsVAO[2]);
	glDeleteVertexArrays(1, &legsVAO[3]);
	glDeleteBuffers(1, &VBO);

	delete &top;

	glfwTerminate();
	return 0;
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const* path)
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
