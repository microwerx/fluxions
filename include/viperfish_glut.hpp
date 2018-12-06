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
#ifndef VIPERFISH_GLUT_HPP
#define VIPERFISH_GLUT_HPP


#include <viperfish_widget.hpp>
#include <fluxions_opengl.hpp>


namespace Viperfish
{
	class GLUTWidget : public Widget
	{
	public:
		GLUTWidget() { }
		GLUTWidget(shared_ptr<Widget> &decoratee) : Widget(decoratee) { }

		void OnInit(const vector<string> &args);

		void OnMainLoop();
		void OnLeaveMainLoop();
	private:
		int createdWindowId = 0;
	};

	void SetGLUTWidget(shared_ptr<Widget> &widget);
	const char *KeyToHTML5Name(char c);
	const char *SpecialKeyToHTML5Name(int key);
	int HTML5NameToKey(const string & key);
}

#endif

