#ifndef FLUXIONS_SSG_SCENE_GRAPH_NODE_HPP
#define FLUXIONS_SSG_SCENE_GRAPH_NODE_HPP

#include <fluxions_base.hpp>

namespace Fluxions {
	struct SimpleSceneGraphNode : public IBaseObject {
		Matrix4f transform;
		Matrix4f addlTransform;
		BoundingBoxf bbox;

		Matrix4f worldMatrix() const { return transform * addlTransform; }

		const char* type() const override { return "SimpleSceneGraphNode"; }
		const char* keyword() const override { return "node"; }
		const char* status() const override { return statusString_.c_str(); }
		virtual Color3f color() const { return FxColors3::Gray67; }
		const char* nodeName() const { return nodeName_.c_str(); }
		const char* updateNodeName();

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;

		bool operator<(const SimpleSceneGraphNode& rhs) {
			std::string type_ = type();
			if (type_ == rhs.type())
				return name_ < rhs.name_;
			return type_ < rhs.type();
		}
	protected:
		std::string statusString_;
		std::string nodeName_;
	};
}

#endif
