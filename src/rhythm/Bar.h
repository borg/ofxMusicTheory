/*
 *  Bar.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 09/02/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Bar
#define _Bar

#include "ofMain.h"
#include "Chord.h"
#include "MusicEvent.h"
#include "NoteTime.h"

namespace MusicTheory{
    
    
    typedef struct BarEvent{
        Chord chord;
        Note note;
        NoteTime time;
    }BarEvent;
    
class Bar {
  public:
    int beats;//eg 4
    int denominator;//eg 4 to create a 4/4 time signature
    float duration;

    int currBeat;

	Bar(){
        beats = 4;
        denominator = 4;
        currBeat = 1;
       // beginTime = 0;
        duration = 2000;//for 120bpm 4/4

    };
    
    void setTimeSignature(int _beats, int _denominator){
        beats = _beats;
        denominator = _denominator;
    }
    
    
    int getBeats(){
        return beats;
    }
    int getDenominator(){
        return denominator;
    }
    
    float getDuration(){
        return duration;
    }
    
    vector<BarEvent> chords;
    vector<BarEvent> notes;
    
    void addChord(Chord c, NoteTime t){
        BarEvent b;
        b.chord =c;
        b.time = t;
        chords.push_back(b);
    
    };
    
    void addNote(Note n, NoteTime t){
        BarEvent b;
        b.note = n;
        b.time = t;
        notes.push_back(b);
        
    };

    
    void syncBeat(int b){
        
        
        currBeat = b;
        if(currBeat == 1){
            //beginTime = ofGetElapsedTimeMillis();
        }
        cout<<"Bar got beat "<<b<<endl;
        cout<<"________________________"<<endl;
    }
    
    void update(){
        
              

    
    }
    
    
    /*
     Resets all events on loop
     */
    void reset(){
    }
	
};
};
#endif
