// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2020 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com

#ifndef VIPERFISH_DEAR_IMGUI_HPP
#define VIPERFISH_DEAR_IMGUI_HPP

#include <viperfish_base.hpp>
#include <viperfish_widget.hpp>
#include <imgui.h>

namespace Colors
{
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

namespace Vf
{

	class DearImGuiWidget : public Widget
	{
	protected:
		DearImGuiWidget();
		DearImGuiWidget(const DearImGuiWidget &widget) = default;
		DearImGuiWidget(DearImGuiWidget &&widget) = default;

	public:
		static bool ContextInitialized;

		using SharedPtr = std::shared_ptr<DearImGuiWidget>;
		using UniquePtr = std::unique_ptr<DearImGuiWidget>;

		DearImGuiWidget(const std::string &name);
		virtual ~DearImGuiWidget() override;

		inline Widget::SharedPtr AsWidgetPtr() noexcept { return Widget::shared_from_this(); }

		virtual void OnInit(const std::vector<std::string> &args) override;
		virtual void OnKill() override;
		virtual void OnUpdate(double timeStamp) override;

		virtual void OnMouseButtonDown(int button) override;
		virtual void OnMouseButtonUp(int button) override;
		virtual void OnMouseMove(int x, int y) override;

		virtual void OnKeyDown(const std::string &key, int keymod) override;
		virtual void OnKeyUp(const std::string &key, int keymod) override;

		virtual void OnPreRender() override;
		virtual void OnPostRender() override;
		virtual void OnRenderDearImGui() override;

	protected:
		GLuint fontTextureId = 0;
		GLint program = 0;
		GLint vshader = 0;
		GLint fshader = 0;
		GLint uTexture0Loc = 0;
		GLint uProjectionMatrixLoc = 0;
		GLint aPositionLoc = 0;
		GLint aTexcoordLoc = 0;
		GLint aColorLoc = 0;
		GLuint abo = 0;
		GLuint vao = 0;
		GLuint eabo = 0;
		ImGuiContext *pImGuiContext = nullptr;
		ImGuiIO *pIO = nullptr;

		bool CreateDeviceObjects();
		void InvalidateDeviceObjects();
		void RenderDrawLists();
	};

	using DearImGuiWidgetPtr = std::shared_ptr<DearImGuiWidget>;

	//////////////////////////////////////////////////////////////////////
	// HELPER FUNCTIONS FOR DEAR IMGUI ///////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	inline void ImGuiMatrix4f(const Matrix4f& m) {
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m11, m.m12, m.m13, m.m14);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m21, m.m22, m.m23, m.m24);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m31, m.m32, m.m33, m.m34);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m41, m.m42, m.m43, m.m44);
	}

	inline void ImGuiBoundingBox(const BoundingBoxf& bbox) {
		ImGui::Text("Min:  [ % 3.2f % 3.2f % 3.2f ]", bbox.minBounds.x, bbox.minBounds.y, bbox.minBounds.z);
		ImGui::Text("Max:  [ % 3.2f % 3.2f % 3.2f ]", bbox.maxBounds.x, bbox.maxBounds.y, bbox.maxBounds.z);
		ImGui::Text("Size: [ %3.2f %3.2f %3.2f ]", bbox.Size().x, bbox.Size().y, bbox.Size().z);
	}

} // namespace Vf

#endif // VIPERFISH_DEAR_IMGUI_HPP
