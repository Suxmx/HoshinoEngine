#include "Akane/Layer/ClientImguiLayer.h"
#include "Hoshino/Application.h"

namespace Akane
{
	void ClientImguiLayer::OnAttach() {}

	void ClientImguiLayer::OnDetach() {}

	void ClientImguiLayer::OnUpdate(Hoshino::Timestep ts) {}

	void ClientImguiLayer::OnImGuiRender()
	{
		ImGui::Begin("Settings");
        
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
	}

	void ClientImguiLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane