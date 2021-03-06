#pragma once

#include "GL/glew.h"

#include "papaya_math.h"
#include "papaya_platform.h"

// Implicit conversions between papaya_math.h structs and imgui.h structs
#define IM_VEC2_CLASS_EXTRA                                               \
        ImVec2(const Vec2& f) { x = f.x; y = f.y; }                       \
        operator Vec2() const { return Vec2(x,y); }

#define IM_VEC4_CLASS_EXTRA                                               \
        ImVec4(const Color& f){ x = f.r; y = f.g; z = f.b; w = f.a; }     \
        operator Color() const { return Color(x,y,z,w); }

#include "imgui.h"
//#include "imgui_draw.cpp"
//#include "imgui.cpp"
//#pragma warning (disable: 4312) // Warning C4312 during 64 bit compilation: 'type cast': conversion from 'uint32' to 'void *' of greater size
//#include "imgui_demo.cpp"
//#pragma warning (default: 4312)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

enum PapayaTex_
{
    PapayaTex_Font,
    PapayaTex_TitleBarButtons,
    PapayaTex_TitleBarIcon,
    PapayaTex_InterfaceIcons,
    PapayaTex_COUNT
};

enum PapayaCol_
{
    PapayaCol_Clear,
    PapayaCol_Workspace,
    PapayaCol_Transparent,
    PapayaCol_Button,
    PapayaCol_ButtonHover,
    PapayaCol_ButtonActive,
    PapayaCol_AlphaGrid1,
    PapayaCol_AlphaGrid2,
    PapayaCol_COUNT
};

enum PapayaMesh_
{
    PapayaMesh_ImGui,
    PapayaMesh_Canvas,
    PapayaMesh_AlphaGrid,
    PapayaMesh_BrushCursor,
    PapayaMesh_EyeDropperCursor,
    PapayaMesh_PickerHStrip,
    PapayaMesh_PickerSVBox,
    PapayaMesh_RTTBrush,
    PapayaMesh_RTTAdd,
    PapayaMesh_COUNT
};

enum PapayaShader_
{
    PapayaShader_ImGui,
    PapayaShader_Brush,
    PapayaShader_BrushCursor,
    PapayaShader_EyeDropperCursor,
    PapayaShader_PickerHStrip,
    PapayaShader_PickerSVBox,
    PapayaShader_AlphaGrid,
    PapayaShader_COUNT
};

enum PapayaUndoOp_
{
    PapayaUndoOp_Brush,
    PapayaUndoOp_COUNT
};

enum Timer_ // TODO: Improve timing subsystem
{
    Timer_Startup,
    Timer_Frame,
    Timer_Sleep,
    Timer_ImageOpen,
    Timer_COUNT
};

enum PapayaTool_
{
    PapayaTool_None,
    PapayaTool_Brush,
    PapayaTool_EyeDropper,
    PapayaTool_COUNT
};

struct SystemInfo
{
    int32 OpenGLVersion[2];
};

struct WindowInfo
{
    int Width, Height;
    uint32 MenuHorizontalOffset, TitleBarButtonsWidth, TitleBarHeight;
    float ProjMtx[4][4];
};

#pragma region Undo structs

struct UndoData // TODO: Convert into a union of structs once multiple types of undo ops are required
{
    uint8 OpCode; // Stores enum of type PapayaUndoOp_
    UndoData* Prev, *Next;
    size_t Size; // Size of the suffixed data block
    // Image data goes after this
};

struct UndoBufferInfo
{
    void* Start;   // Pointer to beginning of undo buffer memory block // TODO: Change pointer types to int8*?
    void* Top;     // Pointer to the top of the undo stack
    UndoData* Base;    // Pointer to the base of the undo stack
    UndoData* Current; // Pointer to the current location in the undo stack. Goes back and forth during undo-redo.
    UndoData* Last;    // Last undo data block in the buffer. Should be located just before Top.
    size_t Size;  // Size of the undo buffer in bytes
    int64 Count;  // Number of undo ops in buffer
    int64 CurrentIndex; // Index of the current undo data block from the beginning
};
#pragma endregion

struct DocumentInfo
{
    int32 Width, Height;
    int32 ComponentsPerPixel;
    uint32 TextureID;
    Vec2 CanvasPosition;
    float CanvasZoom;
    float InverseAspect;
    float ProjMtx[4][4];
    UndoBufferInfo Undo;
};

struct MouseInfo
{
    Vec2 Pos;     // TODO: Switch to int32s
    Vec2 LastPos; // TODO: Switch to int32s
    Vec2 UV;
    Vec2 LastUV;
    bool IsDown[3];
    bool WasDown[3];
    bool Pressed[3];
    bool Released[3];
    bool InWorkspace;
};

struct TabletInfo
{
    int32 PosX, PosY;
    float Pressure;
};

struct ShaderInfo
{
    uint32 Handle;
    int32 Attributes[8];
    int32 Uniforms[8];
};

struct MeshInfo
{
    size_t VboSize;
    uint32 VboHandle, VaoHandle, ElementsHandle;
};

struct BrushInfo
{
    int32 Diameter;
    int32 MaxDiameter;
    float Opacity;  // Range: [0.0, 1.0]
    float Hardness; // Range: [0.0, 1.0]
    bool AntiAlias;

    // TODO: Move some of this stuff to the MouseInfo struct?
    Vec2 RtDragStartPos;
    bool RtDragWithShift;
    int32 RtDragStartDiameter;
    float RtDragStartHardness, RtDragStartOpacity;
    bool BeingDragged;
};

struct EyeDropperInfo
{
    Color CurrentColor;
};

struct PickerInfo
{
    bool Open;
    Color CurrentColor, NewColor;
    Vec2 CursorSV;
    float CursorH;

    Vec2 Pos, Size, HueStripPos, HueStripSize, SVBoxPos, SVBoxSize;
    bool DraggingHue, DraggingSV;
};

struct TimerInfo
{
    uint64 StartCycleCount,   StopCycleCount,   CyclesElapsed;
    int64 StartMilliseconds, StopMilliseconds;
    double MillisecondsElapsed;
};

struct DebugInfo
{
    int64 Time;
    int64 TicksPerSecond;
    TimerInfo Timers[Timer_COUNT];
};

struct MiscInfo // TODO: This entire struct is for stuff to be refactored at some point
{
    uint32 FrameBufferObject;
    uint32 FboRenderTexture, FboSampleTexture;
    bool DrawOverlay;
    bool DrawCanvas;
    bool ShowMetricsWindow;
};

struct PapayaMemory
{
    bool IsRunning;
    SystemInfo System;
    WindowInfo Window;
    DocumentInfo Doc;
    MouseInfo Mouse;
    TabletInfo Tablet;
    DebugInfo Debug;

    uint32 Textures[PapayaTex_COUNT];
    Color Colors[PapayaCol_COUNT];
    MeshInfo Meshes[PapayaMesh_COUNT];
    ShaderInfo Shaders[PapayaShader_COUNT];

    PapayaTool_ CurrentTool;
    BrushInfo Brush;
    EyeDropperInfo EyeDropper;
    PickerInfo Picker;
    MiscInfo Misc;
};


#include "papaya_util.h"
