#pragma once
#include "HoshinoPch.h"
#include "Hoshino/Macro.h"
#include "Hoshino/Layer/Layer.h"
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
		unsigned int m_LayerInsertIndex = 0;
	};
} // namespace Hoshino