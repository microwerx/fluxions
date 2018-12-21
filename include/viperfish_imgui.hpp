// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
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
#ifndef VIPERFISH_GUI_HPP
#define VIPERFISH_GUI_HPP

#include <memory>
#include <string>
#include <viperfish.hpp>
#include <imgui.h>

namespace Viperfish
{

	class ImGuiWidget : public Widget
	{
	protected:
		ImGuiWidget();
		ImGuiWidget(const ImGuiWidget &widget) = default;
		ImGuiWidget(ImGuiWidget &&widget) = default;

	public:
		using SharedPtr = std::shared_ptr<ImGuiWidget>;
		using UniquePtr = std::unique_ptr<ImGuiWidget>;

		template <class... _Types>
		static SharedPtr MakeShared(_Types &&... _Args) { return SharedPtr(new ImGuiWidget(std::forward<_Types>(_Args)...)); }
		template <class... _Types>
		static UniquePtr MakeUnique(_Types &&... _Args) { return UniquePtr(new ImGuiWidget(std::forward<_Types>(_Args)...)); }

		ImGuiWidget(const std::string &name);
		ImGuiWidget(const std::string &name, SharedPtr &decorateeWidget);
		virtual ~ImGuiWidget() override;

		virtual void OnInit(const std::vector<std::string> &args) override;
		virtual void OnKill() override;
		virtual void OnUpdate(double timeStamp) override;

		virtual void OnMouseButtonDown(int button) override;
		virtual void OnMouseButtonUp(int button) override;
		virtual void OnMouseMove(int x, int y) override;

		virtual void OnKeyDown(const std::string &key, int keymod) override;
		virtual void OnKeyUp(const std::string &key, int keymod) override;

		virtual void OnRender3D() override;
		virtual void OnRender2D() override;
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
		ImGuiIO *pIO = nullptr;

		bool CreateDeviceObjects();
		void InvalidateDeviceObjects();
		void RenderDrawLists();
	};
} // namespace Viperfish

#endif
