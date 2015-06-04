#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    karaoke.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    karaoke.update();
        
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    karaoke.draw();
}

void ofApp::audioOut (float *output,int bufferSize, int nChannels) {
    karaoke.audioOut(output, bufferSize, nChannels);
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels) {
    karaoke.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='m') {
        karaoke.setGender(true);
    }
    
    if (key=='p') {
        karaoke.play();
    }
    
    if (key=='t') {
        ofToggleFullscreen();
    }
    
    if (key=='d') {
        karaoke.bDebug = !karaoke.bDebug;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
    karaoke.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
