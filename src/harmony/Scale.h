/*
 *  Scales.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 02/02/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 
 ================================================================================
 
 The scales module allows you to create a plethora of scales. Here's a
 little overview:
 
 === The diatonic scale and its modes ===
 * diatonic(note)
 * ionian(note)
 * dorian(note)
 * phrygian(note)
 * lydian(note)
 * mixolydian(note)
 * aeolian(note)
 * locrian(note)
 
 === The minor scales ===
 * natural_minor(note)
 * harmonic_minor(note)
 * melodic_minor(note)
 
 === Other scales ===
 * chromatic(note)
 * whole_note(note)
 * diminished(note)
 
 
 ================================================================================

 */


#ifndef _Scale
#define _Scale

#include "ofMain.h"
#include "Interval.h"
#include "Note.h"
#include "Diatonic.h"

namespace MusicTheory{
    class Scale;
    typedef shared_ptr<Scale> (*ScaleFunctionPointer)(NotePtr);
    typedef map<string,ScaleFunctionPointer> ScaleFunctionLookup;
    
    const Lookup::value_type chsc[] = {
        
        //Triads
		make_pair("m","dorian,aeolian"),
		make_pair("M","ionian"),
		make_pair("","ionian"),
		make_pair("dim","diminished"),
        
        //Augmented Chord
		make_pair("aug","augmented"),
		make_pair("+","augmented"),
		make_pair("7#5","augmented"),
		make_pair("M7+5","melodicMinorIII"),//lydian aug
		make_pair("M7+","augmented,hindu"),
		make_pair("m7+","augmented,aeolian,dorian"),
		make_pair("7+","melodicMinorV,augmented"),//mixolydian b6
        
        //Suspended Chord
		make_pair("sus47","aeolian,dorian,phrygian,mixolydian"),
		make_pair("sus4","ionian,harmonicMinor,melodicMinor,minorPentatonic,blues,aeolian,dorian,phrygian,mixolydian"),
		make_pair("sus2","harmonicMinor,melodicMinor,minorPentatonic,blues"),
		make_pair("sus","ionian,harmonicMinor,melodicMinor,minorPentatonic,blues,aeolian,dorian,phrygian,mixolydian"),
		make_pair("11","mixolydian"),
		make_pair("sus4b9",""),
		make_pair("susb9","melodicMinorII"),
        
        //Sevenths
		make_pair("m7","dorian,aeolian,minorPentatonic"),
		make_pair("M7","lydian,augmented"),
		make_pair("dom7","mixolydian"),
		make_pair("7","mixolydian"),
		make_pair("m7b5","locrian,melodicMinorVI"),
		make_pair("dim7","lydianDiminished"),
		make_pair("mM7","harmonicMinor"),
		make_pair("mM7","harmonicMinor"),
		
		
        //Sixths
		make_pair("m6","melodicMinor"),
		make_pair("M6","ionian,lydian"),
		make_pair("6","ionian,lydian"),
		make_pair("6/7",""),
		make_pair("67", ""),
		make_pair("6/9","pentatonicMajor"),
		make_pair("69","pentatonicMajor"),
        
        //Ninths
		make_pair("9"," dominant ninth"),
		make_pair("7b9","flamenco"),//so how to m7b9?
		make_pair("7#9","halfDiminished"),
		make_pair("M9","lydian"),
		make_pair("m9","aeolian,dorian"),
        
        //Elevenths
		make_pair("7#11",""),
		make_pair("m11",""),
        
        //Thirteenths
		make_pair("M13",""),
		make_pair("m13",""),
        
		make_pair("13",""),
        
        //Altered Chord
		make_pair("7b5","melodicMinorIV"),//lydian dominant
		
        //Special
		make_pair("hendrix","blues"),
		make_pair("7b12","blues"),
	
        
        //add
        make_pair("7b9b5","melodicMinorVII"),//superlocrian
        make_pair("m7b9","phrygian"),
        
    };
    
    
    const Lookup ChordScaleLookup(chsc, chsc + sizeof chsc / sizeof chsc[0]);

class Scale {
	
  public:
	
	Scale(){};
    
    deque<NotePtr> notes;
    string name;
    
    
        //factory methods
    
    static shared_ptr<Scale>create(){
        return shared_ptr<Scale>(new Scale());
    }
    
    shared_ptr<Scale> copy(){
        return shared_ptr<Scale>(new Scale(*this));//copy
    }
    
    
    //The diatonic scales and its modes
    /*
     Returns the diatonic scale starting on note.
     Example:
     {{{
     >>> diatonic("C")
     ["C", "D", "E", "F", "G", "A", "B"]
     */
    static deque<NotePtr> diatonic(NotePtr note){
	    return Diatonic::getNotes(note);
    }
    static shared_ptr<Scale> getDiatonic(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "diatonic";//same as ionian, doremi etc
        scale->notes = Scale::diatonic(note);
        return scale;
    }
    
    /*
     Returns the ionian mode scale starting on note.
     Example:
     {{{
     >>> ionian("C")
     ["C", "D", "E", "F", "G", "A", "B"]
     */
    
    static deque<NotePtr> ionian(NotePtr note){
        return Scale::diatonic(note);
    }
    
    static shared_ptr<Scale> getIonian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "ionian";//same as ionian, doremi etc
        scale->notes = Scale::diatonic(note);
        return scale;
    }
    
    /*
     Returns the dorian mode scale starting on note.
     Example:
     {{{
     >>> dorian("D")
     ["D", "E", "F", "G", "A", "B", "C"]

     */
    
    static deque<NotePtr> dorian(NotePtr note){

        deque<NotePtr> ionian = Scale::ionian(Interval::minorSeventh(note));
        
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,1);
    }
    
    static shared_ptr<Scale> getDorian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "dorian";
        scale->notes = Scale::dorian(note);
        return scale;
    }
    
    /*
     Returns the phrygian mode scale starting on note.
     Example:
     {{{
     >>> phrygian("E")
     ["E", "F", "G", "A", "B", "C", "D"]
     */
    static deque<NotePtr> phrygian(NotePtr note){
        deque<NotePtr> ionian = Scale::ionian(Interval::minorSixth(note));
    
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,2);
    }
    
    static shared_ptr<Scale> getPhrygian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "phrygian";
        scale->notes = Scale::phrygian(note);
        return scale;
    }
    
    
    
    
    /*
     Returns the lydian mode scale starting on note.
     Example:
     {{{
     >>> lydian("F")
     ["F", "G", "A", B", "C", "D", "E"]
     */
    
    static deque<NotePtr> lydian(NotePtr note){
        deque<NotePtr> ionian = Scale::ionian(Interval::perfectFifth(note));
        
        //all other diatonic scale are created from the ionian
        return offset(ionian,3);
    }
    
    static shared_ptr<Scale> getLydian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "lydian";
        scale->notes = Scale::lydian(note);
        return scale;
    }
    
    /*
     Returns the mixolydian mode scale starting on note.
     Example:
     {{{
     >>> mixolydian("G")
     ["G", "A", "B", "C", "D", "E", "F"]
     */
    static deque<NotePtr> mixolydian(NotePtr note){
        
        deque<NotePtr> ionian = Scale::ionian(Interval::perfectFourth(note));
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,4);
    }
    
    
    static shared_ptr<Scale> getMixolydian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "mixolydian";
        scale->notes = Scale::mixolydian(note);
        return scale;
    }
    
    
    /*
     Returns the aeolian mode scale starting on note.
     Example:
     {{{
     >>> aeolian("A")
     ["A", "B", "C", "D", "E", "F", "G"]
     
     
     also descending melodic minor 
     */
    
    static deque<NotePtr> aeolian(NotePtr note){
        deque<NotePtr> ionian = Scale::ionian(Interval::minorThird(note));
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,5);
    }
    
    static shared_ptr<Scale> getAeolian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "aeolian";
        scale->notes = Scale::aeolian(note);
        return scale;
    }
    
    /*
     Returns the locrian mode scale starting on note.
     Example:
     {{{
     >>> locrian("B")
     ["B", "C", "D", "E", "F", "G", "A"]
     */
    static deque<NotePtr> locrian(NotePtr note){
        deque<NotePtr> ionian = Scale::ionian(Interval::minorSecond(note));
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,6);
    }
    
    static shared_ptr<Scale> getLocrian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "locrian";
        scale->notes = Scale::locrian(note);
        return scale;
    }
    
    
    
    
    //The minor modes
    
    /*
     Returns the natural minor scale starting on note.
     Example:
     {{{
     >>> natural_minor("A")
     ["A", "B", "C", "D", "E", "F", "G"]
     */
    static deque<NotePtr> naturalMinor(NotePtr note){
        deque<NotePtr> scale = Diatonic::getNotes(Interval::minorThird(note));
        return Scale::offset(scale,5);
    }
    
    static shared_ptr<Scale> getNaturalMinor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "naturalMinor";
        scale->notes = Scale::naturalMinor(note);
        return scale;
    }
    
    /*
     Returns the harmonic minor scale starting on note.
     Example:
     {{{
     >>> harmonic_minor("A")
     "A", "B", "C", "D", "E", "F", "G#"]
     */
    static deque<NotePtr> harmonicMinor(NotePtr note){
        deque<NotePtr> scale = Scale::naturalMinor(note);
        scale[6]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getHarmonicMinor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "harmonicMinor";
        scale->notes = Scale::harmonicMinor(note);
        return scale;
    }
    
    /*
     Returns the melodic minor scale starting on note.
     Example:
     {{{
     >>> melodic_minor("A")
     ["A", "B", "C", "D", "E", "F#", "G#"]
     
     ascending melodic minor
     
     
     */
    static deque<NotePtr> melodicMinor(NotePtr note){
        deque<NotePtr> scale = Scale::harmonicMinor(note);
        scale[5]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinor";
        scale->notes = Scale::melodicMinor(note);
        return scale;
    }
    
    /*
     Phrygian #6
     Dorian b2
     */
    static deque<NotePtr> melodicMinorII(NotePtr note){
        deque<NotePtr> scale = Scale::phrygian(note);
        scale[5]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorII";
        scale->notes = Scale::melodicMinorII(note);
        return scale;
    }
    
    static shared_ptr<Scale> getPhrygianRaisedSixth(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "phrygian#6";
        scale->notes = Scale::melodicMinorII(note);
        return scale;
    }
    /*
     Lydian augmented
     */
    
    static deque<NotePtr> melodicMinorIII(NotePtr note){
        deque<NotePtr> scale = Scale::lydian(note);
        scale[4]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorIII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorIII";
        scale->notes = Scale::melodicMinor(note);
        return scale;
    }
    static shared_ptr<Scale> getLydianAugmented(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "lydianAugmented";
        scale->notes = Scale::melodicMinor(note);
        return scale;
    }
    
    /*
     Lydian dominant
     Mixolydian #4
     */
    
    static deque<NotePtr> melodicMinorIV(NotePtr note){
        deque<NotePtr> scale = Scale::lydian(note);
        scale[6]->diminish();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorIV(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorIV";
        scale->notes = Scale::melodicMinorIV(note);
        return scale;
    }
    static shared_ptr<Scale> getLydianDominant(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "lydianDominant";
        scale->notes = Scale::melodicMinorIV(note);
        return scale;
    }
    
    
    
    /*
     Mixolydian b6
     */
    
    static deque<NotePtr> melodicMinorV(NotePtr note){
        deque<NotePtr> scale = Scale::mixolydian(note);
        scale[5]->diminish();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorV(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorV";
        scale->notes = Scale::melodicMinorV(note);
        return scale;
    }
    static shared_ptr<Scale> getMixolydianLoweredSixth(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "mixolydianLoweredSixth";
        scale->notes = Scale::melodicMinorV(note);
        return scale;
    }
    
    /*
     Locrian #2
     */
    
    static deque<NotePtr> melodicMinorVI(NotePtr note){
        deque<NotePtr> scale = Scale::locrian(note);
        scale[1]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorVI(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorVI";
        scale->notes = Scale::melodicMinorVI(note);
        return scale;
    }
    static shared_ptr<Scale> getHalfDiminished(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "halfDiminished";
        scale->notes = Scale::melodicMinorVI(note);
        return scale;
    }
    
    
    
    /*
     SuperLocrian 
     Altered dominant
     Altered scale
     */
    
    static deque<NotePtr> melodicMinorVII(NotePtr note){
        deque<NotePtr> scale = Scale::locrian(note);
        scale[3]->diminish();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorVII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorVII";
        scale->notes = Scale::melodicMinorVII(note);
        return scale;
    }
    static shared_ptr<Scale> getSuperLocrian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "superLocrian";
        scale->notes = Scale::melodicMinorVII(note);
        return scale;
    }
    
    
    
    
    
    //Other scales
    
    
    static deque<NotePtr> lydianDiminished(NotePtr note){
        deque<NotePtr> scale = Scale::ionian(note);
        scale[2]->diminish();
        scale[3]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getLydianDiminished(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "lydianDiminished";
        scale->notes = Scale::lydianDiminished(note);
        return scale;
    }
    
    
    static deque<NotePtr> pentatonicMinor(NotePtr note){
        deque<NotePtr> scale;
        scale.push_back(note);
        scale.push_back(Interval::minorThird(note));
        scale.push_back(Interval::perfectFourth(note));
        scale.push_back(Interval::perfectFifth(note));
        scale.push_back(Interval::minorSeventh(note));
        return scale;
    }
    
    static shared_ptr<Scale> getPentatonicMinor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinor";
        scale->notes = Scale::inSen(note);
        return scale;
    }
   
    
    
    static deque<NotePtr> pentatonicMajor(NotePtr note){
        deque<NotePtr> scale;
        scale.push_back(note);
        scale.push_back(Interval::majorSecond(note));
        scale.push_back(Interval::majorThird(note));
        scale.push_back(Interval::perfectFifth(note));
        scale.push_back(Interval::majorSixth(note));
        return scale;
    }
    
    static shared_ptr<Scale> getPentatonicMajor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMajor";
        scale->notes = Scale::pentatonicMajor(note);
        return scale;
    }
    
    static deque<NotePtr> pentatonicDominant(NotePtr note){
        deque<NotePtr> scale;
        scale.push_back(note);
        scale.push_back(Interval::majorSecond(note));
        scale.push_back(Interval::majorThird(note));
        scale.push_back(Interval::perfectFifth(note));
        scale.push_back(Interval::minorSeventh(note));
        return scale;
    }
    
    static shared_ptr<Scale> getPentatonicDominant(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicDominant";
        scale->notes = Scale::pentatonicDominant(note);
        return scale;
    }
    
    
    
    
    static deque<NotePtr> blues(NotePtr note){
        deque<NotePtr> scale = Scale::pentatonicMinor(note);
        scale.insert(scale.begin()+3, Interval::minorFifth(note));
        return scale;
    }
    
    static shared_ptr<Scale> getBlues(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "blues";
        scale->notes = Scale::blues(note);
        return scale;
    }
    
    /*
     
     http://www.jazzguitar.be/bebopscale.html
     
     The G Bebop Scale can be played on most chords that are diatonic to the key of C major, but not on the C major chord itself because the F is an avoid note for the C major chord.
     
     The Bebop Scale is a dominant scale and has the same function in a key as the Mixolydian scale. It can be played on the dominant and the sub dominant. Our example, the G Bebop Scale, is the dominant of C Major and can be played over G7 and Dm7, giving us a great tool to play over II V I progressions.
     
     */
    
    static deque<NotePtr> bebopDominant(NotePtr note){
        deque<NotePtr> bebop = Scale::mixolydian(note);
        NotePtr maj7 = note->getOctaveUp();
        maj7->diminish();
        bebop.insert(bebop.end(), maj7);
        return bebop;
    }
    
    static shared_ptr<Scale> getBebopDominant(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "bebopDominant";
        scale->notes = Scale::bebopDominant(note);
        return scale;
    }
    
    
    /*
     Borg: This is the unique minor flamenco scale,
     a phrygian with added major third
     */
    static deque<NotePtr> flamenco(NotePtr note){
        deque<NotePtr> scale = Scale::phrygian(note);
        NotePtr majThird = scale[2]->getAugmented();
        scale.insert(scale.begin()+3, majThird);
        return scale;
    }
    
    static shared_ptr<Scale> getFlamenco(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "flamenco";
        scale->notes = Scale::flamenco(note);
        return scale;
    }
    
    static deque<NotePtr> chromatic(NotePtr note){
        deque<NotePtr> scale;
        scale.push_back(note);
        for(int i =1;i<12;i++){
            scale.push_back(Interval::getInterval(note,i));
        }
        return scale;
    }
    
    
    static shared_ptr<Scale> getChromatic(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "chromatic";
        scale->notes = Scale::chromatic(note);
        return scale;
    }
    
    /*
     Japanese scales
     */
    
    static deque<NotePtr> inSen(NotePtr note){
        deque<NotePtr> scale = Scale::pentatonicMinor(note);
        scale[1] = Interval::minorSecond(note);
        return scale;
    }
    
    static shared_ptr<Scale> getInSen(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "insen";
        scale->notes = Scale::inSen(note);
        return scale;
    }
    
    
    static deque<NotePtr> hirajoshi(NotePtr note){
        deque<NotePtr> scale = Scale::pentatonicMinor(note);
        scale[1] = Interval::minorSecond(note);
        scale[4] = Interval::minorSixth(note);
        return scale;
    }
    
    static shared_ptr<Scale> getHirajoshi(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "hirajoshi";
        scale->notes = Scale::hirajoshi(note);
        return scale;
    }
    
    static deque<NotePtr> hindu(NotePtr note){
        deque<NotePtr> scale = Scale::ionian(note);
        scale[5]->diminish();
        return scale;
    }
    
    static shared_ptr<Scale> getHindu(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "hindu";
        scale->notes = Scale::hindu(note);
        return scale;
    }
    
    /*
     Returns the whole note scale starting on note.
     Example:
     {{{
     >>> whole_note("C")
     ["C", "D", "E", "F#", "G#", "A#"]
     */
     static deque<NotePtr> wholeNote(NotePtr note){
         deque<NotePtr> scale;
         scale.push_back(note);
         for(int i =0;i<5;i++){
             note = Interval::majorSecond(note);
             scale.push_back(note);
         }
         return scale;
    }
    
    
    static shared_ptr<Scale> getWholeNote(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "wholeNote";
        scale->notes = Scale::wholeNote(note);
        return scale;
    }
    
    /*
     Returns the diminshed scale on note.
     Example:
     {{{
     >>> diminished("C")
     ['C', 'D', 'Eb', 'F', 'Gb', 'Ab', 'A', 'B']
     */
    
    static deque<NotePtr> diminished(NotePtr note){
         
             
        deque<NotePtr> scale;
        scale.push_back(note);
        NotePtr n = note;
        for(int i=0;i<3;i++){
            deque<NotePtr> step = wholeStepHalfStep(n);
            scale.insert(scale.end(),step.begin(),step.end());
            n = scale[scale.size()-1];
        }
        
        n = Interval::majorSeventh(note);
        scale.push_back(n);
        
        
        scale[scale.size()-2] = Interval::majorSixth(note);
        return scale;
     }
    
    static shared_ptr<Scale> getDiminished(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "diminished";
        scale->notes = Scale::diminished(note);
        return scale;
    }
    
    static deque<NotePtr> wholeStepHalfStep(NotePtr note){
        NotePtr maj2 = Interval::majorSecond(note);
        NotePtr min3 = Interval::minorThird(note);
        deque<NotePtr> scale;
        scale.push_back(maj2);
        scale.push_back(min3);
        return scale;
    }
    
    
    /*
     Hexatonal
     http://stormhorn.com/2009/08/24/the-augmented-scale/
     
     */
    
    static deque<NotePtr> augmented(NotePtr note){
        
        
        deque<NotePtr> scale;
        scale.push_back(note);
        NotePtr n = Interval::minorThird(note);
        scale.push_back(n);
        n = Interval::majorThird(note);
        scale.push_back(n);
        n = Interval::perfectFifth(note);
        scale.push_back(n);
        n = Interval::minorSixth(note);
        scale.push_back(n);
        n = Interval::majorSeventh(note);
        scale.push_back(n);
        
        return scale;
    }
    
    static shared_ptr<Scale> getAugmented(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "augmented";
        scale->notes = Scale::augmented(note);
        return scale;
    }
    
    
    
    
             
         /*
          Helper functions
          */
    static deque<NotePtr> offset(deque<NotePtr> orgscale,int amount){
            deque<NotePtr> scale;
        
            //first inset top ones
            scale.insert(scale.begin(), orgscale.begin(),orgscale.begin()+amount);
            
            //octave up
            Scale::changeOctave(scale,1);
        
            //then add the lower
            scale.insert(scale.begin(), orgscale.begin()+amount,orgscale.end());
       
            return scale;
    }
    
    static void changeOctave(deque<NotePtr> &_notes, int diff){
        for(int i=0;i<_notes.size();i++){
            _notes[i]->changeOctave(diff);
        }
    }
    
    void changeOctave(int diff){
        for(int i=0;i<notes.size();i++){
            notes[i]->changeOctave(diff);
        }
    }
    
    void setOctave(int oct){
        int diff = oct-notes[0]->octave;
        changeOctave(diff);
    }
    
	void octaveUp(){
        changeOctave(1);
    }
    
    Scale getOctaveUp(){
        Scale n = *this;//copy
        n.changeOctave(1);
        return n;
    }
    
	void octaveDown(){
        changeOctave(-1);
    }
    
    Scale getOctaveDown(){
        Scale n = *this;//copy
        n.changeOctave(-1);
        return n;
    }
    
    
    
    NotePtr getFirst(){
        if(notes.size()>0){
            return notes[0];
        }else{
            ofLog()<<"Scale::getFirst has no notes"<<endl;
        }
    }
    
    NotePtr getLast(){
        if(notes.size()>0){
            return notes[notes.size()-1];
        }else{
            ofLog()<<"Scale::getLast has no notes"<<endl;
        }
    }
    
    NotePtr getNotePtr(int i){
        if(notes.size()>0){
            int oct = 0;
            if(i<0){
                oct = - ceil((float)-i/(float)notes.size());
                i = (notes.size()+i) % notes.size();
            }else if(i>=notes.size()){
                oct = floor((float)i/(float)notes.size());
                i = i % notes.size();
            }
            
            
            NotePtr n = notes[i];
            n->changeOctave(oct);
            return n;
            
        }else{
            ofLog()<<"Scale::getNotePtr has no notes"<<endl;
        }
    }
             
        /*
         Determines the kind of scale. Can recognize all the diatonic modes and \
         the minor scales.
         Example:
         {{{
         >>> determine(["C", "D", "E", "F", "G", "A", "B"])
         'C ionian'
         */
             /*
              
        static string determine(deque<NotePtr> scale){
         
         
         possible_result = [
                            ["ionian",
                             ["major second", "major third", "perfect fourth",
                              "perfect fifth", "major sixth", "major seventh"]],
                            ["dorian",
                             ["major second", "minor third", "perfect fourth",
                              "perfect fifth", "major sixth", "minor seventh"]],
                            ["phrygian",
                             ["minor second", "minor third", "perfect fourth",
                              "perfect fifth", "minor sixth", "minor seventh"]],
                            ["lydian",
                             ["major second", "major third", "major fourth",
                              "perfect fifth", "major sixth", "major seventh"]],
                            ["mixolydian",
                             ["major second", "major third", "perfect fourth",
                              "perfect fifth", "major sixth", "minor seventh"]],
                            ["aeolian",
                             ["major second", "minor third", "perfect fourth",
                              "perfect fifth", "minor sixth", "minor seventh"]],
                            ["locrian",
                             ["minor second", "minor third", "perfect fourth",
                              "minor fifth", "minor sixth", "minor seventh"]],
                            ["natural minor",
                             ["major second", "minor third", "perfect fourth",
                              "perfect fifth", "minor sixth", "minor seventh"]],
                            ["harmonic minor",
                             ["major second", "minor third", "perfect fourth",
                              "perfect fifth", "minor sixth", "major seventh"]],
                            ["melodic minor",
                             ["major second", "minor third", "perfect fourth",
                              "perfect fifth", "major sixth", "major seventh"]],
                            ]
         
         tonic = scale[0]
         n = 0
         
// -- Describing the algorithm:
// Filter out all the wrong answers in possible_result
         for note in scale[1:]:
// 1. Determine the interval
         intval = Interval.determine(tonic, note)
         
         a=0
         temp = []
         
// 2. Go through possible_result and add it to temp if 
// it's a hit, do nothing otherwise
         for x in possible_result:
         if x[1][n] == intval:
         temp.append(x)
         n += 1
         
// 3. Set possible_result to temp
         possible_result = temp
         
// Get the results from possible_result and return
         res = []
         for x in possible_result:
         res.append(scale[0] + " " + x[0])
         return res
*/
    
    /*
     Get possible scales for a given chord, eg. maj7
     
     */
    
    
    static vector<string> getScalesForChord(string chordSymbol){
        Lookup m = ChordScaleLookup;
        string str = m[chordSymbol];
        return ofSplitString(str,",");
        
    }
    
    
    static vector<shared_ptr<Scale>> getScalesForChord(ChordPtr chord){
        
        vector<shared_ptr<Scale>> scalesInKey;
        
        Lookup m = ChordScaleLookup;
        string str = m[chord->getChordSymbol()];
        
        
        
        if(str==""){
            cout<<"getScalesForChord found nothing in ChordScaleLookup for "<<chord->getChordSymbol()<<endl;
            return scalesInKey;
        }
        vector<string> scales = ofSplitString(str,",");//scales that go with this symbol
        //how to consider bass and poly...baah..
        
        shared_ptr<Scale> s;
        for(int i=0;i<scales.size();i++){
            s = Scale::getScaleFromString(scales[i],chord->getRoot());
            scalesInKey.push_back(s);
        }
        return scalesInKey;
    }
    
    static shared_ptr<Scale> getScaleFromString(string scaleName, NotePtr n){
        ScaleFunctionLookup lookup = generateLookup();
        
        
        shared_ptr<Scale> s;
        if(lookup[scaleName]){
            s = (*lookup[scaleName])(n);
        }else{
            cout<<"Scale::getScaleFromString not found for chord: "<<scaleName<<endl;
        }
        
        return s;
        
    }
    
    
    
    //convenience
    
    static void print(deque<Note> notes){
        
        
        cout <<"[ ";
        for(int i = 0;i<notes.size();i++){
            cout<<notes[i];
            if(i<notes.size()-1){
                cout<<", ";
            }
        }
        cout<<" ]"<<endl;
    }
    
    
    
    static void print(deque<NotePtr> notes){
        
        
        cout <<"[ ";
        for(int i = 0;i<notes.size();i++){
            cout<<notes[i];
            if(i<notes.size()-1){
                cout<<", ";
            }
        }
        cout<<" ]"<<endl;
    }
    
    
    static void print(vector<string> symbols){
        
        cout <<"[ ";
        for(int i = 0;i<symbols.size();i++){
            cout<<symbols[i];
            if(i<symbols.size()-1){
                cout<<", ";
            }
        }
        cout<<" ]"<<endl;
        
        
    }
    /*
     //fix
    static void print(Scale  s){
        cout<<s;
    }*/
	
    //give access to private parts
    friend ostream& operator<<(ostream& os, const Scale& s);
    friend ostream& operator<<(ostream& os, const shared_ptr<Scale>& s);
    
    
private:
    
    
    static ScaleFunctionLookup generateLookup(){
        
        
        ScaleFunctionLookup::value_type sf[] ={
            make_pair("ionian",&Scale::getIonian),
            make_pair("dorian",&Scale::getDorian),
            make_pair("phrygian",&Scale::getPhrygian),
            make_pair("lydian",&Scale::getLydian),
            make_pair("mixolydian",&Scale::getMixolydian),
            make_pair("aeolian",&Scale::getAeolian),
            make_pair("locrian",&Scale::getLocrian),
            make_pair("pentatonicMinor",&Scale::getPentatonicMinor),
            make_pair("pentatonicMajor",&Scale::getPentatonicMajor),
            make_pair("pentatonicDominant",&Scale::getPentatonicDominant),
            make_pair("melodicMinor",&Scale::getMelodicMinor),
            make_pair("melodicMinorII",&Scale::getMelodicMinorII),
            make_pair("melodicMinorIII",&Scale::getMelodicMinorIII),
            make_pair("melodicMinorIV",&Scale::getMelodicMinorIV),
            make_pair("melodicMinorV",&Scale::getMelodicMinorV),
            make_pair("melodicMinorVI",&Scale::getMelodicMinorVI),
            make_pair("melodicMinorVII",&Scale::getMelodicMinorVII),
            make_pair("naturalMinor",&Scale::getNaturalMinor),
            make_pair("harmonicMinor",&Scale::getHarmonicMinor),
            make_pair("flamenco",&Scale::getFlamenco),
            make_pair("diminished",&Scale::getDiminished),
            make_pair("lydianDiminished",&Scale::getLydianDiminished)
        };
        
        
        
        
        ScaleFunctionLookup ScaleLookup(sf, sf + sizeof sf / sizeof sf[0]);
        return ScaleLookup;
        
        
    }
    
};
    

typedef shared_ptr<Scale> ScalePtr;

//this overloads the cout stream with useful output data
//corresponding friend function above, note: inside class
inline ostream& operator<<(ostream& os, Scale& s){
    os <<"Scale "<<s.notes[0]->getDiatonicName()<<" "<<s.name<<" [ ";
    for(int i=0;i<s.notes.size();i++){
        os<<s.notes[i];
        if(i<s.notes.size()-1){
            os<<", ";
        }
    }
    os<<" ]"<<endl;
    return os;
}

inline ostream& operator<<(ostream& os, ScalePtr& s){
    os <<"Scale "<<s->notes[0]->getDiatonicName()<<" "<<s->name<<" [ ";
    for(int i=0;i<s->notes.size();i++){
        os<<s->notes[i];
        if(i<s->notes.size()-1){
            os<<", ";
        }
    }
    os<<" ]"<<endl;
    return os;
}
    


};//namespace
#endif
