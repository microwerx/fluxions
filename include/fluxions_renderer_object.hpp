#ifndef FLUXIONS_RENDERER_OBJECT_HPP
#define FLUXIONS_RENDERER_OBJECT_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_opengl.hpp>

namespace Fluxions {
	class RendererObject {
	public:
		RendererObject();
		virtual ~RendererObject();

		virtual void init(const std::string& newname, RendererObject* pparent = nullptr);
		virtual void kill();
		virtual void reset();
		virtual const char* type() const;

		const std::string& name() const noexcept { return name_; };

		void setParent(RendererObject* pparent);
		RendererObject* parent();
	private:
		std::string name_;
		RendererObject* parent_{ nullptr };
	};
}

#endif
