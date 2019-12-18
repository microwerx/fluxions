#ifndef FLUXIONS_RENDERER_OBJECT_HPP
#define FLUXIONS_RENDERER_OBJECT_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_opengl.hpp>

namespace Fluxions {
	class RendererObject {
	public:
		RendererObject();
		virtual ~RendererObject();

		virtual void init(const std::string& name);
		virtual void kill();
		virtual void reset();

		const std::string& getName() const noexcept { return name_; };
	private:
		std::string name_;
	};
}

#endif
