//
//  karaokePlayer.cpp
//  karaoke
//
//  Created by Roee Kremer on 5/30/15.
//
//

#define VIDEO_WIDTH 1280.0
#define VIDEO_HEIGHT 720.0
#define PAUSE_DELAY 3.0
#define DEBUG_LOG
#define THRESHOLD 0.5

#include "karaokePlayer.h"

//--------------------------------------------------------------
void karaokePlayer::setup(){
    ofBackground(0);
    
    
    
    vidScale = MIN((float)ofGetWidth()/VIDEO_WIDTH,(float)ofGetHeight()/VIDEO_HEIGHT);
    
//    vector<ofVideoDevice> devices = grabber.listDevices();
//    for (vector<ofVideoDevice>::iterator iter=devices.begin();iter!=devices.end();iter++) {
//        cout << iter->id << '\t' << iter->deviceName << endl;
//        for (vector<ofVideoFormat>::iterator fiter=iter->formats.begin();fiter!=iter->formats.end();fiter++) {
//            cout << 't' << fiter->width << 'x' << fiter->height << endl;
//        }
//    }
    
    grabber.setDeviceID(0);
    grabber.setup(VIDEO_WIDTH, VIDEO_HEIGHT);
    

    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("2000k");
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");
    
    
    buffer.resize(512);
    bPlaying = false;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    lastVoice = ofGetElapsedTimef()-PAUSE_DELAY;
    
    
    
    soundStream.printDeviceList();
    soundStream.setDeviceID(6);
    
    soundStream.setup(this, 1, 1, 44100, 512, 4);
    soundStream.stop();

    bDebug = true;
}

//--------------------------------------------------------------
void karaokePlayer::update(){
    player.update();
    grabber.update();
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    //cout << player.getPosition() << endl;
    
    
    
    if (bPlaying) {
        
        while (currentCut!=cuts.end() && *currentCut<player.getPosition()) {
            currentCut++;
        }
        
        while (currentText!=texts.end() && *currentText<player.getPosition()) {
            currentText++;
        }
        
        if (scaledVol>THRESHOLD ) {
            lastVoice = ofGetElapsedTimef();
        }
        
        
    
        
        if (player.getIsMovieDone()) {
            soundStream.stop();
            bPlaying = false;
        } else {
            
            if (distance(texts.begin(), currentText)%2) {
                player.setPaused(ofGetElapsedTimef()-lastVoice>PAUSE_DELAY);
            }
            
            if (!player.isPaused()) {
                
                if ((distance(cuts.begin(), currentCut)+1)%2) {
                    if(grabber.isFrameNew()){
                        vidRecorder.addFrame(grabber.getPixelsRef());
                    }
                } else {
                    if (player.isFrameNew()) {
                        vidRecorder.addFrame(player.getPixelsRef());
                    }
                }
            }
        }
    } else {
        if (vidRecorder.isInitialized()  && getIsReady() ) {
            vidRecorder.close();
        }
    }
}

//--------------------------------------------------------------
void karaokePlayer::draw(){
    ofPushMatrix();
    ofTranslate(0.5*(ofPoint(ofGetWidth(),ofGetHeight())-vidScale*ofPoint(player.getWidth(),player.getHeight())));
    
    ofScale(vidScale, vidScale);
    player.draw(0,0);
    if (currentCut!=cuts.end() && (distance(cuts.begin(), currentCut)+1)%2) {
        ofPushMatrix();
        ofScale(0.2, 0.2);
        grabber.draw(0,0);
    }
    ofPopMatrix();
    ofPopMatrix();
    
    if (bDebug) {
    
        stringstream ss;
        ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
        << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
        << "FPS: " << ofGetFrameRate() << endl
        << "player:\t" << player.getPosition() * player.getDuration() << endl;
        if (currentText!=texts.end()) {
            
            ss << "text point:\t" << *currentText << endl;
        }
        ss << "Scaled average vol (0-100): " << ofToString(scaledVol * 100.0, 0) << endl;
        
        
        ofDrawBitmapString(ss.str(), 10, 10);
        
        /*
         if (current!=positions.end()) {
         ofDrawBitmapString(ofToString(*current), ofGetWidth()-10, 10);
         }
         */
    }
}

void karaokePlayer::audioOut (float *output,int bufferSize, int nChannels) {
    if (bPlaying && !player.isPaused()) {
        sf_count_t frames= sf_readf_float(sndFile, buffer.data(), bufferSize);
        memcpy(output, buffer.data(), bufferSize*sizeof(float));
    }
}

void karaokePlayer::audioIn(float * input, int bufferSize, int nChannels) {
    
    float curVol = 0.0;
    
    for (int i=0;i<bufferSize;i++) {
        curVol += input[i]*input[i];
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)bufferSize;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    if (bPlaying) {
        
        for (int i=0;i<bufferSize;i++) {
            input[i]*=5;
            input[i]+=buffer[i];
        }
    
        if (!player.isPaused()) {
            vidRecorder.addAudioSamples(input, bufferSize, nChannels);
        }
    }
}

void karaokePlayer::setGender(bool bMale) {
    
    SF_INFO sfInfo;
    sndFile = sf_open(ofToDataPath("audio.wav").c_str(), SFM_READ, &sfInfo);
    if (!sndFile) {
        cout << sf_strerror(NULL) << endl;
    }
    
    bPlaying = false;
    
    player.load("video.mp4");
    player.play();
    player.setPaused(true);
    
    
    ifstream cutsStream(ofToDataPath("cuts.txt").c_str());
    if (cutsStream.is_open()) {
        while (!cutsStream.eof()) {
            float pos;
            cutsStream >> pos;
            //            cout << pos << endl;
            //string str;
            //cutsStream >> str;
            cutsStream.ignore();
            //cuts.push_back(pos/player.getDuration());
        }
    }
    
    currentCut = cuts.begin();
    
    ifstream textsStream(ofToDataPath("texts.txt").c_str());
    if (textsStream.is_open()) {
        while (!textsStream.eof()) {
            float pos;
            textsStream >> pos;
            cout << pos << endl;
            string str;
            textsStream >> str;
            textsStream.ignore();
            texts.push_back(pos/player.getDuration());
        }
    }
    currentText = texts.begin();
    
    
    
    soundStream.start();

    
    filename ="testMovie"+ofGetTimestampString()+".mov";
    
}


void karaokePlayer::play() {
   
    
    vidRecorder.setup(filename, VIDEO_WIDTH, VIDEO_HEIGHT, 25, 44100, 1);
    bPlaying = true;
    player.setPaused(false);
    
}

void karaokePlayer::windowResized(int w, int h){
    
    vidScale = MIN((float)w/player.getWidth(),(float)h/player.getHeight());
}
