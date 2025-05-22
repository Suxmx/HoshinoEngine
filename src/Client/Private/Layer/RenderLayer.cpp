#include "Akane/Layer/RenderLayer.h"
#include "Akane/SandboxApplication.h"
#include "Hoshino/Application.h"
#include "Hoshino/Graphics/MeshRenderObject.h"
#include "glm/fwd.hpp"
using namespace Hoshino;

namespace Akane
{
	void RenderLayer::OnAttach()
	{
		SandboxApplication& app = static_cast<SandboxApplication&>(Hoshino::Application::Instance());
		app.m_Scene = CreateRef<Scene>();
		// m_MeshSource = Hoshino::AssetImporter::ImportMesh("Res/Model/CornellBox-Original.obj");
		m_MeshSource = Hoshino::AssetImporter::ImportMesh("Res/Model/backpack.obj");
		auto meshRo = CreateRef<MeshRenderObject>(m_MeshSource);
		meshRo->TransformRef->SetScale(glm::vec3(0.1f,0.1f,0.1f));
		meshRo->TransformRef->SetRotation(glm::vec3(0, 45, 0));
		app.m_Scene->PushRenderObject(meshRo);

		Hoshino::FrameBufferSpec fboSpec = Hoshino::FrameBufferSpec{
		    .Width = app.GetWindow().GetWidth(),
		    .Height = app.GetWindow().GetHeight(),
		    .AttachmentsSpc =
		        Hoshino::FrameBufferAttachmentSpec{
		            {Hoshino::FrameBufferTextureFormat::RGB, false},   // gPosition
		            {Hoshino::FrameBufferTextureFormat::RGB, false},   // gNormal
		            {Hoshino::FrameBufferTextureFormat::RGBA, false},  // gSpecular
		            {Hoshino::FrameBufferTextureFormat::Depth, false}, // depth
		        },
		};
		m_GbufferShader = Hoshino::Shader::CreateFromFile("Res/Shader/Vert/vGbuffer.glsl","Res/Shader/Frag/fGbuffer.glsl");
		// m_LightingShader =
		//     Hoshino::Shader::Create("Res/Shader/Vert/vLighting.glsl", "Res/Shader/fLighting.glsl");
		m_Framebuffer = Hoshino::Framebuffer::Create(fboSpec);
		// m_Framebuffer=Framebuffer::Create()
	}

	void RenderLayer::OnDetach() {}

	void RenderLayer::OnUpdate(Hoshino::Timestep ts)
	{
		SandboxApplication& app = static_cast<SandboxApplication&>(Hoshino::Application::Instance());
		Renderer::BeginScene(Hoshino::Application::Instance().GetCamera());
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		RenderCommand::Clear();
		for(const auto& renderObject : app.m_Scene->GetRenderObjects())
		{
			renderObject->Render(m_GbufferShader);
		}
		m_Framebuffer->Unbind();

		Renderer::EndScene();
	}

	void RenderLayer::OnImGuiRender() {}

	void RenderLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane