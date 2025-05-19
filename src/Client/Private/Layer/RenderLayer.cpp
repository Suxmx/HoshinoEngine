#include "Akane/Layer/RenderLayer.h"
#include "Akane/SandboxApplication.h"
#include "Hoshino/Application.h"
using namespace Hoshino;

namespace Akane
{
	void RenderLayer::OnAttach()
	{
		m_Shader = Shader::CreateFromFile("Res/Shader/Vert/Vertex.vert", "Res/Shader/Frag/Normal.glsl");
		m_BlueShader = Shader::CreateFromFile("Res/Shader/Vert/Vertex.vert", "Res/Shader/Frag/Blue.glsl");
		m_MeshSource = Hoshino::AssetImporter::ImportMesh("Res/Model/CornellBox-Original.obj");
	}

	void RenderLayer::OnDetach() {}

	void RenderLayer::OnUpdate(Hoshino::Timestep ts)
	{
		SandboxApplication& app = static_cast<SandboxApplication&>(Hoshino::Application::Instance());
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		RenderCommand::Clear();
		Renderer::BeginScene(Hoshino::Application::Instance().GetCamera());
		m_Shader->Bind();
		
		Renderer::RenderStaticMesh(m_MeshSource, m_Shader,app.SqrTransform);
		Renderer::EndScene();
	}

	void RenderLayer::OnImGuiRender() {}

	void RenderLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane