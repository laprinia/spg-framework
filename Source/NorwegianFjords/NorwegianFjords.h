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
	void GenerateBezierSurface(Mesh* mesh, Shader* shader,bool isRiver);
	void GenerateControlPoints();
	float GenerateRandomFloat(float lower, float upper);
	unsigned int UploadCubeMapTexture(const std::string& posx, const std::string& posy, const std::string& posz, const std::string& negx, const std::string& negy, const std::string& negz);

	std::unordered_map<std::string, Texture2D*> textures;
	int cubeMapTexture;
	int numberOfBezierInstances = 3;
	int  numberOfBezierPoints=3;
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
	glm::vec3 controlP11;
	glm::vec3 controlP12;
	glm::vec3 controlP13;

};

