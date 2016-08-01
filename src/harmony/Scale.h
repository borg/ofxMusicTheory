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
 * diatonic(note->copy())
 * ionian(note->copy())
 * dorian(note->copy())
 * phrygian(note->copy())
 * lydian(note->copy())
 * mixolydian(note->copy())
 * aeolian(note->copy())
 * locrian(note->copy())
 
 === The minor scales ===
 * natural_minor(note->copy())
 * harmonic_minor(note->copy())
 * melodic_minor(note->copy())
 
 === Other scales ===
 * chromatic(note->copy())
 * whole_note(note->copy())
 * diminished(note->copy())
 
 
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
    
    static const Lookup ChordScaleLookup = {
        //Triads
		{"m","dorian,aeolian"},
		{"M","ionian"},
		{"","ionian"},
		{"dim","diminished"},
        
        //Augmented Chord
		{"aug","augmented"},
		{"+","augmented"},
		{"7#5","augmented"},
		{"M7+5","melodicMinorIII"},//lydian aug
		{"M7+","augmented,hindu"},
		{"m7+","augmented,aeolian,dorian"},
		{"7+","melodicMinorV,augmented"},//mixolydian b6
        
        //Suspended Chord
		{"sus47","pentatonicMinorV,dorian,aeolian,phrygian,mixolydian"},
        {"sus4","pentatonicMinorV,harmonicMinor,melodicMinor,pentatonicMinor,blues,aeolian,dorian,phrygian,mixolydian"},
		{"sus2","harmonicMinor,melodicMinor,pentatonicMinor,blues"},
		{"sus","ionian,harmonicMinor,melodicMinor,pentatonicMinor,blues,aeolian,dorian,phrygian,mixolydian"},
		{"11","mixolydian"},
		{"sus4b9",""},
		{"susb9","melodicMinorII"},
        
        //Sevenths
		{"m7","dorian,aeolian,pentatonicMinor"},
		{"M7","ionian,lydian"},
		{"dom7","mixolydian"},
		{"7","mixolydian"},
		{"m7b5","locrian,melodicMinorVI"},
		{"dim7","lydianDiminished"},
		{"mM7","harmonicMinor"},
		{"mM7","harmonicMinor"},
		
		
        //Sixths
		{"m6","melodicMinor"},
		{"M6","ionian,lydian"},
		{"6","ionian,lydian"},
		{"6/7",""},
		{"67", ""},
		{"6/9","pentatonicMajor"},
		{"69","pentatonicMajor"},
        
        //Ninths
		{"9","mixolydian,bebopDominant"},
		{"7b9","flamenco"},//so how to m7b9?
		{"7#9","halfDiminished"},
		{"M9","lydian"},
		{"m9","aeolian,dorian"},
        {"9#11","lydian"},
        
        
        //Elevenths
		{"7#11","melodicMinorIV"},
        
       
        
        
		{"m11","dorian,pentatonicMinorV,pentatonicMinorIV,pentatonicMinorIII,pentatonicMinor,blues"},
        
        //Thirteenths
		{"M13",""},
		{"m13",""},
        
		{"13","mixolydian,melodicMinorIV,bebopDominant,pentatonicMajor,blues"},
        
        //Altered Chord
		{"7b5","melodicMinorIV"},//lydian dominant
		
        //Special
		{"hendrix","blues"},
		{"7b12","blues"},
        
        {"5","pentatonicMinor,blues,aeolian,pentatonicMajor,pentatonicMinorIII,pentatonicMinorIV,pentatonicMinorV"},//as you please, since it's common to most scales
        
        //add
        {"7b9b5","melodicMinorVII"},//superlocrian
        {"m7b9","phrygian"},
        
        //altered, melodic minor
        {"m11b5","melodicMinorVI"},
        {"7#9b13","melodicMinorVII"}
        
        
        
    };
    
    //
    //
    //
class Scale : public enable_shared_from_this<Scale> {
	
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
    
    int size(){
        return notes.size();
    }
    
    string getName(){
        string fullName = "";
        
        if(notes.size()){
            fullName += notes[0]->getName();
        }
        
        fullName +=" "+ name;
        
        return fullName;
        
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
	    return Diatonic::getNotes(note->copy());
    }
    static shared_ptr<Scale> getDiatonic(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "diatonic";//same as ionian, doremi etc
        scale->notes = Scale::diatonic(note->copy());
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
        return Scale::diatonic(note->copy());
    }
    
    static shared_ptr<Scale> getIonian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "ionian";//same as ionian, doremi etc
        scale->notes = Scale::diatonic(note->copy());
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

        deque<NotePtr> ionian = Scale::ionian(Interval::minorSeventh(note->copy()));
        
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,1);
    }
    
    static shared_ptr<Scale> getDorian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "dorian";
        scale->notes = Scale::dorian(note->copy());
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
        deque<NotePtr> ionian = Scale::ionian(Interval::minorSixth(note->copy()));
    
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,2);
    }
    
    static shared_ptr<Scale> getPhrygian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "phrygian";
        scale->notes = Scale::phrygian(note->copy());
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
        deque<NotePtr> ionian = Scale::ionian(Interval::perfectFifth(note->copy()));
        
        //all other diatonic scale are created from the ionian
        return offset(ionian,3);
    }
    
    static shared_ptr<Scale> getLydian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "lydian";
        scale->notes = Scale::lydian(note->copy());
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
        
        deque<NotePtr> ionian = Scale::ionian(Interval::perfectFourth(note->copy()));
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,4);
    }
    
    
    static shared_ptr<Scale> getMixolydian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "mixolydian";
        scale->notes = Scale::mixolydian(note->copy());
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
        deque<NotePtr> ionian = Scale::ionian(Interval::minorThird(note->copy()));
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,5);
    }
    
    static shared_ptr<Scale> getAeolian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "aeolian";
        scale->notes = Scale::aeolian(note->copy());
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
        deque<NotePtr> ionian = Scale::ionian(Interval::minorSecond(note->copy()));
        //all other diatonic scale are created from the ionian
        return Scale::offset(ionian,6);
    }
    
    static shared_ptr<Scale> getLocrian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "locrian";
        scale->notes = Scale::locrian(note->copy());
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
        deque<NotePtr> scale = Diatonic::getNotes(Interval::minorThird(note->copy()));
        return Scale::offset(scale,5);
    }
    
    static shared_ptr<Scale> getNaturalMinor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "naturalMinor";
        scale->notes = Scale::naturalMinor(note->copy());
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
        deque<NotePtr> scale = Scale::naturalMinor(note->copy());
        scale[6]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getHarmonicMinor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "harmonicMinor";
        scale->notes = Scale::harmonicMinor(note->copy());
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
        deque<NotePtr> scale = Scale::harmonicMinor(note->copy());
        scale[5]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinor";
        scale->notes = Scale::melodicMinor(note->copy());
        return scale;
    }
    
    /*
     Phrygian #6
     Dorian b2
     */
    static deque<NotePtr> melodicMinorII(NotePtr note){
        deque<NotePtr> scale = Scale::phrygian(note->copy());
        scale[5]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorII";
        scale->notes = Scale::melodicMinorII(note->copy());
        return scale;
    }
    
    static shared_ptr<Scale> getPhrygianRaisedSixth(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "phrygian#6";
        scale->notes = Scale::melodicMinorII(note->copy());
        return scale;
    }
    /*
     Lydian augmented
     */
    
    static deque<NotePtr> melodicMinorIII(NotePtr note){
        deque<NotePtr> scale = Scale::lydian(note->copy());
        scale[4]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorIII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorIII";
        scale->notes = Scale::melodicMinor(note->copy());
        return scale;
    }
    static shared_ptr<Scale> getLydianAugmented(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "lydianAugmented";
        scale->notes = Scale::melodicMinor(note->copy());
        return scale;
    }
    
    /*
     Lydian dominant
     Mixolydian #4
     */
    
    static deque<NotePtr> melodicMinorIV(NotePtr note){
        deque<NotePtr> scale = Scale::lydian(note->copy());
        scale[6]->diminish();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorIV(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorIV";
        scale->notes = Scale::melodicMinorIV(note->copy());
        return scale;
    }
    static shared_ptr<Scale> getLydianDominant(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "lydianDominant";
        scale->notes = Scale::melodicMinorIV(note->copy());
        return scale;
    }
    
    
    
    /*
     Mixolydian b6
     */
    
    static deque<NotePtr> melodicMinorV(NotePtr note){
        deque<NotePtr> scale = Scale::mixolydian(note->copy());
        scale[5]->diminish();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorV(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorV";
        scale->notes = Scale::melodicMinorV(note->copy());
        return scale;
    }
    static shared_ptr<Scale> getMixolydianLoweredSixth(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "mixolydianLoweredSixth";
        scale->notes = Scale::melodicMinorV(note->copy());
        return scale;
    }
    
    /*
     Locrian #2
     */
    
    static deque<NotePtr> melodicMinorVI(NotePtr note){
        deque<NotePtr> scale = Scale::locrian(note->copy());
        scale[1]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorVI(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorVI";
        scale->notes = Scale::melodicMinorVI(note->copy());
        return scale;
    }
    static shared_ptr<Scale> getHalfDiminished(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "halfDiminished";
        scale->notes = Scale::melodicMinorVI(note->copy());
        return scale;
    }
    
    
    
    /*
     SuperLocrian 
     Altered dominant
     Altered scale
     */
    
    static deque<NotePtr> melodicMinorVII(NotePtr note){
        deque<NotePtr> scale = Scale::locrian(note->copy());
        scale[3]->diminish();
        return scale;
    }
    
    static shared_ptr<Scale> getMelodicMinorVII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "melodicMinorVII";
        scale->notes = Scale::melodicMinorVII(note->copy());
        return scale;
    }
    static shared_ptr<Scale> getSuperLocrian(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "superLocrian";
        scale->notes = Scale::melodicMinorVII(note->copy());
        return scale;
    }
    
    
    
    
    
    //Other scales
    
    
    static deque<NotePtr> lydianDiminished(NotePtr note){
        deque<NotePtr> scale = Scale::ionian(note->copy());
        scale[2]->diminish();
        scale[3]->augment();
        return scale;
    }
    
    static shared_ptr<Scale> getLydianDiminished(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "lydianDiminished";
        scale->notes = Scale::lydianDiminished(note->copy());
        return scale;
    }
    
    
    static deque<NotePtr> pentatonicMinor(NotePtr note){
        deque<NotePtr> scale;
        scale.push_back(note->copy());
        scale.push_back(Interval::minorThird(note->copy()));
        scale.push_back(Interval::perfectFourth(note->copy()));
        scale.push_back(Interval::perfectFifth(note->copy()));
        scale.push_back(Interval::minorSeventh(note->copy()));
        return scale;
    }
    
    static shared_ptr<Scale> getPentatonicMinor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinor";
        scale->notes = Scale::pentatonicMinor(note);
        return scale;
    }
   
    
    
    static deque<NotePtr> pentatonicMajor(NotePtr note){
        deque<NotePtr> scale;
        scale.push_back(note->copy());
        scale.push_back(Interval::majorSecond(note->copy()));
        scale.push_back(Interval::majorThird(note->copy()));
        scale.push_back(Interval::perfectFifth(note->copy()));
        scale.push_back(Interval::majorSixth(note->copy()));
        return scale;
    }
    
    static shared_ptr<Scale> getPentatonicMajor(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMajor";
        scale->notes = Scale::pentatonicMajor(note->copy());
        return scale;
    }
    
    static deque<NotePtr> pentatonicDominant(NotePtr note){
        deque<NotePtr> scale;
        scale.push_back(note->copy());
        scale.push_back(Interval::majorSecond(note->copy()));
        scale.push_back(Interval::majorThird(note->copy()));
        scale.push_back(Interval::perfectFifth(note->copy()));
        scale.push_back(Interval::minorSeventh(note->copy()));
        return scale;
    }
    
    static shared_ptr<Scale> getPentatonicDominant(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicDominant";
        scale->notes = Scale::pentatonicDominant(note->copy());
        return scale;
    }
    
    /*
    Pentatonic minor starting on minor second away from note
    */
    
    static deque<NotePtr> pentatonicMinorbII(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(1));
    }
    
    static shared_ptr<Scale> getPentatonicMinorbII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorbII";
        scale->notes = Scale::pentatonicMinorbII(note);
        return scale;
    }
    
    
    /*
    Pentatonic minor starting on major second away from note
    */
    
    static deque<NotePtr> pentatonicMinorII(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(2));
    }
    
    static shared_ptr<Scale> getPentatonicMinorII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorII";
        scale->notes = Scale::pentatonicMinorII(note);
        return scale;
    }
    
    /*
    Pentatonic minor starting on minor third away from note
    */
    
    static deque<NotePtr> pentatonicMinorbIII(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(3));
    }
    
    static shared_ptr<Scale> getPentatonicMinorbIII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorbIII";
        scale->notes = Scale::pentatonicMinorbIII(note);
        return scale;
    }
    /*
    Pentatonic minor starting on major third away from note
    */
    
    static deque<NotePtr> pentatonicMinorIII(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(4));
    }
    
    static shared_ptr<Scale> getPentatonicMinorIII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorIII";
        scale->notes = Scale::pentatonicMinorIII(note);
        return scale;
    }
   
    /*
    Pentatonic minor starting on fourth away from note
    */
    
    static deque<NotePtr> pentatonicMinorIV(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(5));
    }
    
    static shared_ptr<Scale> getPentatonicMinorIV(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorIV";
        scale->notes = Scale::pentatonicMinorIV(note);
        return scale;
    }

    /*
    Pentatonic minor starting on flat fifth away from note
    */
    
    static deque<NotePtr> pentatonicMinorbV(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(6));
    }
    
    static shared_ptr<Scale> getPentatonicMinorbV(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorbV";
        scale->notes = Scale::pentatonicMinorbV(note);
        return scale;
    }
   
   
    /*
    Pentatonic minor starting on perfect fifth away from note
    */
    
    static deque<NotePtr> pentatonicMinorV(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(7));
    }
    
    static shared_ptr<Scale> getPentatonicMinorV(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorV";
        scale->notes = Scale::pentatonicMinorV(note);
        return scale;
    }
   
    /*
    Pentatonic minor starting on minor sixth away from note
    */
    
    static deque<NotePtr> pentatonicMinorbVI(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(8));
    }
    
    static shared_ptr<Scale> getPentatonicMinorbVI(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorbVI";
        scale->notes = Scale::pentatonicMinorbVI(note);
        return scale;
    }
    
   
    /*
    Pentatonic minor starting on major sixth away from note
    */
    
    static deque<NotePtr> pentatonicMinorVI(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(9));
    }
    
    static shared_ptr<Scale> getPentatonicMinorVI(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorVI";
        scale->notes = Scale::pentatonicMinorVI(note);
        return scale;
    }
    
    /*
    Pentatonic minor starting on minor seventh away from note
    */
    
    static deque<NotePtr> pentatonicMinorbVII(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(10));
    }
    
    static shared_ptr<Scale> getPentatonicMinorbVII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorbVII";
        scale->notes = Scale::pentatonicMinorbVII(note);
        return scale;
    }
    
    /*
    Pentatonic minor starting on major seventh away from note
    */
    
    static deque<NotePtr> pentatonicMinorVII(NotePtr note){
        return Scale::pentatonicMinor(note->getTransposed(11));
    }
    
    static shared_ptr<Scale> getPentatonicMinorVII(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "pentatonicMinorVII";
        scale->notes = Scale::pentatonicMinorVII(note);
        return scale;
    }
    
    
    
    
    
    static deque<NotePtr> blues(NotePtr note){
        deque<NotePtr> scale = Scale::pentatonicMinor(note->copy());
        scale.insert(scale.begin()+3, Interval::minorFifth(note->copy()));
        return scale;
    }
    
    static shared_ptr<Scale> getBlues(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "blues";
        scale->notes = Scale::blues(note->copy());
        return scale;
    }
    
    /*
     
     http://www.jazzguitar.be/bebopscale.html
     
     The G Bebop Scale can be played on most chords that are diatonic to the key of C major, but not on the C major chord itself because the F is an avoid note for the C major chord.
     
     The Bebop Scale is a dominant scale and has the same function in a key as the Mixolydian scale. It can be played on the dominant and the sub dominant. Our example, the G Bebop Scale, is the dominant of C Major and can be played over G7 and Dm7, giving us a great tool to play over II V I progressions.
     
     */
    
    static deque<NotePtr> bebopDominant(NotePtr note){
        deque<NotePtr> bebop = Scale::mixolydian(note->copy());
        NotePtr maj7 = note->getOctaveUp();
        maj7->diminish();
        bebop.insert(bebop.end(), maj7);
        return bebop;
    }
    
    static shared_ptr<Scale> getBebopDominant(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "bebopDominant";
        scale->notes = Scale::bebopDominant(note->copy());
        return scale;
    }
    
    
    /*
     Borg: This is the unique minor flamenco scale,
     a phrygian with added major third
     */
    static deque<NotePtr> flamenco(NotePtr note){
        deque<NotePtr> scale = Scale::phrygian(note->copy());
        NotePtr majThird = scale[2]->getAugmented();
        scale.insert(scale.begin()+3, majThird);
        return scale;
    }
    
    static shared_ptr<Scale> getFlamenco(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "flamenco";
        scale->notes = Scale::flamenco(note->copy());
        return scale;
    }
    
    static deque<NotePtr> chromatic(NotePtr note){
        deque<NotePtr> scale;
        scale.push_back(note->copy());
        for(int i =1;i<12;i++){
            NotePtr n = note->copy();
            n->set(note->getInt()+i);
            scale.push_back(n);
        }
        return scale;
    }
    
    
    static shared_ptr<Scale> getChromatic(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "chromatic";
        scale->notes = Scale::chromatic(note->copy());
        return scale;
    }
    
    /*
     Japanese scales
     */
    
    static deque<NotePtr> inSen(NotePtr note){
        deque<NotePtr> scale = Scale::pentatonicMinor(note->copy());
        scale[1] = Interval::minorSecond(note->copy());
        return scale;
    }
    
    static shared_ptr<Scale> getInSen(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "insen";
        scale->notes = Scale::inSen(note->copy());
        return scale;
    }
    
    
    static deque<NotePtr> hirajoshi(NotePtr note){
        deque<NotePtr> scale = Scale::pentatonicMinor(note->copy());
        scale[1] = Interval::minorSecond(note->copy());
        scale[4] = Interval::minorSixth(note->copy());
        return scale;
    }
    
    static shared_ptr<Scale> getHirajoshi(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "hirajoshi";
        scale->notes = Scale::hirajoshi(note->copy());
        return scale;
    }
    
    static deque<NotePtr> hindu(NotePtr note){
        deque<NotePtr> scale = Scale::ionian(note->copy());
        scale[5]->diminish();
        return scale;
    }
    
    static shared_ptr<Scale> getHindu(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "hindu";
        scale->notes = Scale::hindu(note->copy());
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
         scale.push_back(note->copy());
         for(int i =0;i<5;i++){
             note = Interval::majorSecond(note->copy());
             scale.push_back(note->copy());
         }
         return scale;
    }
    
    
    static shared_ptr<Scale> getWholeNote(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "wholeNote";
        scale->notes = Scale::wholeNote(note->copy());
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
        scale.push_back(note->copy());
        NotePtr n = note;
        for(int i=0;i<3;i++){
            deque<NotePtr> step = wholeStepHalfStep(n);
            scale.insert(scale.end(),step.begin(),step.end());
            n = scale[scale.size()-1];
        }
        
        n = Interval::majorSeventh(note->copy());
        scale.push_back(n);
        
        
        scale[scale.size()-2] = Interval::majorSixth(note->copy());
        return scale;
     }
    
    static shared_ptr<Scale> getDiminished(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "diminished";
        scale->notes = Scale::diminished(note->copy());
        return scale;
    }
    
    static deque<NotePtr> wholeStepHalfStep(NotePtr note){
        NotePtr maj2 = Interval::majorSecond(note->copy());
        NotePtr min3 = Interval::minorThird(note->copy());
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
        scale.push_back(note->copy());
        NotePtr n = Interval::minorThird(note->copy());
        scale.push_back(n);
        n = Interval::majorThird(note->copy());
        scale.push_back(n);
        n = Interval::perfectFifth(note->copy());
        scale.push_back(n);
        n = Interval::minorSixth(note->copy());
        scale.push_back(n);
        n = Interval::majorSeventh(note->copy());
        scale.push_back(n);
        
        return scale;
    }
    
    static shared_ptr<Scale> getAugmented(NotePtr note){
        shared_ptr<Scale> scale = Scale::create();
        scale->name = "augmented";
        scale->notes = Scale::augmented(note->copy());
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
    
    
    int getOctave(){
        if(notes.size()){
            return notes[0]->octave;
        }else{
            return 0;
        }
    }
    
    
    void setOctave(int oct){
        int diff = oct-notes[0]->octave;
        changeOctave(diff);
    }
    
	void octaveUp(){
        changeOctave(1);
    }
    
    shared_ptr<Scale> getOctaveUp(){
        shared_ptr<Scale> n = copy();//copy
        n->changeOctave(1);
        return n;
    }
    
	void octaveDown(){
        changeOctave(-1);
    }
    
    shared_ptr<Scale> getOctaveDown(){
        shared_ptr<Scale> n = copy();//copy
        n->changeOctave(-1);
        return n;
    }
    
    
    
    NotePtr getFirst(){
        if(isValid()){
            return notes[0];
        }else{
            ofLogWarning()<<"Scale::getFirst has no notes"<<endl;
        }
    }
    
    NotePtr getLast(){
        if(isValid()){
            return notes[notes.size()-1];
        }else{
            ofLogWarning()<<"Scale invalid"<<endl;
        }
    }
    
    
    NotePtr getRoot(){
        return getFirst();
    }
    
    /*
    Return major or minor third.
    If contains both, minor will be returned
    */
    NotePtr getThird(){
        if(isValid()){
            NotePtr root = getRoot();
            NotePtr third;
            for(NotePtr note:notes){
                int halfTones = Interval::measure(root, note);
                if(halfTones == 3 || halfTones == 4){
                    return note->copy();
                }
            }
            ofLogWarning()<<"Scale does not contain a third"<<endl;
            return 0;
        }else{
            ofLogWarning()<<"Scale invalid"<<endl;
        }
    }
    
    NotePtr getSeventh(){
        if(isValid()){
            NotePtr root = getRoot();
            NotePtr seventh;
            for(NotePtr note:notes){
                int halfTones = Interval::measure(root, note);
                if(halfTones == 10 || halfTones == 11){
                    return note->copy();
                }
            }
            ofLogWarning()<<"Scale does not contain a seventh"<<endl;
            return 0;
        }else{
            ofLogWarning()<<"Scale invalid"<<endl;
        }
    }
    
    
    NotePtr getNote(int i){
        if(isValid()){
            int oct = 0;
            if(i<0){
                oct = - ceil((float)-i/(float)notes.size());
                i = (notes.size()+i) % notes.size();
            }else if(i>=notes.size()){
                oct = floor((float)i/(float)notes.size());
                i = i % notes.size();
            }
            
            
            NotePtr n = notes[i]->copy();
            n->changeOctave(oct);
            return n;
            
        }else{
            ofLogWarning()<<"Scale invalid"<<endl;
        }
    }
    
    
    NotePtr getClosestNote(NotePtr note, bool ifNotInScaleSelectNextHigher = true){
        if(isValid()){
            int startNote = note->getInt();
            int startOct = note->getOctave();
            
            shared_ptr<Scale> stack = copy();
            stack->setOctave(startOct);
            
            while(stack->getNote(0)->getInt()>startNote){
                stack->octaveDown();
            }
            bool found = false;
            int currNote = 0;
            
            while(!found){
                if(stack->getNote(currNote)->getInt() == note->getInt()){
                    found = true;
                    return stack->getNote(currNote)->copy();
                }
                
                if(stack->getNote(currNote)->getInt() > note->getInt()){
                    //just missed it
                    if(ifNotInScaleSelectNextHigher){
                        found = true;
                        return stack->getNote(currNote)->copy();
                    }else{
                        found = true;
                        return stack->getNote(currNote-1)->copy();
                    }
                }
                currNote++;
            }
        
        }else{
            ofLogWarning()<<"Scale invalid"<<endl;
            return note->copy();
        }
    }
    
    bool isValid(){
        return notes.size()>0;
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
 
        shared_ptr<Scale> s;
        if(ScaleLookup()[scaleName]){
            s = (*ScaleLookup()[scaleName])(n);
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
    
    
    static ScaleFunctionLookup ScaleLookup(){
       static ScaleFunctionLookup _scaleLookup ={
            {"diatonic",&Scale::getDiatonic},
            {"ionian",&Scale::getIonian},
            {"dorian",&Scale::getDorian},
            {"phrygian",&Scale::getPhrygian},
            {"lydian",&Scale::getLydian},
            {"mixolydian",&Scale::getMixolydian},
            {"aeolian",&Scale::getAeolian},
            {"locrian",&Scale::getLocrian},
            {"pentatonicMinor",&Scale::getPentatonicMinor},
            {"pentatonicMinorbII",&Scale::getPentatonicMinorbII},
            {"pentatonicMinorII",&Scale::getPentatonicMinorII},
            {"pentatonicMinorbIII",&Scale::getPentatonicMinorbIII},
            {"pentatonicMinorIII",&Scale::getPentatonicMinorIII},
            {"pentatonicMinorIV",&Scale::getPentatonicMinorIV},
            {"pentatonicMinorbV",&Scale::getPentatonicMinorbV},
            {"pentatonicMinorV",&Scale::getPentatonicMinorV},
            {"pentatonicMinorbVI",&Scale::getPentatonicMinorbVI},
            {"pentatonicMinorVI",&Scale::getPentatonicMinorVI},
            {"pentatonicMinorbVII",&Scale::getPentatonicMinorbVII},
            {"pentatonicMinorVII",&Scale::getPentatonicMinorVII},
            {"pentatonicMajor",&Scale::getPentatonicMajor},
            {"pentatonicDominant",&Scale::getPentatonicDominant},
            {"melodicMinor",&Scale::getMelodicMinor},
            {"melodicMinorII",&Scale::getMelodicMinorII},
            {"melodicMinorIII",&Scale::getMelodicMinorIII},
            {"augmented",&Scale::getAugmented},
            {"melodicMinorII",&Scale::getMelodicMinorII},
            {"melodicMinorIII",&Scale::getMelodicMinorIII},
            {"melodicMinorIV",&Scale::getMelodicMinorIV},
            {"melodicMinorV",&Scale::getMelodicMinorV},
            {"melodicMinorVI",&Scale::getMelodicMinorVI},
            {"melodicMinorVII",&Scale::getMelodicMinorVII},
            {"naturalMinor",&Scale::getNaturalMinor},
            {"harmonicMinor",&Scale::getHarmonicMinor},
            {"flamenco",&Scale::getFlamenco},
            {"diminished",&Scale::getDiminished},
            {"bebopDominant",&Scale::getBebopDominant},
            {"blues",&Scale::getBlues},
            {"lydianDiminished",&Scale::getLydianDiminished},
            {"lydianDominant",&Scale::getLydianDominant}
        };
        
        
        return _scaleLookup;
         
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
