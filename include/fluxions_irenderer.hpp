#ifndef FLUXIONS_IRENDERER_HPP
#define FLUXIONS_IRENDERER_HPP

#include <fluxions_iscene.hpp>

namespace Fluxions
{
	// RENDERER INTERFACE
	class IRenderer
	{
	public:
		virtual bool UploadCompileLink() = 0;
		virtual void Render(const IScene* pScene) = 0;
		virtual void Reset() = 0;

		virtual bool IsConfig(const std::string& name) const = 0;
		virtual bool UseConfig(const std::string& name) = 0;
		virtual bool LoadConfig(const std::string& filename) = 0;
	};
}

#endif