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
#include "stdafx.h"
#include <viperfish_imgui.hpp>

namespace Viperfish
{
void ImGuiWidget::OnInit(const std::vector<std::string> &args)
{
    pIO = &ImGui::GetIO();
    pIO->DisplaySize.x = 640.0f;
    pIO->DisplaySize.y = 480.0f;
    pIO->DeltaTime = 0.0f;

    pIO->KeyMap[ImGuiKey_Tab] = 0x09; // Keyboard mapping. ImGui will use those indices to peek into the pIO.KeyDown[] array.
    pIO->KeyMap[ImGuiKey_LeftArrow] = 0x100 + GLUT_KEY_LEFT;
    pIO->KeyMap[ImGuiKey_RightArrow] = 0x100 + GLUT_KEY_RIGHT;
    pIO->KeyMap[ImGuiKey_UpArrow] = 0x100 + GLUT_KEY_UP;
    pIO->KeyMap[ImGuiKey_DownArrow] = 0x100 + GLUT_KEY_DOWN;
    pIO->KeyMap[ImGuiKey_PageUp] = 0x100 + GLUT_KEY_PAGE_UP;
    pIO->KeyMap[ImGuiKey_PageDown] = 0x100 + GLUT_KEY_PAGE_DOWN;
    pIO->KeyMap[ImGuiKey_Home] = 0x100 + GLUT_KEY_HOME;
    pIO->KeyMap[ImGuiKey_End] = 0x100 + GLUT_KEY_END;
    pIO->KeyMap[ImGuiKey_Delete] = 0x100 + GLUT_KEY_DELETE;
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
    //ImGui::NewFrame();

    Widget::OnInit(args);
}

void ImGuiWidget::OnKill()
{
    Widget::OnKill();

    InvalidateDeviceObjects();
    pIO = nullptr;
    ImGui::Shutdown();
}

void ImGuiWidget::OnMouseButtonDown(int button)
{
    Widget::OnMouseButtonDown(button);
    if (within(button, 0, 4) && pIO != nullptr)
        pIO->MouseDown[button] = true;
}

void ImGuiWidget::OnMouseButtonUp(int button)
{
    Widget::OnMouseButtonUp(button);
    if (within(button, 0, 4) && pIO != nullptr)
        pIO->MouseDown[button] = false;
}

void ImGuiWidget::OnMouseMove(int x, int y)
{
    Widget::OnMouseMove(x, y);
    if (pIO != nullptr)
    {
        pIO->MousePos.x = (float)x;
        pIO->MousePos.y = (float)y;
    }
}

void ImGuiWidget::OnKeyDown(const std::string &key, int keymod)
{
    Widget::OnKeyDown(key, keymod);
    if (pIO == nullptr || key.empty())
        return;

    pIO->KeyAlt = keymod & KeyboardState::AltKeyBit;
    pIO->KeyCtrl = keymod & KeyboardState::CtrlKeyBit;
    pIO->KeyShift = keymod & KeyboardState::ShiftKeyBit;

    if (within(HTML5NameToKey(key), 0, 511))
    {
        pIO->KeysDown[HTML5NameToKey(key)] = true;
    }
}

void ImGuiWidget::OnKeyUp(const std::string &key, int keymod)
{
    Widget::OnKeyUp(key, keymod);
    if (pIO == nullptr || key.empty())
        return;

    pIO->KeyAlt = keymod & KeyboardState::AltKeyBit;
    pIO->KeyCtrl = keymod & KeyboardState::CtrlKeyBit;
    pIO->KeyShift = keymod & KeyboardState::ShiftKeyBit;

    if (within(HTML5NameToKey(key), 0, 511))
    {
        pIO->KeysDown[HTML5NameToKey(key)] = false;
    }
}

void ImGuiWidget::OnUpdate(double timeStamp)
{
    if (pIO != nullptr)
    {
        pIO->DisplaySize.x = (float)windowRect().w;
        pIO->DisplaySize.y = (float)windowRect().h;
        pIO->DeltaTime = fabsf((float)(timeStamp - getT1()));
    }

    Widget::OnUpdate(timeStamp);
}

void ImGuiWidget::OnRender()
{
    ImGui::NewFrame();
    Widget::OnRender();
}

void ImGuiWidget::OnRenderOverlay()
{
    Widget::OnRenderOverlay();
    ImGui::Render();
    RenderDrawLists();
}

bool ImGuiWidget::CreateDeviceObjects()
{
    // Backup GL state
    GLint last_texture, last_array_buffer, last_vertex_array;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

    unsigned char *pixels;
    int w;
    int h;
    pIO->Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);
    glGenTextures(1, &fontTextureId);
    glBindTexture(GL_TEXTURE_2D, fontTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    long long id = fontTextureId;
    pIO->Fonts->TexID = (ImTextureID)id;

    const GLchar *vertex_shader = "#version 330\n"
                                  "uniform mat4 ProjMtx;\n"
                                  "in vec2 Position;\n"
                                  "in vec2 UV;\n"
                                  "in vec4 Color;\n"
                                  "out vec2 Frag_UV;\n"
                                  "out vec4 Frag_Color;\n"
                                  "void main()\n"
                                  "{\n"
                                  "	Frag_UV = UV;\n"
                                  "	Frag_Color = Color;\n"
                                  "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
                                  "}\n";

    const GLchar *fragment_shader = "#version 330\n"
                                    "uniform sampler2D Texture;\n"
                                    "in vec2 Frag_UV;\n"
                                    "in vec4 Frag_Color;\n"
                                    "out vec4 Out_Color;\n"
                                    "void main()\n"
                                    "{\n"
                                    "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
                                    "}\n";

    program = glCreateProgram();
    vshader = glCreateShader(GL_VERTEX_SHADER);
    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vshader, 1, &vertex_shader, 0);
    glShaderSource(fshader, 1, &fragment_shader, 0);
    glCompileShader(vshader);
    // check if compiled...
    int shaderCompileStatus;
    int infoLogLength;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &shaderCompileStatus);
    glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!shaderCompileStatus)
    {
        char *infoLog = new char[infoLogLength + 1];
        infoLog[infoLogLength] = '\0';
        glGetShaderInfoLog(vshader, infoLogLength, NULL, infoLog);
        hflog.error("%s(): failed to compile ImGui vertex shader\n%s\n", infoLog);
        delete[] infoLog;
    }

    glCompileShader(fshader);
    // check if compiled...
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &shaderCompileStatus);
    glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!shaderCompileStatus)
    {
        char *infoLog = new char[infoLogLength + 1];
        infoLog[infoLogLength] = '\0';
        glGetShaderInfoLog(fshader, infoLogLength, NULL, infoLog);
        hflog.error("%s(): failed to compile ImGui fragment shader\n%s\n", infoLog);
        delete[] infoLog;
    }

    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);
    // check if linked...
    GLint programLinkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &programLinkStatus);
    if (!programLinkStatus)
    {
        char *infoLog = new char[infoLogLength + 1];
        infoLog[infoLogLength] = '\0';
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        hflog.error("%s(): failed to link ImGui program\n%s\n", infoLog);
        delete[] infoLog;
    }

    uTextureLoc = glGetUniformLocation(program, "Texture");
    uProjMtxLoc = glGetUniformLocation(program, "ProjMtx");
    aPositionLoc = glGetAttribLocation(program, "Position");
    aUVLoc = glGetAttribLocation(program, "UV");
    aColorLoc = glGetAttribLocation(program, "Color");

    glGenBuffers(1, &abo);
    glGenBuffers(1, &eabo);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, abo);
    glEnableVertexAttribArray(aPositionLoc);
    glEnableVertexAttribArray(aUVLoc);
    glEnableVertexAttribArray(aColorLoc);

#define OFFSETOF(TYPE, ELEMENT) ((size_t) & (((TYPE *)0)->ELEMENT))
    glVertexAttribPointer(aPositionLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid *)OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(aUVLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid *)OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(aColorLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid *)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindVertexArray(last_vertex_array);

    return true;
}

void ImGuiWidget::InvalidateDeviceObjects()
{
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

    if (fontTextureId)
    {
        glDeleteTextures(1, &fontTextureId);
        ImGui::GetIO().Fonts->TexID = 0;
        fontTextureId = 0;
    }
}

void ImGuiWidget::RenderDrawLists()
{
    ImDrawData *draw_data = ImGui::GetDrawData();
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO &io = ImGui::GetIO();
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
    glUniform1i(uTextureLoc, 0);
    glUniformMatrix4fv(uProjMtxLoc, 1, GL_FALSE, &ortho_projection[0][0]);
    glBindVertexArray(vao);

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx *idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, abo);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid *)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid *)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
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
} // namespace Viperfish