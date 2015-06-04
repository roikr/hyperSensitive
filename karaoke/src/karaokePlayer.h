//
//  karaokePlayer.h
//  karaoke
//
//  Created by Roee Kremer on 5/30/15.
//
//

#pragma once
#include "ofMain.h"

#include "ofxVideoRecorder.h"
#include "sndfile.h"

class karaokePlayer:public ofBaseApp  {
    
public:
    void setup();
    void update();
    void draw();
    void setGender(bool bMale);
    void play();
    
        
    void windowResized(int w, int h);
    
    void audioOut (float *output,int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    string getFilename(){ return filename;}
    bool getIsFinished() {return player.getIsMovieDone();}
    bool getIsReady() {return vidRecorder.getVideoQueueSize() == 0 && vidRecorder.getAudioQueueSize()==0;}
    
    ofxVideoRecorder vidRecorder;
    ofVideoPlayer player;
    ofVideoGrabber grabber;
    SNDFILE *sndFile;
    vector<float>buffer;
    
    
    float vidScale;
    vector<float> texts;
    vector<float>::iterator currentText;
    
    vector<float> cuts;
    vector<float>::iterator currentCut;
    
    bool bPlaying;
    
    ofParameter<float> inThresh,outThresh,delay;
    
    
    ofSoundStream soundStream;
    
    string filename;
    
    float smoothedVol,scaledVol,lastVoice;
    
    bool bDebug;
};