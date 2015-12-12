#define PAPAYA_DEFAULT_IMAGE "/home/apoorva/Downloads/test4k.jpg"
#include "papaya_platform_common.impl"

#pragma GCC diagnostic ignored "-Wwrite-strings" //TODO: Check if all string warnings can be eliminated without suppression
#include <x86intrin.h>

#include "papaya.cpp"

#include <X11/Xlib.h>
#include <unistd.h>
#include <time.h>

#ifdef USE_GTK
#include <gtk/gtk.h>
#endif

global_variable Display* XlibDisplay;
global_variable Window XlibWindow;
// =================================================================================================

void Platform::Print(char* Message)
{
    printf("%s", Message);
}

void Platform::StartMouseCapture()
{
    //
}

void Platform::ReleaseMouseCapture()
{
    //
}

void Platform::SetMousePosition(Vec2 Pos)
{
    XWarpPointer(XlibDisplay, None, XlibWindow, 0, 0, 0, 0, (int32)Pos.x, (int32)Pos.y);
    XFlush(XlibDisplay);
}

void Platform::SetCursorVisibility(bool Visible)
{
    if (!Visible)
    {
        // Make a blank cursor
        char Empty[1] = {0};
        Pixmap Blank = XCreateBitmapFromData (XlibDisplay, XlibWindow, Empty, 1, 1);
        if(Blank == None) fprintf(stderr, "error: out of memory.\n");
        XColor Dummy;
        Cursor InvisCursor = XCreatePixmapCursor(XlibDisplay, Blank, Blank, &Dummy, &Dummy, 0, 0);
        XFreePixmap (XlibDisplay, Blank);
        XDefineCursor(XlibDisplay, XlibWindow, InvisCursor);
    }
    else
    {
        XUndefineCursor(XlibDisplay, XlibWindow); // TODO: Test what happens if cursor is attempted to be shown when it is not hidden in the first place.
    }
}

int64 Platform::GetMilliseconds()
{
    timespec Time;
    clock_gettime(CLOCK_MONOTONIC, &Time);
    return (int64)(Time.tv_sec * 1000 + Time.tv_nsec / 1000000);
}

// =================================================================================================

int main(int argc, char **argv)
{
    Mem.Debug.TicksPerSecond = 1000;
    Util::StartTime(Timer_Startup, &Mem);

    XVisualInfo* VisualInfo;
    Atom WmDeleteMessage;

#ifdef USE_GTK
    // Initialize GTK for Open/Save file dialogs
    gtk_init(&argc, &argv);
#endif

    // Create window
    {
         XlibDisplay = XOpenDisplay(0);
         if (!XlibDisplay)
         {
            // TODO: Log: Error opening connection to the X server
            exit(1);
         }

        int32 Attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
         VisualInfo = glXChooseVisual(XlibDisplay, 0, Attributes);
        if(VisualInfo == NULL)
        {
            // TODO: Log: No appropriate visual found
            exit(1);
        }

        XSetWindowAttributes SetWindowAttributes = {0};
        SetWindowAttributes.colormap = XCreateColormap(XlibDisplay, DefaultRootWindow(XlibDisplay), VisualInfo->visual, AllocNone);
        SetWindowAttributes.event_mask = ExposureMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;

        XlibWindow = XCreateWindow(XlibDisplay, DefaultRootWindow(XlibDisplay), 0, 0, 600, 600, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask, &SetWindowAttributes);
        Mem.Window.Width = 600;
        Mem.Window.Height = 600;

        XMapWindow(XlibDisplay, XlibWindow);
         XStoreName(XlibDisplay, XlibWindow, "Papaya");

        WmDeleteMessage = XInternAtom(XlibDisplay, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(XlibDisplay, XlibWindow, &WmDeleteMessage, 1);

        // Window icon
        {
            /*
               Utility to print out icon data
               ==============================
               Linux apparently needs pixel data to be in a 64-bit-per-pixel buffer in ARGB format.
               stb_image gives a 32-bit-per-pixel ABGR output. This function is used to pre-calculate
               and print the swizzled and expanded buffer in the correct format to minimize startup time.

               TODO: The 48x48 icon is rendered blurry in the dock (at least on Elementary OS). Investigate why.
            */
            #if 0
            {
                int32 ImageWidth, ImageHeight, ComponentsPerPixel;
                uint8* Image = stbi_load("../../img/icon48.png", &ImageWidth, &ImageHeight, &ComponentsPerPixel, 4);
                printf("{%d,%d",ImageWidth, ImageHeight);
                for (int32 i = 0; i < ImageWidth * ImageHeight; i++)
                {
                    uint32 Pixel = ((uint32*)Image)[i];
                    uint32 ARGB = 	((Pixel & 0xff000000) >> 00) |
                                    ((Pixel & 0x000000ff) << 16) |
                                    ((Pixel & 0x0000ff00) >> 00) |
                                    ((Pixel & 0x00ff0000) >> 16);

                    printf(",%u", ARGB);
                }
                printf("};\n");
            }
            #endif

            uint64 buffer[] = {48,48,0,0,0,0,0,0,0,0,0,0,0,2960687,0,0,2960687,19737903,0,422391087,1445801263,2402102575,3140300079,3710725423,4063046959,4247596335,4247596335,4063046959,3710725423,3140300079,2402102575,1445801263,422391087,0,19737903,2960687,0,0,2960687,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2960687,0,0,2960687,36515119,372059439,1714236719,3375181103,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3375181103,1714236719,372059439,36515119,2960687,0,0,2960687,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2960687,0,0,36515119,556608815,2267884847,4046269743,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4046269743,2267884847,556608815,36515119,0,0,2960687,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2960687,2960687,0,2960687,321727791,1848454447,3928829231,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3928829231,1848454447,321727791,2960687,0,2960687,2960687,0,0,0,0,0,0,0,0,0,0,0,2960687,0,0,19737903,892153135,3089968431,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3089968431,892153135,19737903,0,0,2960687,0,0,0,0,0,0,0,0,0,2960687,0,0,86846767,1445801263,3794611503,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3794611503,1445801263,86846767,0,0,2960687,0,0,0,0,0,0,0,2960687,0,0,137178415,1764568367,4113378607,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4113378607,1764568367,137178415,0,0,2960687,0,0,0,0,0,0,2960687,0,86846767,1764568367,4214041903,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4214041903,1764568367,86846767,0,2960687,0,0,0,0,0,2960687,0,19737903,1445801263,4113378607,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4113378607,1445801263,19737903,0,2960687,0,0,0,2960687,0,2960687,892153135,3794611503,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4280756526,4280756526,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3794611503,892153135,2960687,0,2960687,0,0,0,0,321727791,3089968431,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4280822062,4281282096,4284436027,4284436027,4281282352,4280822062,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3089968431,321727791,0,0,0,2960687,0,36515119,1848454447,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4280887854,4280756526,4283516472,4287327302,4291401300,4291401300,4287327558,4283516472,4280756526,4280887854,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,1848454447,36515119,0,2960687,0,2960687,556608815,3928829231,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281019439,4280493613,4282530868,4286473283,4290284369,4293044058,4292846938,4292846938,4293044058,4290284369,4286473283,4282530868,4280493613,4281019439,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3928829231,556608815,2960687,0,0,36515119,2267884847,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281085231,4280624941,4281545009,4285487423,4289364557,4292518489,4292912730,4292649817,4292649817,4292649817,4292649817,4292912730,4292518489,4289364557,4285487423,4281545009,4280624941,4281085231,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,2267884847,36515119,0,2960687,372059439,4046269743,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4280822318,4280822318,4284304698,4288641611,4291795798,4292912730,4292715609,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292715609,4292912730,4291795798,4288641611,4284304698,4280822318,4280822318,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4046269743,372059439,2960687,19737903,1714236719,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281019182,4280493613,4282990646,4287721799,4291138387,4292781402,4292846938,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292846938,4292781402,4291138387,4287721799,4282990646,4280493613,4281019182,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,1714236719,19737903,0,3375181103,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4280953646,4282070834,4286538819,4290481489,4292452697,4292846938,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292846938,4292452697,4290481489,4286538818,4282070577,4280953646,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3375181103,0,422391087,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281150767,4286867524,4292452696,4292846938,4292715609,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292715353,4292781145,4292584798,4287065420,4281151023,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,422391087,1445801263,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4288050249,4293306715,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649560,4292650331,4292979814,4293834866,4288380246,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,1445801263,2402102575,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649816,4292649817,4292913763,4293177709,4293243760,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,2402102575,3140300079,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292781919,4293111659,4293243760,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3140300079,3710725423,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292715867,4292980073,4293243761,4293243761,4293243761,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3710725423,4063046959,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649818,4292913764,4293243504,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4063046959,4247596335,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292847712,4293177710,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4247596335,4247596335,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4247596335,4063046959,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4063046959,3710725423,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3710725423,3140300079,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3140300079,2402102575,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4287984456,4293175643,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293835124,4288314711,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,2402102575,1445801263,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281216560,4288050249,4293306715,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293900917,4288446039,4281216560,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,1445801263,422391087,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4281150767,4286933060,4292452696,4292846938,4292715609,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293309553,4293440882,4293046640,4287197264,4281150767,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,422391087,0,3375181103,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4280953646,4282136370,4286538819,4290481489,4292452697,4292846938,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293243761,4293441138,4293046640,4291009125,4286803022,4282136628,4280953646,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3375181103,0,19737903,1714236719,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281019182,4280493613,4282990646,4287721799,4291138387,4292781402,4292846938,4292649817,4292649817,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293243761,4293243761,4293440882,4293440882,4291666280,4288117333,4283056697,4280493611,4280953646,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,1714236719,19737903,2960687,372059439,4046269743,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4280822318,4280822318,4284304698,4288641611,4291795798,4292912730,4292715609,4292649817,4292649817,4292649817,4293243761,4293243761,4293243761,4293309553,4293572211,4292389228,4289037402,4284437057,4280822061,4280822061,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4046269743,372059439,2960687,0,36515119,2267884847,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281085231,4280624941,4281545009,4285487423,4289430093,4292518489,4292912730,4292649817,4292649817,4293243761,4293243761,4293572211,4293112176,4289826143,4285751368,4281545265,4280559404,4281085231,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,2267884847,36515119,0,0,2960687,556608815,3928829231,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281019439,4280493613,4282530868,4286473283,4290284369,4293044058,4292846937,4293440882,4293638003,4290812004,4286737230,4282596663,4280493611,4281019438,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3928829231,556608815,2960687,0,2960687,0,36515119,1848454447,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4280887854,4280756526,4283516472,4287327558,4291401300,4291929450,4287591762,4283648317,4280756525,4280887854,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,1848454447,36515119,0,2960687,0,0,0,321727791,3089968431,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4280822062,4281282352,4284436027,4284568386,4281282352,4280756525,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3089968431,321727791,0,0,0,0,2960687,0,2960687,892153135,3794611503,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281084975,4280756526,4280756269,4281084975,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3794611503,892153135,2960687,0,2960687,0,0,0,2960687,0,19737903,1445801263,4113378607,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4113378607,1445801263,19737903,0,2960687,0,0,0,0,0,2960687,0,86846767,1764568367,4214041903,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4214041903,1764568367,86846767,0,2960687,0,0,0,0,0,0,2960687,0,0,137178415,1764568367,4113378607,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4113378607,1764568367,137178415,0,0,2960687,0,0,0,0,0,0,0,2960687,0,0,86846767,1445801263,3794611503,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3794611503,1445801263,86846767,0,0,2960687,0,0,0,0,0,0,0,0,0,2960687,0,0,19737903,892153135,3089968431,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3089968431,892153135,19737903,0,0,2960687,0,0,0,0,0,0,0,0,0,0,0,2960687,2960687,0,2960687,321727791,1848454447,3928829231,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3928829231,1848454447,321727791,2960687,0,2960687,2960687,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2960687,0,0,36515119,556608815,2267884847,4046269743,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4046269743,2267884847,556608815,36515119,0,0,2960687,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2960687,0,0,2960687,36515119,372059439,1714236719,3375181103,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,4281150767,3375181103,1714236719,372059439,36515119,2960687,0,0,2960687,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2960687,0,0,2960687,19737903,0,422391087,1445801263,2402102575,3140300079,3710725423,4063046959,4247596335,4247596335,4063046959,3710725423,3140300079,2402102575,1445801263,422391087,0,19737903,2960687,0,0,2960687,0,0,0,0,0,0,0,0,0,0,0};

            //printf("%d\n", sizeof(buffer));
            //uint64 buffer[] = {2,2,4294901760,4278255360,4292649817,4294967295};
            Atom net_wm_icon = XInternAtom(XlibDisplay, "_NET_WM_ICON", False);
            Atom cardinal = XInternAtom(XlibDisplay, "CARDINAL", False);
            int32 length = sizeof(buffer)/8;
            XChangeProperty(XlibDisplay, XlibWindow, net_wm_icon, cardinal, 32, PropModeReplace, (uint8*) buffer, length);


        }
    }

    // Create OpenGL context
    {
        GLXContext GraphicsContext = glXCreateContext(XlibDisplay, VisualInfo, 0, GL_TRUE);
        glXMakeCurrent(XlibDisplay, XlibWindow, GraphicsContext);
        if (gl3wInit() != 0)
        {
            // TODO: Log: GL3W Init failed
            Platform::Print("Gl3w init failed.\n");
            exit(1);
        }

        if (!gl3wIsSupported(3,1))
        {
            // TODO: Log: Required OpenGL version not supported
            Platform::Print("Required OpenGL version not supported.\n");
            exit(1);
        }

        glGetIntegerv(GL_MAJOR_VERSION, &Mem.System.OpenGLVersion[0]);
        glGetIntegerv(GL_MINOR_VERSION, &Mem.System.OpenGLVersion[1]);
        printf("%d, %d\n", Mem.System.OpenGLVersion[0], Mem.System.OpenGLVersion[1]);

        // TODO: Disable vsync
        // glXSwapIntervalEXT(0);
    }

    EasyTab_Load(XlibDisplay, XlibWindow);

    Papaya::Initialize(&Mem);

    // Initialize ImGui
    {
        // TODO: Profiler timer setup

        ImGuiIO& io = ImGui::GetIO();
        io.RenderDrawListsFn = Papaya::RenderImGui;

        // Keyboard mappings
        {
            io.KeyMap[ImGuiKey_Z] = XK_z;
        }
    }

    Util::StopTime(Timer_Startup, &Mem);

#ifdef PAPAYA_DEFAULT_IMAGE
    Papaya::OpenDocument(PAPAYA_DEFAULT_IMAGE, &Mem);
#endif

    Mem.IsRunning = true;

    while (Mem.IsRunning)
    {
        Util::StartTime(Timer_Frame, &Mem);

        // Event handling
        while (XPending(XlibDisplay))
        {
            XEvent Event;
            XNextEvent(XlibDisplay, &Event);

            if (EasyTab_HandleEvent(&Event) == EASYTAB_OK) { continue; }

            switch (Event.type)
            {
                case Expose:
                {
                    XWindowAttributes WindowAttributes;
                    XGetWindowAttributes(XlibDisplay, XlibWindow, &WindowAttributes);
                    ImGui::GetIO().DisplaySize = ImVec2((float)WindowAttributes.width, (float)WindowAttributes.height);
                    Mem.Window.Width = WindowAttributes.width;
                    Mem.Window.Height = WindowAttributes.height;
                } break;

                case ClientMessage:
                {
                    if (Event.xclient.data.l[0] == WmDeleteMessage) { Mem.IsRunning = false; }
                } break;

                case MotionNotify:
                {
                    ImGui::GetIO().MousePos.x = Event.xmotion.x;
                    ImGui::GetIO().MousePos.y = Event.xmotion.y;
                } break;

                case ButtonPress:
                case ButtonRelease:
                {
                    int32 Button = Event.xbutton.button;
                    if 		(Button == 1) { Button = 0; } // This section maps Xlib's button indices (Left = 1, Middle = 2, Right = 3)
                    else if (Button == 2) { Button = 2; } // to Papaya's button indices              (Left = 0, Right = 1, Middle = 2)
                    else if (Button == 3) { Button = 1; } //

                    if 		(Button < 3)	{ ImGui::GetIO().MouseDown[Button] = (Event.type == ButtonPress); }
                    else
                    {
                        if (Event.type == ButtonPress) { ImGui::GetIO().MouseWheel += (Button == 4) ? +1.0f : -1.0f; }
                    }
                } break;

                case KeyPress:
                case KeyRelease:
                {
                    if (Event.type == KeyRelease && XEventsQueued(XlibDisplay, QueuedAfterReading))
                    {
                        XEvent NextEvent;
                        XPeekEvent(XlibDisplay, &NextEvent);
                        if (NextEvent.type == KeyPress &&
                            NextEvent.xkey.time == Event.xkey.time &&
                            NextEvent.xkey.keycode == Event.xkey.keycode)
                        {
                            break; // Key wasn’t actually released
                        }
                    }

                    int32 keysym = XLookupKeysym(&Event.xkey, 0);
                    switch (keysym) // List of keysym's can be found in keysymdef.h or here [http://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h]
                    {
                        case XK_Control_L:
                        case XK_Control_R: { ImGui::GetIO().KeyCtrl  = (Event.type == KeyPress); } break;
                        case XK_Shift_L:
                        case XK_Shift_R:   { ImGui::GetIO().KeyShift = (Event.type == KeyPress); } break;
                        case XK_Alt_L:
                        case XK_Alt_R:     { ImGui::GetIO().KeyAlt   = (Event.type == KeyPress); } break;

                        case XK_z:
                        { ImGui::GetIO().KeysDown[keysym] = (Event.type == KeyPress); } break;
                    }
                } break;
            }
        }

        // Update and render
        {
            ImGui::NewFrame();

            Papaya::UpdateAndRender(&Mem);
            glXSwapBuffers(XlibDisplay, XlibWindow);
        }

#ifdef USE_GTK
        // Run a GTK+ loop, and *don't* block if there are no events pending
        gtk_main_iteration_do(FALSE);
#endif

        // End Of Frame
        Util::StopTime(Timer_Frame, &Mem);
        double FrameRate = (Mem.CurrentTool == PapayaTool_Brush) ? 500.0 : 60.0;
        double FrameTime = 1000.0 / FrameRate;
        double SleepTime = FrameTime - Mem.Debug.Timers[Timer_Frame].MillisecondsElapsed;
        Mem.Debug.Timers[Timer_Sleep].MillisecondsElapsed = SleepTime;
        if (SleepTime > 0) { usleep((uint32)SleepTime * 1000); }
    }

    //Papaya::Shutdown(&Mem);
    EasyTab_Unload();

    return 0;
}

FILE* Platform::openFile(const char* filename, const char* flags) {
	if (!file || !flags) return NULL;
	FILE* file = fopen(filename, flags);
	return file;
}