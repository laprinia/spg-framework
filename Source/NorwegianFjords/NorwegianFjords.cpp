#include "NorwegianFjords.h"
#include <stdlib.h>
#include <iostream>
#include <vector>

NorwegianFjords::NorwegianFjords()
{

}

NorwegianFjords::~NorwegianFjords() {

}

float NorwegianFjords::GenerateRandomFloat(float lower, float upper) {

	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = upper - lower;
	float r = random * diff;
	return lower + r;
}
void NorwegianFjords::GenerateControlPoints() {
	//main branch
	controlP1 = glm::vec3(0.0, 0.0, 0.0);
	controlP2 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP1.z +GenerateRandomFloat(-5, -1));
	controlP3 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP2.z + GenerateRandomFloat(-5, -1));
	controlP4 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP3.z + GenerateRandomFloat(-5, -1));
	//second branch
	controlP5 = glm::vec3(GenerateRandomFloat(-5, 5), 0.2f, controlP4.z + GenerateRandomFloat(-5, -1));
	controlP6 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP5.z + GenerateRandomFloat(-5, -1));
	controlP7 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP6.z + GenerateRandomFloat(-5, -1));
	//third branch
	controlP8 = glm::vec3(controlP5.x+GenerateRandomFloat(5, 10), 0.4f, controlP4.z + GenerateRandomFloat(-5, -1));
	controlP9 = glm::vec3(controlP6.x+GenerateRandomFloat(5, 10), 0.0f, controlP8.z + GenerateRandomFloat(-5, -1));
	controlP10 = glm::vec3(controlP7.x+GenerateRandomFloat(5, 10), 0.0f, controlP9.z + GenerateRandomFloat(-5, -1));
	//fourth branch
	controlP11 = glm::vec3(controlP5.x-GenerateRandomFloat(5, 10), 0.6f, controlP4.z + GenerateRandomFloat(-5, -1));
	controlP12 = glm::vec3(controlP6.x-GenerateRandomFloat(5, 10), 0.0f, controlP11.z + GenerateRandomFloat(-5, -1));
	controlP13 = glm::vec3(controlP7.x-GenerateRandomFloat(5, 10), 0.0f, controlP12.z + GenerateRandomFloat(-5, -1));

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

		std::vector<VertexFormat> vertices2
		{
			VertexFormat(glm::vec3(controlP4.x, controlP4.y,  controlP4.z), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(controlP7.x, controlP7.y,  controlP7.z), glm::vec3(0, 0, 1))
		};


		std::vector<unsigned short> indices2 =
		{
			0, 1
		};

		meshes["riverSurface2"] = new Mesh("riverSurface2");
		meshes["riverSurface2"]->InitFromData(vertices2, indices2);
		meshes["riverSurface2"]->SetDrawMode(GL_LINES);

		std::vector<VertexFormat> vertices3
		{
			VertexFormat(glm::vec3(controlP4.x, controlP4.y,  controlP4.z), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(controlP10.x, controlP10.y,  controlP10.z), glm::vec3(0, 0, 1))
		};


		std::vector<unsigned short> indices3 =
		{
			0, 1
		};

		meshes["riverSurface3"] = new Mesh("riverSurface3");
		meshes["riverSurface3"]->InitFromData(vertices3, indices3);
		meshes["riverSurface3"]->SetDrawMode(GL_LINES);

		std::vector<VertexFormat> vertices4
		{
			VertexFormat(glm::vec3(controlP4.x, controlP4.y,  controlP4.z), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(controlP13.x, controlP13.y,  controlP13.z), glm::vec3(0, 0, 1))
		};


		std::vector<unsigned short> indices4 =
		{
			0, 1
		};

		meshes["riverSurface4"] = new Mesh("riverSurface4");
		meshes["riverSurface4"]->InitFromData(vertices4, indices4);
		meshes["riverSurface4"]->SetDrawMode(GL_LINES);
		
		{
			TextureManager::LoadTexture(RESOURCE_PATH::TEXTURES, "water.jpg");
		}
	}


}

void NorwegianFjords::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void NorwegianFjords::GenerateRiver(Mesh* mesh, Shader* shader, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4)
{

	glUniform3f(glGetUniformLocation(shader->program, "control_p1"), point1.x, point1.y, point1.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p2"), point2.x, point2.y, point2.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p3"), point3.x, point3.y, point3.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p4"), point4.x, point4.y, point4.z);
	glUniform1i(glGetUniformLocation(shader->program, "instanceNumber"), numberOfBezierInstances);
	glUniform1i(glGetUniformLocation(shader->program, "pointsNumber"), numberOfBezierPoints);




	RenderInstancedMesh(mesh, shader, glm::mat4(1), numberOfBezierInstances);
}
void NorwegianFjords::Update(float deltaTimeSeconds)
{

	ClearScreen();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_BLEND);
	Shader* shader = shaders["SurfaceGeneration"];
	shader->Use();
	TextureManager::GetTexture("water.jpg")->BindToTextureUnit(GL_TEXTURE0);
	TextureManager::GetTexture("water.jpg")->SetWrappingMode(GL_MIRRORED_REPEAT);
	GenerateRiver(meshes["riverSurface"], shader, controlP1, controlP2, controlP3, controlP4);
	GenerateRiver(meshes["riverSurface2"], shader, controlP4, controlP5, controlP6, controlP7);
	GenerateRiver(meshes["riverSurface3"], shader, controlP4, controlP8, controlP9, controlP10);
	GenerateRiver(meshes["riverSurface4"], shader, controlP4, controlP11, controlP12, controlP13);

	
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
