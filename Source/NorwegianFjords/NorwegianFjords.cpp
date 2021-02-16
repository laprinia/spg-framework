#include "NorwegianFjords.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stb/stb_image.h>
#include <glm/gtx/quaternion.hpp>

struct WaterParticle
{
	glm::vec4 position;
	glm::vec4 speed;
	glm::vec4 initialPosition;
	glm::vec4 initialSpeed;

	WaterParticle() {};

	WaterParticle(const glm::vec4& position, const glm::vec4& speed)
	{
		SetInitial(position, speed);
	}

	void SetInitial(const glm::vec4& positionPassed, const glm::vec4& speed)
	{
		position = positionPassed;
		initialPosition = position;

		this->speed = speed;
		initialSpeed = speed;
	}
};

ParticleEffect<WaterParticle>* particleEffect;
SSBO <WaterParticle>* particleSSBO;

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
	controlP2 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP1.z + GenerateRandomFloat(-5, -1));
	controlP3 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP2.z + GenerateRandomFloat(-5, -1));
	controlP4 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP3.z + GenerateRandomFloat(-5, -1));
	//second branch
	controlP5 = glm::vec3(GenerateRandomFloat(-5, 5), 0.2f, controlP4.z + GenerateRandomFloat(-5, -1));
	controlP6 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP5.z + GenerateRandomFloat(-5, -1));
	controlP7 = glm::vec3(GenerateRandomFloat(-5, 5), 0, controlP6.z + GenerateRandomFloat(-5, -1));
	//third branch
	controlP8 = glm::vec3(controlP5.x + GenerateRandomFloat(4, 7), 0.4f, controlP4.z + GenerateRandomFloat(-5, -1));
	controlP9 = glm::vec3(controlP6.x + GenerateRandomFloat(4, 7), 0.0f, controlP8.z + GenerateRandomFloat(-5, -1));
	controlP10 = glm::vec3(controlP7.x + GenerateRandomFloat(4, 7), 0.0f, controlP9.z + GenerateRandomFloat(-5, -1));
	//fourth branch
	controlP11 = glm::vec3(controlP5.x - GenerateRandomFloat(4, 7), 0.6f, controlP4.z + GenerateRandomFloat(-5, -1));
	controlP12 = glm::vec3(controlP6.x - GenerateRandomFloat(4, 7), 0.0f, controlP11.z + GenerateRandomFloat(-5, -1));
	controlP13 = glm::vec3(controlP7.x - GenerateRandomFloat(4, 7), 0.0f, controlP12.z + GenerateRandomFloat(-5, -1));

	controlPoints.push_back(controlP1); controlPoints.push_back(controlP2); controlPoints.push_back(controlP3);
	controlPoints.push_back(controlP4); controlPoints.push_back(controlP5); controlPoints.push_back(controlP6);
	controlPoints.push_back(controlP7); controlPoints.push_back(controlP8); controlPoints.push_back(controlP9);
	controlPoints.push_back(controlP10); controlPoints.push_back(controlP11); controlPoints.push_back(controlP12);
	controlPoints.push_back(controlP13);
}

void NorwegianFjords::Init() {

	CreateFrameBuffer();
	GenerateControlPoints();
	lastBoatPoint = GetPointOnBezier(0);

	nextBoatPoint = GetNextPoint();
	boatFacingMatrix = GetLookAtMatrix(nextBoatPoint + riverInstanceOffset, lastBoatPoint);
	auto camera = GetSceneCamera();

	camera->SetPositionAndRotation(glm::vec3(0, 5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
	camera->Update();



	std::string shaderPath = "Source/NorwegianFjords/Shaders/";
	{
		Shader* shader = new Shader("Regular");
		shader->AddShader(shaderPath + "RegularVS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "RegularFS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader* shader = new Shader("Skybox");
		shader->AddShader(shaderPath + "SkyboxVS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "SkyboxFS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("SurfaceGeneration");
		shader->AddShader(shaderPath + "BezierGeneratorVS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "BezierGeneratorGS.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader(shaderPath + "BezierGeneratorFS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader* shader = new Shader("InBetween");
		shader->AddShader(shaderPath + "BezierInBetweenVS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "BezierInBetweenGS.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader(shaderPath + "BezierInBetweenFS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("DepthOfField");
		shader->AddShader(shaderPath + "DepthOfFieldVS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "DepthOfFieldFS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader* shader = new Shader("WaterParticle");
		shader->AddShader(shaderPath + "WaterParticleVS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "WaterParticleGS.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader(shaderPath + "WaterParticleFS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}



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

		{
			Texture2D* texture = new Texture2D();
			texture->Load2D((RESOURCE_PATH::TEXTURES + "water.jpg").c_str(), GL_REPEAT);
			textures["water"] = texture;

			Texture2D* texture2 = new Texture2D();
			texture2->Load2D((RESOURCE_PATH::TEXTURES + "ground.jpg").c_str(), GL_REPEAT);
			textures["ground"] = texture2;

			Texture2D* texture3 = new Texture2D();
			texture3->Load2D((RESOURCE_PATH::TEXTURES + "boat.png").c_str(), GL_REPEAT);
			textures["boat"] = texture3;

			Texture2D* texture4 = new Texture2D();
			texture4->Load2D((RESOURCE_PATH::TEXTURES + "waterParticle.png").c_str(), GL_REPEAT);
			textures["waterParticle"] = texture4;

		}
		{
			Mesh* mesh = new Mesh("cube");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
			mesh->UseMaterials(false);
			meshes[mesh->GetMeshID()] = mesh;
		}
		{
			Mesh* mesh = new Mesh("quad");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "quad.obj");
			mesh->UseMaterials(false);
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Mesh* mesh = new Mesh("boat");
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Models", "boat.obj");
			mesh->UseMaterials(false);
			meshes[mesh->GetMeshID()] = mesh;
		}

		std::string cubeTexturePath = RESOURCE_PATH::TEXTURES + "Norwegian/";
		cubeMapTexture = UploadCubeMapTexture(
			cubeTexturePath + "posx.png",
			cubeTexturePath + "posy.png",
			cubeTexturePath + "posz.png",
			cubeTexturePath + "negx.png",
			cubeTexturePath + "negy.png",
			cubeTexturePath + "negz.png"
		);
	}
	{
		particleNumber = 4'000;
		particleEffect = new ParticleEffect<WaterParticle>();
		particleEffect->Generate(particleNumber, true);
		particleSSBO = particleEffect->GetParticleBuffer();


	}

}

void NorwegianFjords::FrameStart()
{


	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}


void NorwegianFjords::GenerateBezierSurface(Mesh* mesh, Shader* shader, bool isRiver)
{

	glUniform3f(glGetUniformLocation(shader->program, "control_p1"), controlP1.x, controlP1.y, controlP1.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p2"), controlP2.x, controlP2.y, controlP2.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p3"), controlP3.x, controlP3.y, controlP3.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p4"), controlP4.x, controlP4.y, controlP4.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p5"), controlP5.x, controlP5.y, controlP5.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p6"), controlP6.x, controlP6.y, controlP6.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p7"), controlP7.x, controlP7.y, controlP7.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p8"), controlP8.x, controlP8.y, controlP8.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p9"), controlP9.x, controlP9.y, controlP9.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p10"), controlP10.x, controlP10.y, controlP10.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p11"), controlP11.x, controlP11.y, controlP11.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p12"), controlP12.x, controlP12.y, controlP12.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p13"), controlP13.x, controlP13.y, controlP13.z);

	glUniform1i(glGetUniformLocation(shader->program, "instanceNumber"), numberOfBezierInstances);
	glUniform1i(glGetUniformLocation(shader->program, "pointsNumber"), numberOfBezierPoints);


	//water text
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textures["water"]->GetTextureID());

	glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

	//ground text
	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, textures["ground"]->GetTextureID());

	glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);


	//cubemap
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	int loc_texture = shader->GetUniformLocation("texture_cubemap");
	glUniform1i(loc_texture, 2);

	auto cameraPosition = GetSceneCamera()->transform->GetWorldPosition();
	int loc_camera = shader->GetUniformLocation("camera_position");
	glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);



	RenderInstancedMesh(mesh, shader, glm::mat4(1), numberOfBezierInstances);
}

void NorwegianFjords::GenerateInBetweenBezier(Mesh* mesh, Shader* shader)
{

	glUniform3f(glGetUniformLocation(shader->program, "control_p1"), controlP1.x, controlP1.y, controlP1.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p2"), controlP2.x, controlP2.y, controlP2.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p3"), controlP3.x, controlP3.y, controlP3.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p4"), controlP4.x, controlP4.y, controlP4.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p5"), controlP5.x, controlP5.y, controlP5.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p6"), controlP6.x, controlP6.y, controlP6.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p7"), controlP7.x, controlP7.y, controlP7.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p8"), controlP8.x, controlP8.y, controlP8.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p9"), controlP9.x, controlP9.y, controlP9.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p10"), controlP10.x, controlP10.y, controlP10.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p11"), controlP11.x, controlP11.y, controlP11.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p12"), controlP12.x, controlP12.y, controlP12.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p13"), controlP13.x, controlP13.y, controlP13.z);

	glUniform1i(glGetUniformLocation(shader->program, "instanceNumber"), numberOfBezierInstances);
	glUniform1i(glGetUniformLocation(shader->program, "pointsNumber"), numberOfBezierPoints);


	//water text
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textures["ground"]->GetTextureID());

	glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

	RenderInstancedMesh(mesh, shader, glm::mat4(1), 1);
}
void NorwegianFjords::CreateFrameBuffer()
{
	auto resolution = window->GetResolution();
	frameBuffer = new FrameBuffer();
	frameBuffer->Generate(resolution.x, resolution.y, 1);

}

void NorwegianFjords::ResetParticleData() {

	//std::cout << "resetting data to " << currentBoatPropellerPosition.x;
	WaterParticle* particleData = const_cast<WaterParticle*>(particleSSBO->GetBuffer());
	int cubeSize = 20;
	int hSize = cubeSize / 2;

	for (unsigned int i = 0; i < particleNumber; i++)
	{
		glm::vec4 pos(1);
		pos.x = currentBoatPropellerPosition.x + (rand() % cubeSize - hSize) / 30.0f;
		pos.y = currentBoatPropellerPosition.y + (rand() % cubeSize - hSize) / 30.0f;
		pos.z = currentBoatPropellerPosition.z + (rand() % cubeSize - hSize) / 30.0f;

		glm::vec4 speed(0);
		speed.x = (rand() % 5) / 100.0f;
		speed.z = (rand() % 5) / 100.0f;
		speed.y = (rand() % 20 - 10) / 10.0f;

		particleData[i].SetInitial(pos, speed);
	}

	particleSSBO->SetBufferData(particleData);
}
glm::vec3 NorwegianFjords::GetNextPoint() {
	if (currentPointIndex == 4) {
		std::vector<int> randomOptions;
		randomOptions.push_back(5); randomOptions.push_back(8); randomOptions.push_back(11);
		currentPointIndex = randomOptions[(rand() % 3)];
		isBoatReturning = false;
	}
	else if (currentPointIndex == 7 || currentPointIndex == 10 || currentPointIndex == 13)
	{
		currentPointIndex--;
		isBoatReturning = true;
	}
	else if ((currentPointIndex == 5 && isBoatReturning) || (currentPointIndex == 8 && isBoatReturning) || (currentPointIndex == 11 && isBoatReturning)) {
		currentPointIndex = 4;
		isBoatReturning = false;
	}
	else if (currentPointIndex > 4 && isBoatReturning) {
		currentPointIndex--;

	}
	else {
		currentPointIndex++;
		isBoatReturning = false;
	}
	std::cout << currentPointIndex << std::endl;
	return GetPointOnBezier(currentPointIndex - 1);
}

glm::mat4 NorwegianFjords::GetLookAtMatrix(glm::vec3 targetPosition, glm::vec3 position) {
	glm::vec3 targetVector = glm::normalize(targetPosition - position);
	glm::quat rotationInQuaternions = glm::rotation(glm::vec3(0, 0, 1), targetVector);
	glm::mat4 rotationMatrix = glm::toMat4(rotationInQuaternions);
	return rotationMatrix;
}
glm::vec3 NorwegianFjords::GetCubicBezier(float t, glm::vec3 cp1, glm::vec3 cp2, glm::vec3 cp3, glm::vec3 cp4)
{
	return cp1 * glm::vec3(pow((1 - t), 3)) + cp2 * glm::vec3(3) * t * glm::vec3(pow((1 - t), 2)) + cp3 * glm::vec3(3) * glm::vec3(pow(t, 2)) * (1 - t) + cp4 * glm::vec3(pow(t, 3));

}
glm::vec3 NorwegianFjords::GetPointOnBezier(int point)
{
	glm::vec3 newPoint;
	float t;
	bool isEven = ((point + 1) % 2) == 0;
	if (point <= 3) {
		t = point < 2 ? 0 : 0.5;
		newPoint = GetCubicBezier(isEven ? t + 1.0 / numberOfBezierPoints : t, controlP1, controlP2, controlP3, controlP4);

	}
	else if (point <= 6) {

		t = point < 5 ? 0 : 0.5;
		newPoint = GetCubicBezier(!isEven ? t + 1.0 / numberOfBezierPoints : t, controlP4, controlP5, controlP6, controlP7);
	}
	else if (point <= 9) {

		t = point < 8 ? 0 : 0.5;
		newPoint = GetCubicBezier(isEven ? t + 1.0 / numberOfBezierPoints : t, controlP4, controlP8, controlP9, controlP10);
	}
	else {

		t = point < 11 ? 0 : 0.5;
		newPoint = GetCubicBezier(!isEven ? t + 1.0 / numberOfBezierPoints : t, controlP4, controlP11, controlP12, controlP13);
	}

	return newPoint;
}

void NorwegianFjords::Update(float deltaTimeSeconds)
{
	float speed = 0.1;
	ClearScreen();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	frameBuffer->Bind();

	{
		Shader* shader = shaders["Skybox"];
		shader->Use();

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(100));

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		int loc_texture = shader->GetUniformLocation("texture_cubemap");
		glUniform1i(loc_texture, 0);
		meshes["cube"]->Render();

	}

	{
		Shader* shader = shaders["SurfaceGeneration"];
		shader->Use();
		GenerateBezierSurface(meshes["riverSurface"], shader, true);

	}
	{
		Shader* shader = shaders["InBetween"];
		shader->Use();
		GenerateInBetweenBezier(meshes["riverSurface"], shader);
	}

	{
		Shader* shader = shaders["Regular"];
		shader->Use();
		//CRUISE

		glm::vec3 zForward = glm::normalize((nextBoatPoint + riverInstanceOffset) - lastBoatPoint);
		moveOffset += zForward * deltaTimeSeconds;
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1), moveOffset + riverInstanceOffset);
		modelMatrix = modelMatrix * boatFacingMatrix;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3));

		if (distance(lastBoatPoint, nextBoatPoint + riverInstanceOffset) <= 0.3) {

			nextBoatPoint = GetNextPoint();

		}
		boatFacingMatrix = GetLookAtMatrix(nextBoatPoint + riverInstanceOffset, lastBoatPoint);
		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures["boat"]->GetTextureID());
		int loc_texture = shader->GetUniformLocation("texture_1");
		glUniform1i(loc_texture, 0);

		meshes["boat"]->Render();

		glm::vec3 propellerPoint = glm::vec3((lastBoatPoint).x + 1.0, (lastBoatPoint).y + 0.5, (lastBoatPoint).z + 0.7);
		if (distance(propellerPoint, currentBoatPropellerPosition) > 0.3) {
			currentBoatPropellerPosition = propellerPoint;
			ResetParticleData();
		}
		lastBoatPoint = moveOffset;
	}
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	{

		Shader* shader = shaders["WaterParticle"];
		shader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures["waterParticle"]->GetTextureID());
		int loc_texture = shader->GetUniformLocation("texture_1");
		glUniform1i(loc_texture, 0);

		particleEffect->Render(GetSceneCamera(), shader);
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// glDepthMask(GL_TRUE);


	 //DoF
	{
		frameBuffer->BindDefault();
		auto shader = shaders["DepthOfField"];
		shader->Use();

		{
			int textureColorLoc = shader->GetUniformLocation("colorTexture");
			glUniform1i(textureColorLoc, 0);
			frameBuffer->BindTexture(0, GL_TEXTURE0);
		}

		{
			int textureDepthLoc = shader->GetUniformLocation("depthTexture");
			glUniform1i(textureDepthLoc, 1);
			frameBuffer->BindDepthTexture(GL_TEXTURE1);
		}
		{
			glUniform2f(glGetUniformLocation(shader->program, "screenSize"), window->GetResolution().x, window->GetResolution().y);
		}

		RenderMesh(meshes["quad"], shader, glm::vec3(0, 0, 0));

	}

}

unsigned int NorwegianFjords::UploadCubeMapTexture(const std::string& posx, const std::string& posy, const std::string& posz, const std::string& negx, const std::string& negy, const std::string& negz)
{
	int width, height, chn;

	unsigned char* data_posx = stbi_load(posx.c_str(), &width, &height, &chn, 0);
	unsigned char* data_posy = stbi_load(posy.c_str(), &width, &height, &chn, 0);
	unsigned char* data_posz = stbi_load(posz.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negx = stbi_load(negx.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negy = stbi_load(negy.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negz = stbi_load(negz.c_str(), &width, &height, &chn, 0);

	// TODO - create OpenGL texture
	unsigned int textureID = 0;
	glGenTextures(1, &textureID);

	// TODO - bind the texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// TODO - load texture information for each face
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posx);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negx);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posy);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negy);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_posz);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_negz);


	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	SAFE_FREE(data_posx);
	SAFE_FREE(data_posy);
	SAFE_FREE(data_posz);
	SAFE_FREE(data_negx);
	SAFE_FREE(data_negy);
	SAFE_FREE(data_negz);

	return textureID;
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
	//rawCoordinatSystem();
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
	frameBuffer->Resize(width, height, 32);
}
