#include "Akane/Layer/ClientImguiLayer.h"
#include "Akane/SandboxApplication.h"
#include "Hoshino/Application.h"
#include "Hoshino/Transform.h"
#include <memory>

namespace Akane
{
	void ClientImguiLayer::OnAttach() {}

	void ClientImguiLayer::OnDetach() {}

	void ClientImguiLayer::OnUpdate(Hoshino::Timestep ts) {
        m_Time = ts;
    }

	void ClientImguiLayer::OnImGuiRender()
	{
		ImGui::Begin("Settings");
        ImGui::Text("Time: %.3f", m_Time);
        ImGui::Text("FPS: %.1f", 1.0f / m_Time);
        ImGui::Text("Camera Position");
        auto pos = Hoshino::Application::Instance().GetCamera()->GetPosition();
        ImGui::SliderFloat("X", &pos.x, -3.0f, 3.0f);
        ImGui::SliderFloat("Y", &pos.y, -3.0f, 3.0f);
        ImGui::SliderFloat("Z", &pos.z, -3.0f, 3.0f);
        Hoshino::Application::Instance().GetCamera()->SetPosition(pos);
        ImGui::Text("Camera Rotation");
        auto rot = Hoshino::Application::Instance().GetCamera()->GetRotationEuler();
        ImGui::SliderFloat("Pitch", &rot.x, -180.0f, 180.0f);
        ImGui::SliderFloat("Yaw", &rot.y, -180.0f, 180.0f);
        ImGui::SliderFloat("Roll", &rot.z, -180.0f, 180.0f);
        Hoshino::Application::Instance().GetCamera()->SetRotationEuler(rot);

		ImGui::End();

        Akane::SandboxApplication& app = static_cast<Akane::SandboxApplication&>(Hoshino::Application::Instance());
        ImGui::Begin("Mesh");
        
        // ImGui::Text("Position");
        // Hoshino::Transform& triTransform = app.TriTransform;
        // pos=triTransform.GetPosition();
        // ImGui::SliderFloat("X##1", &pos.x, -3.0f, 3.0f);
        // ImGui::SliderFloat("Y##1", &pos.y, -3.0f, 3.0f);
        // ImGui::SliderFloat("Z##1", &pos.z, -3.0f, 3.0f);
        // triTransform.SetPosition(pos);
        // ImGui::Text("Rotation");
        // rot = triTransform.GetRotation();
        // ImGui::SliderFloat("Pitch##1", &rot.x, -180.0f, 180.0f);
        // ImGui::SliderFloat("Yaw##1", &rot.y, -180.0f, 180.0f);
        // ImGui::SliderFloat("Roll##1", &rot.z, -180.0f, 180.0f);
        // triTransform.SetRotation(rot);
        // ImGui::Text("Scale");
        // auto scale = triTransform.GetScale();
        // ImGui::SliderFloat("X##3", &scale.x, 0.0f, 3.0f);
        // ImGui::SliderFloat("Y##3", &scale.y, 0.0f, 3.0f);
        // ImGui::SliderFloat("Z##3", &scale.z, 0.0f, 3.0f);
        // triTransform.SetScale(scale);
        ImGui::Text("Square");
		auto sqrTransform = app.m_Scene->GetRenderObjects()[0]->TransformRef;
		pos=sqrTransform->GetPosition();
        ImGui::SliderFloat("X##2", &pos.x, -3.0f, 3.0f);
        ImGui::SliderFloat("Y##2", &pos.y, -3.0f, 3.0f);
        ImGui::SliderFloat("Z##2", &pos.z, -3.0f, 3.0f);
        sqrTransform->SetPosition(pos);
        ImGui::Text("Rotation");
        rot = sqrTransform->GetRotation();
        ImGui::SliderFloat("Pitch##2", &rot.x, -180.0f, 180.0f);
        ImGui::SliderFloat("Yaw##2", &rot.y, -180.0f, 180.0f);
        ImGui::SliderFloat("Roll##2", &rot.z, -180.0f, 180.0f);
        sqrTransform->SetRotation(rot);
        ImGui::Text("Scale");
        glm::vec3 scale = sqrTransform->GetScale();
        ImGui::SliderFloat("X##4", &scale.x, 0.0f, 3.0f);
        ImGui::SliderFloat("Y##4", &scale.y, 0.0f, 3.0f);
        ImGui::SliderFloat("Z##4", &scale.z, 0.0f, 3.0f);
        sqrTransform->SetScale(scale);

		ImGui::End();
	}

	void ClientImguiLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane