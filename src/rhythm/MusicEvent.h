/*
 *  MusicEvent.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 11/02/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MusicEvent
#define _MusicEvent

#include "ofMain.h"
#include "Note.h"

namespace MusicTheory{


typedef struct UniqueNote{
    Note note;
    string uid;
}UniqueNote;




class MusicEvent : public ofEventArgs {
    
public:
    
    MusicEvent(){};
   // MusicEventType   type;
    int position;
    int beat;
    int denominator;
    deque<UniqueNote> notes;
    string meta;
    float value;
    
    //these are global to keep all in sync
    static ofEvent <MusicEvent> NOTE_ON;
    static ofEvent <MusicEvent> NOTE_OFF;
    static ofEvent <MusicEvent> TICK;
    static ofEvent <MusicEvent> BEAT;
    static ofEvent <MusicEvent> TEMPO_CHANGE;
    
    //timer spoecific
    static ofEvent <MusicEvent> TIMER_TICK;
    static ofEvent <MusicEvent> TIMER_BEAT;
    
    //these should be used locally since different playable units can have transport
    static ofEvent <MusicEvent> LOOP;
    static ofEvent <MusicEvent> PLAY;
    static ofEvent <MusicEvent> STOP;
    static ofEvent <MusicEvent> SET_POSITION;
    static ofEvent <MusicEvent> CLEAR_TRACK;
    
    
};

    

};
#endif
