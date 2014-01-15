// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <string>
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "ofTypes.h"


namespace ofx {
namespace HTTP {


class AbstractInterruptible
    /// \brief Defines an abstract interface for stopping subclasses.
{
public:
    virtual ~AbstractInterruptible()
        /// \brief Destroy the AbstractInterruptible instance.
    {
    }

    virtual void stop() = 0;
        ///< \brief Interrupt the activity defined in the subclass.
    
};


class WebSocketFrame;


class AbstractWebSocketConnection:
    public AbstractInterruptible
    /// \brief Defines an abstract interface for sending a WebSocketFrame.
{
public:
    virtual ~AbstractWebSocketConnection()
        ///< \brief Destroy the AbstractWebSocketConnection instance.
    {
    }

    virtual bool sendFrame(const WebSocketFrame& frame) const = 0;
        ///< \brief Send a WebSocketFrame using this connection.
        ///< \param frame The WebSocketFrame to send.
        ///< \returns true iff the sending operation was successful.

};


class AbstractRouteHandler:
    public AbstractInterruptible,
    public Poco::Net::HTTPRequestHandler
    /// \brief Defines an abstract HTTP route handler.
    /// \details Route handlers are invoked in route handling threads
    ///         created by classes that inherit from AbstractRoute.
{
public:
    typedef std::shared_ptr<AbstractRouteHandler> SharedPtr;
        ///< \brief A typedef for a shared pointer.

    typedef std::weak_ptr<AbstractRouteHandler> WeakPtr;
        ///< \brief A typedef for a weak pointer.

    virtual ~AbstractRouteHandler()
        ///< \brief Destroy the AbstractRouteHandler instance.
    {
    }

};


class AbstractRoute:
    public AbstractInterruptible,
    public Poco::Net::HTTPRequestHandler,
    public Poco::Net::HTTPRequestHandlerFactory
    /// \brief Defines an abstract HTTP server route.
{
public:
    typedef std::shared_ptr<AbstractRoute> SharedPtr;
        ///< \brief A typedef for a shared pointer.

    typedef std::weak_ptr<AbstractRoute> WeakPtr;
        ///< \brief A typedef for a weak pointer.

    virtual ~AbstractRoute()
        ///< \brief Destroy the AbstractRoute instance.
    {
    }

    virtual std::string getRoutePathPattern() const = 0;
        ///< \returns The Route path regex pattern.

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const = 0;
        ///< \brief Determine if this route can handle the given request.
        ///< \param request The incoming Poco::Net::HTTPServerRequest to be
        ///<        tested.
        ///< \param isSecurePort true iff the connection is SSL encrypted.
        ///<        Some implmenetations of this interface may choose to only
        ///<        handle requests on secure ports.
        ///< \returns true iff the route can handle the given request.

};


} } // namespace ofx::HTTP
