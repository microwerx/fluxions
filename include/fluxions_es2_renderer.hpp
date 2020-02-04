#ifndef FLUXIONS_ES2_RENDERER_HPP
#define FLUXIONS_ES2_RENDERER_HPP

#include <fluxions_base.hpp>
#include <fluxions_irenderer.hpp>

namespace Fluxions {
	class ES2Renderer : public IRenderer {
	public:
		ES2Renderer();
		virtual ~ES2Renderer();

		// support move operations
		ES2Renderer(ES2Renderer&& rhs);
		ES2Renderer& operator=(ES2Renderer&& rhs);

		// delete the copy functions...
		ES2Renderer(const ES2Renderer& rhs) = delete;
		ES2Renderer& operator=(const ES2Renderer& rhs) = delete;

		virtual void Render(const IScene* pScene);

	private:
		struct Impl;
		std::unique_ptr<Impl> pImpl;
	};

} // namespace Fluxions

#endif
