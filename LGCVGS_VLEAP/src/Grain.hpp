//
//  Grain.hpp
//  LGCVGS
////
//

#ifndef Grain_hpp
#define Grain_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxATK.hpp"


class Grain{
    
public:
    
    Grain(SoundFile* _soundOne,SoundFile* _soundTwo,SoundFile* _soundThree,SoundFile* _soundFour ){
        
        soundOne = _soundOne;
        soundTwo = _soundTwo;
        soundThree = _soundThree;
        soundFour = _soundFour;
        
        soundPlayers[0] = new SoundPlayer(soundOne);
        soundPlayers[1] = new SoundPlayer(soundTwo);
        soundPlayers[2] = new SoundPlayer(soundThree);
        soundPlayers[3] = new SoundPlayer(soundFour);
        
        alive = false;
        phasor = new TPhasor(1);
    }
    
    void start();
    
    void end(){alive = false;}
    
    void process();
    
    float getSample(){
        return currentSample;
    }
    
    bool getAlive(){
        return alive; 
    }
    
    void setCenterDuration(float newDuration){
        centerDuration = newDuration;
    }
    void setCenterAmplitude(float newAmplitude){
        centerAmplitude = newAmplitude;
    }
    void setLeftHandSpread(float newSpread){
        leftHandSpread = newSpread;
    }
    void setRightHandSpread(float newSpread){
        rightHandSpread = newSpread;
    }
    void setYPos(float newYPos){
        yPosCenter = newYPos;
    }
    void setXPos(float newXPos){
        xPosCenter = newXPos;
    }
    
private:
    SoundFile* soundOne;
    SoundFile* soundTwo;
    SoundFile* soundThree;
    SoundFile* soundFour;
    
    SoundPlayer* soundPlayers[4];
    
    float currentSample;
    float position;
    
    float centerDuration;
    float duration;
    float centerAmplitude;
    float amplitude;
    float speed;
    
    float yPosCenter;
    float yPos;
    
    float xPosCenter;
    float xPos;
    
    float leftHandSpread, rightHandSpread;
    
    bool alive;
    
    float prevValue;//for phasor to edge detect 
    
    TPhasor* phasor;
    
};


#endif /* Grain_hpp */
