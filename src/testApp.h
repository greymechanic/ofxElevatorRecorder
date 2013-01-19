#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		
		ofVideoGrabber 			vidGrabber;
		ofPtr<ofQTKitGrabber>	vidRecorder;
    
    	ofVideoPlayer recordedVideoPlayback;
        
    
		void videoSaved(ofVideoSavedEventArgs& e);
    
        //bool getIsMovieDone();
    
	
    	vector<string> videoDevices;
	    vector<string> audioDevices;
    
        bool bLaunchInQuicktime;
    
        bool bFileToggle;
    
        string fileName;
    
        bool presentMode;
    
};
