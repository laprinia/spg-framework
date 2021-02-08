#pragma once
#include <Component/SimpleScene.h>

class NorwegianFjords : public SimpleScene
{
public:
	NorwegianFjords();
	~NorwegianFjords();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	void RenderInstancedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int instances);
	void GenerateRiver(Mesh* mesh, Shader* shader,glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4);
	void GenerateControlPoints();
	float GenerateRandomFloat(float lower, float upper);

	int numberOfBezierInstances = 2;
	int  numberOfBezierPoints=10;
	float maxLevelOfTranslate=10.0f;
	glm::vec3 controlP1;
	glm::vec3 controlP2;
	glm::vec3 controlP3;
	glm::vec3 controlP4;
	glm::vec3 controlP5;
	glm::vec3 controlP6;
	glm::vec3 controlP7;
	glm::vec3 controlP8;
	glm::vec3 controlP9;
	glm::vec3 controlP10;

};

