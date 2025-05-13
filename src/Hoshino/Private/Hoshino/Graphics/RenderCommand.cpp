#include "Hoshino/Graphics/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"
namespace Hoshino
{
	Ref<RenderAPI> RenderCommand::s_RenderAPI = Ref<RenderAPI>(new OpenGLRenderAPI());
}