/* cprocess.h

Enigma's Diablo trainer, Copyright (c) 2020 Robert Martin

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "stdafx.h"
#include "resource.h"
#include "cprocess.h"
#include "bitdefinition.h"

//Global Variables:
HINSTANCE hInst;
//current instance
CProcess gameProcess;				

// Foward declarations of functions included in this code module:
BOOL InitInstance(HINSTANCE, int);
BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void WriteStrength(HWND hwnd, UINT);
void WriteDexterity(HWND hwnd, UINT);
void WriteMagic(HWND hwnd, UINT);
void WriteVitality(HWND hwnd, UINT);
void WriteName(HWND hwnd, char x[]);
void WriteCharLevel(HWND hwnd, UINT);
int WriteDistribPts(HWND hwnd, UINT);
void WriteHealth(HWND hwnd, UINT);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hDialog;

   hInst = hInstance; // Store instance handle in our global variable

   hDialog = CreateDialog(hInst, MAKEINTRESOURCE(MAIN_DIAG), 0, (DLGPROC)DialogProc);

   if (!hDialog)
   {
      return FALSE;
   }

   ShowWindow(hDialog, nCmdShow);
   UpdateWindow(hDialog);

   return TRUE;
}

// Mesage handler for about box.
BOOL CALLBACK DialogProc (HWND hwnd, UINT message, WPARAM wParam,
						  LPARAM lParam)
{
	UINT x;
	char y[10];

	switch (message){
		case WM_COMMAND:
			switch (LOWORD(wParam)){
				case STR_EDIT_INIT:
					x = GetDlgItemInt(hwnd, STR_EDITBOX, NULL, TRUE);
					//Check for values over 255
					if ( x > 255 )
					{
						char buf[100];
						wsprintf(buf, "Only values > 255", NULL);
						MessageBox(0, buf, "Value too high", MB_ICONEXCLAMATION | MB_OK);
						break;
					}
					WriteStrength(hwnd, x);
					break;
				case DEX_EDIT_INIT:
					x = GetDlgItemInt(hwnd, DEX2_EDITBOX, NULL, TRUE);
					//Check for values over 255
					if ( x > 255 )
					{
						char buf[100];
						wsprintf(buf, "Only values > 255", NULL);
						MessageBox(0, buf, "Value too high", MB_ICONEXCLAMATION | MB_OK);
						break;
					}
					WriteDexterity(hwnd, x);
					break;
				case MAG_EDIT_INIT:
					x = GetDlgItemInt(hwnd, MAG2_EDITBOX, NULL, TRUE);
					//Check for values over 255
					if ( x > 255 )
					{
						char buf[100];
						wsprintf(buf, "Only values > 255", NULL);
						MessageBox(0, buf, "Value too high", MB_ICONEXCLAMATION | MB_OK);
						break;
					}					
					WriteMagic(hwnd, x);
					break;
				case VIT_EDIT_INIT:
					x = GetDlgItemInt(hwnd, VIT2_EDITBOX, NULL, TRUE);
					//Check for values over 255
					if ( x > 255 )
					{
						char buf[100];
						wsprintf(buf, "Only values > 255", NULL);
						MessageBox(0, buf, "Value too high", MB_ICONEXCLAMATION | MB_OK);
						break;
					}				
					WriteVitality(hwnd, x);
					break;
				case HEALTH_EDIT_INIT:
					x = GetDlgItemInt(hwnd, HEALTH_EDITBOX, NULL, FALSE);
					//Check for values over 65535
					if ( x > 65535 )
					{
						char buf[100];
						wsprintf(buf, "Only values > 65535", NULL);
						MessageBox(0, buf, "Value too high", MB_ICONEXCLAMATION | MB_OK);
						break;
					}				
					WriteHealth(hwnd, x);
					break;

				case NAME_CHANGE_INIT:
					*y = GetDlgItemText(hwnd, NAME_EDITBOX, y, 10);
					WriteName(hwnd, y);
					break;
				case LEV_CHANGE_INIT:
					x = GetDlgItemInt(hwnd, LEVEL_EDITBOX, NULL, TRUE);
					//Check for values over 50
					if ( x > 50 )
					{
						char buf[100];
						wsprintf(buf, "Only values > 50", NULL);
						MessageBox(0, buf, "Value too high", MB_ICONEXCLAMATION | MB_OK);
						break;
					}					
					WriteCharLevel(hwnd, x);
					break;
				case DISTRIB_CHANGE_INIT:
					x = GetDlgItemInt(hwnd, DISTRIB_EDITBOX, NULL, TRUE);
					//Check for values over 255
					if ( x > 255 )
					{
						char buf[100];
						wsprintf(buf, "Only values > 255", NULL);
						MessageBox(0, buf, "Value too high", MB_ICONEXCLAMATION | MB_OK);
						break;
					}					
					WriteDistribPts(hwnd, x);
					break;
				case IDM_EXIT:
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
	}
	return false;
}

void WriteStrength(HWND hwnd, unsigned int x)
{
	bool isProcessRunning = gameProcess.IsRunning();

	if (!isProcessRunning)
		isProcessRunning = gameProcess.FindProcess("DIABLO");

	if (isProcessRunning)
	{
		gameProcess.WriteByte(STRENGTHBYTE, x);
		gameProcess.WriteByte(STRENGTHBYTE2, x);

	}
}

void WriteDexterity(HWND hwnd, unsigned int x)
{
	bool isProcessRunning = gameProcess.IsRunning();

	if (!isProcessRunning)
		isProcessRunning = gameProcess.FindProcess("DIABLO");

	if (isProcessRunning)
	{
		gameProcess.WriteByte(DEXBYTE, x);
		gameProcess.WriteByte(DEXBYTE2, x);

	}
}

void WriteMagic(HWND hwnd, unsigned int x)
{
	bool isProcessRunning = gameProcess.IsRunning();

	if (!isProcessRunning)
		isProcessRunning = gameProcess.FindProcess("DIABLO");

	if (isProcessRunning)
	{
		gameProcess.WriteByte(MAGICBYTE, x);
		gameProcess.WriteByte(MAGICBYTE2, x);

	}
}

void WriteVitality(HWND hwnd, unsigned int x)
{

	bool isProcessRunning = gameProcess.IsRunning();

	if (!isProcessRunning)
		isProcessRunning = gameProcess.FindProcess("DIABLO");

	if (isProcessRunning)
	{
		gameProcess.WriteByte(VITBYTE, x);
		gameProcess.WriteByte(VITBYTE2, x);

	}
}

void WriteName(HWND hwnd, char x[])
{
	int a = 0;

	bool isProcessRunning = gameProcess.IsRunning();

	if (!isProcessRunning)
		isProcessRunning = gameProcess.FindProcess("DIABLO");

	if (isProcessRunning)
	{
		while ( a < 10 )
		{
			gameProcess.WriteByte(NAMEBYTE, x[0]);
			a++;
		}
	}
}

void WriteCharLevel(HWND hwnd, unsigned int x)
{
	bool isProcessRunning = gameProcess.IsRunning();

	if (!isProcessRunning)
		isProcessRunning = gameProcess.FindProcess("DIABLO");

	if (isProcessRunning)
	{
		gameProcess.WriteByte(CLVLBYTE, x);
	}
}

int WriteDistribPts(HWND hwnd, unsigned int x)
{
	bool isProcessRunning = gameProcess.IsRunning();

	if (!isProcessRunning)
		isProcessRunning = gameProcess.FindProcess("DIABLO");

	if (isProcessRunning)
	{
		gameProcess.WriteByte(DISTRIBYTE, x);

	}
	return x;
}

void WriteHealth(HWND hwnd, unsigned int x)
{
	bool isProcessRunning = gameProcess.IsRunning();

	if (!isProcessRunning)
		isProcessRunning = gameProcess.FindProcess("DIABLO");

	if (isProcessRunning)
	{
		gameProcess.WriteByte(HEALTHBYTE, x);
	}
}