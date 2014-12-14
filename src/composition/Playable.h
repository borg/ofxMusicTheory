/*
 *  Playable.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 03/04/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Playable
#define _Playable

#include "ofMain.h"
#include "MusicEvent.h"
#include "Track.h"

namespace MusicTheory{
    
class Playable {
	
  public:
	
    double bpm;//tempo
        
	Playable(){
        bpm = 120;
        position = 0;
        doLoop = 0;
        isPlaying = 0;
        duration = 0;

        addListeners();
    };
    

	
    int position;//
    int duration;//of longest track
    
    bool doLoop;
    
    bool isPlaying;
    
    vector<Track *> tracks;
    
    
    
    void play(){
        
        position = 0;
        MusicEvent mEvent;
        ofNotifyEvent(PLAY,mEvent);
        
        
        
        mEvent.position = position;
        ofNotifyEvent(BEAT,mEvent);
        
        
        /*
        if(isPlaying){
            return;
        }
        */
      /*
        int beatInMilli = 60000.0000f/bpm;
        timer.setup(beatInMilli, true);
        timer.setResolution(5);
        timer.startTimer();*/
        isPlaying=1;
    };
    

    
    
    
    
    void onTimerTick(MusicEvent & args){
        if(!isPlaying){
            return;
        }
        MusicEvent mEvent;
        //pass on local version to tracks
        ofNotifyEvent(TICK,mEvent);
    }
    
    void onTimerBeat(MusicEvent & args){
        
        if(!isPlaying){
            return;
        }
        
        
        if((position+1)>=duration){
            //cout<<"track to looop "<<(position+1)<<" dur: "<<duration<<endl;
            if(doLoop){
                position=0;
                MusicEvent mEvent;
                ofNotifyEvent(LOOP,mEvent);
            }else{
                pause();
                return;
            }
        }else{
            position++;
        }
        
        
        MusicEvent mEvent;
        mEvent.position = position;
        //TODO: Pass on time signature to editor
        //mEvent.denominator =
        ofNotifyEvent(BEAT,mEvent);
        
        
        //cout<<"duration "<<duration<<" position "<<position<<endl;
    }
	void stop(){
        isPlaying=0;
    }
    void setPosition(int p){
        position = p;
        
        MusicEvent mEvent;
        mEvent.value = position;
        ofNotifyEvent(SET_POSITION,mEvent);
    };
    
    
    void pause(){
        // timer.stopTimer();
        MusicEvent mEvent;
        //notify tracks
        ofNotifyEvent(STOP,mEvent);
        isPlaying = 0;
    };
    
    void setLoop(bool b){
        doLoop = b;
    }
    void load(string);
    void save(string);
    
    void addTrack(Track *t){
        //add track as listener
        
              //local events...
        //I don't remember the thought behing all LOOP events?
        ofAddListener(LOOP,t,&Track::onPlay);
        ofAddListener(LOOP,t,&Track::onStop);
        ofAddListener(LOOP,t,&Track::onLoop);
        ofAddListener(BEAT,t,&Track::onBeat);
        ofAddListener(TICK,t,&Track::onTick);
        //t->addListeners();
        
        //global events
        ofAddListener(MusicEvent::TEMPO_CHANGE,t,&Track::onTempoChange);
        
        ofAddListener(MusicEvent::SET_POSITION,t,&Track::onSetPosition);
        
        tracks.push_back(t);
        
        if(t->totalBeats>duration){
            duration = t->totalBeats;
        }
    };
    void deleteTrackByName(string name){
        
    }
    
    void clearAllTracks(){
        duration=0;
        //cout<<"clearAllTracks tracks.size() "<<tracks.size()<<endl;
        for(int i=0;i<tracks.size();i++){
            
            //local events
            ofRemoveListener(LOOP,tracks[i],&Track::onPlay);
            ofRemoveListener(LOOP,tracks[i],&Track::onStop);
            ofRemoveListener(LOOP,tracks[i],&Track::onLoop);
            ofRemoveListener(BEAT,tracks[i],&Track::onBeat);
            ofRemoveListener(TICK,tracks[i],&Track::onTick);
            //t->addListeners();
            
            //global events
            ofRemoveListener(MusicEvent::TEMPO_CHANGE,tracks[i],&Track::onTempoChange);
            
            ofRemoveListener(MusicEvent::SET_POSITION,tracks[i],&Track::onSetPosition);
            
            
            tracks[i]->removeListeners();
            //TODO: Check proper delete
            //delete tracks[i];
        }
        tracks.clear();
        
        MusicEvent mEvent;
        ofNotifyEvent(MusicEvent::CLEAR_TRACK,mEvent);
        setPosition(0);
    }
    void muteTrackByName(string name){
        
    }
    
    
    
    
    
    //these events are local
    ofEvent<MusicEvent> TICK;
    ofEvent<MusicEvent> BEAT;//with local position data
    ofEvent<MusicEvent> LOOP;
    ofEvent<MusicEvent> PLAY;
    ofEvent<MusicEvent> STOP;
    ofEvent<MusicEvent> SET_POSITION;
    ofEvent<MusicEvent> CLEAR_TRACK;
    
    
    void addListeners(){
        ofAddListener(MusicEvent::TIMER_BEAT, this, &Playable::onTimerBeat);
        ofAddListener(MusicEvent::TIMER_TICK, this, &Playable::onTimerTick);
    }
};

};
#endif
