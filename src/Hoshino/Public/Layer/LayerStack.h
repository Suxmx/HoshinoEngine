#pragma once
#include "Core/HoshinoPch.h"
#include "Core/Macro.h"
#include "Layer/Layer.h"
#include <vector>

namespace Hoshino
{
	class HOSHINO_API LayerStack
	{
	public:
        LayerStack();
        ~LayerStack();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay); // PushOverlay的layer始终在普通的Layer上层
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin()
		{
			return m_Layers.begin();
		}
		std::vector<Layer*>::iterator end()
		{
			return m_Layers.end();
		}

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_NormalLayerTopIt; // 常规Layer的顶层
	};
} // namespace Hoshino