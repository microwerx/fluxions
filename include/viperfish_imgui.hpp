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
#include <viperfish.hpp>
#include <imgui.h>

namespace Viperfish
{

class ImGuiWidget : public Widget
{
  protected:
	ImGuiWidget() {}
	explicit ImGuiWidget(const ImGuiWidget &widget) = default;

  public:
	ImGuiWidget(SharedPtr &decorateeWidget) { decorate(decorateeWidget); }
	virtual ~ImGuiWidget() {}

	using SharedPtr = shared_ptr<ImGuiWidget>;
	using UniquePtr = unique_ptr<ImGuiWidget>;

	template <class... _Types>
	static SharedPtr MakeShared(_Types &&... _Args) { return SharedPtr(new ImGuiWidget(forward<_Types>(_Args)...)); }
	template <class... _Types>
	static UniquePtr MakeUnique(_Types &&... _Args) { return UniquePtr(new ImGuiWidget(forward<_Types>(_Args)...)); }

	virtual void OnInit(const vector<string> &args) override;
	virtual void OnKill() override;
	virtual void OnUpdate(double timeStamp) override;

	virtual void OnMouseButtonDown(int button) override;
	virtual void OnMouseButtonUp(int button) override;
	virtual void OnMouseMove(int x, int y) override;

	virtual void OnKeyDown(const string &key, int keymod) override;
	virtual void OnKeyUp(const string &key, int keymod) override;

	virtual void OnRender() override;
	virtual void OnRenderOverlay() override;

  protected:
	GLuint fontTextureId = 0;
	GLint program = 0;
	GLint vshader = 0;
	GLint fshader = 0;
	GLint uTextureLoc = 0;
	GLint uProjMtxLoc = 0;
	GLint aPositionLoc = 0;
	GLint aUVLoc = 0;
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
