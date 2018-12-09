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
#include <viperfish_glut.hpp>

namespace Viperfish
{
namespace vfglut
{
static std::shared_ptr<Widget> widget;
static MouseState mouseState;
static std::map<int, MouseState> multitouchState;
static KeyboardState keyboardState;
static std::map<int, GamepadState> gamepadState;

static void reshape(int width, int height)
{
    if (!widget)
        return;

    widget->OnReshape(width, height);
}

static void display()
{
    if (!widget)
        return;

    widget->OnRender();
}

static void idle()
{
    if (!widget)
        return;
    static double t0 = 0.0;
    static double t1 = 0.0;
    t0 = t1;
    t1 = SteadyClockNow();
    double dt = t1 - t0;

    widget->OnUpdate(dt);
}

static void keyboard(unsigned char key, int x, int y)
{
    if (!widget)
        return;

    std::string keyName = Viperfish::KeyToHTML5Name(key);
    keyboardState.SetKey(keyName, true);

    widget->OnKeyDown(keyName, 0);
}

static void keyboardup(unsigned char key, int x, int y)
{
    if (!widget)
        return;

    std::string keyName = Viperfish::KeyToHTML5Name(key);
    keyboardState.SetKey(keyName, false);

    widget->OnKeyUp(keyName, 0);
}

static void special(int key, int x, int y)
{
    if (!widget)
        return;

    std::string keyName = Viperfish::SpecialKeyToHTML5Name(key);
    keyboardState.SetKey(keyName, false);

    widget->OnKeyDown(keyName, 0);
}

static void specialup(int key, int x, int y)
{
    if (!widget)
        return;

    std::string keyName = Viperfish::SpecialKeyToHTML5Name(key);
    keyboardState.SetKey(keyName, false);

    widget->OnKeyUp(keyName, 0);
}

static void mouse(int x, int y, int button, int state)
{
    if (!widget)
        return;
}

static void mouseWheel(int wheel, int direction, int x, int y)
{
    if (!widget)
        return;
}

static void motion(int x, int y)
{
    if (!widget)
        return;
}

static void passivemotion(int x, int y)
{
    if (!widget)
        return;
}

static void appstatus(int state)
{
#ifdef GLUT_APPSTATUS_PAUSE
    // GLUT_APPSTATUS_PAUSE and GLUT_APPSTATUS_RESUME
    if (state == GLUT_APPSTATUS_PAUSE)
        widget->OnPauseApp();
#endif
#ifdef GLUT_APPSTATUS_RESUME
    if (state == GLUT_APPSTATUS_RESUME)
        widget->OnResumeApp();
#endif
}

static void close()
{
    if (!widget)
        return;
}

static void entry(int state)
{
    if (!widget)
        return;
    // GLUT_LEFT or GLUT_ENTERED
}

static void visibility(int state)
{
    if (!widget)
        return;
    // GLUT_NOT_VISIBLE or GLUT_VISIBLE
}

static void joystick(unsigned int buttonMask, int x, int y, int z)
{
    if (!widget)
        return;
}

static void multibutton(int id, int x, int y, int button, int state)
{
    if (!widget)
        return;

    if (state == GLUT_DOWN)
        multitouchState[id].OnButtonDown(button);
    if (state == GLUT_UP)
        multitouchState[id].OnButtonUp(button);

    if (state == GLUT_DOWN)
        widget->OnMultiButtonDown(id, button, multitouchState[id]);
    if (state == GLUT_UP)
        widget->OnMultiButtonUp(id, button, multitouchState[id]);
}

static void multimotion(int id, int x, int y)
{
    if (!widget)
        return;

    multitouchState[id].OnMove(x, y);
}

static void multientry(int id, int state)
{
    if (!widget)
        return;

    if (state == GLUT_ENTERED)
        widget->OnMultiEnter(id);
    if (state == GLUT_LEFT)
        widget->OnMultiLeave(id);
}

static void multipassive(int id, int x, int y)
{
    if (!widget)
        return;

    multitouchState[id].OnMove(x, y);
}

static void position(int x, int y)
{
    if (!widget)
        return;

    widget->OnWindowMove(x, y);
}

static void spaceballbutton(int button, int state)
{
}

static void spaceballmotion(int x, int y, int z)
{
}

static void spaceballrotate(int x, int y, int z)
{
}

static void tabletbutton(int button, int state, int x, int y)
{
}

static void tabletmotion(int x, int y)
{
}

static void windowstatus(int state)
{
    if (!widget)
        return;
    // GLUT_HIDDEN, GLUT_FULLY_RETAINED, GLUT_PARTIALLY_RETAINED, or GLUT_FULLY_COVERED
    // windowstatus() is preferred over visibility()

    if (state == GLUT_HIDDEN)
        widget->OnWindowHidden();
    else
    {
        widget->OnWindowVisible();
    }
}

static void initcontext()
{
    if (!widget)
        return;

    widget->OnInitContext();
}
} // namespace vfglut

void SetGLUTWidget(std::shared_ptr<Widget> &widget)
{
    vfglut::widget = widget;
}

void GLUTWidget::OnInit(const std::vector<std::string> &args)
{
    // Create the GLUT instance
    //vfglut::widget = Widget::shared_from_this();

    int argc = (int)args.size();
    std::vector<char *> argv;
    for_each(args.begin(), args.end(), [&argv](const std::string &s) {
        int len = (int)s.length();
        char *arg = new char[len + 1];
#ifdef WIN32
        strncpy_s(arg, len + 1, s.c_str(), len);
#elif __unix__
        strncpy(arg, s.c_str(), len);
#endif
        argv.push_back(arg);
    });

    GLenum displayMode = GLUT_SRGB | GLUT_RGB | GLUT_DOUBLE | GLUT_STENCIL | GLUT_DEPTH | GLUT_MULTISAMPLE;
    int majorVersion = 4;
    int minorVersion = 5;
    // int profile = GLUT_CORE_PROFILE;
    int profile = GLUT_COMPATIBILITY_PROFILE;
    int flags = GLUT_DEBUG;
    int x = -1;
    int y = -1;
    int width = 640;
    int height = 360;

    glutInit(&argc, &argv[0]);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitDisplayMode(displayMode);
    glutInitContextVersion(majorVersion, minorVersion);
    glutInitContextProfile(profile);
    glutInitContextFlags(flags);

    glutInitWindowPosition(x, y);
    glutInitWindowSize(width, height);
    createdWindowId = glutCreateWindow("GLUTWidget");

    // register callbacks
    glutKeyboardFunc(vfglut::keyboard);
    glutKeyboardUpFunc(vfglut::keyboardup);
    glutSpecialFunc(vfglut::special);
    glutSpecialUpFunc(vfglut::specialup);
    glutMouseFunc(vfglut::mouse);
    glutMouseWheelFunc(vfglut::mouseWheel);
    glutMotionFunc(vfglut::motion);
    glutPassiveMotionFunc(vfglut::passivemotion);
    glutIdleFunc(vfglut::idle);
    glutReshapeFunc(vfglut::reshape);
    glutDisplayFunc(vfglut::display);

    // Mobile applications and other functions in FREEGLUT
    // This is one way to check for these
#ifdef GLUT_APPSTATUS_PAUSE
    glutAppStatusFunc(vfglut::appstatus);
    glutInitContextFunc(vfglut::initcontext);
    glutPositionFunc(vfglut::position);
#endif

    glutCloseFunc(vfglut::close);
    glutEntryFunc(vfglut::entry);
    glutWindowStatusFunc(vfglut::windowstatus);

    //glutJoystickFunc(vfglut::joystick, 10);
    //glutMultiButtonFunc(vfglut::multibutton);
    //glutMultiEntryFunc(vfglut::multientry);
    //glutMultiMotionFunc(vfglut::multimotion);
    //glutMultiPassiveFunc(vfglut::multipassive);
    //glutSpaceballButtonFunc(vfglut::spaceballbutton);
    //glutSpaceballMotionFunc(vfglut::spaceballmotion);
    //glutSpaceballRotateFunc(vfglut::spaceballrotate);
    //glutTabletButtonFunc(vfglut::tabletbutton);
    //glutTabletMotionFunc(vfglut::tabletmotion);
    // glutVisibilityFunc(vfglut::visibility); // deprecated in favor of glutWindowStatusFunc
    // glutWMCloseFunc(vfglut::wmclose); // deprecated in factor of glutCloseFunc

    for_each(argv.begin(), argv.end(), [](char *cstr) { if (cstr) delete[] cstr; });

    glewInit();

    std::cerr << __FUNCTION__ << "() -- initialized GLUT" << std::endl;
    std::cerr << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
    // cerr << "GL_EXTENSIONS: "

    Widget::OnInit(args);
}

void GLUTWidget::OnMainLoop()
{
    glutMainLoop();
}

void GLUTWidget::OnLeaveMainLoop()
{
    glutLeaveMainLoop();
}

const char *KeyToHTML5Name(char c)
{
    static char s[10];

    if (c <= 0x20)
    {
        switch (c)
        {
        case 0x09:
            return "Tab";
        case 0x0D:
            return "Enter";
        case 0x1B:
            return "Escape";
        case 0x20:
            return " ";
        }
    }

    int keymod = glutGetModifiers();
    if (keymod & GLUT_ACTIVE_SHIFT)
    {
        c = (char)toupper(c);
    }
    else
    {
        c = (char)tolower(c);
    }
    s[0] = c;
    s[1] = 0;

    return s;
}

int HTML5NameToKey(const std::string &key)
{
    if (key == "Tab")
        return 0x09;
    if (key == "Enter")
        return 0x0D;
    if (key == "Escape")
        return 0x1B;
    if (key == " ")
        return 0x20;
    if (key.front() <= 0x7f)
        return tolower(key.front());
    if (key == "F1")
        return 0x100 + GLUT_KEY_F1;
    if (key == "F2")
        return 0x100 + GLUT_KEY_F2;
    if (key == "F3")
        return 0x100 + GLUT_KEY_F3;
    if (key == "F4")
        return 0x100 + GLUT_KEY_F4;
    if (key == "F5")
        return 0x100 + GLUT_KEY_F5;
    if (key == "F6")
        return 0x100 + GLUT_KEY_F6;
    if (key == "F7")
        return 0x100 + GLUT_KEY_F7;
    if (key == "F8")
        return 0x100 + GLUT_KEY_F8;
    if (key == "F9")
        return 0x100 + GLUT_KEY_F9;
    if (key == "F10")
        return 0x100 + GLUT_KEY_F10;
    if (key == "F11")
        return 0x100 + GLUT_KEY_F11;
    if (key == "F12")
        return 0x100 + GLUT_KEY_F12;
    if (key == "Left")
        return 0x100 + GLUT_KEY_LEFT;
    if (key == "Up")
        return 0x100 + GLUT_KEY_UP;
    if (key == "Right")
        return 0x100 + GLUT_KEY_RIGHT;
    if (key == "Down")
        return 0x100 + GLUT_KEY_DOWN;
    if (key == "PageUp")
        return 0x100 + GLUT_KEY_PAGE_UP;
    if (key == "PageDown")
        return 0x100 + GLUT_KEY_PAGE_DOWN;
    if (key == "Home")
        return 0x100 + GLUT_KEY_HOME;
    if (key == "End")
        return 0x100 + GLUT_KEY_END;
    if (key == "Insert")
        return 0x100 + GLUT_KEY_INSERT;
    if (key == "NumLock")
        return 0x100 + GLUT_KEY_NUM_LOCK;
    if (key == "Begin")
        return 0x100 + GLUT_KEY_BEGIN;
    if (key == "Delete")
        return 0x100 + GLUT_KEY_DELETE;
    if (key == "Shift")
        return 0x100 + GLUT_KEY_SHIFT_L;
    if (key == "Ctrl")
        return 0x100 + GLUT_KEY_CTRL_L;
    if (key == "Alt")
        return 0x100 + GLUT_KEY_ALT_L;
    return 0;
}

const char *SpecialKeyToHTML5Name(int key)
{
    if (key >= 0x100)
        key -= 0x100;

    switch (key)
    {
    case 0x0001: // GLUT_KEY_F1
        return "F1";
        break;
    case 0x0002: // GLUT_KEY_F2
        return "F2";
        break;
    case 0x0003: // GLUT_KEY_F3
        return "F3";
        break;
    case 0x0004: // GLUT_KEY_F4
        return "F4";
        break;
    case 0x0005: // GLUT_KEY_F5
        return "F5";
        break;
    case 0x0006: // GLUT_KEY_F6
        return "F6";
        break;
    case 0x0007: // GLUT_KEY_F7
        return "F7";
        break;
    case 0x0008: // GLUT_KEY_F8
        return "F8";
        break;
    case 0x0009: // GLUT_KEY_F9
        return "F9";
        break;
    case 0x000A: // GLUT_KEY_F10
        return "F10";
        break;
    case 0x000B: // GLUT_KEY_F11
        return "F11";
        break;
    case 0x000C: // GLUT_KEY_F12
        return "F12";
        break;
    case 0x0064: // GLUT_KEY_LEFT
        return "Left";
        break;
    case 0x0065: // GLUT_KEY_UP
        return "Up";
        break;
    case 0x0066: // GLUT_KEY_RIGHT
        return "Right";
        break;
    case 0x0067: // GLUT_KEY_DOWN
        return "Down";
        break;
    case 0x0068: // GLUT_KEY_PAGE_UP
        return "PageUp";
        break;
    case 0x0069: // GLUT_KEY_PAGE_DOWN
        return "PageDown";
        break;
    case 0x006A: // GLUT_KEY_HOME
        return "Home";
        break;
    case 0x006B: // GLUT_KEY_END
        return "End";
        break;
    case 0x006C: // GLUT_KEY_INSERT
        return "Insert";
        break;
    case 0x006D: // GLUT_KEY_NUM_LOCK
        return "NumLock";
        break;
    case 0x006E: // GLUT_KEY_BEGIN
        return "Begin";
        break;
    case 0x006F: // GLUT_KEY_DELETE
        return "Delete";
        break;
    case 0x0070: // GLUT_KEY_SHIFT_L
        return "Shift";
        break;
    case 0x0071: // GLUT_KEY_SHIFT_R
        return "Shift";
        break;
    case 0x0072: // GLUT_KEY_CTRL_L
        return "Ctrl";
        break;
    case 0x0073: // GLUT_KEY_CTRL_R
        return "Ctrl";
        break;
    case 0x0074: // GLUT_KEY_ALT_L
        return "Alt";
        break;
    case 0x0075: // GLUT_KEY_ALT_R
        return "Alt";
        break;
    }
    return "Undefined";
}
} // namespace Viperfish
