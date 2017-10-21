#pragma once

#include "ofMain.h"
#include "ofxATK.hpp"
#include "ofxGUI.h"
#include "ofxLeapMotion2.h"

#include "Grain.hpp"

#define MAX_GRAINS 64

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    void audioOut(float* buffer, int bufferSize, int nChannels);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //-----------LEAP STUFF----
    ofxLeapMotion leap;
    vector <ofxLeapMotionSimpleHand> simpleHands;
    
    vector <int> fingersFound;
    ofEasyCam cam;
    
    bool useLEAP;
    
    
    //temp--delete this
    float lowestX, lowestY, highestY, lowestZ, highestZ;
    //------------------------
    
    
    SoundFile* soundOne;
    SoundFile* soundTwo;
    SoundFile* soundThree;
    SoundFile* soundFour;
    
    Reverb* reverb;
    
    ofSoundStream soundStream;
    
    ImpulseGenerator* impulse;
    
    Grain* grains[128];
    
    bool displayGUI;
    ofxPanel gui;
    ofxFloatSlider emissionRate;
    ofxFloatSlider amplitude;
    ofxFloatSlider duration;
    ofxFloatSlider xLocation;
    ofxFloatSlider yLocation;
    ofxFloatSlider leftOpen;
    ofxFloatSlider rightOpen;
    ofxFloatSlider reverbFeed;
    
};
