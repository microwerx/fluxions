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
#include <viperfish_dear_imgui.hpp>
#include <fluxions_fileio.hpp>

namespace Vf
{
	constexpr int MAX_RUN_MESSAGES = 2;

	DearImGuiWidget::DearImGuiWidget()
		: Widget("imguiwidget") {

	}

	DearImGuiWidget::DearImGuiWidget(const std::string& name)
		: Widget(name) {

	}

	DearImGuiWidget::~DearImGuiWidget() {

	}

	void DearImGuiWidget::OnInit(const std::vector<std::string>& args) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HFLOGINFO("Creating Dear ImGui Context");
		pImGuiContext = ImGui::CreateContext();
		pIO = &ImGui::GetIO();
		pIO->DisplaySize.x = 640.0f;
		pIO->DisplaySize.y = 480.0f;
		pIO->DeltaTime = 0.0f;

		FilePathInfo urwdock("resources/fonts/dock-medium.otf");
		if (urwdock.Exists()) {
			ImFont* font = pIO->Fonts->AddFontFromFileTTF("resources/fonts/dock-medium.otf", 24.0f);
			if (!font) {
				HFLOGWARN("Dock Font cannot to be loaded");
			}
		}


		pIO->KeyMap[ImGuiKey_Tab] = 0x09; // Keyboard mapping. ImGui will use those indices to peek into the pIO.KeyDown[] array.
		pIO->KeyMap[ImGuiKey_LeftArrow] = VF_KEY_LEFT;
		pIO->KeyMap[ImGuiKey_RightArrow] = VF_KEY_RIGHT;
		pIO->KeyMap[ImGuiKey_UpArrow] = VF_KEY_UP;
		pIO->KeyMap[ImGuiKey_DownArrow] = VF_KEY_DOWN;
		pIO->KeyMap[ImGuiKey_PageUp] = VF_KEY_PAGE_UP;
		pIO->KeyMap[ImGuiKey_PageDown] = VF_KEY_PAGE_DOWN;
		pIO->KeyMap[ImGuiKey_Home] = VF_KEY_HOME;
		pIO->KeyMap[ImGuiKey_End] = VF_KEY_END;
		pIO->KeyMap[ImGuiKey_Delete] = VF_KEY_DELETE;
		pIO->KeyMap[ImGuiKey_Backspace] = 0x08;
		pIO->KeyMap[ImGuiKey_Enter] = 0x10;
		pIO->KeyMap[ImGuiKey_Escape] = 0x27;
		pIO->KeyMap[ImGuiKey_A] = 'a';
		pIO->KeyMap[ImGuiKey_C] = 'c';
		pIO->KeyMap[ImGuiKey_V] = 'v';
		pIO->KeyMap[ImGuiKey_X] = 'x';
		pIO->KeyMap[ImGuiKey_Y] = 'y';
		pIO->KeyMap[ImGuiKey_Z] = 'z';

		CreateDeviceObjects();

		Widget::OnInit(args);
	}

	void DearImGuiWidget::OnKill() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		InvalidateDeviceObjects();
		HFLOGINFO("Destroying Dear ImGui Context");
		ImGui::DestroyContext(pImGuiContext);
		pIO = nullptr;
		pImGuiContext = nullptr;

		Widget::OnKill();
	}

	void DearImGuiWidget::OnMouseButtonDown(int button) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		Widget::OnMouseButtonDown(button);
		if (within(button, 0, 4) && pIO != nullptr)
			pIO->MouseDown[button] = true;
	}

	void DearImGuiWidget::OnMouseButtonUp(int button) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		Widget::OnMouseButtonUp(button);
		if (within(button, 0, 4) && pIO != nullptr)
			pIO->MouseDown[button] = false;
	}

	void DearImGuiWidget::OnMouseMove(int x, int y) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		Widget::OnMouseMove(x, y);
		if (pIO != nullptr) {
			pIO->MousePos.x = (float)x;
			pIO->MousePos.y = (float)y;
		}
	}

	void DearImGuiWidget::OnKeyDown(const std::string& key, int keymod) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		Widget::OnKeyDown(key, keymod);
		if (pIO == nullptr || key.empty())
			return;

		pIO->KeyAlt = keymod & KeyboardState::AltKeyBit;
		pIO->KeyCtrl = keymod & KeyboardState::CtrlKeyBit;
		pIO->KeyShift = keymod & KeyboardState::ShiftKeyBit;

		if (within(HTML5NameToKey(key), 0, 511)) {
			pIO->KeysDown[HTML5NameToKey(key)] = true;
		}
	}

	void DearImGuiWidget::OnKeyUp(const std::string& key, int keymod) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		Widget::OnKeyUp(key, keymod);
		if (pIO == nullptr || key.empty())
			return;

		pIO->KeyAlt = keymod & KeyboardState::AltKeyBit;
		pIO->KeyCtrl = keymod & KeyboardState::CtrlKeyBit;
		pIO->KeyShift = keymod & KeyboardState::ShiftKeyBit;

		if (within(HTML5NameToKey(key), 0, 511)) {
			pIO->KeysDown[HTML5NameToKey(key)] = false;
		}
	}

	void DearImGuiWidget::OnUpdate(double timeStamp) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (program && pIO != nullptr) {
			pIO->DisplaySize.x = (float)windowRect().w;
			pIO->DisplaySize.y = (float)windowRect().h;
			pIO->DeltaTime = fabsf((float)(timeStamp - getT1()));
		}

		Widget::OnUpdate(timeStamp);
	}

	void DearImGuiWidget::OnPreRender() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);

		Widget::OnPreRender();
		if (!program) return;
		ImGui::NewFrame();
	}

	void DearImGuiWidget::OnPostRender() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);

		Widget::OnPostRender();
		if (!program) return;
		ImGui::Render();
		RenderDrawLists();
	}

	void DearImGuiWidget::OnRenderDearImGui() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);

		if (!program) return;
		Widget::OnRenderDearImGui();
	}

	bool DearImGuiWidget::CreateDeviceObjects() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HFLOGDEBUG("Creating Dear ImGui device objects...");
		// Backup GL state
		GLint last_texture, last_array_buffer, last_vertex_array;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

		glGenTextures(1, &fontTextureId);
		if (fontTextureId == 0) {
			HFLOGERROR("Unable to create Dear ImGui font texture");
			return false;
		}
		unsigned char* pixels;
		int w;
		int h;
		pIO->Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);
		glBindTexture(GL_TEXTURE_2D, fontTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		long long id = fontTextureId;
		pIO->Fonts->TexID = (ImTextureID)id;

		const GLchar* vertex_shader = "#version 100\n"
			"precision highp float;\n"
			"uniform mat4 ProjectionMatrix;\n"
			"attribute vec4 aPosition;\n"
			"attribute vec3 aTexcoord;\n"
			"attribute vec4 aColor;\n"
			"varying vec3 vTexcoord;\n"
			"varying vec4 vColor;\n"
			"void main()\n"
			"{\n"
			"  vTexcoord = aTexcoord;\n"
			"  vColor = aColor;\n"
			"  gl_Position = ProjectionMatrix * vec4(aPosition.xyz,1.0);\n"
			"}\n";

		const GLchar* fragment_shader = "#version 100\n"
			"precision highp float;\n"
			"uniform sampler2D Texture0;\n"
			"varying vec3 vTexcoord;\n"
			"varying vec4 vColor;\n"
			"void main()\n"
			"{\n"
			"  gl_FragColor = vColor * texture2D(Texture0, vTexcoord.st);\n"
			"}\n";
		if (!glCreateProgram) {
			HFLOGERROR("Unable to call glCreateProgram()! Is this a STATIC/DYNAMIC GLEW issue?");
			return false;
		}
		program = glCreateProgram();
		vshader = glCreateShader(GL_VERTEX_SHADER);
		fshader = glCreateShader(GL_FRAGMENT_SHADER);
		bool result = true;
		if (program == 0 || vshader == 0 || fshader == 0) {
			HFLOGERROR("Unable to create Dear ImGui shader program and/or shaders");
			return false;
		}
		glShaderSource(vshader, 1, &vertex_shader, 0);
		glShaderSource(fshader, 1, &fragment_shader, 0);
		glCompileShader(vshader);
		// check if compiled...
		int shaderCompileStatus;
		int infoLogLength;
		glGetShaderiv(vshader, GL_COMPILE_STATUS, &shaderCompileStatus);
		glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (!shaderCompileStatus) {
			char* infoLog = new char[infoLogLength + 1];
			infoLog[infoLogLength] = '\0';
			glGetShaderInfoLog(vshader, infoLogLength, NULL, infoLog);
			HFLOGERROR("failed to compile Dear ImGui vertex shader\n-----\n%s-----\n%s\n-----\n", infoLog, vertex_shader);
			delete[] infoLog;
			glDeleteShader(vshader);
			vshader = 0;
			result = false;
		}

		glCompileShader(fshader);
		// check if compiled...
		glGetShaderiv(fshader, GL_COMPILE_STATUS, &shaderCompileStatus);
		glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (!shaderCompileStatus) {
			char* infoLog = new char[infoLogLength + 1];
			infoLog[infoLogLength] = '\0';
			glGetShaderInfoLog(fshader, infoLogLength, NULL, infoLog);
			HFLOGERROR("failed to compile Dear ImGui fragment shader\n-----\n%s-----\n%s\n-----\n", infoLog, fragment_shader);
			delete[] infoLog;
			glDeleteShader(fshader);
			fshader = 0;
			result = false;
		}

		if (!result) {
			glDeleteProgram(program);
			program = 0;
			return false;
		}

		glAttachShader(program, vshader);
		glAttachShader(program, fshader);
		glLinkProgram(program);
		// check if linked...
		GLint programLinkStatus;
		glGetProgramiv(program, GL_LINK_STATUS, &programLinkStatus);
		if (!programLinkStatus) {
			char* infoLog = new char[infoLogLength + 1];
			infoLog[infoLogLength] = '\0';
			glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
			Hf::Log.error("%s(): failed to link Dear ImGui program\n%s\n", infoLog);
			delete[] infoLog;
		}

		uTexture0Loc = glGetUniformLocation(program, "Texture0");
		uProjectionMatrixLoc = glGetUniformLocation(program, "ProjectionMatrix");
		aPositionLoc = glGetAttribLocation(program, "aPosition");
		aTexcoordLoc = glGetAttribLocation(program, "aTexcoord");
		aColorLoc = glGetAttribLocation(program, "aColor");

		glGenBuffers(1, &abo);
		glGenBuffers(1, &eabo);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, abo);
		glEnableVertexAttribArray(aPositionLoc);
		glEnableVertexAttribArray(aTexcoordLoc);
		glEnableVertexAttribArray(aColorLoc);

#define OFFSETOF(TYPE, ELEMENT) ((size_t) & (((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(aPositionLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(aTexcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(aColorLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

		// Restore modified GL state
		glBindTexture(GL_TEXTURE_2D, last_texture);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
		glBindVertexArray(last_vertex_array);

		HFLOGDEBUG("Dear ImGui Device Objects Created");
		return true;
	}

	void DearImGuiWidget::InvalidateDeviceObjects() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (vao)
			glDeleteVertexArrays(1, &vao);
		if (abo)
			glDeleteBuffers(1, &abo);
		if (eabo)
			glDeleteBuffers(1, &eabo);
		vao = abo = eabo = 0;

		if (program && vshader)
			glDetachShader(program, vshader);
		if (vshader)
			glDeleteShader(vshader);
		vshader = 0;

		if (program && fshader)
			glDetachShader(program, fshader);
		if (fshader)
			glDeleteShader(fshader);
		fshader = 0;

		if (program)
			glDeleteProgram(program);
		program = 0;

		if (fontTextureId) {
			glDeleteTextures(1, &fontTextureId);
			ImGui::GetIO().Fonts->TexID = 0;
			fontTextureId = 0;
		}
		Hf::Log.infofn(__FUNCTION__, "Dear ImGui Device Objects Invalidated");
	}

	void DearImGuiWidget::RenderDrawLists() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (program == 0)
			return;
		ImDrawData* draw_data = ImGui::GetDrawData();
		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		ImGuiIO& io = ImGui::GetIO();
		int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
		int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
		if (fb_width == 0 || fb_height == 0)
			return;
		draw_data->ScaleClipRects(io.DisplayFramebufferScale);

		// Backup GL state
		GLint last_active_texture;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
		glActiveTexture(GL_TEXTURE0);
		GLint last_program;
		glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		GLint last_texture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		GLint last_array_buffer;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
		GLint last_element_array_buffer;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
		GLint last_vertex_array;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
		GLint last_blend_src_rgb;
		glGetIntegerv(GL_BLEND_SRC_RGB, &last_blend_src_rgb);
		GLint last_blend_dst_rgb;
		glGetIntegerv(GL_BLEND_DST_RGB, &last_blend_dst_rgb);
		GLint last_blend_src_alpha;
		glGetIntegerv(GL_BLEND_SRC_ALPHA, &last_blend_src_alpha);
		GLint last_blend_dst_alpha;
		glGetIntegerv(GL_BLEND_DST_ALPHA, &last_blend_dst_alpha);
		GLint last_blend_equation_rgb;
		glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
		GLint last_blend_equation_alpha;
		glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
		GLint last_viewport[4];
		glGetIntegerv(GL_VIEWPORT, last_viewport);
		GLint last_scissor_box[4];
		glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
		GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
		GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
		GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
		GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);

		// Setup viewport, orthographic projection matrix
		glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
		const float ortho_projection[4][4] = {
			{2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f},
			{0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f},
			{0.0f, 0.0f, -1.0f, 0.0f},
			{-1.0f, 1.0f, 0.0f, 1.0f},
		};
		glUseProgram(program);
		glUniform1i(uTexture0Loc, 0);
		glUniformMatrix4fv(uProjectionMatrixLoc, 1, GL_FALSE, &ortho_projection[0][0]);
		glBindVertexArray(vao);

		for (int n = 0; n < draw_data->CmdListsCount; n++) {
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			const ImDrawIdx* idx_buffer_offset = 0;

			glBindBuffer(GL_ARRAY_BUFFER, abo);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback) {
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else {
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
					glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}

		// Restore modified GL state
		glUseProgram(last_program);
		glBindTexture(GL_TEXTURE_2D, last_texture);
		glActiveTexture(last_active_texture);
		glBindVertexArray(last_vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
		glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
		glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
		if (last_enable_blend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
		if (last_enable_cull_face)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		if (last_enable_depth_test)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		if (last_enable_scissor_test)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
		glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
		glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
	}
} // namespace Vf
