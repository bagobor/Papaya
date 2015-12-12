#pragma once

#include <stdio.h>

struct PapayaMemory;

namespace Platform
{
    void Print(char* Message);
    void StartMouseCapture();
    void ReleaseMouseCapture();
    void SetMousePosition(Vec2 Pos);
    void SetCursorVisibility(bool Visible);
    char* OpenFileDialog();
    char* SaveFileDialog();

	FILE* openFile(const char* filename, const char* flags);
	
	int64 GetMilliseconds();

	PapayaMemory* GetMem();
}
