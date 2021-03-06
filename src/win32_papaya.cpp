#include <windows.h>
#include <windowsx.h>
#include <malloc.h>
#include <commdlg.h>

#undef GetWindowFont


#include "papaya_platform_common.impl"

#define PAPAYA_DEFAULT_IMAGE "C:\\Users\\Apoorva\\Pictures\\ImageTest\\test4k.jpg"
#include "papaya.cpp"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

// =================================================================================================
//global_variable RECT WindowsWorkArea; // Needed because WS_POPUP by default maximizes to cover task bar

// =================================================================================================
void Platform::Print(char* Message)
{
    OutputDebugStringA((LPCSTR)Message);
}

void Platform::StartMouseCapture()
{
    SetCapture(GetActiveWindow());
}

void Platform::ReleaseMouseCapture()
{
    ReleaseCapture();
}

void Platform::SetMousePosition(Vec2 Pos)
{
    RECT Rect;
    GetWindowRect(GetActiveWindow(), &Rect);
    SetCursorPos(Rect.left + (int32)Pos.x, Rect.top + (int32)Pos.y);
}

void Platform::SetCursorVisibility(bool Visible)
{
    ShowCursor(Visible);
}

int64 Platform::GetMilliseconds()
{
	//return (int64)glfwGetTime()*1000.0;
    LARGE_INTEGER ms;
    QueryPerformanceCounter(&ms);
    return ms.QuadPart;
}

void GlfwMouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/)
{
	ImGuiIO& io = ImGui::GetIO();
	if (button >= 0 && button < 3)
		io.MouseDown[button] = (action == GLFW_PRESS);
}

void GlfwScrollCallback(GLFWwindow*, double /*xoffset*/, double yoffset)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += (float)yoffset;//GET_WHEEL_DELTA_WPARAM(WParam) > 0 ? +1.0f : -1.0f;
}


void GlfwKeyCallback(GLFWwindow*, int key, int, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = (action == GLFW_PRESS);

	(void)mods; // Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
}

void GlfwCharCallback(GLFWwindow*, unsigned int c)
{
	ImGuiIO& io = ImGui::GetIO();
	if (c > 0 && c < 0x10000)
		io.AddInputCharacter((unsigned short)c);
}


static LRESULT CALLBACK WinProcExtent(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	if (EasyTab_HandleEvent(Window, Message, LParam, WParam) == EASYTAB_OK)
		return S_OK;

	if (Message != WM_NCHITTEST)
		return (LRESULT)-1;
	
	const LONG BorderWidth = 8; //in pixels
	RECT WindowRect;
	GetWindowRect(Window, &WindowRect);
	long X = GET_X_LPARAM(LParam);
	long Y = GET_Y_LPARAM(LParam);

	if (!IsMaximized(Window))
	{
		//bottom left corner
		if (X >= WindowRect.left && X < WindowRect.left + BorderWidth &&
			Y < WindowRect.bottom && Y >= WindowRect.bottom - BorderWidth)
		{
			return HTBOTTOMLEFT;
		}
		//bottom right corner
		if (X < WindowRect.right && X >= WindowRect.right - BorderWidth &&
			Y < WindowRect.bottom && Y >= WindowRect.bottom - BorderWidth)
		{
			return HTBOTTOMRIGHT;
		}
		//top left corner
		if (X >= WindowRect.left && X < WindowRect.left + BorderWidth &&
			Y >= WindowRect.top && Y < WindowRect.top + BorderWidth)
		{
			return HTTOPLEFT;
		}
		//top right corner
		if (X < WindowRect.right && X >= WindowRect.right - BorderWidth &&
			Y >= WindowRect.top && Y < WindowRect.top + BorderWidth)
		{
			return HTTOPRIGHT;
		}
		//left border
		if (X >= WindowRect.left && X < WindowRect.left + BorderWidth)
		{
			return HTLEFT;
		}
		//right border
		if (X < WindowRect.right && X >= WindowRect.right - BorderWidth)
		{
			return HTRIGHT;
		}
		//bottom border
		if (Y < WindowRect.bottom && Y >= WindowRect.bottom - BorderWidth)
		{
			return HTBOTTOM;
		}
		//top border
		if (Y >= WindowRect.top && Y < WindowRect.top + BorderWidth)
		{
			return HTTOP;
		}
	}

	if (Y - WindowRect.top <= (float)Mem.Window.TitleBarHeight &&
		X > WindowRect.left + 200.0f &&
		X < WindowRect.right - (float)(Mem.Window.TitleBarButtonsWidth + 10))
	{
		return HTCAPTION;
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return HTCLIENT;
}


int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
	if (!glfwInit())
		exit(EXIT_FAILURE);

    QueryPerformanceFrequency((LARGE_INTEGER *)&Mem.Debug.TicksPerSecond);
    QueryPerformanceCounter((LARGE_INTEGER *)&Mem.Debug.Time);
    Util::StartTime(Timer_Startup, &Mem);

    Mem.IsRunning = true;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);


	uint32 ScreenWidth = mode->width;
	uint32 ScreenHeight = mode->height;
	const float WindowSize = 0.8f;
	Mem.Window.Width = (uint32)((float)ScreenWidth * WindowSize);
	Mem.Window.Height = (uint32)((float)ScreenHeight * WindowSize);
	uint32 WindowX = (ScreenWidth - Mem.Window.Width) / 2;
	uint32 WindowY = (ScreenHeight - Mem.Window.Height) / 2;


    //HWND Window;
    // Create Window
	glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);	
	//glfwWindowHint(GLFW_AUTO_ICONIFY, 1);	
	
	GLFWwindow* window = glfwCreateWindow(Mem.Window.Height, Mem.Window.Width, "Papaya", NULL, NULL);
	glfwSetWindowUserPointer(window, &WinProcExtent);

	HWND Window = glfwGetWin32Window(window);
	SetWindowPos(Window, HWND_TOP, WindowX, WindowY, Mem.Window.Width, Mem.Window.Height, NULL);
	//SetWindowPos(Window, HWND_TOP, 0, 0, 1280, 720, NULL);


	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    // TODO: Add an icon
    // WindowClass.hIcon = (HICON)LoadImageA(0, "../../img/papaya.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

	// NOTE:
	// http://www.gamedev.net/topic/545558-how-do-i-assign-an-icon-to-a-glfw-window-solved/
	// Add a resource file to your project(something.rc) with contents :
	// GLFW_ICON ICON "icon.ico"

    // Initialize OpenGL
	glfwMakeContextCurrent(window);

    if (glewInit() != 0)
    {
        // TODO: Log: GL3W Init failed
        exit(1);
    }

	//TODO: switch to GLWF
    //if (!glewIs(3,1))
    //{
    //    // TODO: Log: Required OpenGL version not supported
    //    exit(1);
    //}

    glGetIntegerv(GL_MAJOR_VERSION, &Mem.System.OpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &Mem.System.OpenGLVersion[1]);

   // Disable vsync
	glfwSwapInterval(0);



    // Initialize tablet
    EasyTab_Load(glfwGetWin32Window(window));

    Papaya::Initialize(&Mem);

    // Initialize ImGui
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeyMap[ImGuiKey_Tab] = VK_TAB;          // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
        io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
        io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
        io.KeyMap[ImGuiKey_Home] = VK_HOME;
        io.KeyMap[ImGuiKey_End] = VK_END;
        io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
        io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
        io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
        io.KeyMap[ImGuiKey_A] = 'A';
        io.KeyMap[ImGuiKey_C] = 'C';
        io.KeyMap[ImGuiKey_V] = 'V';
        io.KeyMap[ImGuiKey_X] = 'X';
        io.KeyMap[ImGuiKey_Y] = 'Y';
        io.KeyMap[ImGuiKey_Z] = 'Z';

        io.RenderDrawListsFn = Papaya::RenderImGui;
#ifdef _WIN32
        io.ImeWindowHandle = glfwGetWin32Window(window);
#endif
		glfwSetMouseButtonCallback(window, GlfwMouseButtonCallback);
		glfwSetScrollCallback(window, GlfwScrollCallback);
		glfwSetKeyCallback(window, GlfwKeyCallback);
		glfwSetCharCallback(window, GlfwCharCallback);
    }


    Mem.Window.MenuHorizontalOffset = 32;
    Mem.Window.TitleBarButtonsWidth = 109;
    Mem.Window.TitleBarHeight = 30;

    Util::StopTime(Timer_Startup, &Mem);

    // Handle command line arguments (if present)
    if (strlen(CommandLine)) { Papaya::OpenDocument(CommandLine, &Mem); }

#ifdef PAPAYA_DEFAULT_IMAGE
    Papaya::OpenDocument(PAPAYA_DEFAULT_IMAGE, &Mem);
#endif

    while (Mem.IsRunning && !glfwWindowShouldClose(window))
    {
        Util::StartTime(Timer_Frame, &Mem);

        // Tablet input // TODO: Put this in papaya.cpp
        {
            Mem.Tablet.Pressure = EasyTab->Pressure;
            Mem.Tablet.PosX = EasyTab->PosX;
            Mem.Tablet.PosY = EasyTab->PosY;
        }

        BOOL IsMaximized = IsMaximized(Window);
        if (IsIconic(Window)) { goto EndOfFrame; }

        // Start new ImGui frame
        {
            ImGuiIO& io = ImGui::GetIO();

            // Setup display size (every frame to accommodate for window resizing)
            RECT rect;
            GetClientRect(Window, &rect);
            io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
			io.DisplayFramebufferScale = ImVec2(1, 1);

            // Read keyboard modifiers inputs
            io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
            io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
            io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;

            // Setup time step
            INT64 current_time;
            QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
            io.DeltaTime = (float)(current_time - Mem.Debug.Time) / Mem.Debug.TicksPerSecond;
            Mem.Debug.Time = current_time; // TODO: Move Imgui timers from Debug to their own struct

			if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
			{
				double mouse_x, mouse_y;
				glfwGetCursorPos(window, &mouse_x, &mouse_y);
				io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);   // Mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
			}
			else
			{
				io.MousePos = ImVec2(-1, -1);
			}

            // Hide OS mouse cursor if ImGui is drawing it
            //SetCursor(io.MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));

            // Start the frame
            ImGui::NewFrame();
        }

        // Title Bar Icon
        {
            ImGui::SetNextWindowSize(ImVec2((float)Mem.Window.MenuHorizontalOffset,(float)Mem.Window.TitleBarHeight));
            ImGui::SetNextWindowPos(ImVec2(1.0f, 1.0f));

            ImGuiWindowFlags WindowFlags = 0; // TODO: Create a commonly-used set of Window flags, and use them across ImGui windows
            WindowFlags |= ImGuiWindowFlags_NoTitleBar;
            WindowFlags |= ImGuiWindowFlags_NoResize;
            WindowFlags |= ImGuiWindowFlags_NoMove;
            WindowFlags |= ImGuiWindowFlags_NoScrollbar;
            WindowFlags |= ImGuiWindowFlags_NoCollapse;
            WindowFlags |= ImGuiWindowFlags_NoScrollWithMouse;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2,2));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0,0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

            ImGui::PushStyleColor(ImGuiCol_WindowBg, Mem.Colors[PapayaCol_Transparent]);

            bool bTrue = true;
            ImGui::Begin("Title Bar Icon", &bTrue, WindowFlags);
            ImGui::Image((void*)(intptr_t)Mem.Textures[PapayaTex_TitleBarIcon], ImVec2(28,28));
            ImGui::End();

            ImGui::PopStyleColor(1);
            ImGui::PopStyleVar(5);
        }

        // Title Bar Buttons
        {
            ImGui::SetNextWindowSize(ImVec2((float)Mem.Window.TitleBarButtonsWidth,24.0f));
            ImGui::SetNextWindowPos(ImVec2((float)Mem.Window.Width - Mem.Window.TitleBarButtonsWidth, 0.0f));

            ImGuiWindowFlags WindowFlags = 0;
            WindowFlags |= ImGuiWindowFlags_NoTitleBar;
            WindowFlags |= ImGuiWindowFlags_NoResize;
            WindowFlags |= ImGuiWindowFlags_NoMove;
            WindowFlags |= ImGuiWindowFlags_NoScrollbar;
            WindowFlags |= ImGuiWindowFlags_NoCollapse;
            WindowFlags |= ImGuiWindowFlags_NoScrollWithMouse;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0,0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

            ImGui::PushStyleColor(ImGuiCol_Button, Mem.Colors[PapayaCol_Transparent]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Mem.Colors[PapayaCol_ButtonHover]);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, Mem.Colors[PapayaCol_ButtonActive]);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, Mem.Colors[PapayaCol_Transparent]);

            bool bTrue = true;
            ImGui::Begin("Title Bar Buttons", &bTrue, WindowFlags);

            ImGui::PushID(0);
            if(ImGui::ImageButton((void*)(size_t)Mem.Textures[PapayaTex_TitleBarButtons], ImVec2(34,26), ImVec2(0.5,0), ImVec2(1,0.5f), 1, ImVec4(0,0,0,0)))
            {
                ShowWindow(Window, SW_MINIMIZE);
            }

            ImVec2 StartUV = IsMaximized ? ImVec2(0.0f,0.5f) : ImVec2(0.5f,0.5f);
            ImVec2 EndUV   = IsMaximized ? ImVec2(0.5f,1.0f) : ImVec2(1.0f,1.0f);

            ImGui::PopID();
            ImGui::SameLine();
            ImGui::PushID(1);

            if(ImGui::ImageButton((void*)(size_t)Mem.Textures[PapayaTex_TitleBarButtons], ImVec2(34,26), StartUV, EndUV, 1, ImVec4(0,0,0,0)))
            {
                if (IsMaximized)
                {
                    ShowWindow(Window, SW_RESTORE);
                }
                else
                {
                    ShowWindow(Window, SW_MAXIMIZE);
                }
            }

            ImGui::PopID();
            ImGui::SameLine();
            ImGui::PushID(2);

            if(ImGui::ImageButton((void*)(size_t)Mem.Textures[PapayaTex_TitleBarButtons], ImVec2(34,26), ImVec2(0,0), ImVec2(0.5f,0.5f), 1, ImVec4(0,0,0,0)))
            {
                SendMessage(Window, WM_CLOSE, 0, 0);
            }

            ImGui::PopID();

            ImGui::End();

            ImGui::PopStyleVar(5);
            ImGui::PopStyleColor(4);
        }

        //ImGui::ShowTestWindow();
        Papaya::UpdateAndRender(&Mem);
		glfwSwapBuffers(window);
		glfwPollEvents();

    EndOfFrame:
        Util::StopTime(Timer_Frame, &Mem);
        double FrameRate = (Mem.CurrentTool == PapayaTool_Brush) ? 500.0 : 60.0;
        double FrameTime = 1000.0 / FrameRate;
        double SleepTime = FrameTime - Mem.Debug.Timers[Timer_Frame].MillisecondsElapsed;
        Mem.Debug.Timers[Timer_Sleep].MillisecondsElapsed = SleepTime;
        if (SleepTime > 0) { Sleep((int32)SleepTime); }
    }

    Papaya::Shutdown(&Mem);

    EasyTab_Unload();

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}


FILE* Platform::openFile(const char* filename, const char* flags) {
	const int requiredSizeFilename = MultiByteToWideChar(CP_UTF8, 0, filename, -1, 0, 0);
	const int requiredSizeFlags = MultiByteToWideChar(CP_UTF8, 0, flags, -1, 0, 0);
	if (requiredSizeFilename <= 0 || requiredSizeFlags <=0 ) return NULL;
	wchar_t *buffFilename = (wchar_t *)malloc(sizeof(wchar_t)*requiredSizeFilename);
	wchar_t *buffFlags = (wchar_t *)malloc(sizeof(wchar_t)*requiredSizeFlags);
	
	const int resFilename = MultiByteToWideChar(CP_UTF8, 0, filename, -1, buffFilename, requiredSizeFilename);
	const int resFlags = MultiByteToWideChar(CP_UTF8, 0, flags, -1, buffFlags, requiredSizeFlags);

	FILE* file = NULL;
	if (resFilename > 0 && resFilename > 0) {
		file = _wfopen(buffFilename, buffFlags);
	}

	free(buffFilename);
	free(buffFlags);

	return file;
}