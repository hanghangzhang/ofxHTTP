// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/DefaultProxyProcessor.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


DefaultProxyProcessor::DefaultProxyProcessor()
{
}


DefaultProxyProcessor::~DefaultProxyProcessor()
{
}


void DefaultProxyProcessor::requestFilter(BaseRequest&,
                                          Context& context)
{
    if (!context.getProxyRedirectURI().empty())
    {
        context.getClientSession()->setProxy(context.getProxyRedirectURI().getHost(),
                                             context.getProxyRedirectURI().getPort());
    }
    else
    {
        const ProxySettings& proxySettings = context.getClientSessionSettings().getProxySettings();

        if (!proxySettings.getHost().empty())
        {
            context.getClientSession()->setProxyHost(proxySettings.getHost());
        }

        if (0 != proxySettings.getPort())
        {
            context.getClientSession()->setProxyPort(proxySettings.getPort());
        }

        if (proxySettings.hasCredentials())
        {
            context.getClientSession()->setProxyCredentials(proxySettings.getUsername(),
                                                            proxySettings.getPassword());
        }
    }
}


void DefaultProxyProcessor::responseFilter(BaseRequest& request,
                                           BaseResponse& response,
                                           Context& context)
{
    // The requested resource MUST be accessed through the proxy
    // given by the Location field. The Location field gives the
    // URI of the proxy. The recipient is expected to repeat this
    // single request via the proxy. 305 responses MUST only be
    // generated by origin servers. only use for one single request!

    if (canFilterResponse(request, response, context))
    {
        if (context.getProxyRedirectURI().empty() && response.has("Location"))
        {
            Poco::URI proxyRedirectURI;
            proxyRedirectURI.resolve(response.get("Location"));
            context.setProxyRedirectURI(proxyRedirectURI);
            context.getClientSession().reset(); // delete session

            // Set the context to resubmit.
            context.setResubmit(true);
        }
        else
        {
            throw Poco::Net::HTTPException(response.getReasonForStatus(response.getStatus()),
                                           response.getStatus());
        }

    }
}


bool DefaultProxyProcessor::canFilterResponse(BaseRequest&,
                                              BaseResponse& response,
                                              Context&) const
{
    return Poco::Net::HTTPResponse::HTTP_USEPROXY == response.getStatus();
}



} } // namespace ofx::HTTP
