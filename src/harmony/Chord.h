/*
 *  Chord.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 29/01/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 
 The Chord module is a huge module that builds on the \
 [refMingusCoreInterval Interval] module. It can be used to generate and \
 recognise a plethora of Chord. The following overview groups some of the functions you are most likely to use together.
 
 == Generate Diatonic Chord ==
 * Triads
 * triad
 * triads
 * Sevenths
 * seventh
 * sevenths
 
 == Generate Absolute Chord ==
 * Triads
 * minor_triad
 * major_triad
 * diminished_triad
 * Sixths
 * minor_sixth
 * major_sixth
 * Sevenths
 * minor_seventh
 * major_seventh
 * dominant_seventh
 * minor_major_seventh
 * minor_seventh_flat_five
 * diminished_seventh
 * Ninths
 * minor_ninth
 * major_ninth
 * dominant_ninth
 * Elevenths
 * minor_eleventh
 * eleventh
 * Thirteenths
 * minor_thirteenth
 * major_thirteenth
 * dominant_thirteenth
 * Augmented Chord
 * augmented_triad
 * augmented_major_seventh
 * augmented_minor_seventh
 * Suspended Chord
 * suspended_second_triad
 * suspended_fourth_triad
 * suspended_seventh
 * suspended_fourth_ninth
 * suspended_ninth
 * Altered Chord
 * dominant_flat_ninth
 * dominant_sharp_ninth
 * dominant_flat_five
 * sixth_ninth
 * hendrix_chord
 
 == Get Chord by Function ==
 
 * Function
 * tonic and tonic7
 * supertonic and supertonic7
 * mediant and mediant7
 * subdominant and subdominant7
 * dominant and dominant7
 * submediant and submediant7
 * Aliases
 * I, II, III, IV, V, VI
 * ii, iii, vi, vii
 * I7, II7, III7, IV7, V7, VI7
 * ii7, iii7, vi7
 
 == Useful Functions ==
 
 * determine - Can recognize all the Chord that can be generated with from_shorthand (a lot) and their inversions.
 * from_shorthand - Generates Chord from shorthand (eg. 'Cmin7')
 
 
 ================================================================================
 */

#ifndef _Chord
#define _Chord

#include "ofMain.h"
#include "Note.h"


namespace MusicTheory{

    class Chord;//forward declaration for these cache containers
    
    
    typedef map<string, string> Lookup;
    
     
    
    //A cache for composed triads
    static map<string, vector<Chord> > _triads_cache;
    
    //A cache for composed sevenths
    static map<string, vector<Chord> > _sevenths_cache;
    
    
    

  /*
   A dictionairy that can be used to present
   lookup chord abbreviations. This dictionairy is also
   used in determine_seventh()
   */
        
        
    const Lookup::value_type x[] = {

        //Triads
		make_pair("m"," minor triad"),
		make_pair("M"," major triad"),
		make_pair(""," major triad"),
		make_pair("dim"," diminished triad"),
        
        //Augmented Chord
		make_pair("aug"," augmented triad"),
		make_pair("+"," augmented triad"),
		make_pair("7#5"," augmented minor seventh"),
		make_pair("M7+5"," augmented major seventh"),//corrected borg
		make_pair("M7+"," augmented major seventh"),
		make_pair("m7+"," augmented minor seventh"),
		make_pair("7+"," augmented major seventh"),
        
        //Suspended Chord
		make_pair("sus47"," suspended seventh"),
		make_pair("sus4"," suspended fourth triad"),
		make_pair("sus2"," suspended second triad"),
		make_pair("sus"," suspended fourth triad"),
		make_pair("11"," eleventh"),
		make_pair("sus4b9"," suspended fourth ninth"),
		make_pair("susb9"," suspended fourth ninth"),
        
        //Sevenths
		make_pair("m7"," minor seventh"),
		make_pair("M7"," major seventh"),
		make_pair("dom7"," dominant seventh"),
		make_pair("7"," dominant seventh"),
		make_pair("m7b5"," half diminished seventh"),
		make_pair("dim7"," diminished seventh"),
		make_pair("mM7"," minor/major seventh"),
		
		
        //Sixths
		make_pair("m6"," minor sixth"),
		make_pair("M6"," major sixth"),
		make_pair("6"," major sixth"),
		make_pair("6/7"," dominant sixth"),
		make_pair("67", " dominant sixth"),
		make_pair("6/9"," sixth ninth"),
		make_pair("69"," sixth ninth"),
        
        //Ninths
		make_pair("9"," dominant ninth"),
		make_pair("7b9"," dominant flat ninth"),
		make_pair("7#9"," dominant sharp ninth"),
		make_pair("M9"," major ninth"),
		make_pair("m9"," minor ninth"),
        
        //Elevenths
		make_pair("7#11"," lydian dominant seventh"),
		make_pair("m11"," minor eleventh"),
        
        //Thirteenths
		make_pair("M13"," major thirteenth"),
		make_pair("m13"," minor thirteenth"),
        
		make_pair("13"," dominant thirteenth"),
        
        //Altered Chord
		make_pair("7b5"," dominant flat five"),
		
        //Special
		make_pair("hendrix"," hendrix chord"),
		make_pair("7b12"," hendrix chord"),
		make_pair("5"," perfect fifth")
    };
    
          
      const Lookup ChordLookup(x, x + sizeof x / sizeof x[0]);
    
class Chord {
   
	
  // const map<int, int> m = {{1,2}, {3,4}};
    
  public:
    
    deque<Note> notes;
    string name;
    Note root;
    //Note bass;
    
    /*
     Recursive classes in C++? A class that contains instances of itself as children.  Apparently it was banned.
    
    I would rather use this but to include boost just for this seems unecessary
    http://www.boost.org/doc/libs/1_48_0/doc/html/container/containers_of_incomplete_types.html
     
     Using of smart pointers instead
    */
    
    vector< ofPtr<Chord> > polychords;
    
    Chord(string _name = ""){
        if(_name !=""){
            set(_name);
        }else{
        //incomplete...fix this
            name = "";
        }
    };
    
    
    
    void set(string _name = "C"){
        Chord c = Chord::fromShorthand(_name);
        name = c.name;
        notes = c.notes;
        
        Note r(getRootNote(_name));
        setRoot(r);
    }
    
    string getName(){
        string fullName;
        if(getBass().getName()!=getRoot().getName()){
            //slash chord
            fullName = getRoot().getDiatonicName() + name+"/"+getBass().getDiatonicName();
        }else if(polychords.size()>0){
            //ignoring for the moment more than one nested polychord...
            fullName = getRoot().getDiatonicName() + name+"/"+getBass().getDiatonicName()+"|"+polychords[0]->getName();
        }else{
            //normal chord
            fullName= getRoot().getDiatonicName() + name;
        }
        return fullName;
    }
    
    /*
     This sets the chord symbol. If you want o change dom7 to 7 for instance, use it.
     Might need to be fixed as it is not consistent with getName.
     Name and symbol? What should be the convention.
     */
    void setName(string str){
        name = str;
    }
    /*
     Get all notes including those of nested polychords
     */
    deque<Note> getAllNotes(){
        deque<Note> allNotes = notes;
        if(polychords.size()>0){
            allNotes.insert(allNotes.end(), polychords[0]->notes.begin(),polychords[0]->notes.end());
        }
        return allNotes;
    }
    
    
    
    
    /*
     STATIC METHODS
     */
    
    static string getFullName(string str){
        //why do I need to instantiate here? Odd
        Lookup m = ChordLookup;
        string name = m[str];
        
        
        if(name.size()>0){
            return name;
        }else{
            return "Chord not found";
        }

    }
    
    static vector<string> getAllKnownChords(){
        //why do I need to instantiate here? Odd
        Lookup m = ChordLookup;
        vector<string> chords;
        
        typedef map<string, string>::iterator it_type;
         for(it_type iterator = m.begin(); iterator != m.end(); iterator++) {
             chords.push_back(iterator->first);
         }
        return chords;
        
    }
    
 
    /*
     INSTANCE METHODS
     
     */
    string getFullName(){
        return Chord::getFullName(name);
    }
    /*
     root might not be first note in notes
     http://en.wikipedia.org/wiki/Root_%28chord%29
     */
   /* Note root(){
  
        if(root){
        string rootStr = Chord::getRootNote(name);
        cout<<"rootStr "<<rootStr<<endl;
        Note n(rootStr);
        
        return n;
        }else{
            return notes[0];
        //}
    }*/
    
    Note getRoot(){
        
        //return notes[0]
        
        return root;
    }
    
    void setRoot(Note n){
        root = n;
    }
    /*
     Returns the lowest note.
     
     Need to sort??
     */
    Note getBass(){
        return notes[0];
        
    }
    
    void setBass(Note bass){
        
        Note n = bass;//copy
        n.octave = notes[0].getOctaveDown().octave;
        n.octaveDown();//arbitrary hack ha ha
        notes.push_front(n);
    }
    
    
    void setPolyChord(Chord pc){
        
        
        ofPtr<Chord> subchord(new Chord());
        subchord->name = pc.getChordSymbol();
        subchord->notes = pc.notes;
        subchord->setRoot(pc.getRoot());
        
        polychords.push_back(subchord);
    }
    
    string getChordSymbol(){
        string nm = getName();
       // cout<<"Chord::getChordSymbol for "<<name<<" "<<nm<<endl;
        string str = Chord::getChordSymbol(nm);
        return str;
    }
    
/*===================================================================
							Triads
===================================================================*/
    
    
    //diatonic
    /*
     Returns the triad on note in key as a list.
     Example:
    {{{
        >>> triad("E", "C")
        ["E", "G", "B"]
        >>> triad("E", "B")
        ["E", "G#", "B"]
    */
    
    static Chord triad(Note note,Note key){
        Chord chord;
        chord.name = "";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::third(note, key);
        chord.notes.push_back(n);
        n = Interval::fifth(note, key);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    /*
     Returns all the triads in key. Implemented using a cache.
    */
    
    static vector<Chord> triads(Note key){
        if(_triads_cache[key.name].size()>0){
            return _triads_cache[key.name];
        }
    
        deque<Note> triads = Diatonic::getNotes(key);
        
        vector<Chord> chords;
        for(int i = 0;i<triads.size();i++){
            Chord chord = Chord::triad(triads[i], key);
            chords.push_back(chord);
        }
        
        
        _triads_cache[key.name] = chords;
        return chords;
    }
     

    
    
    //absolute
    
    /*
     Builds a major triad on note.
     Example:      
     major_triad("C")
     ["C", "E", "G"]
     
     */
    
    static Chord majorTriad(Note note){
        Chord chord;
        chord.name = "M";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::majorThird(note);
        chord.notes.push_back(n);
        n = Interval::perfectFifth(note);
        chord.notes.push_back(n);
        return chord;

    }
    
    //TODO: Check where setRoot and setBass are needed
   
    /*
     Builds a minor triad on note.
     Example: 
     minor_triad("C")
     ["C", "Eb", "G"]
     
     */
    static Chord minorTriad(Note note){
        Chord chord;
        chord.name = "m";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::minorThird(note);
        chord.notes.push_back(n);
        n = Interval::perfectFifth(note);
        chord.notes.push_back(n);
        return chord;
        
    }
    
    
    
    /*
     Builds a diminished triad on note.
     Example: 
     diminished_triad("C")
     ["C", "Eb", "Gb"]
     
     */
    static Chord diminishedTriad(Note note){
        Chord chord;
        chord.name = "dim";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::minorThird(note);
        chord.notes.push_back(n);
        n = Interval::minorFifth(note);
        chord.notes.push_back(n);
        return chord;
        
    }
    
    
    
    
    
    /*
     Builds an augmented triad on note.
     Example: 
     augmented_triad("C")
     ["C", "E", "G#"]
     
     */
    static Chord augmentedTriad(Note note){
        Chord chord;
        chord.name = "aug";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::majorThird(note);
        chord.notes.push_back(n);
        n = Interval::majorFifth(note);
        chord.notes.push_back(n);
        return chord;
        
    }
    
    
        

    
    
    
    
/*===================================================================
	Sevenths
===================================================================*/
    
    
    //diatonic
    
    
    
    /*
     Returns the seventh chord on note in key.
     Example: 
     seventh("C", "C")
     ["C", "E", "G", "B"]
     
     */
    
    static Chord seventh(Note note,Note key){
        Chord chord = Chord::triad(note,key);
        Note n = Interval::seventh(note, key);
        chord.notes.push_back(n);
        chord.name = "7";
        chord.setRoot(note);
        return chord;
    }
    
    
    

    
    
    
    /*
     Returns all the sevenths in key. Implemented using a cache.
     */
    
    static vector<Chord> sevenths(Note key){
        if(_sevenths_cache[key.name].size()>0){
            return _sevenths_cache[key.name];
        }
        
        deque<Note> triads = Diatonic::getNotes(key);
        
        vector<Chord> chords;
        for(int i = 0;i<triads.size();i++){
            Chord chord = Chord::seventh(triads[i], key);
            chords.push_back(chord);
        }
        
        
        _sevenths_cache[key.name] = chords;
        return chords;
    }
    
    
    
    
    
    
    //absolute
    
    
    /*
     Builds a major seventh on note.
     Example: 
     major_seventh("C")
     ["C", "E", "G", "B"]
          */
    
    static Chord majorSeventh(Note note){
        Chord chord = Chord::majorTriad(note);
        chord.name = "M7";
        chord.setRoot(note);
        Note n = Interval::majorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    /*
     Builds a minor seventh on note.
     Example: 
     minor_seventh("C")
     ["C", "Eb", "G", "Bb"]
     
     */
    
    static Chord minorSeventh(Note note){
        Chord chord = Chord::minorTriad(note);
        chord.name = "m7";
        chord.setRoot(note);
        Note n = Interval::minorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
          
    
    /*
     Builds a dominant seventh on note.
     Example: 
     dominant_seventh("C")
     ["C", "E", "G", "Bb"]
     
     */
    
    static Chord dominantSeventh(Note note){
        Chord chord = Chord::majorTriad(note);
        chord.name = "dom7";
        chord.setRoot(note);
        Note n = Interval::minorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a half diminished seventh (=minor seventh flat five) \
     chord on note.
     Example: 
     half_diminished_seventh("C")
     ["C", "Eb", "Gb", "Bb"]
     
     */
    
    static Chord halfDiminishedSeventh(Note note){
        Chord chord = Chord::diminishedTriad(note);
        chord.name = "m7b5";
        chord.setRoot(note);
        Note n = Interval::minorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    /*
     See half_diminished_seventh(note) for docs.
     Seems just a duplicate naming convention
    */
    
    static Chord minorSeventhFlatFive(Note note){
        return halfDiminishedSeventh(note);
    }
    
    
    
    
    
    /*
     Builds a diminished seventh chord on note.
     Example: 
     diminished_seventh("C")
     ["C", "Eb", "Gb", "Bbb"]
     
     */
    
    static Chord diminishedSeventh(Note note){
        Chord chord = Chord::diminishedTriad(note);
        chord.name = "dim7";
        chord.setRoot(note);
        Note n = Interval::minorSeventh(note);
        n.diminish();
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    
    /*
     Builds a minor major seventh chord on note.
     Example: 
     minor_major_seventh("C")
     ["C", "Eb", "G", "B"]
     
     */
    
    static Chord minorMajorSeventh(Note note){
        Chord chord = Chord::minorTriad(note);
        chord.name = "mM7";
        chord.setRoot(note);
        Note n = Interval::majorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
	    
    
    
    
/*===================================================================
	Sixths
===================================================================*/
    
    
    // absolute
    
    
    /*
     Builds a minor sixth chord on note.
     Example: 
     minor_sixth("C")
     ['C', 'Eb', 'G', 'A']
     
     */
    
    static Chord minorSixth(Note note){
        Chord chord = Chord::minorTriad(note);
        chord.name = "m6";
        chord.setRoot(note);
        Note n = Interval::majorSixth(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    

    
    /*
     Builds a major sixth chord on note.
     Example: 
     major_sixth("C")
     ['C', 'E', 'G', 'A']
     
     */
    
    static Chord majorSixth(Note note){
        Chord chord = Chord::majorTriad(note);
        chord.name = "M6";
        chord.setRoot(note);
        Note n = Interval::majorSixth(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    

    
    /*
     Builds the altered chord 6/7 on note.
     Example: 
     dominant_sixth("C")
     ['C', 'E', 'G', 'A', 'Bb']
     
     */
    
    static Chord dominantSixth(Note note){
        Chord chord = Chord::majorSixth(note);
        chord.name = "67";
        chord.setRoot(note);
        Note n = Interval::minorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds the sixth/ninth chord on note.
     Example: 
     sixth_ninth('C')
     ['C', 'E', 'G', 'A', 'D']
     
     */
    
    static Chord sixthNith(Note note){
        Chord chord = Chord::majorSixth(note);
        chord.name = "69";
        chord.setRoot(note);
        Note n = Interval::majorSecond(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
        
    
 
    
    
/*===================================================================
	Ninths
===================================================================*/
    
    
    
    //absolute
    
    
    /*
     Builds a minor ninth chord on note.
     Example: 
     minor_ninth("C")
     ['C', 'Eb', 'G', 'Bb', 'D']
     
     */
    
    static Chord minorNinth(Note note){
        Chord chord = Chord::minorSeventh(note);
        chord.name = "m9";
        chord.setRoot(note);
        Note n = Interval::majorSecond(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a major ninth chord on note.
     Example: 
     major_ninth("C")
     ['C', 'E', 'G', 'B', 'D']
     
     */
    
    static Chord majorNinth(Note note){
        Chord chord = Chord::majorSeventh(note);
        chord.name = "M9";
        chord.setRoot(note);
        Note n = Interval::majorSecond(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a dominant ninth chord on note.
     Example: 
     dominant_ninth("C")
     ['C', 'E', 'G', 'Bb', 'D']
     
     */
    
    static Chord dominantNinth(Note note){
        Chord chord = Chord::dominantSeventh(note);
        chord.name = "9";
        chord.setRoot(note);
        Note n = Interval::majorSecond(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a dominant flat ninth chord on note.
     Example: 
     dominant_ninth("C")
     ['C', 'E', 'G', 'Bb', 'Db']
     
     
     This is pretty flamenco
     */
    
    static Chord dominantFlatNinth(Note note){
        Chord chord = Chord::dominantNinth(note);
        chord.name = "7b9";
        chord.setRoot(note);
        Note n = Interval::minorSecond(note);
        n.changeOctave(1);
        chord.notes[4]= n;//lower ninth
        return chord;
    }
    
    

    
    /*
     Builds a dominant sharp ninth chord on note.
     Example: 
     dominant_ninth("C")
     ['C', 'E', 'G', 'Bb', 'D#']
     
     
     Hendrix right?
     */
    
    static Chord dominantSharpNinth(Note note){
        Chord chord = Chord::dominantNinth(note);
        chord.name = "7#9";
        chord.setRoot(note);
        Note n = Interval::majorSecond(note);
        n.changeOctave(1);
        n.augment();
        chord.notes[4]= n;//raise ninth
        return chord;
    }
    
    

    
    
    
/*===================================================================
	Elevenths
===================================================================*/
    
    //diatonic
    
    
    //absolute
    
    
    
    /*
     Builds an eleventh chord on note.
     Example: 
     eleventh("C")
     ['C', 'G', 'Bb', 'F']
     
     */
    
    static Chord eleventh(Note note){
        Chord chord;
        chord.name = "11";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::perfectFifth(note);
        chord.notes.push_back(n);
        n = Interval::minorSeventh(note);
        chord.notes.push_back(n);
        n = Interval::perfectFourth(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a minor eleventh chord on note.
     Example: 
     minor_eleventh("C")
     ['C', 'Eb', 'G', 'Bb', 'F']
     
     */
    
    static Chord minorEleventh(Note note){
        Chord chord = Chord::minorSeventh(note);
        chord.name = "m11";
        chord.setRoot(note);
        Note n = Interval::perfectFourth(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
/*===================================================================
	Thirteenths
===================================================================*/
    
    // absolute
    
    /*
     Builds a minor thirteenth chord on note.
     Example: 
     minor_thirteenth('C')
     ['C', 'Eb', 'G', 'Bb', 'D', 'A']
     
     */
    
    static Chord minorThirteenth(Note note){
        Chord chord = Chord::minorNinth(note);
        chord.name = "m13";
        chord.setRoot(note);
        Note n = Interval::majorSixth(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a major thirteenth chord on note.
     Example: 
     major_thirteenth('C')
     ['C', 'E', 'G', 'B', 'D', 'A']
     
     */
    
    static Chord majorThirteenth(Note note){
        Chord chord = Chord::majorNinth(note);
        chord.name = "M13";
        chord.setRoot(note);
        Note n = Interval::majorSixth(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    
    
    /*
     Builds a dominant thirteenth chord on note.
     Example: 
     dominant_thirteenth('C')
     ['C', 'E', 'G', 'Bb', 'D', 'A']
     
     */
    
    static Chord dominantThirteenth(Note note){
        Chord chord = Chord::dominantNinth(note);
        chord.name = "13";
        chord.setRoot(note);
        Note n = Interval::majorSixth(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    
    
    
    
/*===================================================================
	Suspended Chords
===================================================================*/
    
    
    //absolute
    
    
    
    
    /*
    An alias for suspended_fourth_triad
     */
    
    static Chord suspendedTriad(Note note){
        Chord chord = Chord::suspendedFourthTriad(note);
        return chord;
    }
    
    
    
    
    /*
     Builds a suspended second triad on note.
     Example: 
     suspended_second_triad("C")
     ["C", "D", "G"]
     
     */
    
    static Chord suspendedSecondTriad(Note note){
        Chord chord;
        chord.name = "sus2";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::majorSecond(note);
        chord.notes.push_back(n);
        n = Interval::perfectFifth(note);
        chord.notes.push_back(n);
        return chord;
    }
        
    
    /*
     Builds a suspended fourth triad on note.
     Example: 
     suspended_fourth_triad("C")
     ["C", "F", "G"]
     
     */
    
    static Chord suspendedFourthTriad(Note note){
        Chord chord;
        chord.name = "sus4";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::perfectFourth(note);
        chord.notes.push_back(n);
        n = Interval::perfectFifth(note);
        chord.notes.push_back(n);
        return chord;
    }
    

    
    
    /*
     Builds a suspended (flat) seventh chord on note.
     Example: 
     suspended_seventh("C")
     ["C", "F", "G", "Bb"]
     
     
     */
    
    static Chord suspendedSeventh(Note note){
        Chord chord = Chord::suspendedFourthTriad(note);
        chord.name = "sus47";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::minorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    
    /*
     Builds a suspended fourth flat ninth chord on note.
     Example: 
     suspended_ninth("C")
     ['C', 'F', 'G', 'Db']
     
     */
    
    static Chord suspendedFourthNinth(Note note){
        Chord chord = Chord::suspendedFourthTriad(note);
        chord.name = "sus4b9";
        chord.setRoot(note);
        chord.notes.push_back(note);
        Note n = Interval::minorSecond(note);
        n.changeOctave(1);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    
    
    
    
/*===================================================================
	Augmented Chords
===================================================================*/
    
    
    
    
    /*
     Builds an augmented major seventh chord on note.
     Example: 
     augmented_major_seventh("C")
     ["C", "E", "G#", "B"]
     
     */
    
    static Chord augmentedMajorSeventh(Note note){
        Chord chord = Chord::augmentedTriad(note);
        chord.name = "M7+";
        chord.setRoot(note);
        Note n = Interval::majorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    
    /*
     Builds an augmented minor seventh chord on note.
     Example: 
     augmented_minor_seventh("C")
     ["C", "E", "G#", "Bb"]
     
     */
    
    static Chord augmentedMinorSeventh(Note note){
        Chord chord = Chord::augmentedTriad(note);
        chord.name = "m7+";
        chord.setRoot(note);
        Note n = Interval::minorSeventh(note);
        chord.notes.push_back(n);
        return chord;
    }
    
        
/*===================================================================
	Various
	Altered and Special chords
===================================================================*/
    
    
    
    
    
    
    /*
     Builds a dominant flat five chord on note.
     Example: 
     dominant_flat_five("C")
     ['C', 'E', 'Gb', 'Bb']
     
     */
    
    static Chord dominantFlatFive(Note note){
        Chord chord = Chord::dominantSeventh(note);
        chord.name = "7b5";
        chord.setRoot(note);
        chord.notes[2].diminish();
        return chord;
    }
    
    
    
    
    
    
    /*
     Builds the lydian dominant seventh (7#11) on note
     Example: 
     lydian_dominant_seventh('C')
     ['C', 'E', 'G', 'Bb', 'F#']
     
     
     */
    
    static Chord lydianDominantSeventh(Note note){
        Chord chord = Chord::dominantSeventh(note);
        chord.name = "7#11";
        chord.setRoot(note);
        Note n = Interval::perfectFourth(note);
        n.augment();
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
    /*
     Builds the famous Hendrix chord (7b12)
     Example: 
     hendrix_chord('C')
     ['C', 'E', 'G', 'Bb', 'Eb']
     
     */
    
    static Chord hendrixChord(Note note){
        Chord chord = Chord::dominantSeventh(note);
        chord.name = "hendrix";
        chord.setRoot(note);
        Note n = Interval::minorThird(note);
        chord.notes.push_back(n);
        return chord;
    }
    
    
    
        
    
    
/*===================================================================
	Chords by harmonic function
===================================================================*/
    
    
    
    
    
    /*
     Returns the tonic chord in key.
     Example:
     tonic("C")
     ["C", "E", "G"]
     
     */
    
    static Chord tonic(Note key){
        return Chord::triads(key)[0];
    }
    
    
    
    
    /*
     Same as tonic(key), but returns seventh chord instead     */
    
     static Chord tonic7(Note key){
         Chord chord = Chord::sevenths(key)[0];
         return chord;
     }
    
    

    
    /*
     Returns the supertonic chord in key.
     Example:
     supertonic("C")
     ["D", "F", "A"]
     */
    
     static Chord supertonic(Note key){
         return Chord::triads(key)[1];
     }
    
    
    
        
    
    
    /*
     Same as supertonic(key), but returns seventh chord
     */
    
     static Chord supertonic7(Note key){
         Chord chord = Chord::sevenths(key)[1];
         return chord;
     }
    
    
    
    
    
    /*
     Returns the mediant chord in key.
     Example:
     mediant("C")
     ["E", "G", "B"]
     */
    
    static Chord mediant(Note key){
        return Chord::triads(key)[2];
    }
    
    
    
    /*
     Same as mediant(key), but returns seventh chord
     */
    
    static Chord mediant7(Note key){
        Chord chord = Chord::sevenths(key)[2];
        //chord.name += "7";//added above already
        
        return chord;

    }
    

    
    
    
    /*
     Returns the subdominant chord in key.
     Example:
     subdominant("C")
     ["F", "A", "C"]
     */
    
    static Chord subdominant(Note key){
        return Chord::triads(key)[3];
    }
    
    
    
    
    /*
     Same as subdominant(key), but returns seventh chord
     */
    
    static Chord subdominant7(Note key){
        Chord chord = Chord::sevenths(key)[3];
        //chord.name += "7";
        return chord;

    }
    
    
    
    /*
     Returns the dominant chord in key.
     Example:
     dominant("C")
     ["G", "B", "D"]
     */
    
    static Chord dominant(Note key){
        return Chord::triads(key)[4];
    }
    

    /*
     Same as dominant(key), but returns seventh chord
     */
    
    static Chord dominant7(Note key){
        Chord chord = Chord::sevenths(key)[4];
        //chord.name += "7";
        return chord;

    }
    
    
    
    /*
     Returns the submediant chord in key.
     Example:
     submediant("C")
     ["A", "C", "E"]
     */
    
    static Chord submediant(Note key){
        return Chord::triads(key)[5];
    }
    
        
    
    /*
     Same as submediant(key), but returns seventh chord
     */
    
    static Chord submediant7(Note key){
        Chord chord = Chord::sevenths(key)[5];
        //chord.name += "7";
        return chord;
    }
    
    

    
    
    /*
     Returns the subtonic in key.
     Example:
     subtonic("C")
     ['B', 'D', 'F']
     NOTE: The Mingus version is incorrect as far as I can tell. Subtonic is being mistaken for the leading tone.
     The subtonic is a whole step below the tonic and exists only naturally in minor keys
     */
    
    static Chord subtonic(Note key){
        Note wholeNoteDown = key.getTransposed(-2);
        Chord ch = Chord::majorTriad(wholeNoteDown);
        ch.setName("");
        return ch;
        //return Chord::triads(key)[6];
    }
    
    
    
    /*
     Same as subtonic(key), but returns seventh chord
     */
    
    static Chord subtonic7(Note key){
        
        Note wholeNoteDown = key.getTransposed(-2);
        Chord ch = Chord::dominantSeventh(wholeNoteDown);
        ch.setName("7");
        return ch;
        
        
        //Chord chord = Chord::sevenths(key)[6];
        //return chord;
    }
    
    static Chord leadingtone(Note key){
        return Chord::triads(key)[6];
    }
    
    
    
    /*
     Same as subtonic(key), but returns seventh chord
     */
    
    static Chord leadingtone7(Note key){
        Chord chord = Chord::sevenths(key)[6];
        return chord;
    }

    
   
    
/*===================================================================
	 Aliases
===================================================================*/

    
    
    static Chord I(Note key){
        return Chord::tonic(key);
    }
    
    static Chord I7(Note key){
        return Chord::tonic7(key);
    }
    
    static Chord ii(Note key){
        return Chord::supertonic(key);
    }
    
    static Chord II(Note key){
        return Chord::supertonic(key);
    }
    
    static Chord ii7(Note key){
        return Chord::supertonic7(key);
    }
    
    static Chord II7(Note key){
        return Chord::supertonic7(key);
    }
    
    static Chord iii(Note key){
        return Chord::mediant(key);
    }
    
    static Chord III(Note key){
        return Chord::mediant(key);
    }
    
    static Chord iii7(Note key){
        return Chord::mediant7(key);
    }
    
    static Chord III7(Note key){
        return Chord::mediant7(key);
    }
    //In current project there is a conflict, the IV is blue, indicating it's defined somewhere
    //I cannot find it, and I don't like defines.
    
    static Chord IV(Note key){
        return Chord::subdominant(key);
    }
    
    static Chord IV7(Note key){
        return Chord::subdominant7(key);
    }
    
    static Chord V(Note key){
        return Chord::dominant(key);
    }
    
    static Chord V7(Note key){
        return Chord::dominant7(key);
    }
    
    static Chord vi(Note key){
        return Chord::submediant(key);
    }
    
    static Chord VI(Note key){
        return Chord::submediant(key);
    }
    
    static Chord vi7(Note key){
        return Chord::submediant7(key);
    }
    
    static Chord VI7(Note key){
        return Chord::submediant7(key);
    }
    
    static Chord bvii(Note key){
        return Chord::subtonic(key);
    }
    
    static Chord bVII(Note key){
        return Chord::subtonic(key);
    }
    
    
    static Chord vii(Note key){
        return Chord::leadingtone(key);
    }
    
    static Chord VII(Note key){
        return Chord::leadingtone(key);
    }
    
    
    static Chord bvii7(Note key){
        return Chord::subtonic7(key);//was subtonic in Mingus...why?
    }
    
    static Chord bVII7(Note key){
        return Chord::subtonic7(key);
    }
    
    static Chord vii7(Note key){
        return Chord::leadingtone7(key);
    }
    
    static Chord VII7(Note key){
        return Chord::leadingtone7(key);
    }

    
    
    
    
    
/*===================================================================
 	Inversions
===================================================================*/
    
    /*
     Inverts a given chord one time
     Returns a copy
     */
    static Chord invert(Chord chord){
        Chord inv(chord);
        Chord::invert(inv.notes);
        return inv;
    }
    
    /*
     This does not return a copy
     */
    static void invert(deque<Note> &chord){
        chord.push_front(chord[chord.size()-1]);
        chord.pop_back();
    }
    
    /*
    The first inversion of a chord
    */
    
    static Chord firstInversion(Chord chord){
        return Chord::invert(chord);
    }
    
    
    
    /*
     The second inversion of a chord
     */
    static Chord secondInversion(Chord chord){    
        return Chord::invert(Chord::invert(chord));
    }
    
    /*
     The third inversion of a chord
     */
    static Chord thirdInversion(Chord chord){
        return Chord::invert(Chord::invert(Chord::invert(chord)));
    }
    
    
    
    
    
    
    
    
    
/*===================================================================
 	Other
===================================================================*/
    
    
    /*
     Takes a chord written in shorthand and returns the notes in the \
     chord. The function can recognize triads, sevenths, sixths, ninths, elevenths, \
     thirteenths, slashed chords and a number of altered chords. \
     The second argument should not be given and is only used for a recursive call \
     when a slashed chord or polychord is found. See [http://en.wikibooks.org/wiki/Music_Theory/Complete_List_of_Chord_Patterns Wikibooks] for a nice overview of chord patterns.
     Example:
     
     >>> from_shorthand("Amin")
     ["A", "C", "E"]
     >>> from_shorthand("Am/M7")
     ["F", "Ab", "C", "E"]
     >>> from_shorthand("A")
     ["A", "C#", "E"]
     >>> from_shorthand("A/G")
     ["G", "A", "C#", "E"]
     >>> from_shorthand("Dm|G")
     ["G", "B", "D", "F", "A"]
     
     
     Recognised abbreviations: the letters `m` and `M` in the following abbreviations  \
     can always be substituted by respectively `min`, `mi` or `-` and `maj` or `ma` (eg. \
     `from_shorthand("Amin7") == from_shorthand("Am7")`, etc.).
     * Triads: *'m'*, *'M'* or *''*, *'dim'*.
     * Sevenths: *'m7'*, *'M7'*, *'7'*, *'m7b5'*, *'dim7'*, *'m/M7'* or *'mM7'*
     * Augmented chords: *'aug'* or *'+'*, *'7#5'* or *'M7+5'*, *'M7+'*, *'m7+'*, *'7+'*
     * Suspended chords: *'sus4'*, *'sus2'*, *'sus47'*, *'sus'*, *'11'*, *'sus4b9'* or *'susb9'*
     * Sixths: *'6'*, *'m6'*, *'M6'*, *'6/7'* or *'67'*, *6/9* or *69*
     * Ninths: *'9'*, *'M9'*, *'m9'*, *'7b9'*, *'7#9'*
     * Elevenths: *'11'*, *'7#11'*, *'m11'*
     * Thirteenths: *'13'*, *'M13'*, *'m13'*
     * Altered chords: *'7b5'*, *'7b9'*, *'7#9'*, *'67'* or *'6/7'*
     * Special: *'5'*, *'NC'*, *'hendrix'*
     
     
     */
    //alias
    //Accepts C13
    static Chord getChordFromString(string shorthand_string){
        return Chord::fromShorthand(shorthand_string);
    }
    //accepts 13 Note("C")
    static Chord getChordFromString(string shorthand_string,Note note){
        return Chord::chordFromShorthand(shorthand_string,note);
    }
    
        
        
    //the python version of this is rubbish
    static Chord fromShorthand(string shorthand_string){
        //Shrink shorthand_string to a format recognised by chord_shorthand
        ofStringReplace(shorthand_string, "min", "m");
        ofStringReplace(shorthand_string, "mi", "m");
        ofStringReplace(shorthand_string, "-", "m");
        ofStringReplace(shorthand_string, "maj", "M");
        ofStringReplace(shorthand_string, "ma", "M");
        
        
        
        vector<string> slash = ofSplitString(shorthand_string, "/");//different bass
        vector<string> poly = ofSplitString(shorthand_string, "|");//combined chords
        
        
        string top = shorthand_string;
        
        
        if(poly.size()==2){
            top = poly[0];
        }else if(slash.size()==2){
            top = slash[0];
        }

        string name = Chord::getRootNote(top);
        Note note(name); 
        string chordSymbol = Chord::getChordSymbol(top);
        
        //this retrives the actual notes

        Chord chord = Chord::chordFromShorthand(chordSymbol,note);
        chord.name = chordSymbol;
        chord.setRoot(note);
        
        if(poly.size()==2){
            //get polychord
            string name = Chord::getRootNote(poly[1]);
            Note note(name);
            string chordSymbol = Chord::getChordSymbol(poly[1]);//was top?
            
            Chord chord2 = Chord::chordFromShorthand(chordSymbol,note);
            
            
            Chord subchord;
            
            subchord.name = chordSymbol;
            subchord.notes = chord.notes;
            subchord.setRoot(note);
            
            chord.setPolyChord(subchord);
            
            /*
            ofPtr<Chord> subchord(new Chord());
            subchord->name = chordSymbol;
            subchord->notes = chord.notes;
            subchord->setRoot(note);
            
            chord.polychords.push_back(subchord);
            */
            //no longer appending these...get all notes by calling getAllNotes
            //chord.notes.insert(chord.notes.begin(), chord2.notes.begin(),chord2.notes.end());
            
        }else if(slash.size()==2){
            //add bass from slash chord..needs to be checked for format
            Note bass(slash[1]);
            //bass.octaveDown();
            
            chord.setBass(bass);
            
            /*
            bass.octave = chord.notes[0].getOctaveDown().octave;
            bass.octaveDown();//arbitrary hack ha ha
            chord.notes.push_front(bass);
             */
        }else{
            //adding bass note here...a bit arbitrarily
           // chord.notes.push_front(chord.notes[0].getOctaveDown());
            
            chord.setBass(chord.notes[0]);
        }
        
        
        return chord;
        
         
    }
    
    
        
    /*
     Names the triad. Returns answers in a list. The third argument should \
     not be given. If shorthand is True the answers will be in abbreviated form.
     
     Can determine major, minor, diminished and suspended triads. \
     Also knows about invertions.
     
     Examples:
     {{{
     >>> determine_triad(["A", "C", "E"])
     'A minor triad'
     >>> determine_triad(["C", "E", "A"])
     'A minor triad, first inversion'
     >>> determine_triad(["A", "C", "E"], True)
     'Am'
    
     */
    
   static vector<string> determineTriad(deque<Note> triad, bool shorthand = false, bool allowInversions = true){

       //the functions required sorted list I think
       //sort(triad.begin(),triad.end(),Note::compare);
       
       vector<string> inversions;
        if (triad.size() != 3){
            ofLog()<<"warning: raise exception: not a triad";
            return inversions;
        }
        Chord::triadInversionExhauster(triad, shorthand, 1, &inversions, allowInversions);
       return inversions;
        
    }
    
    /*
     Determines the type of seventh chord. Returns the results in a \
     lists, ordered on inversions. Expects `seventh` to be a \
     list of 4 notes. If `shorthand` is set to True, results \
     will be returned in chord shorthand ('Cmin7', etc.) - inversions will be \
     dropped in that case.
     Example:
     >>> determine_seventh(['C', 'E', 'G', 'B'])
     ['C major seventh']
     */

    
    static vector<string>  determineSeventh(deque<Note> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = true){
        
        
        //the functions required sorted list I think
        //sort(chord.begin(),chord.end(),Note::compare);
        
        
        vector<string> inversions;
        
        if(chord.size() != 4){
            ofLog()<<"warning raise exception: seventh chord is not a seventh chord"<<endl;
            return inversions;
        }
        Chord::seventhInversionExhauster(chord, shorthand, 1, &inversions, allowInversions);
        
        if(allowPolychords){
            //add polychords
            Chord::polychordExhauster(chord,  shorthand,1, &inversions, allowInversions);
        }
        return inversions;
    }
    
    
    
    
    
    
    
    /*
     Determines the names of an extended chord
     */
    
    
    static vector<string>  determineExtended5Chord(deque<Note> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = true){
	
        //the functions required sorted list I think
        //sort(chord.begin(),chord.end(),Note::compare);
        vector<string> inversions;
       
        if(chord.size() != 5){
            ofLog()<<"warning raise exeption: not an extended chord"<<endl;
            return inversions;
        }
        
        Chord::chord5InversionExhauster(chord, shorthand, 1, &inversions,allowInversions);
        if(allowPolychords){
            //add polychords
            Chord::polychordExhauster(chord,  shorthand,1, &inversions, allowInversions);
        }
        
        return inversions;

    }
        
    
    
    /*
     Determines the names of an 6 note chord, eg. E13
     */

    
    static vector<string>  determineExtended6Chord(deque<Note> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = true){

        //the functions required sorted list I think
        //sort(chord.begin(),chord.end(),Note::compare);
        vector<string> inversions;
      
        if(chord.size() != 6){
            ofLog()<<"warning raise exeption: not an extended chord"<<endl;
            return inversions;
        }
        
        
        Chord::chord6InversionExhauster(chord, shorthand, 1, &inversions,allowInversions);
        if(allowPolychords){
            //add polychords
            Chord::polychordExhauster(chord,  shorthand,1, &inversions, allowInversions);
        }
        return inversions;
    }
    
        
    static vector<string>  determineExtended7Chord(deque<Note> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = true){
       
        
        
          //not sure how it likes it sorted
        //sort(chord.begin(),chord.end(),Note::compare);
        
        
        //the functions required sorted list I think
        //sort(chord.begin(),chord.end(),Note::compare);
        vector<string> inversions;
        
        
        if(chord.size() != 7){
            ofLog()<<"warning raise exeption: not an extended chord"<<endl;
            return inversions;
        }
        
        
        Chord::chord7InversionExhauster(chord, shorthand, 1, &inversions,allowInversions);
        
        if(allowPolychords){
            //add polychords
            Chord::polychordExhauster(chord,  shorthand,1, &inversions, allowInversions);
        }
        return inversions;
    }

    
    
    /*
     Determines the polychords in chord. Can handle anything from polychords based on two triads to 6 note extended chords.
     */
    
    
    static vector<string>  determinePolychords(deque<Note> chord, bool shorthand = false, bool allowInversions = true){
	    
  
        //the functions required sorted list I think
       
        
       //sort(chord.begin(),chord.end(),Note::compare);

        
        
        vector<string> inversions;
        Chord::polychordExhauster(chord,  shorthand,1, &inversions, allowInversions);
        return inversions;
    
    }
    
    
        
/*===================================================================
	Chord recognition
===================================================================*/
    
    /*
     Names a chord. Can determine almost every chord, from a simple triad to a fourteen note polychord.
     */
    
    static vector<string>  determine(deque<Note> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = false){
        //cout<<"Chord::determine"<<endl;
        vector<string> str;
	
        if (chord.size()<2){
            return str;
        }else if(chord.size()==2){
            str.push_back(Interval::determine(chord[0], chord[1]));
            return str;
        }else if(chord.size() == 3){
            return Chord::determineTriad(chord, shorthand, allowInversions);
        }else if(chord.size() == 4){
            return Chord::determineSeventh(chord, shorthand, allowInversions, allowPolychords);
        }else if(chord.size() == 5){
            return Chord::determineExtended5Chord(chord, shorthand, allowInversions, allowPolychords);
        }else if(chord.size() == 6){
            return Chord::determineExtended6Chord(chord, shorthand, allowInversions, allowPolychords);
        }else if(chord.size() == 7){
            return Chord::determineExtended7Chord(chord, shorthand, allowInversions, allowPolychords);
        }else{
            return Chord::determinePolychords(chord, shorthand,allowInversions);
        }
    }
    
    
    
    /*
     Name a chord.
     
     This function can determine almost every chord, from a simple triad to a
     fourteen note polychord.
     
     */
    
    vector<string>  determine(vector<string> chords, bool shorthand=false, bool no_inversions=false, bool no_polychords=false){
    }
    /*
    vector<string> determine(chord, bool shorthand=false, bool no_inversions=false, bool no_polychords=false){
    }
      */
    
    
    
    /*
     Alias
     */
    
    static vector<string>  analyse(deque<Note> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = false){
        return Chord::determine(chord, shorthand,allowInversions,allowPolychords);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        /*
         Return eg. Eb# from Eb#Maj7
         */
        
        static string getRootNote(string chordname){
            string root = chordname.substr(0,1);
            bool end = false;//chord name begin
            string sign;
            for(int i=1;i<chordname.size() && !end ;i++){
                sign  = chordname.substr(i,1);
                if(sign == "#" || sign == "b"){
                    root+=sign;
                }else{
                    end=true;
                }
            }
            return root;
        }
        
        /*
         Return eg. Maj7 from Eb#Maj7/D
         */
        static string getChordSymbol(string chordname){
            if(chordname.size()==0){
                return "";
            }
            //cout<<"Chord::getChordSymbol "<<chordname<<endl;
            chordname = ofSplitString(chordname, "/")[0];//if different bass
            int augs = Utils::occurenceNum(chordname,"#");
            int dims = Utils::occurenceNum(chordname,"b");
            
            if((chordname.size()-augs-dims)==1){
                return "";//just triad
            }
            string sign;
            for(int i=1;i<chordname.size();i++){
                sign  = chordname.at(i);
                if(sign != "#" && sign != "b"){
                    return chordname.substr(i);
                }
                
            }
        }
    
        /*
         This accepts chord symbols without root note, eg. aug, or dim7
         
         //TODO: Turn into function pointer lookup
         */
        
        static Chord chordFromShorthand(string c,Note note){
           
            if(c== "m"){
               return Chord::minorTriad(note);
            }else if(c== "M"){
                return Chord::majorTriad(note);
            }else if(c== ""){
                return Chord::majorTriad(note);
            }
            else if(c== "dim"){
                return Chord::diminishedTriad(note);
            }
            //Augmented chords
            else if(c== "aug" || c=="+"){
                return Chord::augmentedTriad(note);
            }
            else if(c== "7#5"  || c== "m7+5"){
                return Chord::augmentedMinorSeventh(note);
            }
            else if(c== "M7+" || c== "M7+5"|| c== "7+"){
                return Chord::augmentedMajorSeventh(note);
            }
            
            
            // Suspended chords
            
            else if(c== "sus47"){
                return Chord::suspendedSeventh(note);
            }
            else if(c== "sus4"){
                return Chord::suspendedFourthTriad(note);
            }
            else if(c== "sus2"){
                return Chord::suspendedSecondTriad(note);
            }
            else if(c== "sus"){
                return Chord::suspendedTriad(note);
            }
            else if(c== "11"){
                return Chord::eleventh(note);
            }
            else if(c== "sus4b9" || c== "susb9"){
                return Chord::suspendedFourthNinth(note);
            }

            
            //Sevenths
            else if(c== "m7"){
                return Chord::minorSeventh(note);
            }
            else if(c== "M7"){
                return Chord::majorSeventh(note);
            }
            else if(c== "7" || c=="dom7"){
                return Chord::dominantSeventh(note);
            }
            else if(c== "m7b5"){
                return Chord::minorSeventhFlatFive(note);
            }
            else if(c== "dim7"){
                return Chord::diminishedSeventh(note);
            }
            else if(c== "m/M7" || c=="mM7"){
                return Chord::minorMajorSeventh(note);
            }

            
            
            //Sixths
            else if(c== "m6"){
                return Chord::minorSixth(note);
            }
            else if(c== "M6" || c== "6"){
                return Chord::majorSixth(note);
            }
            else if(c== "6/7" || c== "67"){
                return Chord::dominantSixth(note);
            }
            else if(c== "6/9" || c== "69"){
                return Chord::sixthNith(note);
            }
            
            
            //Ninths
            else if(c== "9"){
                return Chord::dominantNinth(note);
            }
            else if(c== "7b9"){
                return Chord::dominantFlatNinth(note);
            }
            else if(c== "7#9"){
                return Chord::dominantSharpNinth(note);
            }
            else if(c== "M9"){
                return Chord::majorNinth(note);
            }
            else if(c== "m9"){
                return Chord::minorNinth(note);
            }
           
            
            //Elevenths
            else if(c== "7#11"){
                return Chord::lydianDominantSeventh(note);
            }
            else if(c== "m11"){
                return Chord::minorEleventh(note);
            }

            
            //Thirteenths
            else if(c== "M13"){
                return Chord::majorThirteenth(note);
            }
            else if(c== "m13"){
                return Chord::minorThirteenth(note);
            }
            else if(c== "13"){
                return Chord::dominantThirteenth(note);
            }
       
            
            //Altered Chords
            else if(c== "7b5"){
                return Chord::dominantFlatFive(note);
            }

            
            //Special
            else if(c== "hendrix" || c=="7b12"){
                return Chord::hendrixChord(note);
            }else{
                ofLog()<<"Warning: Chord abbr "<<c<<" not recognized"<<endl;
            }

            //"5" : (lambda x: [x, Interval.perfect_fifth(x)])
            
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
    
    void print(){
        cout <<this;
    }
    
    private:
        
    
    /*
     Recursive helper function that runs tries every inversion
     and saves the result.
     */
    
    static void triadInversionExhauster(deque<Note> triad, bool shorthand,int tries, vector<string> *result,bool allowInversions){
        
        
        
        /*
         Ok I rewrote this to use numeric interval instead of strings, because it is silly 
         that the same interval value gets different treatment depending on the note name. 
         I know many music theorists make a big fuss about it not being the same,
         and if you are one of them we can talk about it outside man to man.
         
         One of the main reasons that kind of musicology hairsplitting won't work
         here is because I want to use live midi input, and it doesn't tell you
         if a note is aug or dim.
         
         This should be rolled out across all functions below instead.
         
         
         
         string intval1 = Interval::determine(C, C#, true);//#1
         string intval2 = Interval::determine(C, Db, true);//b2
         
         int int1 = Interval::measure(C, C#);//1
         int int2 = Interval::measure(C, Db);//1

         
         
         
         1 1 = 0
         #1 b2 = 1
         2 2 = 2
         #2 b3 = 3
         3 3 = 4
         4 4 = 5
         #4 b5 = 6
         5 5 = 7
         #5 b6 = 8
         6 6 = 9
         #6 b7 = 10
         7 7 = 11 
         
         */
        
        
        
        int int1 = Interval::measure(Note(triad[0]), Note(triad[1]));
        int int2 = Interval::measure(Note(triad[0]), Note(triad[2]));

        if (int1 == 2 && int2 == 7){
            result->push_back("sus2,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 4 && int2 == 10){
            result->push_back("dom7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( int1 == 4 && int2 == 6){
            result->push_back("7b5,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( int1 == 4 && int2 == 7){
            result->push_back("M,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 4 && int2 == 8){
            result->push_back("aug,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 4 && int2 == 4){
            result->push_back("M6,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 4 && int2 == 11){
            result->push_back("M7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 3 && int2 == 6){
            result->push_back("dim,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 3 && int2 == 7){
            result->push_back("m,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 3 && int2 == 9){
            result->push_back("m6,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 3 && int2 == 10){
            result->push_back("m7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 3 && int2 == 11){
            result->push_back("m/M7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 5 && int2 == 7){
            result->push_back("sus4,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 7 && int2 == 10){
            result->push_back("m7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if(int1 == 7 && int2 == 11){
            result->push_back("M7,"+ofToString(tries)+ ","+ triad[0].name);
        }
        /*
        
        
		string intval1 = Interval::determine(Note(triad[0]), Note(triad[1]), true);
		string intval2 = Interval::determine(Note(triad[0]), Note(triad[2]), true);
        
		
        
		string intval = intval1 + intval2;
        if (intval == "25"){
            result->push_back("sus2,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "3b7"){
            result->push_back("dom7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "3b5"){
            result->push_back("7b5,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "35"){
            result->push_back("M,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "3#5"){
            result->push_back("aug,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "36"){
            result->push_back("M6,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "37"){
            result->push_back("M7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "b3b5"){
            result->push_back("dim,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "b35"){
            result->push_back("m,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "b36"){
            result->push_back("m6,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "b3b7"){
            result->push_back("m7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "b37"){
            result->push_back("m/M7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "45"){
            result->push_back("sus4,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "5b7"){
            result->push_back("m7,"+ofToString(tries)+ ","+ triad[0].name);
        }else if( intval == "57"){
            result->push_back("M7,"+ofToString(tries)+ ","+ triad[0].name);
        }
         
         */
         
        if(tries != 3 && allowInversions){
            Chord::invert(triad);
            Chord::triadInversionExhauster(triad, shorthand,tries + 1, result,allowInversions);
            return;
        }else{
            //return in right format
            int rearrange = result->size();
            for(int r=0;r<rearrange;r++){
                vector<string> split = ofSplitString((*result)[r], ",");
                if(split.size()!=3){
                    return;
                }
                if(shorthand){
                    (*result)[r] = split[2]+split[0];
                }else{
                    (*result)[r] = split[2]+Chord::getFullName(split[0])+ Chord::int_desc(split[1]);
                }
                
            }
        }
    }
    
    
    
    
    
    
    
    
    
    static void seventhInversionExhauster(deque<Note> seventh, bool shorthand,int tries, vector<string> *result,bool allowInversions){
    
    
        
    
        // Check whether the first three notes of seventh
        //are part of some triad.
        deque<Note> sub;
        sub.insert(sub.begin(),seventh.begin(),seventh.begin()+3);
        
        //not sure why no inversions here
       vector<string> triads = Chord::determineTriad(sub, true, allowInversions);
        
        
       /*
        
        major unison major unison 0 0
        augmented unison minor second 1 1
        major second major second 2 2
        augmented second minor third 3 3
        major third major third 4 4
        perfect fourth perfect fourth 5 5
        augmented fourth minor fifth 6 6
        perfect fifth perfect fifth 7 7
        augmented fifth minor sixth 8 8
        major sixth major sixth 9 9
        augmented sixth minor seventh 10 10
        major seventh major seventh 11 11
        dominant thirteenth
        */
    
 
        

            //Recognizing sevenths
        for(int i=0; i<triads.size();i++){
            //Basic triads
            //string triadStr = triads[i].substr(seventh[0].name.size());
           
            string chStr =  Chord::getChordSymbol(triads[i]);
            string root = Chord::getRootNote(triads[i]);
            
            //Get the interval between the first and last note
            string intval3 = Interval::determine(Note(root), seventh[3]);
            
            
            int int1 = Interval::measure(Note(root), seventh[3]);

            if(chStr == "m"){
                if(int1 == 10){
                    result->push_back("m7,"+ofToString(tries)+ ","+ root);
                }else if( int1 ==11){
                    result->push_back("m/M7,"+ofToString(tries)+ ","+ root);
                }else if( int1 == 9){
                    result->push_back("m6,"+ofToString(tries)+ ","+ root);
                }
            }else if( chStr == "M"){
                
                if( int1 == 11){
                    result->push_back("M7,"+ofToString(tries)+ ","+ root);
                }else if(int1 == 10){
                    result->push_back("7,"+ofToString(tries)+ ","+ root);
                }else if(int1 == 9){
                    result->push_back("M6,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "dim"){
                
                if( int1 == 10){
                    result->push_back("m7b5,"+ofToString(tries)+ ","+ root);
                }else if(intval3 == "diminished seventh"){
                    //not sure how to make this int
                    result->push_back("dim7,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "aug"){
                
                if (int1 == 10){
                    result->push_back("m7+,"+ofToString(tries)+ ","+ root);
                }
                if(int1 == 11){
                    result->push_back("M7+,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "sus4"){
                
                if(int1 == 10){
                    result->push_back("sus47,"+ofToString(tries)+ ","+ root);
                }else if(int1 == 1){
                    result->push_back("sus4b9,"+ofToString(tries)+ ","+ root);
                }
                //Other
            }else if(chStr == "m7"){
                if( int1 == 5){
                    result->push_back("11,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "7b5"){
                if( int1 == 10){
                    result->push_back("7b5,"+ofToString(tries)+ ","+root);
                }
            }
          //cout<<triads.size()<<" intval3 "<<intval3<<endl;
        
            /*
            if(chStr == "m"){
                if(intval3 == "minor seventh"){
                    result->push_back("m7,"+ofToString(tries)+ ","+ root);
                }else if( intval3 == "major seventh"){
                    result->push_back("m/M7,"+ofToString(tries)+ ","+ root);
                }else if( intval3 == "major sixth"){
                    result->push_back("m6,"+ofToString(tries)+ ","+ root);
                }
            }else if( chStr == "M"){
             
                if( intval3 == "major seventh"){
                    result->push_back("M7,"+ofToString(tries)+ ","+ root);
                }else if(intval3 == "minor seventh"){
                    result->push_back("7,"+ofToString(tries)+ ","+ root);
                }else if(intval3 == "major sixth"){
                    result->push_back("M6,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "dim"){
             
                if( intval3 == "minor seventh"){
                    result->push_back("m7b5,"+ofToString(tries)+ ","+ root);
                }else if(intval3 == "diminished seventh"){
                        result->push_back("dim7,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "aug"){
             
                if (intval3 == "minor seventh"){
                    result->push_back("m7+,"+ofToString(tries)+ ","+ root);
                }
                if(intval3 == "major seventh"){
                        result->push_back("M7+,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "sus4"){
             
                if(intval3 == "minor seventh"){
                    result->push_back("sus47,"+ofToString(tries)+ ","+ root);
                }else if(intval3 == "minor second"){
                    result->push_back("sus4b9,"+ofToString(tries)+ ","+ root);
                }
                //Other
            }else if(chStr == "m7"){
                if( intval3 == "perfect fourth"){
                    result->push_back("11,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "7b5"){
                if( intval3 == "minor seventh"){
                    result->push_back("7b5,"+ofToString(tries)+ ","+root);
                }
             }
             */
        }
    
        //Loop until we have exhausted all the inversions
        if(tries != 4 && allowInversions){
            Chord::invert(seventh);
            Chord::seventhInversionExhauster(seventh, shorthand, tries + 1,result,allowInversions);
           // cout<<"No inversion for seventh found"<<endl;
            return;
            
        }else{
            //Reset seventh
           // seventh = [seventh[3]] + seventh[0:3];
            //return in right format
            int rearrange = result->size();
            for(int r=0;r<rearrange;r++){
                vector<string> split = ofSplitString((*result)[r], ",");
                if(split.size()!=3){
                   // cout<<"No inversion for seventh found 2"<<endl;
                    return;
                }
                if(shorthand){
                    (*result)[r] = split[2]+split[0];
                }else{
                    (*result)[r] = split[2]+Chord::getFullName(split[0])+ Chord::int_desc(split[1]);
                }
                
            }
            
                                                  
    
        }
    }

    
    
    
    
    static void chord5InversionExhauster(deque<Note> chord, bool shorthand,int tries, vector<string> *result,bool allowInversions){
    
    
        deque<Note> sub;
        sub.insert(sub.begin(),chord.begin(),chord.begin()+3);
        vector<string> triads = Chord::determineTriad(sub, true, true);
        
        
        sub.push_back(chord[3]);
        vector<string> sevenths = Chord::determineSeventh(sub, true, allowInversions, false);//polychords called outside
    
        
        /*
         
         major unison major unison 0 0
         augmented unison minor second 1 1
         major second major second 2 2
         augmented second minor third 3 3
         major third major third 4 4
         perfect fourth perfect fourth 5 5
         augmented fourth minor fifth 6 6
         perfect fifth perfect fifth 7 7
         augmented fifth minor sixth 8 8
         major sixth major sixth 9 9
         augmented sixth minor seventh 10 10
         major seventh major seventh 11 11
         dominant thirteenth
         */

    
        for(int s=0;s< sevenths.size();s++){
            
            //string seventh = sevenths[s].substr(chord[0].name.size());
            
            string chStr =  Chord::getChordSymbol(sevenths[s]);
            string root = Chord::getRootNote(sevenths[s]);
            
            
            //Get the interval between the first and last note
            string intval4 = Interval::determine(Note(root), chord[4]);
            int int4 = Interval::measure(Note(root), chord[4]);
            
            if(chStr == "M7"){
                if (int4 == 2){
                    result->push_back("M9,"+ofToString(tries)+ ","+ root);
                }
            }else if( chStr == "m7"){
                if (int4 == 2){
                    result->push_back("m9,"+ofToString(tries)+ ","+ root);
                }else if(int4 == 5){
                    result->push_back("m11,"+ofToString(tries)+ ","+ root);
                }
            }else if( chStr == "7"){
                if (int4 == 2){
                    result->push_back("9,"+ofToString(tries)+ ","+ root);
                }else if( int4 == 1){
                    result->push_back("7b9,"+ofToString(tries)+ ","+ root);
                }else if( int4 == 3){
                    result->push_back("7#9,"+ofToString(tries)+ ","+ root);
                }else if( int4 == 3){
                    result->push_back("7b12,"+ofToString(tries)+ ","+ root);
                }else if( int4 == 6){
                    result->push_back("7#11,"+ofToString(tries)+ ","+ root);
                }else if( int4 == 9){
                    result->push_back("13,"+ofToString(tries)+ ","+ root);
                }
             }else if( chStr == "M6"){
                if (int4 == 2){
                    result->push_back("6/9,"+ofToString(tries)+ ","+ root);
                }else if( int4 == 11){
                    result->push_back("6/7,"+ofToString(tries)+ ","+ root);
                }
             }
        }
            
        if(tries != 5 && allowInversions){
            Chord::invert(chord);
            Chord::chord5InversionExhauster(chord, shorthand, tries + 1,result,allowInversions);
            return;
            
        }else{
            
            
            //Reset seventh
            // seventh = [seventh[3]] + seventh[0:3];
            //return in right format
            int rearrange = result->size();
            for(int r=0;r<rearrange;r++){
                vector<string> split = ofSplitString((*result)[r], ",");
                if(split.size()!=3){
                    return;
                }
                if(shorthand){
                    (*result)[r] = split[2]+split[0];
                }else{
                    (*result)[r] = split[2]+Chord::getFullName(split[0])+ Chord::int_desc(split[1]);
                }
                
            }
            
        }
    }
    
    
    
    

    
    
    
    
    
	static void chord6InversionExhauster(deque<Note> chord, bool shorthand,int tries, vector<string> *result,bool allowInversions){
        
        
        deque<Note> sub;
        sub.insert(sub.begin(),chord.begin(),chord.begin()+5);
        vector<string> ch = Chord::determineExtended5Chord(sub, true, allowInversions,false);
        

    
        for(int c=0;c<ch.size();c++){
            string ochStr = ch[c].substr(chord[0].name.size());

            string chStr =  Chord::getChordSymbol(ch[c]);
            string root = Chord::getRootNote(ch[c]);

            
            //Get the interval between the first and last note
            string intval5 = Interval::determine(Note(root), chord[5]);
            int int5 = Interval::measure(Note(root), chord[5]);
            
            
            /*
             
             major unison major unison 0 0
             augmented unison minor second 1 1
             major second major second 2 2
             augmented second minor third 3 3
             major third major third 4 4
             perfect fourth perfect fourth 5 5
             augmented fourth minor fifth 6 6
             perfect fifth perfect fifth 7 7
             augmented fifth minor sixth 8 8
             major sixth major sixth 9 9
             augmented sixth minor seventh 10 10
             major seventh major seventh 11 11
             dominant thirteenth
             */
               
            if (chStr == "9"){
                if  (int5 == 5){
                    result->push_back("11,"+ofToString(tries)+ ","+ root);
                }else if( int5 == 6){
                    result->push_back("7#11,"+ofToString(tries)+ ","+ root);
                }else if( int5 == 9){
                    result->push_back("13,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "m9"){
                if  (int5 == 5){
                    result->push_back("m11,"+ofToString(tries)+ ","+ root);
                }else if( int5 == 9){
                    result->push_back("m13,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "M9"){
                if  (int5 == 5){
                    result->push_back("M11,"+ofToString(tries)+ ","+ root);
                }else if( int5 == 9){
                    result->push_back("M13,"+ofToString(tries)+ ","+ root);
                }
            }
    }
    
        if(tries != 6 && allowInversions){
            Chord::invert(chord);
            Chord::chord6InversionExhauster(chord, shorthand, tries + 1,result,allowInversions);
            return;

        }else{
            //return in right format
            int rearrange = result->size();
            for(int r=0;r<rearrange;r++){
                vector<string> split = ofSplitString((*result)[r], ",");
                if(split.size()!=3){
                    return;
                }
                if(shorthand){
                    (*result)[r] = split[2]+split[0];
                }else{
                    (*result)[r] = split[2]+Chord::getFullName(split[0])+ Chord::int_desc(split[1]);
                }
                
            }

        }

    }

    
    
    
    
    static void chord7InversionExhauster(deque<Note> chord, bool shorthand,int tries, vector<string> *result,bool allowInversions){
        
        
        deque<Note> sub;
        sub.insert(sub.begin(),chord.begin(),chord.begin()+6);
        vector<string> ch = Chord::determineExtended6Chord(sub, true, true,false);
        
        

        
        for(int c=0;c<ch.size();c++){
            
            
            string chStr =  Chord::getChordSymbol(ch[c]);
            string root = Chord::getRootNote(ch[c]);
               
            //Get the interval between the first and last note
            string intval6 = Interval::determine(Note(root), chord[6]);
            int int6 = Interval::measure(Note(root), chord[6]);
            
            if (chStr == "11"){
                if (int6 == 9){
                    result->push_back("13,"+ofToString(tries)+ ","+ root);
                }
            }else if(chStr == "m11"){
                 //cout<<"--------m13, "<<chord[0]<<"  "<<intval6<<endl;
                    if (int6 == 9){
                        result->push_back("m13,"+ofToString(tries)+ ","+ root);
                    }
            }else if(chStr == "M11"){
                if (int6 == 9){
                    result->push_back("M13,"+ofToString(tries)+ ","+ root);
    
                }
            }
        }
        
        
        
        //surely it should be 7 here
        if(tries != 7 && allowInversions){
            Chord::invert(chord);
            Chord::chord7InversionExhauster(chord, shorthand, tries + 1,result,allowInversions);
            return;
            
        }else{
            //return in right format
            int rearrange = result->size();
            for(int r=0;r<rearrange;r++){
                vector<string> split = ofSplitString((*result)[r], ",");
                if(split.size()!=3){
                    return;
                }
                if(shorthand){
                    (*result)[r] = split[2]+split[0];
                }else{
                    (*result)[r] = split[2]+Chord::getFullName(split[0])+ Chord::int_desc(split[1]);
                }
                
                
            }
            
        }

            
    
    }
    

    
    
    static void polychordExhauster(deque<Note> chord, bool shorthand,int tries, vector<string> *result, bool allowInversions){
    
        
        
        /*
         polychords = []
         
         function_list = [
         determine_triad, determine_seventh,
         determine_extended_chord5, determine_extended_chord6,
         determine_extended_chord7
         ]
         */
        int function_nr;
        //Range tracking.
        if (chord.size() <= 3){
            ofLog()<<"No polychord with less than 3 notes"<<endl;
            return;
        }else if(chord.size() > 14){
            ofLog()<<"A polychord with more than 14 notes?! You kidding?"<<endl;
            return;
        }else if(chord.size() - 3 <= 5){
             function_nr = chord.size() - 3;
        }else{
             function_nr = 5;
        }
        for (int f=0;f<function_nr;f++){
            
            vector<string> chord1options;
            
            //build new inversion
            deque<Note> chord1;
           // cout<<chord.size() -(3 + f)<<endl;
            chord1.insert(chord1.begin(), chord.begin()+chord.size() -(3 + f),chord.end() );
            
            //explore top inversions
            Chord::subcall(chord1, shorthand, 1, &chord1options, allowInversions);
            
            
           /// cout<<"poly exploration turn "<<f*function_nr<<" found opt1 "<<chord1options.size()<<endl;
            //Chord::print(chord1);
            
           // Chord::print(chord1options);
            
            for (int f2=0;f2<function_nr;f2++){
        
        /*
         The clever part:
         Try the function_list[f] on the len(chord) - (3 + f)
         last notes of the chord. Then try the function_list[f2]
         on the f2 + 3 first notes of the chord. Thus, trying
         all possible combinations.
         
         Borg: Correction. I don't think this exhaust all possibilities at all since the order of notes makes a difference.
         
         */
        
                
                
               
                
                //
                    
                    
                    vector<string> chord2options;
                    
                    //build new inversion
                    deque<Note> chord2;
                    
                    chord2.insert(chord2.begin(), chord.begin(),chord.begin()+3 + f2);
                    
                    //explore bottom inversions
                    Chord::subcall(chord2, shorthand, 1, &chord2options, allowInversions);

                    for (int i=0;i<chord1options.size();i++){
                        for (int ii=0;ii<chord2options.size();ii++){
                            result->push_back(chord1options[i]+"|"+chord2options[ii]);
                                          
                        }
                    }
             }
        }
        
    }
    
    static void subcall(deque<Note> chord, bool shorthand,int tries, vector<string> *result, bool allowInversions){
    
        //Chord::print(chord);
        
        int func = chord.size()-3;
        
        switch(func){
            case 0:
                Chord::triadInversionExhauster(chord, shorthand, 1, result, allowInversions);
                break;
            case 1:
                Chord::seventhInversionExhauster(chord, shorthand, 1, result, allowInversions);
                break;
            case 2:
                Chord::chord5InversionExhauster(chord, shorthand, 1, result, allowInversions);
                break;
            case 3:
                Chord::chord6InversionExhauster(chord, shorthand, 1, result, allowInversions);
                break;
            case 4:
                Chord::chord7InversionExhauster(chord, shorthand, 1, result, allowInversions);
                break;
        }
      }
    /*
     Helper function that returns the inversion of the triad in a string
     */
    
    static string int_desc(string tries){
        if(tries == "1"){
            return "";
        }else if(tries == "2"){
            return ", first inversion";
        }else if( tries == "3"){
            return ", second inversion";
        }else if(tries == "4"){
            return ", third inversion";
        }else if(tries == "5"){
            return ", fourth inversion";
        }else if(tries == "6"){
            return ", fifth inversion";
        }else if(tries == "7"){
            return ", sixth inversion";
        }
        return ", inversion unknown";
    }

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //give access to private parts
    friend ostream& operator<<(ostream& os, const Chord& n);

};//class
    
   
    
    
    
    //this overloads the cout stream with useful output data
    //corresponding friend function above, note: inside class
    inline ostream& operator<<(ostream& os, Chord& c){
        if(c.notes.size()==0){
            os <<"Chord undefined"<<endl;
        }else{
            //os <<"Chord "<<c.getRoot().getDiatonicName() <<c.name<<" [ ";
            
            os <<"Chord "<<c.getName()<<" [ ";
            for(int i=0;i<c.notes.size();i++){
                os<<c.notes[i];
                if(i<c.notes.size()-1){
                    os<<", ";
                }
            }
            os<<" ]"<<endl;
            
        }
        return os;
    }
        

        
        

}//namespace
//how to output to ofLog()<<
        
       // MusicTheory::operator<<(std::ostream&, MusicTheory::Chord const&);
        
    // ofLog& ofLog::operator<< <MusicTheory::Chord>(MusicTheory::Chord const&);
#endif
