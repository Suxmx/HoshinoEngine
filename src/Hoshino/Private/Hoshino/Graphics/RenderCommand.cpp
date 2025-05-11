#include "Hoshino/Graphics/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"
namespace Hoshino
{
	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI;
}