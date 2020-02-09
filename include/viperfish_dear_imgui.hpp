#ifndef VIPERFISH_DEAR_IMGUI_HPP
#define VIPERFISH_DEAR_IMGUI_HPP

#include <fluxions_opengl.hpp>
#include <viperfish_base.hpp>
#include <viperfish_widget.hpp>

namespace Colors {
	const ImVec4 Black{ 0.000f, 0.000f, 0.000f, 1.000f };
	const ImVec4 Gray33{ 0.333f, 0.333f, 0.333f, 1.000f };
	const ImVec4 Gray67{ 0.667f, 0.667f, 0.667f, 1.000f };
	const ImVec4 White{ 1.000f, 1.000f, 1.000f, 1.000f };
	const ImVec4 Red{ 1.000f, 0.000f, 0.000f, 1.000f };
	const ImVec4 Orange{ 0.894f, 0.447f, 0.000f, 1.000f };
	const ImVec4 Yellow{ 0.894f, 0.894f, 0.000f, 1.000f };
	const ImVec4 Green{ 0.000f, 1.000f, 0.000f, 1.000f };
	const ImVec4 Cyan{ 0.000f, 0.707f, 0.707f, 1.000f };
	const ImVec4 Azure{ 0.000f, 0.447f, 0.894f, 1.000f };
	const ImVec4 Blue{ 0.000f, 0.000f, 1.000f, 1.000f };
	const ImVec4 Violet{ 0.447f, 0.000f, 0.894f, 1.000f };
	const ImVec4 Rose{ 0.894f, 0.000f, 0.447f, 1.000f };
	const ImVec4 Brown{ 0.500f, 0.250f, 0.000f, 1.000f };
	const ImVec4 Gold{ 0.830f, 0.670f, 0.220f, 1.000f };
	const ImVec4 ForestGreen{ 0.250f, 0.500f, 0.250f, 1.000f };
}

namespace Vf {
	class DearImGuiWidget : public Widget {
	protected:
		DearImGuiWidget();
		DearImGuiWidget(const DearImGuiWidget& widget) = default;
		DearImGuiWidget(DearImGuiWidget&& widget) = default;

	public:
		static bool ContextInitialized;

		using SharedPtr = std::shared_ptr<DearImGuiWidget>;
		using UniquePtr = std::unique_ptr<DearImGuiWidget>;

		DearImGuiWidget(const std::string& name);
		virtual ~DearImGuiWidget() override;

		inline Widget::SharedPtr AsWidgetPtr() noexcept { return Widget::shared_from_this(); }

		virtual void OnInit(const std::vector<std::string>& args) override;
		virtual void OnKill() override;
		virtual void OnUpdate(double timeStamp) override;

		virtual void OnMouseButtonDown(int button) override;
		virtual void OnMouseButtonUp(int button) override;
		virtual void OnMouseMove(int x, int y) override;

		virtual void OnKeyDown(const std::string& key, int keymod) override;
		virtual void OnKeyUp(const std::string& key, int keymod) override;

		virtual void OnPreRender() override;
		virtual void OnPostRender() override;
		virtual void OnRenderDearImGui() override;

	protected:
		GLuint fontTextureId = 0;
		GLuint program = 0;
		GLuint vshader = 0;
		GLuint fshader = 0;
		GLint uTexture0Loc = 0;
		GLint uProjectionMatrixLoc = 0;
		GLint aPositionLoc = 0;
		GLint aTexcoordLoc = 0;
		GLint aColorLoc = 0;
		GLuint abo = 0;
		GLuint vao = 0;
		GLuint eabo = 0;
		ImGuiContext* pImGuiContext = nullptr;
		ImGuiIO* pIO = nullptr;

		bool CreateDeviceObjects();
		void InvalidateDeviceObjects();
		void RenderDrawLists();
	};

	using DearImGuiWidgetPtr = std::shared_ptr<DearImGuiWidget>;

	//////////////////////////////////////////////////////////////////////
	// HELPER FUNCTIONS FOR DEAR IMGUI ///////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	inline void ImGuiMatrix4f(const Fx::Matrix4f& m) {
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m11, m.m12, m.m13, m.m14);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m21, m.m22, m.m23, m.m24);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m31, m.m32, m.m33, m.m34);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m41, m.m42, m.m43, m.m44);
	}

	inline void ImGuiBoundingBox(const Fx::BoundingBoxf& bbox) {
		ImGui::Text("Min:  [ % 3.2f % 3.2f % 3.2f ]", bbox.minBounds.x, bbox.minBounds.y, bbox.minBounds.z);
		ImGui::Text("Max:  [ % 3.2f % 3.2f % 3.2f ]", bbox.maxBounds.x, bbox.maxBounds.y, bbox.maxBounds.z);
		ImGui::Text("Size: [ %3.2f %3.2f %3.2f ]", bbox.Size().x, bbox.Size().y, bbox.Size().z);
	}

	inline void ImGuiAlignAt(float& offset, const char* referencestr = nullptr) {
		if (offset == 0.0f && referencestr) {
			ImVec2 box = ImGui::CalcTextSize(referencestr);
			offset = box.x;
		}
		ImGui::SameLine(offset);
	}

	inline void ImGui2NSlider(const char* desc, int* choice, int* size, int minvalue, int maxvalue) {
		ImGui::SliderInt(desc, choice, minvalue, maxvalue);
		ImGui::SameLine();
		ImGui::Text("= %d", 2 << *choice);
		*size = 2 << *choice;
	}

	inline void ImGuiValue2f(const char* m, const Fx::Vector2f& v) {
		ImGui::Text(m);
		ImGui::SameLine();
		ImGui::Value("x", v.x);
		ImGui::SameLine();
		ImGui::Value("y", v.y);
	}

	inline void ImGuiValue3f(const char* m, const Fx::Vector3f& v) {
		ImGui::Text(m);
		ImGui::SameLine();
		ImGui::Value("x", v.x);
		ImGui::SameLine();
		ImGui::Value("y", v.y);
		ImGui::SameLine();
		ImGui::Value("z", v.z);
	}

	inline void ImGuiValue4f(const char* m, const Fx::Vector4f& v) {
		ImGui::LabelText(m, "% 2.3f % 2.3f % 2.3f %2.3f", v.x, v.y, v.z, v.w);
		//ImGui::Text(m);
		//ImGui::SameLine();
		//ImGui::Value("x", v.x);
		//ImGui::SameLine();
		//ImGui::Value("y", v.y);
		//ImGui::SameLine();
		//ImGui::Value("z", v.z);
		//ImGui::SameLine();
		//ImGui::Value("w", v.w);
	}

	inline void ImGuiValue3f(const char* m, const Fx::Color3f& v) {
		ImGui::Text(m);
		ImGui::SameLine();
		ImGui::Value("r", v.r);
		ImGui::SameLine();
		ImGui::Value("g", v.g);
		ImGui::SameLine();
		ImGui::Value("b", v.b);
	}

	inline void ImGuiValue4f(const char* m, const Fx::Color4f& v) {
		ImGui::Text(m);
		ImGui::SameLine();
		ImGui::Value("r", v.r);
		ImGui::SameLine();
		ImGui::Value("g", v.g);
		ImGui::SameLine();
		ImGui::Value("b", v.b);
		ImGui::SameLine();
		ImGui::Value("a", v.a);
	}

	inline void ImGuiValueQuaternion(const char* m, const Fx::Quaternionf& q) {
		ImGui::Text(m);
		ImGui::SameLine();
		ImGui::Value("a", q.a);
		ImGui::SameLine();
		ImGui::Value("b", q.b);
		ImGui::SameLine();
		ImGui::Value("c", q.c);
		ImGui::SameLine();
		ImGui::Value("d", q.d);
	}
} // namespace Vf

#endif // VIPERFISH_DEAR_IMGUI_HPP
