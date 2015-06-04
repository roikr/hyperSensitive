#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    cuts.open(ofToDataPath( "cuts.txt").c_str(),ofstream::app);
    texts.open(ofToDataPath( "texts.txt").c_str(),ofstream::app);
    player.load("lea_final.mp4");
    windowResized(ofGetWidth(), ofGetHeight());
    player.play();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    player.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(0.5*(ofPoint(ofGetWidth(),ofGetHeight())-vidScale*ofPoint(player.getWidth(),player.getHeight())));
    
    ofScale(vidScale, vidScale);
    player.draw(0,0);
    ofPopMatrix();
    
}

void ofApp::exit() {
    cuts.close();
    texts.close();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    float pos = player.getPosition()*player.getDuration();
    switch (key) {
        case 'i':
            texts << pos << '\t' << "in" << endl;
            break;
        case 'o':
            texts << pos << '\t' << "out" << endl;
            break;
        case '1':
            cuts << pos << '\t' << "1" << endl;
            break;
        case '2':
            cuts << pos << '\t' << "2" << endl;
            break;
        default:
            break;
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
    
    vidScale = MIN((float)w/player.getWidth(),(float)h/player.getHeight());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
