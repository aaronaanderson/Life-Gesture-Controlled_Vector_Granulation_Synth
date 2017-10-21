//
//  Grain.cpp
//  LGCVGS
////
//

#include "Grain.hpp"


void Grain::start(){
    alive = true;
    //centerAmplitude = ofRandom(0.05, 0.5);
    amplitude = ofClamp((centerAmplitude - ofRandom((rightHandSpread*centerAmplitude))), 0, 1);
    float tempOctave = ofRandom(-1, 3) * rightHandSpread;
    speed = pow(2, tempOctave);
    //centerDuration = ofRandom(20, 200);
    duration = centerDuration * (1 + ofRandom(-rightHandSpread, rightHandSpread));
    float tempFreq = (1000/duration);
    prevValue = 0;
    phasor->setPhase(0);
    phasor->setFreq(tempFreq);
    xPos = ofClamp(xPosCenter + ofRandom(leftHandSpread*-0.5, leftHandSpread*0.5), 0, 1);
    yPos = ofClamp(yPosCenter + ofRandom(leftHandSpread*-0.5, leftHandSpread*0.5), 0, 3);
    for(int i = 0; i < 2; i++){
        soundPlayers[(int)ofClamp(int(yPos) + i, 0, 3)]->setLoopEnabled(false);
        soundPlayers[(int)ofClamp(int(yPos) + i, 0, 3)]->play(speed);
        soundPlayers[(int)ofClamp(int(yPos) + i, 0, 3)]->setLocation(xPos);
    }
}


void Grain::process(){
    
    
    if(alive){
        soundPlayers[int(yPos)]->process();
        if(yPos < 3){
           soundPlayers[int(yPos)+1]->process();
        }
        float sampleOne = soundPlayers[int(yPos)]->getSample() * (1 - (yPos - int(yPos)));
        float sampleTwo;
        if(yPos < 3){
            sampleTwo = soundPlayers[int(yPos)+1]->getSample() * (yPos-int(yPos));
        }else{
            sampleTwo = sampleOne * (yPos-int(yPos));
        }
        
        
        phasor->process();
        currentSample = (sampleOne+sampleTwo) * amplitude * hannWindow(phasor->getSample());
        
        if(phasor->getSample() - prevValue < 0){
            currentSample = 0;
            end();
        }
        prevValue = phasor->getSample();
    }else{
        currentSample = 0;
    }
    
    
}

