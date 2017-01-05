//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxHTTP.h"


class ofApp: public ofBaseApp
{
public:
    void setup();
    void draw();

    void onHTTPPostEvent(ofxHTTP::PostEventArgs& evt);
    void onHTTPFormEvent(ofxHTTP::PostFormEventArgs& evt);
    void onHTTPUploadEvent(ofxHTTP::PostUploadEventArgs& evt);

    ofxHTTP::SimplePostServer server;

};
