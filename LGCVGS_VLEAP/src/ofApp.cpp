#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ///--------------leap
    useLEAP = true;
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    leap.open();
    
    // keep app receiving data from leap motion even when it's in the background
    leap.setReceiveBackgroundFrames(true);
    
    cam.setOrientation(ofPoint(-20, 0, 0));
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    //--------------------------end leap
    
    ofSetCircleResolution(80);
    
    soundOne = new SoundFile(ofToDataPath("soundOne.aif"));
    soundTwo = new SoundFile(ofToDataPath("soundTwo.wav"));
    soundThree = new SoundFile(ofToDataPath("soundThree.wav"));
    soundFour = new SoundFile(ofToDataPath("soundFour.wav"));
    
    
    impulse = new ImpulseGenerator(40);
    
    for(int i = 0; i < MAX_GRAINS; i++){
        grains[i] = new Grain(soundOne, soundTwo, soundThree, soundFour);
    }
    //float absorption = 0.03, float absorptionFreq = 5000, float mix = 0.7, float initPreDelay = 40
    reverb = new Reverb(0.1, 2000, 1.0);
    
    //------------------------gui stuff
    displayGUI = true;
    gui.setup();
    gui.add(emissionRate.setup("Emmision", 40, 0.25, 256));
    gui.add(amplitude.setup("amplitude", 0.0, 0.0, 1.0));
    gui.add(duration.setup("duration", 100, 0.1, 200));
    gui.add(xLocation.setup("xLocation", 0.5, 0.0, 1.0));
    gui.add(yLocation.setup("yLocation", 0.0, 0.0, 3.0));
    gui.add(leftOpen.setup("leftOpen", 0.0, 0.0, 1.0));
    gui.add(rightOpen.setup("rightOpen", 0.0, 0.0, 1.0));
    gui.add(reverbFeed.setup("reverb", 0.3, 0.0, 1.0));
    gui.setPosition(ofPoint(ofGetWidth()-220, 20));
    //impulse->setPulseDeviation(1.0);
    
    ofSoundStreamSetup(2, 0, ATKSettings::sampleRate, ATKSettings::bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    gui.setPosition(ofPoint(ofGetWidth()-220, 20));
    //-----------LEAP
    fingersFound.clear();
    
    //here is a simple example of getting the hands and drawing each finger and joint
    //the leap data is delivered in a threaded callback - so it can be easier to work with this copied hand data
    
    //if instead you want to get the data as it comes in then you can inherit ofxLeapMotion and implement the onFrame method.
    //there you can work with the frame data directly.
    
    
    
    //Option 1: Use the simple ofxLeapMotionSimpleHand - this gives you quick access to fingers and palms.
    
    
    simpleHands = leap.getSimpleHands();
    
    
    
    float distanceOne = 25;
    float distanceTwo = 25;
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        
        
        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                fingersFound.push_back(id);
            }
            
        }
    }
    //cout << "distance, thumb and index hand one: " << distanceOne << endl;
    
    if(useLEAP){
        if(simpleHands.size() > 1){
            
            if(simpleHands[0].handPos.x < lowestX){
                lowestX = simpleHands[0].handPos.x;
            }
            if(simpleHands[0].handPos.y < lowestY){
                lowestY = simpleHands[0].handPos.y;
            }
            if(simpleHands[0].handPos.y > highestY){
                highestY = simpleHands[0].handPos.y;
            }
            if(simpleHands[0].handPos.z < lowestZ){
                lowestZ = simpleHands[0].handPos.z;
            }
            if(simpleHands[0].handPos.z > highestZ){
                highestZ = simpleHands[0].handPos.z;
            }
            
            cout << "Lowest X = " << lowestX;
            cout << "lowest Y = " << lowestY;
            cout << "highest Y = " << highestY;
            cout << "lowest Z = " << lowestZ;
            cout << "hieghest Z = " << highestZ << endl;
            //Lowest X = -600.947lowest Y = -381.211highest Y = 362.292lowest Z = -166.057hieghest Z = -166.057
            
            int left;
            if(simpleHands[0].fingers[fingerTypes[4]].tip.x < simpleHands[0].fingers[fingerTypes[0]].tip.x){
                left = 0;
            }else{
                left = 1;
            }
            
            xLocation = ofMap(simpleHands[left].handPos.x, -600, -20, 0.0, 1.0, true);
            yLocation = ofMap(simpleHands[left].handPos.y, -280, 200, 3.0, 0.0, true);
            reverbFeed = ofMap(simpleHands[left].handPos.z, -166, 254, 1.0, 0.0, true);
            
            int right;
            if(simpleHands[0].fingers[fingerTypes[4]].tip.x > simpleHands[0].fingers[fingerTypes[0]].tip.x){
                right = 0;
            }else{
                right = 1;
            }
            
            amplitude = ofMap(simpleHands[right].handPos.y, -280, 200, 0.0, 1.0, true);
            
            float tempOctave = ofMap(simpleHands[right].handPos.x, 50, 300, -2, 8, true);
            emissionRate = pow(2, tempOctave);
            tempOctave = ofMap(simpleHands[right].handPos.z, -166, 254, 1, 8, true);
            duration = pow(2, tempOctave);
            
            distanceOne = ofDist(simpleHands[left].fingers[fingerTypes[0]].tip.x,simpleHands[left].fingers[fingerTypes[0]].tip.y, simpleHands[left].fingers[fingerTypes[0]].tip.z,
                                 simpleHands[left].fingers[fingerTypes[2]].tip.x,
                                 simpleHands[left].fingers[fingerTypes[2]].tip.y,
                                 simpleHands[left].fingers[fingerTypes[2]].tip.z);
            leftOpen = ofMap(distanceOne, 75, 250, 0.0, 1.0, true);
            
            distanceTwo = ofDist(simpleHands[right].fingers[fingerTypes[0]].tip.x,simpleHands[right].fingers[fingerTypes[0]].tip.y, simpleHands[right].fingers[fingerTypes[0]].tip.z,
                                 simpleHands[right].fingers[fingerTypes[2]].tip.x,
                                 simpleHands[right].fingers[fingerTypes[2]].tip.y,
                                 simpleHands[right].fingers[fingerTypes[2]].tip.z);
            rightOpen = ofMap(distanceTwo, 75, 250, 0.0, 1.0, true);
        }else{
            amplitude = fmax(amplitude - 0.01, 0.0);
        }
    }
    
    //------------------------end leap
    int n = 0;
    for(int i = 0; i < MAX_GRAINS; i++){
        if(grains[i]->getAlive()){
            n++;
        }
        
        grains[i]->setCenterAmplitude(amplitude);
        grains[i]->setCenterDuration(duration);
        grains[i]->setLeftHandSpread(leftOpen);
        grains[i]->setRightHandSpread(rightOpen);
        grains[i]->setYPos(yLocation);
        grains[i]->setXPos(xLocation);
        grains[i]->setLeftHandSpread(leftOpen);
    }
    //cout << "number active: " << n << endl;
    
    impulse->setFreq(emissionRate);
    impulse->setBurstMasking(rightOpen*0.5);
    impulse->setPulseDeviation(rightOpen);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //----------------------LEAP stuff--------
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
    ofSetDepthTest(true);
    ofSetColor(200);
    //ofDrawBitmapString("Leap Connected? " + ofToString(leap.isConnected()), 20, 20);
    
    cam.begin();
    
    ofPushMatrix();
    ofRotate(90, 0, 0, 1);
    ofSetColor(20);
    ofDrawGridPlane(800, 20, false);
    ofPopMatrix();
    
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
        ofSetColor(0, 0, 255);
        ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
        ofSetColor(255, 255, 0);
        ofDrawArrow(handPos, handPos + 100*handNormal);
        
        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            ofSetColor(0, 255, 0);
            ofDrawSphere(mcp.x, mcp.y, mcp.z, 12);
            ofDrawSphere(pip.x, pip.y, pip.z, 12);
            ofDrawSphere(dip.x, dip.y, dip.z, 12);
            ofDrawSphere(tip.x, tip.y, tip.z, 12);
            
            ofSetColor(255, 0, 0);
            ofSetLineWidth(20);
            ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
        }
    }
    cam.end();
    //--------------------------end leap
    
    ofSetDepthTest(false);
    
    if(displayGUI){
        soundOne->draw(20, 20, 400, 120);
        soundTwo->draw(20, 140, 400, 120);
        soundThree->draw(20, 260, 400, 120);
        soundFour->draw(20, 380, 400, 120);
        
        ofSetColor(220, 220, 255, 100);
        float tempX = (xLocation * 400) + 20;
        float tempY = (yLocation * 120) + 20 + 60;
        float tempSize = fmax(leftOpen * 120, 10);
        ofDrawCircle(tempX, tempY, tempSize);
        
        
        gui.draw();
    }
    
    
    ofSetColor(255);
    ofDrawBitmapString("Press 'h' to hide gui, l to disable LEAP", 20, 20);
    
}
//--------------------------------------------------------------
void ofApp::audioOut(float* buffer, int bufferSize, int nChannels){
    
    for(int i = 0; i < bufferSize; i++){
        float currentSample = 0;
        
        impulse->process();
        
        //trigger a new grain via the impulse generator
        if(impulse->getSample() == 1){
            for(int i = 0; i < MAX_GRAINS; i++){
                if(grains[i]->getAlive() == false){
                    //start new grain
                    grains[i]->start();
                    break;
                }
            }
        }
        
        for(int i = 0; i < MAX_GRAINS; i++){
            grains[i]->process();
            currentSample += grains[i]->getSample();
        }
        
        reverb->process(currentSample * reverbFeed);
        
        currentSample += reverb->getSample();
        
        
        buffer[i*nChannels+0] = currentSample;
        buffer[i*nChannels+1] = currentSample;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'h'){
        displayGUI = !displayGUI;
    }else if(key == 'l'){
        useLEAP = !useLEAP;
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
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    ofSoundStreamClose();
    leap.close();
}
