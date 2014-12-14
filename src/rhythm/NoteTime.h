/*
 *  Time.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 11/02/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 
 *  This will probably change to be compatible with guido
 *
 */

#ifndef _NoteTime
#define _NoteTime

#include "ofMain.h"
#include "Constants.h"

namespace MusicTheory{
        
   /*
    typedef enum NoteValue{
        NOTE_0,
        NOTE_1,
        NOTE_2,
        NOTE_4,
        NOTE_8,
        NOTE_8_3,
        NOTE_8_5,
        NOTE_8_7,
        NOTE_16,
        NOTE_16_3,
        NOTE_16_5,
        NOTE_16_7,
        NOTE_32,
        NOTE_32_3,
        NOTE_32_5,
        NOTE_32_7
     }NoteValue;
    
    */
    
    //const char * NoteNames[] = {"NOTE_1","NOTE_2","NOTE_3","NOTE_4","NOTE_8","NOTE_8_3","NOTE_8_5","NOTE_8_7"};
    
    /*
    inline ostream& operator<<(ostream& os, NoteValue& n){
        string NoteNames[] = {"EMPTY NOTE","NOTE_1","NOTE_2","NOTE_4","NOTE_8","NOTE_8_3","NOTE_8_5","NOTE_8_7","NOTE_16","NOTE_16_3","NOTE_16_5","NOTE_16_7","NOTE_32","NOTE_32_3","NOTE_32_5","NOTE_32_7"};
        os<<NoteNames[n];
        
        return os;
    }*/
    /*
    typedef map<NoteValue, float> NoteLookup;
    
    
    const NoteLookup::value_type nn[] = {
		make_pair(NOTE_1,1.0f),
        make_pair(NOTE_2,.5f),
        make_pair(NOTE_4,.25f),
        make_pair(NOTE_8,.125f),
        //a triplet eighth note is a third of a quarter note.
        make_pair(NOTE_8_3,.25f/3.0f),
        make_pair(NOTE_8_5,.25f/5.0f),
        make_pair(NOTE_8_7,.25f/7.0f),
        make_pair(NOTE_16,0.0625f),
        make_pair(NOTE_16_3,.125f/3.0f),
        make_pair(NOTE_16_5,.125f/5.0f),
        make_pair(NOTE_16_7,.125f/7.0f),
        make_pair(NOTE_32,.03125f),
        make_pair(NOTE_32_3,.03125f/3.0f),
        make_pair(NOTE_32_5,.03125f/5.0f),
        make_pair(NOTE_32_7,.03125f/7.0f)
    };
    const NoteLookup NOTE_LENGTHS(nn, nn + sizeof nn / sizeof nn[0]);
    */
    
    /*
    typedef map<int, float> BeatDurationLookup;
    
    const BeatDurationLookup::value_type bb[] = {
        make_pair(2,.5f),
        make_pair(4,.25f),
    };
    
    const BeatDurationLookup BEAT_LENGTHS(bb, bb + sizeof bb / sizeof bb[0]);
        */
        
class NoteTime {
	
  public:
    
    
 
    
    
    //...complete

    
	//bars, beats, division, and ticks.
    /*
     A beat corresponds to the denominator in the time signature.
     
     The division value is set in the Transport bar, below the time signature.
     
     A tick is the smallest possible bar subdivision. It is equal to 1/3840th of a note.
     
     
     
     Specifying a note time requires
     -begin time relative to current bar
     -end time relative to current bar
     
     I want to optimize the need for loops and therefore subdivide
     notes within each beat
     
     */
    /*
     Relative to bar
     */
    
    int beat;
    int division;
    int tick;
    double begin;//measured in notebeginvalue after beat, eg. delay
    double duration;//measured in durationvalue
    //double noteBeginValue;//after beat
    //double noteDurationValue;
    
    
     /*
     
     Specify the beat
     The rest note vale after the beat and how much of that
     Same for duration
     
     Eg a dotted eigth at 2.5 is NoteTime(2, NOTE_4,0.5,NOTE_8,1.5);
     
     */
    
     NoteTime(int _beat =1 ,double _duration = NOTE_4,double _delay = 0){
        set(_beat,_duration,_delay);
    }
    
    

    
    void set(int _beat=1,double _duration = NOTE_4,double _delay = 0){
        beat = _beat;
        begin = _delay;
        duration = _duration;
        
        
       // NoteLookup NL =  NOTE_LENGTHS;
        
    }
    
    /*
     Return values in number of beats from beginning of bar.
     Makes it all relative to BPM.
     */
    double getDuration(){
        //NoteLookup NL =  NOTE_LENGTHS;
        return  duration;
    }
    /*
     denominator defines the length of each beat.
     In a 4/4 each beat is a fourth of the bar.
     In 8/8 each beat is an eighth.
     
     */
    double getBegin(){
       // NoteLookup NL =  NOTE_LENGTHS;
        return begin;
    }
    
    
    int getBeat(){
        return beat;
    }
    /*
     float getEnd(){
        return getBegin()+getDuration();
    }*/
    
	
};
};
#endif
