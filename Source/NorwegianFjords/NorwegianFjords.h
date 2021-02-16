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
	void GenerateInBetweenBezier(Mesh* mesh, Shader* shader);
	void GenerateBezierSurface(Mesh* mesh, Shader* shader,bool isRiver);
	void GenerateControlPoints();
	float GenerateRandomFloat(float lower, float upper);
	unsigned int UploadCubeMapTexture(const std::string& posx, const std::string& posy, const std::string& posz, const std::string& negx, const std::string& negy, const std::string& negz);
	void CreateFrameBuffer();
	void ResetParticleData();
	glm::vec3 GetNextPoint();
	glm::mat4 GetLookAtMatrix(glm::vec3 targetPosition,glm::vec3 position);
	glm::vec3 GetCubicBezier(float t, glm::vec3 cp1, glm::vec3 cp2, glm::vec3 cp3, glm::vec3 cp4);
	glm::vec3 GetPointOnBezier(int point);

	
	std::unordered_map<std::string, Texture2D*> textures;
	int cubeMapTexture;
	int numberOfBezierInstances = 3;
	int  numberOfBezierPoints=2;
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
	FrameBuffer *frameBuffer;
	unsigned int particleNumber;
	glm::vec3 currentBoatPropellerPosition=glm::vec3(0,0,0);
	glm::mat4 boatFacingMatrix;

	int currentPointIndex = 1;
	std::vector<glm::vec3> controlPoints;
	glm::vec3 nextBoatPoint;
	glm::vec3 lastBoatPoint=glm::vec3(10,10,10);
	float boatSpeed;
	glm::vec3 moveOffset=glm::vec3(0,0,0);
	bool isBoatReturning = false;
	glm::vec3 riverInstanceOffset = glm::vec3(1, 0, 0);
};

