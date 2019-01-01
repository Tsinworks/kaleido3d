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
#ifndef __WebSocket_H__
#define __WebSocket_H__

namespace k3d
{
    namespace net
    {
        class WebSocketImpl;

        class K3D_CORE_API WebSocket : public os::Socket
        {
            friend class WebSocketImpl;
        public:
            WebSocket();
            virtual				~WebSocket();

            os::Socket*	        Accept(os::IpAddress const& Ip) override;
            I32				    Receive(void * pData, I32 recvLen) override;
            I32				    Send(const char * pData, I32 sendLen) override;

        protected:
            WebSocketImpl*      d;
        };

        class K3D_CORE_API HttpRequest
        {
        public:
            HttpRequest();
        };

        class K3D_CORE_API HttpClient
        {
        public:
            HttpClient();
            ~HttpClient();


        };


    }
}

#endif