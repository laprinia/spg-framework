#include "NorwegianFjords.h"
#include <stdlib.h>
#include <iostream>
#include <vector>

NorwegianFjords::NorwegianFjords()
{

}

NorwegianFjords::~NorwegianFjords() {

}

float NorwegianFjords::GenerateRandomFloat(float upper) {
	
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / upper));
}
void NorwegianFjords::GenerateControlPoints() {
	controlP1 = glm::vec3(0, 0,0.0);
	controlP2 = glm::vec3(10.0,0, -4.0);
	controlP3 = glm::vec3(-10.0, 0,-10.0);
	controlP4 = glm::vec3(4.0,0, -20.0);
}

void NorwegianFjords::Init() {

	auto camera = GetSceneCamera();

	camera->SetPositionAndRotation(glm::vec3(0, 5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
	camera->Update();
	

	std::string shaderPath = "Source/NorwegianFjords/Shaders/";
	{
		Shader* shader = new Shader("SurfaceGeneration");
		shader->AddShader(shaderPath + "BezierGeneratorVS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "BezierGeneratorGS.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader(shaderPath + "BezierGeneratorFS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	GenerateControlPoints();

	{
		std::vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(controlP1.x, controlP1.y,  controlP1.z), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(controlP4.x, controlP4.y,  controlP4.z), glm::vec3(0, 0, 1))
		};


		std::vector<unsigned short> indices =
		{
			0, 1
		};

		meshes["riverSurface"] = new Mesh("riverSurface");
		meshes["riverSurface"]->InitFromData(vertices, indices);
		meshes["riverSurface"]->SetDrawMode(GL_LINES);
	}
	
}

void NorwegianFjords::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void NorwegianFjords::GenerateRiver(Shader* shader)
{

	glUniform3f(glGetUniformLocation(shader->program, "control_p1"), controlP1.x, controlP1.y, controlP1.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p2"), controlP2.x, controlP2.y, controlP1.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p3"), controlP3.x, controlP3.y, controlP3.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p4"), controlP4.x, controlP4.y, controlP4.z);
	glUniform1i(glGetUniformLocation(shader->program, "instanceNumber"), numberOfBezierInstances);
	glUniform1i(glGetUniformLocation(shader->program, "pointsNumber"), numberOfBezierPoints);
	

	Mesh* mesh = meshes["riverSurface"];

	RenderInstancedMesh(mesh, shader, glm::mat4(1), numberOfBezierInstances);
}
void NorwegianFjords::Update(float deltaTimeSeconds)
{

	ClearScreen();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader* shader = shaders["SurfaceGeneration"];
	shader->Use();
	GenerateRiver(shader);
	

}

void NorwegianFjords::RenderInstancedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int instances) {

	if (!mesh || !shader || !shader->GetProgramID())
		return;
	
	
	glUseProgram(shader->program);


	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, instances);
	

}

void NorwegianFjords::FrameEnd()
{
	DrawCoordinatSystem();
}

void NorwegianFjords::OnInputUpdate(float deltaTime, int mods)
{

}

void NorwegianFjords::OnKeyPress(int key, int mods)
{
	
};

void NorwegianFjords::OnKeyRelease(int key, int mods)
{
	
};

void NorwegianFjords::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	
};

void NorwegianFjords::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	
};

void NorwegianFjords::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	
}

void NorwegianFjords::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	
}

void NorwegianFjords::OnWindowResize(int width, int height)
{
	
}
