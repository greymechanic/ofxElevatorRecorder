#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    recordedVideoPlayback.setLoopState(OF_LOOP_NONE);
    
    bFileToggle = true;
    presentMode = false;

    // This example shows how to use the OS X specific
    // video grabber to record synced video and audio to disk.
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // 1. Create a new recorder object.  ofPtr will manage this
    // pointer for us, so no need to delete later.
    vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    
    // 2. Set our video grabber to use this source.
    vidGrabber.setGrabber(vidRecorder);
    
    // 3. Make lists of our audio and video devices.
    videoDevices = vidRecorder->listVideoDevices();
    audioDevices = vidRecorder->listAudioDevices();
    
    // 3a. Optionally add audio to the recording stream.
    // vidRecorder->setAudioDeviceID(2);
    // vidRecorder->setUseAudio(true);
    
	// 4. Register for events so we'll know when videos finish saving.
	ofAddListener(vidRecorder->videoSavedEvent, this, &testApp::videoSaved);
	
    // 4a.  If you would like to list available video codecs on your system,
    // uncomment the following code.
    // vector<string> videoCodecs = vidRecorder->listVideoCodecs();
    // for(size_t i = 0; i < videoCodecs.size(); i++){
    //     ofLogVerbose("Available Video Codecs") << videoCodecs[i];
    // }
	
	// 4b. You can set a custom / non-default codec in the following ways if desired.
    // vidRecorder->setVideoCodec("QTCompressionOptionsJPEGVideo");
    // vidRecorder->setVideoCodec(videoCodecs[2]);
	
    // 5. Initialize the grabber.
    vidGrabber.initGrabber(1280,720);

    // If desired, you can disable the preview video.  This can
    // help help speed up recording and remove recording glitches.
    // vidRecorder->initGrabberWithoutPreview();
    
    // 6. Initialize recording on the grabber.  Call initRecording()
    // once after you've initialized the grabber.
    vidRecorder->initRecording();

    // 7. If you'd like to launch the newly created video in Quicktime
    // you can enable it here.
    //bLaunchInQuicktime = true;
    
}


//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(60,60,60);
	
	vidGrabber.update();
    
    if(recordedVideoPlayback.isLoaded()){
        recordedVideoPlayback.update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    recordedVideoPlayback.setLoopState(OF_LOOP_NONE);
    
    ofRectangle previewWindow(20,20,640,480);
    ofRectangle playbackWindow(20+640,20,640,480);
    ofRectangle presentationWindow(0,0,1280,720);
    
    if (presentMode) {
        
        if (recordedVideoPlayback.isPlaying()) {
            ofPushStyle();
            ofFill();
            ofSetColor(255);
            // fit it into the preview window, but use the correct aspect ratio
            ofRectangle videoGrabberRect(0,0,vidGrabber.getWidth(),vidGrabber.getHeight());
            videoGrabberRect.scaleTo(presentationWindow);
            vidGrabber.draw(videoGrabberRect);
            ofPopStyle();
        } else {
        
        if(vidRecorder->hasPreview()){
            ofPushStyle();
            ofFill();
            ofSetColor(255);
            // fit it into the preview window, but use the correct aspect ratio
            ofRectangle videoGrabberRect(0,0,vidGrabber.getWidth(),vidGrabber.getHeight());
            videoGrabberRect.scaleTo(presentationWindow);
            vidGrabber.draw(videoGrabberRect);
            ofPopStyle();
        } else{
            ofPushStyle();
            // x out to show there is no video preview
            ofSetColor(255);
            ofSetLineWidth(3);
            ofLine(20, 20, 640+20, 480+20);
            ofLine(20+640, 20, 20, 480+20);
            ofPopStyle();
        }
        }
        
        if(recordedVideoPlayback.isLoaded() && vidRecorder->isRecording()){
            ofPushStyle();
            ofFill();
            ofSetColor(255);
            // fit it into the preview window, but use the correct aspect ratio
            ofRectangle recordedRect(ofRectangle(0,0,recordedVideoPlayback.getWidth(),recordedVideoPlayback.getHeight()));
            recordedRect.scaleTo(presentationWindow);
            recordedVideoPlayback.draw(recordedRect);
            ofPopStyle();
        }
        

        
        
        
        
        
        
        
        
    } else {
    
    
    
    
    
    
    // draw the background boxes
    ofPushStyle();
    ofSetColor(0);
    ofFill();
    ofRect(previewWindow);
    ofRect(playbackWindow);
    ofPopStyle();
    
    // draw the preview if available
	if(vidRecorder->hasPreview()){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        // fit it into the preview window, but use the correct aspect ratio
        ofRectangle videoGrabberRect(0,0,vidGrabber.getWidth(),vidGrabber.getHeight());
        videoGrabberRect.scaleTo(previewWindow);
        vidGrabber.draw(videoGrabberRect);
        ofPopStyle();
    } else{
		ofPushStyle();
		// x out to show there is no video preview
        ofSetColor(255);
		ofSetLineWidth(3);
		ofLine(20, 20, 640+20, 480+20);
		ofLine(20+640, 20, 20, 480+20);
		ofPopStyle();
	}
    
    // draw the playback video
    if(recordedVideoPlayback.isLoaded()){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        // fit it into the preview window, but use the correct aspect ratio
        ofRectangle recordedRect(ofRectangle(0,0,recordedVideoPlayback.getWidth(),recordedVideoPlayback.getHeight()));
        recordedRect.scaleTo(playbackWindow);
        recordedVideoPlayback.draw(recordedRect);
        ofPopStyle();
    }

    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(3);
    if(vidRecorder->isRecording()){
        //make a nice flashy red record color
        int flashRed = powf(1 - (sin(ofGetElapsedTimef()*10)*.5+.5),2)*255;
		ofSetColor(255, 255-flashRed, 255-flashRed);
    }
    else{
    	ofSetColor(255,80);
    }
    ofRect(previewWindow);
    ofPopStyle();
    
    
    //draw instructions
    ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString("' ' space bar to toggle recording", 680, 540);
    ofDrawBitmapString("'v' switches video device", 680, 560);
    ofDrawBitmapString("'a' swiches audio device", 680, 580);
    
    //draw video device selection
    ofDrawBitmapString("VIDEO DEVICE", 20, 540);
    for(int i = 0; i < videoDevices.size(); i++){
        if(i == vidRecorder->getVideoDeviceID()){
			ofSetColor(255, 100, 100);
        }
        else{
            ofSetColor(255);
        }
        ofDrawBitmapString(videoDevices[i], 20, 560+i*20);
    }
    
    //draw audio device;
    int startY = 580+20*videoDevices.size();
    ofDrawBitmapString("AUDIO DEVICE", 20, startY);
    startY += 20;
    for(int i = 0; i < audioDevices.size(); i++){
        if(i == vidRecorder->getAudioDeviceID()){
			ofSetColor(255, 100, 100);
        }
        else{
            ofSetColor(255);
        }
        ofDrawBitmapString(audioDevices[i], 20, startY+i*20);
    }
    ofPopStyle();
        
    } // end else for presentationMode
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == 'p') {
        presentMode = true;
    }
    
    if (key == 'd') {
        presentMode = false;
    }

	if(key == ' '){ 
        
        // treat " " like the message from the camera
        // SO " " is like the door opening
        // cue recording
        // cue playing of previous file 
        
        //if it is recording, stop
        if(vidRecorder->isRecording()){
            vidRecorder->stopRecording();
            
            if (bFileToggle) {
                recordedVideoPlayback.loadMovie("fileTwo.mov");
                
            } else {
                recordedVideoPlayback.loadMovie("fileOne.mov");
            }
            //recordedVideoPlayback.play();
        }
        
        else {
            //check to see if there is a video file playing 
            //if(recordedVideoPlayback.isLoaded()){
                //recordedVideoPlayback.close();
            //}
            if (bFileToggle) {
                recordedVideoPlayback.loadMovie("fileTwo.mov");
                vidRecorder->startRecording("fileOne.mov");
                
            } else {
                recordedVideoPlayback.loadMovie("fileOne.mov");
                vidRecorder->startRecording("fileTwo.mov");
                
            }
            recordedVideoPlayback.play();
            //recordedVideoPlayback.play();
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	if(key == 'v'){
		vidRecorder->setVideoDeviceID( (vidRecorder->getVideoDeviceID()+1) % videoDevices.size() );
    }
	if(key == 'a'){
        vidRecorder->setAudioDeviceID( (vidRecorder->getAudioDeviceID()+1) % audioDevices.size() );
    }
}

//--------------------------------------------------------------
void testApp::videoSaved(ofVideoSavedEventArgs& e){
	// the ofQTKitGrabber sends a message with the file name and any errors when the video is done recording
	if(e.error.empty()){
        
        ofFile file(e.videoPath);
        
        ;
        
        cout << "just got video : " << file.getFileName() << endl;
        
        fileName = file.getFileName();
        
        cout << "filename : " << fileName << endl;

        
        if (bFileToggle) {
            recordedVideoPlayback.loadMovie(e.videoPath);
            bFileToggle = false;
        } else {
            recordedVideoPlayback.loadMovie(e.videoPath);
            bFileToggle = true;
        }
        
	    //recordedVideoPlayback.play();
        
//        if(bLaunchInQuicktime) {
//            ofSystem("open " + e.videoPath);
//        }
	}
	else {
		ofLogError("videoSavedEvent") << "Video save error: " << e.error;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}
