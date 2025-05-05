#include "Hoshino/Application.h"
#include "Hoshino/Log.h"
#include "Hoshino/Macro.h"
#include "Hoshino/Layer/ImGuiLayer.h"
#include "Hoshino/Event/Event.h"
#include "Hoshino/KeyCode.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "imgui.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
namespace Hoshino
{
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach() {}
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_CLASS_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_CLASS_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_CLASS_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_CLASS_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_CLASS_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_CLASS_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_CLASS_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizedEvent>(BIND_CLASS_FN(ImGuiLayer::OnWindowResizeEvent));
	}
	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Instance();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
#pragma region EventHandlers

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButtonCode()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButtonCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetOffsetX();
		io.MouseWheel += e.GetOffsetY();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(HoshinoKeyToImGuiKey(e.GetKeyCode()), true);
		io.AddKeyEvent(ImGuiMod_Shift, (e.GetMods() & GLFW_MOD_SHIFT) != 0);
		io.AddKeyEvent(ImGuiMod_Ctrl, (e.GetMods() & GLFW_MOD_CONTROL) != 0);
		io.AddKeyEvent(ImGuiMod_Alt, (e.GetMods() & GLFW_MOD_ALT) != 0);
		io.AddKeyEvent(ImGuiMod_Super, (e.GetMods() & GLFW_MOD_SUPER) != 0);

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(HoshinoKeyToImGuiKey(e.GetKeyCode()), false);
		io.AddKeyEvent(ImGuiMod_Shift, (e.GetMods() & GLFW_MOD_SHIFT) != 0);
		io.AddKeyEvent(ImGuiMod_Ctrl, (e.GetMods() & GLFW_MOD_CONTROL) != 0);
		io.AddKeyEvent(ImGuiMod_Alt, (e.GetMods() & GLFW_MOD_ALT) != 0);
		io.AddKeyEvent(ImGuiMod_Super, (e.GetMods() & GLFW_MOD_SUPER) != 0);

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(e.GetCharacter());

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

#pragma endregion

} // namespace Hoshino