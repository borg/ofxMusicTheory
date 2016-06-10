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
#include "MusicalTime.h"

namespace MusicTheory{


class BarEvent{
    public:
    BarEvent(){};
    BarEvent(ChordPtr c, MusicalTimePtr t){
        chords.push_back(c);
        time = t;
    };
    BarEvent(NotePtr n, MusicalTimePtr t){
        notes.push_back(n);
        time = t;
    };

    vector<ChordPtr> chords;
    vector<NotePtr> notes;
    MusicalTimePtr time;
};

typedef shared_ptr<BarEvent> BarEventPtr;

class Bar {
  public:
    int numerator = 4;//eg 4
    int denominator = 4;//eg 4 to create a 4/4 time signature
    float duration = 2000;

    int currBeat=1;

	Bar(){};
    
    void setTimeSignature(int _numerator, int _denominator){
        numerator = _numerator;
        denominator = _denominator;
    }
    
    
    int getNumerator(){
        return numerator;
    }
    int getDenominator(){
        return denominator;
    }
    
    float getDuration(){
        return duration;
    }
    
    vector<BarEventPtr> events;

    
    
    void addBarEvents(vector<BarEventPtr>&arr){
        events.insert(events.end(), arr.begin(),arr.end());
    }
    
    void addChord(ChordPtr c, MusicalTimePtr t){
        BarEventPtr b = BarEventPtr(new BarEvent());
        b->chords.push_back(c);
        b->time = t;
        events.push_back(b);
    
    };
    
    void addNote(NotePtr n, MusicalTimePtr t){
        BarEventPtr b = BarEventPtr(new BarEvent());
        b->notes.push_back(n);
        b->time = t;
        events.push_back(b);
        
    };

 
};

typedef shared_ptr<Bar> BarPtr;
};
#endif
