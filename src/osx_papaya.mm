#include <memory.h>
#include "papaya_platform_common.impl"

#define PAPAYA_DEFAULT_IMAGE "C:\\Users\\Apoorva\\Pictures\\ImageTest\\test4k.jpg"
#include "papaya.cpp"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#include "GLFW/glfw3native.h"

GLFWwindow* window = NULL;

// =================================================================================================
//global_variable RECT WindowsWorkArea; // Needed because WS_POPUP by default maximizes to cover task bar

// =================================================================================================
void Platform::Print(char* Message)
{
    printf(Message);
//    OutputDebugStringA((LPCSTR)Message);
}

void Platform::StartMouseCapture()
{
//    SetCapture(GetActiveWindow());
}

void Platform::ReleaseMouseCapture()
{
//    ReleaseCapture();
}

void Platform::SetMousePosition(Vec2 Pos)
{
    glfwSetCursorPos(window, Pos.x, Pos.y);
}

void Platform::SetCursorVisibility(bool Visible)
{
    glfwSetInputMode(window, GLFW_CURSOR, Visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

int64 Platform::GetMilliseconds()
{
	return (int64)glfwGetTime()*1000.0;
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
	io.MouseWheel += (float)yoffset;
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

void GlfwDropCalback(GLFWwindow* window,int num,const char** paths) {
    Papaya::OpenDocument(paths[0], &Mem);
}


int main()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
    
    Mem.Debug.TicksPerSecond= 1000;
    Mem.Debug.Time = glfwGetTime();
    
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
//	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwWindowHint(GLFW_AUTO_ICONIFY, 1);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	window = glfwCreateWindow(800, 600, "Papaya", NULL, NULL);

    glfwSetWindowPos(window, WindowX, WindowY);
    glfwSetWindowSize(window, Mem.Window.Width, Mem.Window.Height);
    

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

    int  minor = -1;
    int major = -1;
    
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);// >= 2 ||
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);// >= 3)
    
    glGetIntegerv(GL_MAJOR_VERSION, &Mem.System.OpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &Mem.System.OpenGLVersion[1]);
    
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

   // Disable vsync
	glfwSwapInterval(0);



    // Initialize tablet
//    EasyTab_Load(glfwGetWin32Window(window));
//    EasyTab_Load

    Papaya::Initialize(&Mem);

    // Initialize ImGui
    {
        ImGuiIO& io = ImGui::GetIO();
        
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;          // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
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
    
    glfwSetDropCallback(window, &GlfwDropCalback);

    Mem.Window.MenuHorizontalOffset = 32;
    Mem.Window.TitleBarButtonsWidth = 109;
    Mem.Window.TitleBarHeight = 30;

    Util::StopTime(Timer_Startup, &Mem);

    // Handle command line arguments (if present)
//    if (strlen(CommandLine)) { Papaya::OpenDocument(CommandLine, &Mem); }

#ifdef PAPAYA_DEFAULT_IMAGE
    Papaya::OpenDocument(PAPAYA_DEFAULT_IMAGE, &Mem);
#endif
    
    BOOL IsMaximized = FALSE;

    while (Mem.IsRunning && !glfwWindowShouldClose(window))
    {
        Util::StartTime(Timer_Frame, &Mem);

        // Tablet input // TODO: Put this in papaya.cpp
        if (EasyTab)
        {
            Mem.Tablet.Pressure = EasyTab->Pressure;
            Mem.Tablet.PosX = EasyTab->PosX;
            Mem.Tablet.PosY = EasyTab->PosY;
        }

       if (glfwGetWindowAttrib(window, GLFW_ICONIFIED)) { goto EndOfFrame; }

        // Start new ImGui frame
        {
            ImGuiIO& io = ImGui::GetIO();
            int width, height;
            glfwGetWindowSize(window,&width, &height);
            
            glfwGetFramebufferSize(window,&Mem.Window.Width, &Mem.Window.Height);

            // Setup display size (every frame to accommodate for window resizing)
            io.DisplaySize = ImVec2(width, height);
			io.DisplayFramebufferScale = ImVec2(1, 1);
            
            // Read keyboard modifiers inputs
            io.KeyCtrl = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_RELEASE) ||
                         (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) != GLFW_RELEASE);

            io.KeyShift = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_RELEASE) ||
                         (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) != GLFW_RELEASE);
            
            io.KeyAlt = (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_RELEASE) ||
                        (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) != GLFW_RELEASE);

            // Setup time step
            double current_time = Platform::GetMilliseconds();
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
            Platform::SetCursorVisibility(io.MouseDrawCursor);

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
                glfwIconifyWindow(window);
            }

            bool IsMaximized = false;
            ImVec2 StartUV = IsMaximized ? ImVec2(0.0f,0.5f) : ImVec2(0.5f,0.5f);
            ImVec2 EndUV   = IsMaximized ? ImVec2(0.5f,1.0f) : ImVec2(1.0f,1.0f);

            ImGui::PopID();
            ImGui::SameLine();
            ImGui::PushID(1);

            if(ImGui::ImageButton((void*)(size_t)Mem.Textures[PapayaTex_TitleBarButtons], ImVec2(34,26), StartUV, EndUV, 1, ImVec4(0,0,0,0)))
            {
                if (IsMaximized)
                {
                    glfwRestoreWindow(window);
                }
                else
                {
                    glfwMaximizeWindow(window);
//                    ShowWindow(Window, SW_MAXIMIZE);
                }
            }

            ImGui::PopID();
            ImGui::SameLine();
            ImGui::PushID(2);

            if(ImGui::ImageButton((void*)(size_t)Mem.Textures[PapayaTex_TitleBarButtons], ImVec2(34,26), ImVec2(0,0), ImVec2(0.5f,0.5f), 1, ImVec4(0,0,0,0)))
            {
                glfwSetWindowShouldClose(window, GL_TRUE);            }

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
        if (SleepTime > 0) {
//            Sleep((int32)SleepTime);
        }
    }

    Papaya::Shutdown(&Mem);

//    EasyTab_Unload();

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}


FILE* Platform::openFile(const char* filename, const char* flags) {
    FILE* file = NULL;
    
    if (!filename || !flags)
        return NULL;
    
    /* If the file mode is writable, skip all the bundle stuff because generally the bundle is read-only. */
    if(strcmp("r", flags) && strcmp("rb", flags))
    {
        return fopen(filename, flags);
    }
    
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];
    
    
    NSFileManager* file_manager = [NSFileManager defaultManager];
    NSString* resource_path = [[NSBundle mainBundle] resourcePath];
    
    NSString* ns_string_file_component = [file_manager stringWithFileSystemRepresentation:filename length:strlen(filename)];
    
    NSString* full_path_with_file_to_try = [resource_path stringByAppendingPathComponent:ns_string_file_component];
    if([file_manager fileExistsAtPath:full_path_with_file_to_try])
    {
        file = fopen([full_path_with_file_to_try fileSystemRepresentation], flags);
    }
    else
    {
        file = fopen(filename, flags);
    }
    
    [autorelease_pool drain];
    
    return file;
}