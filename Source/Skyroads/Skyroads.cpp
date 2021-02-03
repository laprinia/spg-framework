
#include "Skyroads.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

Skyroads::Skyroads(){

}

Skyroads::~Skyroads() {

}

void Skyroads::Init() {
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(0, 8, 8), glm::vec3(-40 * TO_RADIANS, 0, 0));
	camera->Update();

	//ToggleGroundPlane();

	{
		Shader* shader = new Shader("SurfaceGeneration");
		shader->AddShader("Source/Laboratoare/Skyroads/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Skyroads/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

}

void Skyroads::FrameStart() {

	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 1, 0,0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

}

void Skyroads::Update(float deltaTimeSeconds) {
	ClearScreen();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader* shader = shaders["SurfaceGeneration"];
	shader->Use();

}

void Skyroads::FrameEnd() {

}

void Skyroads::OnInputUpdate(float deltaTime, int mods) {

}

void OnKeyPress(int key, int mods) {

}



