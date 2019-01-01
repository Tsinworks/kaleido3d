/**
 * MIT License
 *
 * Copyright (c) 2019 Zhou Qin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once 
#ifndef __Window_h__
#define __Window_h__

namespace k3d
{
	class Message;
	class App;

	enum class WindowMode 
    {
		NORMAL,
		FULLSCREEN
	};

	class K3D_CORE_API IWindow
	{
	public:
		typedef SharedPtr<IWindow> Ptr;

		virtual ~IWindow() {}

		virtual void	SetWindowCaption(const char * name) = 0;
		virtual void	Show(WindowMode mode = WindowMode::NORMAL) = 0;
		virtual void	Resize(int width, int height) = 0;
		virtual void	Move(int x, int y) = 0;
		virtual bool	IsOpen() = 0;
		virtual void*	GetHandle() const = 0;

		virtual bool	PollMessage(Message & messge) = 0;

		virtual U32 	Width() const = 0;
		virtual U32 	Height() const = 0;

		IWindow & operator = (IWindow const &) = delete;
		IWindow(const IWindow&) = delete;
		IWindow(const IWindow&&) = delete;

	protected:
		IWindow() {} 
		IWindow(const char *windowName, int width, int height) {}

		friend Ptr MakePlatformWindow(const char *windowName, int width, int height);
	};

	extern IWindow::Ptr MakePlatformWindow(const char *windowName, int width, int height);

#if K3DPLATFORM_OS_ANDROID
	extern IWindow::Ptr MakeAndroidWindow(void * window);
#endif
}
#endif