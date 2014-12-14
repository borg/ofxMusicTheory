/*
 *  MelodyGenerator.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 04/03/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 
 
 *  A pitch collection, just like a scale, is just a bunch of notes with no inner significance.
 *  A chord however is made up of notes with different significance. Too many solos are just
 *  running up and down the scales. The aim of the melody generator is to create interesting
 *  snippets, both harmonically and rhytmically.
 *
 *  Ideally it would be context aware...
 *  Todo: Research http://www.langston.com/Papers/amc.pdf
 *
 */

#ifndef _MelodyGenerator
#define _MelodyGenerator

#include "ofMain.h"
#include "Track.h"

namespace MusicTheory{

class MelodyGenerator {
	
  public:
	
	MelodyGenerator();
    
    map<int, deque<Note> > pitchCollections;
    
	
	static Track * generateMelodyOverTrack(Track const * comp, int lowerOctaveLim = 5, int higherOctaveLim = 7){
        Track *track = new Track();
        
        //timeline starts on 1
        Bar b;
        BarEvent be;
        
        
        for(int i=0;i<comp->bars.size();i++){
            Bar * barWithMelody = new Bar();
            b = comp->bars[i];
            
            for(int ii=0;ii<b.chords.size();ii++){
                be = b.chords[ii];
                
                Chord nextChord = be.chord;
                
                if(ii<(b.chords.size()-1)){
                    //more chords to come in this bar
                    nextChord = b.chords[ii+1].chord;
                }else if(i<comp->bars.size()-1){
                    //more bars...more chords too?
                    if(comp->bars[i+1].chords.size()>0){
                        //chose first chord in next bar
                        nextChord = comp->bars[i+1].chords[0].chord;
                    };
                    
                }
                //cout<<"beat: "<<be.time.beat<<" chord "<<be.chord<<endl;
                
                vector<Scale> chordScales = Scale::getScalesForChord(be.chord);
                if(chordScales.size()>0){
                    MelodyGenerator::generateMelodyOverChord(barWithMelody, be.chord,nextChord,be.time, chordScales[0]);//first scale is the deepest
                }
                // 1 get a scale
                // 2 chose endpoint for phrase
                // 3 establish length of phrase
                //bar->addNote(scale(13),NoteTime(1,NOTE_8_3));
            }
           // Chord chords = comp->
            track->addBar(*barWithMelody);
        }
        
        
        
        return track;
        
    }
    
    
    //get from a to b during this time
    
    static void generateMelodyOverChord(Bar * bar,Chord chord, Chord nextChord, NoteTime position, Scale scale, int lowerOctaveLim = 5, int higherOctaveLim = 7){
        
        scale.setOctave((int) ((lowerOctaveLim+higherOctaveLim)/2));
        
        
        for(int i=0;i<12;i++){
            NoteTime nt(position.beat,Note::SIXTEENTH_TRIPLET,Note::SIXTEENTH_TRIPLET*(float)i);
            bar->addNote(scale.getNote(i),nt);
        }
    
    }
	
};
}

#endif
