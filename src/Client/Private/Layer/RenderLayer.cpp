#include "Akane/Layer/RenderLayer.h"
#include "Akane/SandboxApplication.h"
#include "Hoshino/Application.h"
#include "Hoshino/Graphics/MeshRenderObject.h"
using namespace Hoshino;

namespace Akane
{
	void RenderLayer::OnAttach()
	{
		SandboxApplication& app = static_cast<SandboxApplication&>(Hoshino::Application::Instance());
		app.m_Scene = CreateRef<Scene>();
		m_Shader = Shader::CreateFromFile("Res/Shader/Vert/Vertex.vert", "Res/Shader/Frag/Normal.glsl");
		m_MeshSource = Hoshino::AssetImporter::ImportMesh("Res/Model/backpack.obj");
		auto meshRo = CreateRef<MeshRenderObject>(m_MeshSource);
		app.m_Scene->PushRenderObject(meshRo);
	}

	void RenderLayer::OnDetach() {}

	void RenderLayer::OnUpdate(Hoshino::Timestep ts)
	{
		SandboxApplication& app = static_cast<SandboxApplication&>(Hoshino::Application::Instance());
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		RenderCommand::Clear();
		Renderer::BeginScene(Hoshino::Application::Instance().GetCamera());
		app.m_Scene->Render();
		Renderer::EndScene();
	}

	void RenderLayer::OnImGuiRender() {}

	void RenderLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane