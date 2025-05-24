#include "Akane/Layer/RenderLayer.h"
#include "Akane/SandboxApplication.h"
#include "Hoshino/Application.h"
#include "Hoshino/Graphics/MeshRenderObject.h"
#include "Hoshino/Graphics/Material.h"
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
		            {Hoshino::TextureFormat::RGB, false},   // gPosition
		            {Hoshino::TextureFormat::RGB, false},   // gNormal
		            {Hoshino::TextureFormat::RGBA, false},  // gSpecular
		            {Hoshino::TextureFormat::Depth, false}, // depth
		        },
		};
		// Hoshino::Texture::Create(TextureSpec(Hoshino::TextureFormat::RGBA, 1, 1));
		// Hoshino::Texture::Create(TextureSpec(Hoshino::TextureFormat::Depth, 1, 1));
		m_GbufferShader = Hoshino::Shader::CreateFromFile("Res/Shader/Vert/vGbuffer.glsl","Res/Shader/Frag/fGbuffer.glsl");
		m_LightingShader =
		    Hoshino::Shader::CreateFromFile("Res/Shader/Vert/vLighting.glsl", "Res/Shader/Frag/fLighting.glsl");
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
		for (const auto& renderObject : app.m_Scene->GetRenderObjects())
		{
			renderObject->Render(m_GbufferShader);
		}
		m_Framebuffer->Unbind();

		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		RenderCommand::Clear();

		for (const auto& renderObject : app.m_Scene->GetRenderObjects())
		{
			for (int i = 0; i < m_MeshSource->m_Submeshes.size(); i++)
			{
				auto& submesh = m_MeshSource->m_Submeshes[i];
				Ref<Material>& material = m_MeshSource->m_Materials[submesh.MaterialIndex];
				Ref<Shader>& meshShader = material->GetShader();

				material->SetShader(m_LightingShader);
				material->Apply();
				material->GetShader()->UploadUniformMat4("u_ViewProjection",
				                                         Renderer::GetRenderData()->ViewProjectionMatrix);
				material->GetShader()->UploadUniformMat4("u_Transform",
				                                         renderObject->TransformRef->GetTransformMatrix());
				m_Framebuffer->GetColorAttachmentTexture(0)->Bind(0);
				m_Framebuffer->GetColorAttachmentTexture(1)->Bind(1);
				m_Framebuffer->GetColorAttachmentTexture(2)->Bind(2);

				material->GetShader()->UploadUniformInt(
				    "gPosition", 0);
				material->GetShader()->UploadUniformInt(
				    "gNormal", 1);
				material->GetShader()->UploadUniformInt(
				    "gAlbedoSpec", 2);
				auto vao = m_MeshSource->GetVertexArray();
				vao->Bind();
				RenderCommand::DrawIndexed(vao, m_MeshSource, i);
				material->SetShader(meshShader);
			}
			Renderer::EndScene();
		}
	}

	void RenderLayer::OnImGuiRender() {}

	void RenderLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane