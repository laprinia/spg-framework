#include "NorwegianFjords.h"


NorwegianFjords::NorwegianFjords()
{

}

NorwegianFjords::~NorwegianFjords() {

}

void NorwegianFjords::Init() {

	auto camera = GetSceneCamera();

	camera->SetPositionAndRotation(glm::vec3(0, 5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
	camera->Update();
	{
		Mesh* mesh = new Mesh("bamboo");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation/Bamboo", "bamboo.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	std::string shaderPath = "Source/NorwegianFjords/Shaders/";
	{
		Shader* shader = new Shader("BaseShader");
		shader->AddShader(shaderPath + "VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "GeometryShader.glsl", GL_GEOMETRY_SHADER);
		shader->AddShader(shaderPath + "FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	
}

void NorwegianFjords::FrameStart()
{

}

void NorwegianFjords::Update(float deltaTimeSeconds)
{
	ClearScreen();
	{
		auto shader = shaders["BaseShader"];
		shader->Use();

	    RenderMesh(meshes["bamboo"], shaders["BaseShader"], glm::vec3(-3.3f, 0.5f, 0), glm::vec3(0.1f));
	
	}

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
