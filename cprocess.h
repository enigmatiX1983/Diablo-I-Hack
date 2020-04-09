#ifndef _CPROCESS_
#define _CPROCESS_

class CProcess
{
public:
	CProcess();
	virtual ~CProcess();

	bool IsRunning();
	bool FindProcess(char *p_WindowTitle);
	BYTE ReadByte(DWORD p_Address);
	bool WriteByte(DWORD p_Address, BYTE p_Value);
	bool WriteTwoByte(DWORD p_Address, BYTE p_Value);
	bool IsKeyPressed(int p_ikey);

private:
	HANDLE OpenProcess(char *p_ClassName, char *p_WindowTitle);

	HANDLE m_hProcess;
	bool m_bGameRunning;
};

#endif