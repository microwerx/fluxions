#include "viperfish_pch.hpp"
#include <viperfish_base.hpp>
#include <viperfish_dear_imgui.hpp>
#include <fluxions_renderer_utilities.hpp>

namespace Vf {
	using namespace Fluxions;

	bool DearImGuiWidget::ContextInitialized = false;

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

		std::vector<std::string> fontpaths{
			"resources/fonts/ibmplexsanscond-medium.ttf",
			"resources/fonts/dock-medium.otf",
			"resources/fonts/inconsolata.ttf" };
		FilePathInfo fontfpi;
		ImFont* font{ nullptr };
		for (auto fontpath : fontpaths) {
			fontfpi.reset(fontpath);
			if (fontfpi.exists()) {
				font = pIO->Fonts->AddFontFromFileTTF(fontfpi.shortestPathC(), 24.0f);
				break;
			}
		}
		if (!font) {
			HFLOGWARN("custom DearImGui fonts were not found");
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

		ContextInitialized = CreateDeviceObjects();

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
		pIO->KeySuper = keymod & KeyboardState::MetaKeyBit;

		int html5key = KeyboardState::HTML5NameToKey(key);
		if (within(html5key, 0, 511)) {
			pIO->KeysDown[html5key] = true;
		}

		if (within(html5key, 0, 0x7f)) {
			pIO->AddInputCharacter(html5key);
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
		pIO->KeySuper = keymod & KeyboardState::MetaKeyBit;

		int html5key = KeyboardState::HTML5NameToKey(key);
		if (within(html5key, 0, 511)) {
			pIO->KeysDown[html5key] = false;
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
		FxSaveGraphicsState glstate;

		bool result = true;
		if (!FxCreateTexture(GL_TEXTURE_2D, &fontTextureId)) {
			HFLOGERROR("Unable to create Dear ImGui font texture");
			result = false;
		}
		else {
			unsigned char* pixels;
			int w;
			int h;
			pIO->Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);
			glBindTexture(GL_TEXTURE_2D, fontTextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//FxGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
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
			FxDeleteShader(&vshader);
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
			FxDeleteShader(&fshader);
			result = false;
		}

		if (!result) {
			FxDeleteProgram(&program);
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
			HFLOGERROR("failed to link Dear ImGui program\n%s\n", infoLog);
			delete[] infoLog;
		}

		uTexture0Loc = glGetUniformLocation(program, "Texture0");
		uProjectionMatrixLoc = glGetUniformLocation(program, "ProjectionMatrix");
		aPositionLoc = glGetAttribLocation(program, "aPosition");
		aTexcoordLoc = glGetAttribLocation(program, "aTexcoord");
		aColorLoc = glGetAttribLocation(program, "aColor");

		FxCreateVertexArray(&vao);
		FxCreateBuffer(GL_ARRAY_BUFFER, &abo, 0, nullptr, GL_STREAM_DRAW);
		FxCreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &eabo, 0, nullptr, GL_STREAM_DRAW);

		glEnableVertexAttribArray(aPositionLoc);
		glEnableVertexAttribArray(aTexcoordLoc);
		glEnableVertexAttribArray(aColorLoc);

#define OFFSETOF(TYPE, ELEMENT) ((size_t) & (((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(aPositionLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(aTexcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(aColorLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

		HFLOGDEBUG("Dear ImGui Device Objects Created");
		return true;
	}

	void DearImGuiWidget::InvalidateDeviceObjects() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		FxDeleteVertexArray(&vao);
		FxDeleteBuffer(&abo);
		FxDeleteBuffer(&eabo);


		if (program && vshader)
			glDetachShader(program, vshader);
		if (program && fshader)
			glDetachShader(program, fshader);

		FxDeleteShader(&vshader);
		FxDeleteShader(&fshader);
		FxDeleteProgram(&program);

		if (fontTextureId) {
			ImGui::GetIO().Fonts->TexID = 0;
			FxDeleteTexture(&fontTextureId);
		}
		HFLOGINFO("Dear ImGui Device Objects Invalidated");
		ContextInitialized = false;
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

		FxSaveGraphicsState glstate;

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
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(program);
		glUniform1i(uTexture0Loc, 0);
		glUniformMatrix4fv(uProjectionMatrixLoc, 1, GL_FALSE, &ortho_projection[0][0]);
		glBindVertexArray(vao);

		for (int n = 0; n < draw_data->CmdListsCount; n++) {
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			const ImDrawIdx* idx_buffer_offset = 0;

			FxCreateBuffer(GL_ARRAY_BUFFER, &abo, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
			FxCreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &eabo, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

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
	}
} // namespace Vf
