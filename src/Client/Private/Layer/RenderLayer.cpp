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
		auto meshRo1 = CreateRef<MeshRenderObject>(m_MeshSource);
		auto meshRo2 = CreateRef<MeshRenderObject>(m_MeshSource);
		auto meshRo3 = CreateRef<MeshRenderObject>(m_MeshSource);
		meshRo1->TransformRef->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		meshRo1->TransformRef->SetRotation(glm::vec3(0, 45, 0));
		meshRo1->TransformRef->SetPosition(glm::vec3(-0.6, 0, 0));
		app.m_Scene->PushRenderObject(meshRo1);
		meshRo2->TransformRef->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		meshRo2->TransformRef->SetRotation(glm::vec3(0, 0, 0));
		meshRo2->TransformRef->SetPosition(glm::vec3(0, 0, 0));
		app.m_Scene->PushRenderObject(meshRo2);
		meshRo3->TransformRef->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		meshRo3->TransformRef->SetRotation(glm::vec3(0, -45, 0));
		meshRo3->TransformRef->SetPosition(glm::vec3(0.6, 0, 0));
		app.m_Scene->PushRenderObject(meshRo3);

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
		m_ScreenMeshSource = Hoshino::MeshSource::CreateScreenQuad();
		    // m_Framebuffer=Framebuffer::Create()
		    // lighting info
		    // -------------
		    const unsigned int NR_LIGHTS = 3;

		// 复用learn opengl 
		srand(13);
		m_LightPositions.push_back(glm::vec3(0, 0, 1.5));
		m_LightPositions.push_back(glm::vec3(-1.5, 1, 1.5));
		m_LightPositions.push_back(glm::vec3(1.5, -1, 1.5));

		m_LightColors.push_back(glm::vec3(1.0, 0, 0)); 
		m_LightColors.push_back(glm::vec3(0, 1, 0)); 
		m_LightColors.push_back(glm::vec3(0, 0, 1)); 
		// for (unsigned int i = 0; i < NR_LIGHTS; i++)
		// {
		// 	m_LightPositions.push_back(glm::vec3(-2.0+4.0/(NR_LIGHTS-1), 0, 1));

		// 	float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		// 	float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		// 	float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		// 	m_LightColors.push_back(glm::vec3(rColor, gColor, bColor));
		// }

		m_LightingShader->Bind();
		for (unsigned int i = 0; i < m_LightPositions.size(); i++)
		{
			m_LightingShader->UploadUniformFloat3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
			m_LightingShader->UploadUniformFloat3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);

			const float linear = 0.7f;
			const float quadratic = 1.8f;
			m_LightingShader->UploadUniformFloat("lights[" + std::to_string(i) + "].Linear", linear);
			m_LightingShader->UploadUniformFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
		}
		m_LightingShader->UploadUniformInt("gPosition", 0);
		m_LightingShader->UploadUniformInt("gNormal", 1);
		m_LightingShader->UploadUniformInt("gAlbedoSpec", 2);
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

		m_LightingShader->Bind();
		m_LightingShader->UploadUniformFloat3("u_ViewPos", Renderer::GetRenderData()->ViewPos);
		m_Framebuffer->GetColorAttachmentTexture(0)->Bind(0);
		m_Framebuffer->GetColorAttachmentTexture(1)->Bind(1);
		m_Framebuffer->GetColorAttachmentTexture(2)->Bind(2);
		m_ScreenMeshSource->GetVertexArray()->Bind();
		auto vao = m_ScreenMeshSource->GetVertexArray();
		RenderCommand::DrawIndexed(vao, m_ScreenMeshSource, 0);
		Renderer::EndScene();
	}

	void RenderLayer::OnImGuiRender() {}

	void RenderLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane