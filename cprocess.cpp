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

#include "cprocess.h"
#include "stdafx.h"

CProcess::CProcess()
{
	m_hProcess = NULL;
	m_bGameRunning = false;
}

CProcess::~CProcess()
{
	if (m_bGameRunning)
		CloseHandle(m_hProcess);
}

HANDLE CProcess::OpenProcess( char *p_ClassName, char *p_WindowTitle)
{
	HWND hWindow;
	DWORD pid;

	hWindow = FindWindow(p_ClassName, p_WindowTitle);
	if (hWindow)
	{
		GetWindowThreadProcessId(hWindow, &pid);
		return ::OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	}
	return NULL;
}

bool CProcess::FindProcess(char *p_WindowTitle)
{
	if (m_hProcess == NULL)
	{
		m_hProcess = this->OpenProcess(NULL, p_WindowTitle);
			if (m_hProcess)
			{
				m_bGameRunning = true;
			}
		return m_bGameRunning;
	}
	else
		return false;
}

bool CProcess::WriteByte(DWORD p_Address, BYTE p_Value)
{
	DWORD bytes;

	if (m_bGameRunning)
		return (WriteProcessMemory(m_hProcess,(void*)p_Address,
				                   (void*)&p_Value, 1, &bytes) != 0);
	return false;
}

bool CProcess::WriteTwoByte(DWORD p_Address, BYTE p_Value)
{
	DWORD bytes;

	if (m_bGameRunning)
		return (WriteProcessMemory(m_hProcess,(void*)p_Address,
				                   (void*)&p_Value, 2, &bytes) != 0);
	return false;
}

BYTE CProcess::ReadByte(DWORD p_Address)
{
	DWORD bytes;
	BYTE tmpValue;

	if (m_bGameRunning)
	{
		if (ReadProcessMemory(m_hProcess, (void*)p_Address,
			                  (void*)&tmpValue, 1, &bytes) == 0)
			return 0;
		else
			return tmpValue;
	}
	return 0;
}

bool CProcess::IsRunning()
{
	return m_bGameRunning;
}

bool CProcess::IsKeyPressed( int p_ikey)
{
	return ((GetAsyncKeyState(p_ikey) & 1) == 1);
}
