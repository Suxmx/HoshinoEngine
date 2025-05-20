#pragma once
#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Application.h"
#include "Hoshino/Transform.h"

namespace Akane
{
	class SandboxApplication : public Hoshino::Application
	{
	public:
		SandboxApplication()
		{
			m_Camera = std::make_shared<Hoshino::OrthographicCamera>(1.5f, -10.0f, 1.0f);
			m_Camera->SetRotationEuler(0, 0, 0);
			m_Camera->SetPosition(0.2f, 0.1f, 0);
		}
		~SandboxApplication() override = default;

		public:
		   	Hoshino::Ref<Hoshino::Scene> m_Scene;
	};
} // namespace Akane
