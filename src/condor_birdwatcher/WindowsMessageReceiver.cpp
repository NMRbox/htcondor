/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

#include "stdafx.h"
#include "WindowsMessageReceiver.h"
#include <windows.h>

#pragma warning(disable:4786)
#include "birdwatcher.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////

map<HWND, WindowsMessageReceiver *> &mapWindowsMessageReceivers()
{
	static map<HWND, WindowsMessageReceiver *> mapWMRs;
	return mapWMRs;
}

///////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WMR_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//OutputDebugString(L"WinProc\n");
	//exit(0);
	WindowsMessageReceiver *pCurrentWMR = mapWindowsMessageReceivers()[hwnd];

	if (!pCurrentWMR)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	else 
	{
		if (pCurrentWMR->pOwner)
		{
			pCurrentWMR->pOwner->onReceivedWindowsMessage(pCurrentWMR, uMsg, wParam, lParam);
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

///////////////////////////////////////////////////////////////////////////////

WindowsMessageReceiver::WindowsMessageReceiver() : m_hWnd(NULL), pOwner(NULL)
{
	createHwnd();
}

///////////////////////////////////////////////////////////////////////////////

void WindowsMessageReceiver::createHwnd()
{
	static WNDCLASS	wc;
	static bool bFirstTime = true;
	static WCHAR pszClassNameBuf[] = L"WindowsMessageReceiver";

	srand(GetTickCount());

	HINSTANCE hinst = hInst;

	if (bFirstTime)
	{
		bFirstTime = false;

		wc.style					= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc				= WMR_WindowProc;
		wc.cbClsExtra				= 0x0;
		wc.cbWndExtra				= sizeof(DWORD);
		wc.hInstance				= hInst;// NULL
		wc.hIcon					= NULL;
		wc.hCursor					= NULL;
		wc.hbrBackground			= (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName				= NULL;
		wc.lpszClassName			= pszClassNameBuf;

		//WCHAR wmreceiver[] = ;
		//wcscpy_s(pszClassNameBuf, wcslen(wmreceiver), wmreceiver);
		
		int iTries = 0;
		while (!RegisterClass(&wc)) 
		{
			int iRand = rand()%10000;
			WCHAR sRand[32];
			_itow_s(iRand, sRand, wcslen(sRand), 10);
			//wcscpy_s(pszClassNameBuf, wcslen(wmreceiver), wmreceiver);
			//wcscat_s(pszClassNameBuf, wcslen(sRand), sRand);
			
			iTries++;
			if (iTries > 10)
			{
				break;
			}
		}
	}
	m_hWnd = CreateWindow(pszClassNameBuf, TEXT(""), 0, CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, NULL,	NULL, hinst, NULL);
	if(!m_hWnd)
	{
		DWORD temp = GetLastError();
		WCHAR buffer[256];
		_ltow(temp, buffer, 10);
		OutputDebugString(buffer);
	}
	mapWindowsMessageReceivers()[m_hWnd] = this;
}

///////////////////////////////////////////////////////////////////////////////

WindowsMessageReceiver::~WindowsMessageReceiver()
{
	DestroyWindow(m_hWnd);
	mapWindowsMessageReceivers().erase(m_hWnd);
}

///////////////////////////////////////////////////////////////////////////////

HWND WindowsMessageReceiver::getHWnd() const
{
	return m_hWnd;
}
