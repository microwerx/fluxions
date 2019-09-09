// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
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
#ifndef FLUXIONS_ES2_RENDERER_HPP
#define FLUXIONS_ES2_RENDERER_HPP

#include <memory>
#include <fluxions_renderer.hpp>

namespace Fluxions
{
class ES2Renderer : public IRenderer
{
  public:
	ES2Renderer();
	virtual ~ES2Renderer();

	// support move operations
	ES2Renderer(ES2Renderer &&rhs);
	ES2Renderer &operator=(ES2Renderer &&rhs);

	// delete the copy functions...
	ES2Renderer(const ES2Renderer &rhs) = delete;
	ES2Renderer &operator=(const ES2Renderer &rhs) = delete;

	virtual void Render(const IScene *pScene);

  private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

} // namespace Fluxions

#endif
