#include "Akane/Layer/TestLayer.h"

namespace Akane
{
	void TestLayer::OnUpdate()
	{
		APP_TRACE("Testlayer Update");
	}
	void TestLayer::OnEvent(Hoshino::Event& event)
	{
		APP_INFO("Testlayer received a event : {0}", event.ToString());
	}
} // namespace Akane