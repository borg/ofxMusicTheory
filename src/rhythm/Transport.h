/*
 *  Transport.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 03/04/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 
 *  Transport is the global time keeper and also the play/stop for the singleton Composition.
 *  Because individual playables can lay when comp is silent the tick should keep on running always.
 *
 *  Temporarily disabled until I have decided how to best handle different clock sources
 *  (using a treaded timer with listers worked ok, but ofTimeline seems steady with new ofxMSATimer,
 *  but also want to be able to use external clock source, eg. Logic, over OSC and Midi. TBD).
 */

#ifndef _Transport
#define _Transport

#include "ofMain.h"
//#include "ofxTimer.h"

namespace MusicTheory{
class Transport {
	
  public:
	
	Transport(){
    
        isPlaying = 0;
        
        /*
        ofAddListener(timer.TIMER_COMPLETE, this, &Transport::onTimerComplete);
        ofAddListener(timer.TIMER_TICK, this, &Transport::onTimerTick);
        
        
        int beatInMilli = 60000.0000f/bpm;
        timer.setup(beatInMilli, true);
        timer.setResolution(5);
        timer.startTimer();
        */
        
        
    };
    double bpm;//tempo
    bool isPlaying;
    void play(){
        
        
        if(isPlaying){
            return;
        }
        
        
        
        
        MusicEvent mEvent;
        //this global event starts composition and mutes other playables
        ofNotifyEvent(MusicEvent::PLAY,mEvent);
        
        ofNotifyEvent(MusicEvent::BEAT,mEvent);
        
        

        

        isPlaying=1;
    };
    
    
    void pause(){
        
        if(!isPlaying){
            return;
        }
        //timer.stopTimer();
        MusicEvent mEvent;
        ofNotifyEvent(MusicEvent::STOP,mEvent);
        isPlaying = 0;
    };

    
    void setBPM(double _bpm){
        bpm = _bpm;
        MusicEvent mEvent;
        mEvent.value = _bpm;
        
        
        int beatInMilli = 60000.0f/bpm;
        
        //timer.setup(beatInMilli, true);
        
        
        ofNotifyEvent(MusicEvent::TEMPO_CHANGE,mEvent);
    }
    
    //the tick happens every 5 msecs
    void onTimerTick(ofEventArgs & args){
       // cout<<"onTimerTick"<<endl;
        MusicEvent mEvent;
        //mEvent.position = position;
        ofNotifyEvent(MusicEvent::TIMER_TICK,mEvent);
    }
    
    //complete every bpm
    void onTimerComplete(ofEventArgs & args){
        MusicEvent mEvent;
        ofNotifyEvent(MusicEvent::TIMER_BEAT,mEvent);
    }
    
    
    //ofxTimer timer;
};
};
#endif
