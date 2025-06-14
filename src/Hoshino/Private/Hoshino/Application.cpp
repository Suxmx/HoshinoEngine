#include "Hoshino/Application.h"

#include "Hoshino/Log.h"
#include "Hoshino/Renderer/DeviceManager.h"
#include "Hoshino/Renderer/ShaderCompiler.h"
#include "Hoshino/Renderer/ShaderFactory.h"

#include "Hoshino/TimeStep.h"
#include <GLFW/glfw3.h>
#include <nvrhi/utils.h>

#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Hoshino
{
	Application* Application::s_Instance;

	Application::Application()
	{
		CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->Init();
		m_Window->SetEventCallbackFn(BIND_APP_EVENT_FN(OnEvent));

		m_CommandList=GetDeviceManager()->GetDevice()->createCommandList();
		// m_Window->SetEventCallbackFn(BIND_APP_EVENT_FN(OnEvent));
		// m_Running = true;

		// m_ImGuiLayer = new ImGuiLayer();
		// PushOverlay(m_ImGuiLayer);

		ShaderCompileDesc vertDesc{.filePath = "Res/Shader/Hlsl/triangle_vert.hlsl",
		                           .outputPath = ".\\CompiledShader/triangle_vert.spv",
		                           .compilerType = ShaderCompiler::CompilerType::SPIRV,
		                           .shaderType = ShaderType::Vertex,
		                           .logCmd = true};

		ShaderCompileDesc fragDesc{.filePath = "Res/Shader/Hlsl/triangle_frag.hlsl",
		                           .outputPath = ".\\CompiledShader/triangle_frag.spv",
		                           .compilerType = ShaderCompiler::CompilerType::SPIRV,
		                           .shaderType = ShaderType::Pixel,
		                           .logCmd = true};
		auto shaderFactory = ShaderFactory(GetDeviceManager()->GetDevice());
		m_VertexShader = shaderFactory.CreateShaderFromSource(vertDesc,"main");
		m_PixelShader = shaderFactory.CreateShaderFromSource(fragDesc,"main");
	}

	Application::~Application() {}

	void Application::Run()
	{
		while (m_Running)
		{
			static int frameCount = 0;
			
			// 开始帧并获取命令列表
			GetDeviceManager()->BeginFrame();
			auto framebuffer = GetDeviceManager()->GetCurrentFramebuffer();
			if (!m_Pipeline)
			{
				nvrhi::GraphicsPipelineDesc psoDesc;
				psoDesc.VS = m_VertexShader;
				psoDesc.PS = m_PixelShader;
				psoDesc.primType = nvrhi::PrimitiveType::TriangleList;
				psoDesc.renderState.depthStencilState.depthTestEnable = false;

				m_Pipeline = GetDeviceManager()->GetDevice()->createGraphicsPipeline(psoDesc, framebuffer);
			}

			m_CommandList->open();

			nvrhi::utils::ClearColorAttachment(m_CommandList, framebuffer, 0, nvrhi::Color(0.f));

			nvrhi::GraphicsState state;
			state.pipeline = m_Pipeline;
			state.framebuffer = framebuffer;
			state.viewport.addViewportAndScissorRect(framebuffer->getFramebufferInfo().getViewport());

			m_CommandList->setGraphicsState(state);

			nvrhi::DrawArguments args;
			args.vertexCount = 3;
			m_CommandList->draw(args);

			m_CommandList->close();
			GetDeviceManager()->GetDevice()->executeCommandList(m_CommandList);

			// CORE_TRACE("Frame: {0}", frameCount++);

			// 显示帧
			GetDeviceManager()->Present();
			m_Window->UpdateWindowSize();
			glfwPollEvents();
		}

		
		// ShaderCompiler::CompileShaderToPath(vertDesc);
		// ShaderCompileDesc fragDesc{.filePath = "Res/Shader/Hlsl/frag.hlsl",
		//                            .outputPath = "CompiledShader/frag.bin",
		//                            .compilerType = ShaderCompiler::CompilerType::SPIRV,
		//                            .shaderType = ShaderType::Pixel,
		//                            .generateHeader = true};
		// ShaderCompiler::CompileShaderToPath(fragDesc);
		//CORE_INFO("Engine ShutDown!");
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_APP_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizedEvent>(BIND_APP_EVENT_FN(OnWindowResize));

		if (event.Handled) return;

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled) break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
	
	bool Application::OnWindowResize(WindowResizedEvent& event)
	{
		return true;
	}

#pragma region LayerStack
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}
#pragma endregion
} // namespace Hoshino