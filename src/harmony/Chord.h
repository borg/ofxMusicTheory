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
 
 Borg: I have made major changes to these, totally as I felt like it.
 
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
    static map<string, vector<shared_ptr<Chord>>> _triads_cache;
    
    //A cache for composed sevenths
    static map<string, vector<shared_ptr<Chord>>> _sevenths_cache;
    

  /*
   A dictionairy that can be used to present
   lookup chord abbreviations. This dictionairy is also
   used in determine_seventh()
   */
    static Lookup ChordLookup = {
        
        //Triads
		{"m"," minor triad"},
		{"M"," major triad"},
		{""," major triad"},
		{"dim"," diminished triad"},
        
        //Augmented Chord
		{"aug"," augmented triad"},
		{"+"," augmented triad"},
		{"m7#5"," augmented minor seventh"},
        {"7#5"," augmented major seventh"},
        {"7#5#9"," augmented major seventh sharp ninth"},
        {"7#9#5"," augmented major seventh sharp ninth"},
		{"M7+5"," augmented major seventh"},
		{"M7+"," augmented major seventh"},
		{"m7+"," augmented minor seventh"},
		{"7+"," augmented major seventh"},
        
        
        
        //Suspended Chord
		{"sus47"," suspended seventh"},
		{"sus4"," suspended fourth triad"},
		{"sus2"," suspended second triad"},
		{"sus"," suspended fourth triad"},
		{"11"," eleventh"},
		{"sus4b9"," suspended fourth ninth"},
		{"susb9"," suspended fourth ninth"},
        
        //Sevenths
		{"m7"," minor seventh"},
		{"M7"," major seventh"},
		{"dom7"," dominant seventh"},
		{"7"," dominant seventh"},
		{"m7b5"," half diminished seventh"},
        {"m9b5"," half diminished ninth"},
        {"m11b5"," half diminished eleventh"},
		{"dim7"," diminished seventh"},
		{"mM7"," minor/major seventh"},
		
		
        //Sixths
		{"m6"," minor sixth"},
		{"M6"," major sixth"},
		{"6"," major sixth"},
		{"6/7"," dominant sixth"},
		{"67", " dominant sixth"},
		{"6/9"," sixth ninth"},
		{"69"," sixth ninth"},
        
        //Ninths
		{"9"," dominant ninth"},
		{"7b9"," dominant flat ninth"},
		{"7#9"," dominant sharp ninth"},
		{"M9"," major ninth"},
		{"m9"," minor ninth"},
        
        //Elevenths
		{"7#11"," lydian dominant seventh"},
        {"9#11"," lydian dominant ninth"},
		{"m11"," minor eleventh"},
        {"m9/11"," minor ninth eleventh"},
        
        //Thirteenths
		{"M13"," major thirteenth"},
		{"m13"," minor thirteenth"},
        
		{"13"," dominant thirteenth"},
        
        //Altered Chord
		{"7b5"," dominant flat five"},
        {"7#9b13"," dominant sharp nine flat thirteen"},
        
		
        //Special
		{"hendrix"," hendrix chord"},
		{"7b12"," hendrix chord"},
		{"5"," perfect fifth"}
    };
    
    
    
    typedef shared_ptr<Chord> (*ChordShorthandFuncPointer)(NotePtr);
    typedef map<string,ChordShorthandFuncPointer> ChordShorthandFuncLookup;
    
    
    
class Chord : public enable_shared_from_this<Chord>{
    
  public:
    
    deque<NotePtr> notes;
    string name = "";
    NotePtr root;
    int octave = 4;

    vector< shared_ptr<Chord> > polychords;
    
    Chord(string _name = ""){
        if(_name !=""){
            set(_name);
        }
    };
    
    
    
    void set(string _name = "C"){
        shared_ptr<Chord> c = Chord::fromShorthand(_name);
        if(c->isValid()){
            name = c->name;
            notes = c->notes;
        
            NotePtr r = NotePtr(new Note(getRootNote(_name)));
            setRoot(r);
        }else{
            ofSystemAlertDialog("Chord "+_name+" not recognized");
        }
    }
    
//===================================================================
#pragma mark - FACTORY METHODS
//===================================================================
  
    
    static shared_ptr<Chord>create(string _name = ""){
        return shared_ptr<Chord>(new Chord(_name));
    }
    
    shared_ptr<Chord> copy(){
        return shared_ptr<Chord>(new Chord(*this));//copy
    }

//===================================================================
#pragma mark - INSTANCE METHODS
//===================================================================
  
    
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
    deque<NotePtr> getAllNotes(){
        deque<NotePtr> allNotes = notes;
        if(polychords.size()>0){
            allNotes.insert(allNotes.end(), polychords[0]->notes.begin(),polychords[0]->notes.end());
        }
        return allNotes;
    }
    
    

    string getName(){
        if(!isValid()){
            return "Invalid chord";
        }
        string fullName;
        if(getBass()->getName()!=getRoot()->getName()){
            //slash chord
            fullName = getRoot()->getDiatonicName() + name+"/"+getBass()->getDiatonicName();
        }else if(polychords.size()>0){
            //ignoring for the moment more than one nested polychord...
            fullName = getRoot()->getDiatonicName() + name+"/"+getBass()->getDiatonicName()+"|"+polychords[0]->getName();
        }else{
            //normal chord
            fullName= getRoot()->getDiatonicName() + name;
        }
        return fullName;
    }
    
    //Careful calling this on empty pointers
    bool isValid(){
        return notes.size()>1;
    }


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
        NotePtr n(rootStr);
        
        return n;
        }else{
            return notes[0];
        //}
    }*/
    
    NotePtr getRoot(){
        return root->copy();
    }
    
    /*
    Not sure what is the best strategy here.
    If made a copy root will linger like an orphan during transformations.
    If getRoot return a copy then you can transform it and re assign with setRoot if
    needed. Rootless chords still need a reference to the original root.
    */
    void setRoot(NotePtr n){
        //root = n->copy();
        root = n;
    }
    /*
     Returns the lowest note.
     */
    NotePtr getBass(){
        sort(notes.begin(),notes.end(),Note::comparePtr);
        if(notes.size()){
            return notes[0]->copy();
        }else{
            ofLogError()<<"Chord is missing notes"<<endl;
            return Note::create();
        }
        
    }
    
    void setBass(NotePtr bass){
        if(notes.size()){
            NotePtr n = bass->copy();//copy
            n->octave = notes[0]->getOctaveDown()->octave;
            n->octaveDown();//arbitrary hack ha ha
            notes.push_front(n);
        }else{
            ofLogError()<<"Chord is missing notes"<<endl;
            return Note::create();
        }
    }
    


    
	void changeOctave(int diff){
        octave += diff;
        if(octave < 0){
            octave = 0;
        }
        
        for(NotePtr note:notes){
            note->changeOctave(diff);
        }
    }
    
    void setOctave(int oct){
        
        //Ableton starts at -2
        oct = MAX(oct,-2);
        
        octave = oct;
        
        if(isValid()){
            int firstNoteOct = notes[0]->getOctave();
            int diff = oct - firstNoteOct;
            for(NotePtr note:notes){
                note->changeOctave(diff);
            }
        }
    }
    
    int getOctave(){
        return octave;
    }
    
	void octaveUp(){
        changeOctave(1);
    }
    
	void octaveDown(){
        changeOctave(-1);
    }


    
    void setPolyChord(shared_ptr<Chord> pc){
        
        
        shared_ptr<Chord> subchord = Chord::create();
        subchord->name = pc->getChordSymbol();
        subchord->notes = pc->notes;
        subchord->setRoot(pc->getRoot());
        
        polychords.push_back(subchord);
    }
    
    string getChordSymbol(){
        string nm = getName();
       // cout<<"Chord::getChordSymbol for "<<name<<" "<<nm<<endl;
        string str = Chord::getChordSymbol(nm);
        return str;
    }
  
    

//===================================================================
#pragma mark -		 Harmonic function
//===================================================================
   
   
   /*
   Note: isDominant will report true for isMajor as well. Best to check that first.
   
   */
    bool isMajor(){
        if(!isValid()){
            return false;
        }
       
        shared_ptr<Chord> copy = rootPosition();
        return Interval::measure(copy->notes[0], copy->notes[1]) == 4;
    }
    
    bool isMinor(){
        if(!isValid()){
            return false;
        }
       
        shared_ptr<Chord> copy = rootPosition();
        return Interval::measure(copy->notes[0], copy->notes[1]) == 3;
    }
    
    bool isDominant(){
        if(!isValid()){
            return false;
        }
        shared_ptr<Chord> copy = rootPosition();
        if(!(copy->notes.size() >= 4)){
            return false;
        }
        
        return Interval::measure(copy->notes[0], copy->notes[1]) == 4 && Interval::measure(copy->notes[0], copy->notes[3]) == 10;
    }
    
    /*
    Note: This is true for minor as well.
    */

    bool isDiminished(){
        if(!isValid()){
            return false;
        }
        shared_ptr<Chord> copy = rootPosition();
        if(!(copy->notes.size() >= 3)){
            return false;
        }
        
        return Interval::measure(copy->notes[0], copy->notes[1]) == 3 && Interval::measure(copy->notes[0], copy->notes[2]) == 6;
    }
    
    
    
//===================================================================
#pragma mark -		 Voice leading functions
//===================================================================
   
   /*
   Useful for voice leading, keeping chord progressions dense.
   Iterate through other chord notes to find notes at least distance to middle of this range.
   Returns a copy.
   */
   
    

    shared_ptr<Chord> findNearestVoicing(shared_ptr<Chord> chord){
        return Chord::findNearestVoicing(shared_from_this(),chord);
    }
    
   /*
   Useful for voice leading, keeping chord progressions dense.
   Iterate through other chord notes to find notes at least distance to this note, ie. 
   centering around this note.
   Returns a copy.
   */
   
    shared_ptr<Chord> findNearestVoicing(shared_ptr<Note> note){
        return Chord::findNearestVoicing(shared_from_this(),note);
    }
    
    /*
   Useful for voice leading & chord melodies.
   Iterate through other chord notes to find notes at least distance below this note.
   If okToOverlap will not let any note duplicate, but will pitch down an octave.
   Returns a copy.
    */
   
    shared_ptr<Chord> findNearestVoicingBelow(shared_ptr<Note> note, bool okToOverlap = true){
        return Chord::findNearestVoicingBelow(shared_from_this(),note,okToOverlap);
    }
    
   /*
   Useful for voice leading & chord melodies
   Iterate through other chord notes to find notes at least distance above this note.
   If okToOverlap will not let any note duplicate, but will pitch up an octave.
   Returns a copy.
    */
   
    shared_ptr<Chord> findNearestVoicingAbove(shared_ptr<Note> note, bool okToOverlap = true){
      return Chord::findNearestVoicingAbove(shared_from_this(),note,okToOverlap);
    }

   /*
   Move up/down octaves without changing voicing
   Returns a copy.
   */
   
    shared_ptr<Chord> findNearestTransposition(shared_ptr<Note> note){
        return Chord::findNearestTransposition(shared_from_this(),note);
    }
 
    
    
//===================================================================
#pragma mark - STATIC METHODS
//===================================================================
      
    //this is ok to check for empty pointers
    static bool isValid(shared_ptr<Chord>c){
        if(!c){
            return false;
        }else{
            return c->notes.size()>1;
        }
    }

    static bool isValidName(string str){
        string name = ChordLookup[str];
        if(name.size()>0){
            return true;
        }else{
            return false;
        }
    }
    
    static string getFullName(string str){
        string name = ChordLookup[str];//map entries without key instantiates to empty string
        if(name.size()>0){
            return name;
        }else{
            return "Chord not found";
        }

    }
    
    static vector<string> getAllKnownChords(){
        vector<string> chords;
        typedef map<string, string>::iterator it_type;
         for(it_type iterator = ChordLookup.begin(); iterator != ChordLookup.end(); iterator++) {
             chords.push_back(iterator->first);
         }
        return chords;
        
    }
    
 
    
//===================================================================
#pragma mark - Triads
//===================================================================
    
    
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
    
    static shared_ptr<Chord> triad(NotePtr note,NotePtr key){
        shared_ptr<Chord> chord = Chord::create();
        chord->notes.clear();
        chord->name = "";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::third(note, key);
        chord->notes.push_back(n);
        n = Interval::fifth(note, key);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    
    /*
     Returns all the triads in key. Implemented using a cache.
    */
    
    static vector<shared_ptr<Chord>> triads(NotePtr key){
        if(_triads_cache[key->name].size()>0){
            return Chord::copyCache(_triads_cache[key->name]);
        }
    
        deque<NotePtr> triads = Diatonic::getNotes(key);
        
        vector<shared_ptr<Chord>> chords;
        for(int i = 0;i<triads.size();i++){
            shared_ptr<Chord> chord = Chord::triad(triads[i], key);
            chords.push_back(chord);
        }
        
        
        _triads_cache[key->name] = Chord::copyCache(chords);
        return chords;
    }
     

    
    
    //absolute
    
    /*
     Builds a major triad on note.
     Example:      
     major_triad("C")
     ["C", "E", "G"]
     
     */
    
    static shared_ptr<Chord> majorTriad(NotePtr note){
        shared_ptr<Chord> chord = Chord::create();
        chord->notes.clear();
        chord->name = "M";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::majorThird(note);
        chord->notes.push_back(n);
        n = Interval::perfectFifth(note);
        chord->notes.push_back(n);
        return chord;

    }
    
    //TODO: Check where setRoot and setBass are needed
   
    /*
     Builds a minor triad on note.
     Example: 
     minor_triad("C")
     ["C", "Eb", "G"]
     
     */
    static shared_ptr<Chord> minorTriad(NotePtr note){
        shared_ptr<Chord> chord = Chord::create();
        chord->notes.clear();
        chord->name = "m";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::minorThird(note);
        chord->notes.push_back(n);
        n = Interval::perfectFifth(note);
        chord->notes.push_back(n);
        return chord;
        
    }
    
    
    
    /*
     Builds a diminished triad on note.
     Example: 
     diminished_triad("C")
     ["C", "Eb", "Gb"]
     
     */
    static shared_ptr<Chord> diminishedTriad(NotePtr note){
        shared_ptr<Chord> chord = Chord::create();
        chord->notes.clear();
        chord->name = "dim";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::minorThird(note);
        chord->notes.push_back(n);
        n = Interval::minorFifth(note);
        chord->notes.push_back(n);
        return chord;
        
    }
    
    
    
    
    
    /*
     Builds an augmented triad on note.
     Example: 
     augmented_triad("C")
     ["C", "E", "G#"]
     
     */
    static shared_ptr<Chord> augmentedTriad(NotePtr note){
        shared_ptr<Chord> chord = Chord::create();
        chord->notes.clear();
        chord->name = "aug";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::majorThird(note);
        chord->notes.push_back(n);
        n = Interval::augmentedFifth(note);
        chord->notes.push_back(n);
        return chord;
        
    }
    
    
        

    
    
    
    
//===================================================================
#pragma mark -	Sevenths
//===================================================================
    
    
    //diatonic
    
    
    
    /*
     Returns the seventh chord on note in key.
     Example: 
     seventh("C", "C")
     ["C", "E", "G", "B"]
     
     */
    
    static shared_ptr<Chord> seventh(NotePtr note,NotePtr key){
        shared_ptr<Chord> chord = Chord::triad(note,key);
        NotePtr n = Interval::seventh(note, key);
        chord->notes.push_back(n);
        chord->name = "7";
        chord->setRoot(note);
        return chord;
    }
    
    
    

    
    
    
    /*
     Returns all the sevenths in key. Implemented using a cache.
     */
    
    static vector<shared_ptr<Chord>> sevenths(NotePtr key){
        if(_sevenths_cache[key->name].size()>0){
            return Chord::copyCache(_sevenths_cache[key->name]);
        }
        
        deque<NotePtr> triads = Diatonic::getNotes(key);
        
        vector<shared_ptr<Chord>> chords;
        for(int i = 0;i<triads.size();i++){
            shared_ptr<Chord> chord = Chord::seventh(triads[i], key);
            chords.push_back(chord);
        }
        
        
        _sevenths_cache[key->name] = Chord::copyCache(chords);
        return chords;
    }
    
    
    
    static vector<shared_ptr<Chord>> copyCache(vector<shared_ptr<Chord>>cache){
        vector<shared_ptr<Chord>> _copy_cache;
        for(shared_ptr<Chord> chord:cache){
            _copy_cache.push_back(chord->copy());
        }
        return _copy_cache;
    }
    
    
    //absolute
    
    
    /*
     Builds a major seventh on note.
     Example: 
     major_seventh("C")
     ["C", "E", "G", "B"]
          */
    
    static shared_ptr<Chord> majorSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::majorTriad(note);
        chord->name = "M7";
        chord->setRoot(note);
        NotePtr n = Interval::majorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    /*
     Builds a minor seventh on note.
     Example: 
     minor_seventh("C")
     ["C", "Eb", "G", "Bb"]
     
     */
    
    static shared_ptr<Chord> minorSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::minorTriad(note);
        chord->name = "m7";
        chord->setRoot(note);
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
          
    
    /*
     Builds a dominant seventh on note.
     Example: 
     dominant_seventh("C")
     ["C", "E", "G", "Bb"]
     
     */
    
    static shared_ptr<Chord> dominantSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::majorTriad(note);
        chord->name = "7";
        chord->setRoot(note);
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a half diminished seventh (=minor seventh flat five) \
     chord on note.
     Example: 
     half_diminished_seventh("C")
     ["C", "Eb", "Gb", "Bb"]
     
     */
    
    static shared_ptr<Chord> halfDiminishedSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::diminishedTriad(note);
        chord->name = "m7b5";
        chord->setRoot(note);
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    static shared_ptr<Chord> halfDiminishedNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::diminishedTriad(note);
        chord->name = "m9b5";
        chord->setRoot(note);
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        n = Interval::majorSecond(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    //Very Bill Evans
    //https://www.freejazzlessons.com/bill-evans-jazz-chords/
    static shared_ptr<Chord> halfDiminishedEleventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::diminishedTriad(note);
        chord->name = "m11b5";
        chord->setRoot(note);
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        n = Interval::majorSecond(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        n = Interval::majorFourth(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     See half_diminished_seventh(note) for docs.
     Seems just a duplicate naming convention
    */
    
    static shared_ptr<Chord> minorSeventhFlatFive(NotePtr note){
        return halfDiminishedSeventh(note);
    }
    
    
    
    
    
    /*
     Builds a diminished seventh chord on note.
     Example: 
     diminished_seventh("C")
     ["C", "Eb", "Gb", "Bbb"]
     
     */
    
    static shared_ptr<Chord> diminishedSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::diminishedTriad(note);
        chord->name = "dim7";
        chord->setRoot(note);
        NotePtr n = Interval::minorSeventh(note);
        n->diminish();
        chord->notes.push_back(n);
        return chord;
    }
    
    
    
    
    /*
     Builds a minor major seventh chord on note.
     Example: 
     minor_major_seventh("C")
     ["C", "Eb", "G", "B"]
     
     */
    
    static shared_ptr<Chord> minorMajorSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::minorTriad(note);
        chord->name = "mM7";
        chord->setRoot(note);
        NotePtr n = Interval::majorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    
	    
    
    
    
//===================================================================
#pragma mark -	Sixths
//===================================================================
    
    
    // absolute
    
    
    /*
     Builds a minor sixth chord on note.
     Example: 
     minor_sixth("C")
     ['C', 'Eb', 'G', 'A']
     
     */
    
    static shared_ptr<Chord> minorSixth(NotePtr note){
        shared_ptr<Chord> chord = Chord::minorTriad(note);
        chord->name = "m6";
        chord->setRoot(note);
        NotePtr n = Interval::majorSixth(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    

    
    /*
     Builds a major sixth chord on note.
     Example: 
     major_sixth("C")
     ['C', 'E', 'G', 'A']
     
     */
    
    static shared_ptr<Chord> majorSixth(NotePtr note){
        shared_ptr<Chord> chord = Chord::majorTriad(note);
        chord->name = "M6";
        chord->setRoot(note);
        NotePtr n = Interval::majorSixth(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    

    
    /*
     Builds the altered chord 6/7 on note.
     Example: 
     dominant_sixth("C")
     ['C', 'E', 'G', 'A', 'Bb']
     
     */
    
    static shared_ptr<Chord> dominantSixth(NotePtr note){
        shared_ptr<Chord> chord = Chord::majorSixth(note);
        chord->name = "67";
        chord->setRoot(note);
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds the sixth/ninth chord on note.
     Example: 
     sixth_ninth('C')
     ['C', 'E', 'G', 'A', 'D']
     
     */
    
    static shared_ptr<Chord> sixthNith(NotePtr note){
        shared_ptr<Chord> chord = Chord::majorSixth(note);
        chord->name = "69";
        chord->setRoot(note);
        NotePtr n = Interval::majorSecond(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
        
    
 
    
    
//===================================================================
#pragma mark -	Ninths
//===================================================================
    
    
    
    //absolute
    
    
    /*
     Builds a minor ninth chord on note.
     Example: 
     minor_ninth("C")
     ['C', 'Eb', 'G', 'Bb', 'D']
     
     */
    
    static shared_ptr<Chord> minorNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::minorSeventh(note);
        chord->name = "m9";
        chord->setRoot(note);
        NotePtr n = Interval::majorSecond(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a major ninth chord on note.
     Example: 
     major_ninth("C")
     ['C', 'E', 'G', 'B', 'D']
     
     */
    
    static shared_ptr<Chord> majorNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::majorSeventh(note);
        chord->name = "M9";
        chord->setRoot(note);
        NotePtr n = Interval::majorSecond(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a dominant ninth chord on note.
     Example: 
     dominant_ninth("C")
     ['C', 'E', 'G', 'Bb', 'D']
     
     */
    
    static shared_ptr<Chord> dominantNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantSeventh(note);
        chord->name = "9";
        chord->setRoot(note);
        NotePtr n = Interval::majorSecond(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a dominant flat ninth chord on note.
     Example: 
     dominant_ninth("C")
     ['C', 'E', 'G', 'Bb', 'Db']
     
     
     This is pretty flamenco
     */
    
    static shared_ptr<Chord> dominantFlatNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantNinth(note);
        chord->name = "7b9";
        chord->setRoot(note);
        NotePtr n = Interval::minorSecond(note);
        n->changeOctave(1);
        chord->notes[4]= n;//lower ninth
        return chord;
    }
    
    

    
    /*
     Builds a dominant sharp ninth chord on note.
     Example: 
     dominant_ninth("C")
     ['C', 'E', 'G', 'Bb', 'D#']
     
     
     Hendrix right?
     */
    
    static shared_ptr<Chord> dominantSharpNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantNinth(note);
        chord->name = "7#9";
        chord->setRoot(note);
        NotePtr n = Interval::majorSecond(note);
        n->changeOctave(1);
        n->augment();
        chord->notes[4]= n;//raise ninth
        return chord;
    }
    
    

    
    
    
//===================================================================
#pragma mark -	Elevenths
//===================================================================
    
    //diatonic
    
    
    //absolute
    
    
    
    /*
     Builds an eleventh chord on note.
     Example: 
     eleventh("C")
     ['C', 'G', 'Bb', 'F']
     
     */
    
    static shared_ptr<Chord> eleventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::create();
        chord->notes.clear();
        chord->name = "11";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::perfectFifth(note);
        chord->notes.push_back(n);
        n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        n = Interval::perfectFourth(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a minor eleventh chord on note.
     Example: 
     minor_eleventh("C")
     ['C', 'Eb', 'G', 'Bb', 'F']
     Sometimes includes the ninth
     */
    
    static shared_ptr<Chord> minorEleventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::minorSeventh(note);
        chord->name = "m11";
        chord->setRoot(note);
        NotePtr n = Interval::perfectFourth(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    static shared_ptr<Chord> minorNinthEleventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::minorNinth(note);
        chord->name = "m9/11";
        chord->setRoot(note);
        NotePtr n = Interval::perfectFourth(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
//===================================================================
#pragma mark -	Thirteenths
//===================================================================
    
    // absolute
    
    /*
     Builds a minor thirteenth chord on note.
     Example: 
     minor_thirteenth('C')
     ['C', 'Eb', 'G', 'Bb', 'D', 'A']
     
     */
    
    static shared_ptr<Chord> minorThirteenth(NotePtr note){
        shared_ptr<Chord> chord = Chord::minorNinth(note);
        chord->name = "m13";
        chord->setRoot(note);
        NotePtr n = Interval::majorSixth(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds a major thirteenth chord on note.
     Example: 
     major_thirteenth('C')
     ['C', 'E', 'G', 'B', 'D', 'A']
     
     */
    
    static shared_ptr<Chord> majorThirteenth(NotePtr note){
        shared_ptr<Chord> chord = Chord::majorNinth(note);
        chord->name = "M13";
        chord->setRoot(note);
        NotePtr n = Interval::majorSixth(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    
    
    
    /*
     Builds a dominant thirteenth chord on note.
     Example: 
     dominant_thirteenth('C')
     ['C', 'E', 'G', 'Bb', 'D', 'A']
     
     Normally root & fifth are dropped and third possibly replaced by 11th
     
     */
    
    static shared_ptr<Chord> dominantThirteenth(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantNinth(note);
        chord->name = "13";
        chord->setRoot(note);
        NotePtr n = Interval::majorSixth(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    
    
    
    
    
//===================================================================
#pragma mark -	Suspended Chords
//===================================================================
    
    
    //absolute
    
    
    
    
    /*
    An alias for suspended_fourth_triad
     */
    
    static shared_ptr<Chord> suspendedTriad(NotePtr note){
        shared_ptr<Chord> chord = Chord::suspendedFourthTriad(note);
        return chord;
    }
    
    
    
    
    /*
     Builds a suspended second triad on note.
     Example: 
     suspended_second_triad("C")
     ["C", "D", "G"]
     
     */
    
    static shared_ptr<Chord> suspendedSecondTriad(NotePtr note){
        shared_ptr<Chord> chord = Chord::create();
        chord->notes.clear();
        chord->name = "sus2";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::majorSecond(note);
        chord->notes.push_back(n);
        n = Interval::perfectFifth(note);
        chord->notes.push_back(n);
        return chord;
    }
        
    
    /*
     Builds a suspended fourth triad on note.
     Example: 
     suspended_fourth_triad("C")
     ["C", "F", "G"]
     
     */
    
    static shared_ptr<Chord> suspendedFourthTriad(NotePtr note){
        shared_ptr<Chord> chord = Chord::create();
        chord->notes.clear();
        chord->name = "sus4";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::perfectFourth(note);
        chord->notes.push_back(n);
        n = Interval::perfectFifth(note);
        chord->notes.push_back(n);
        return chord;
    }
    

    
    
    /*
     Builds a suspended (flat) seventh chord on note.
     Example: 
     suspended_seventh("C")
     ["C", "F", "G", "Bb"]
     
     
     */
    
    static shared_ptr<Chord> suspendedSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::suspendedFourthTriad(note);
        chord->name = "sus47";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    
    
    /*
     Builds a suspended fourth flat ninth chord on note.
     Example: 
     suspended_ninth("C")
     ['C', 'F', 'G', 'Db']
     
     */
    
    static shared_ptr<Chord> suspendedFourthNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::suspendedFourthTriad(note);
        chord->name = "sus4b9";
        chord->setRoot(note);
        chord->notes.push_back(note);
        NotePtr n = Interval::minorSecond(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    
    
    
    
    
//===================================================================
#pragma mark -	Augmented Chords
//===================================================================
    
    
    
    
    /*
     Builds an augmented major seventh chord on note.
     Example: 
     augmented_major_seventh("C")
     ["C", "E", "G#", "B"]
     
     */
    
    static shared_ptr<Chord> augmentedMajorSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::augmentedTriad(note);
        chord->name = "M7+";
        chord->setRoot(note);
        NotePtr n = Interval::majorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    
    
    /*
     Builds an augmented minor seventh chord on note.
     Example: 
     augmented_minor_seventh("C")
     ["C", "E", "G#", "Bb"]
     
     */
    
    static shared_ptr<Chord> augmentedMinorSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::minorTriad(note);
        chord->name = "m7+";
        chord->setRoot(note);
        chord->notes[2]->augment();
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    
    static shared_ptr<Chord> augmentedDominantSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::augmentedTriad(note);
        chord->name = "7+";
        chord->setRoot(note);
        NotePtr n = Interval::minorSeventh(note);
        chord->notes.push_back(n);
        return chord;
    }
    
    static shared_ptr<Chord> augmentedDominantSharpNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantNinth(note);
        chord->name = "7+#9";
        chord->setRoot(note);
        chord->notes[2]->augment();
        return chord;
    }
        
//===================================================================
//	Various
#pragma mark -	Altered and Special chords
//===================================================================
    
    
    
    
    
    
    /*
     Builds a dominant flat five chord on note.
     Example: 
     dominant_flat_five("C")
     ['C', 'E', 'Gb', 'Bb']
     
     */
    
    static shared_ptr<Chord> dominantFlatFive(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantSeventh(note);
        chord->name = "7b5";
        chord->setRoot(note);
        chord->notes[2]->diminish();
        return chord;
    }
    
    static shared_ptr<Chord> dominantSharpNineFlatThirteen(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantSeventh(note);
        chord->name = "7#9b13";
        chord->setRoot(note);
        
        NotePtr n = Interval::minorThird(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        
        n = Interval::minorSixth(note);
        n->changeOctave(1);
        chord->notes.push_back(n);
        
        return chord;
    }
    
    
    static shared_ptr<Chord> powerChord(NotePtr note){
        shared_ptr<Chord> chord = Chord::create();
        chord->name = "5";
        chord->notes.clear();
        chord->setRoot(note);
        chord->notes.push_back(note);
        chord->notes.push_back(Interval::fifth(note, note));
        return chord;
    }
    
    
    
    
    
    /*
     Builds the lydian dominant seventh (7#11) on note
     Example: 
     lydian_dominant_seventh('C')
     ['C', 'E', 'G', 'Bb', 'F#']
     
     
     */
    
    static shared_ptr<Chord> lydianDominantSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantSeventh(note);
        chord->name = "7#11";
        chord->setRoot(note);
        NotePtr n = Interval::perfectFourth(note);
        n->augment();
        n->octaveUp();
        chord->notes.push_back(n);
        return chord;
    }
    
    static shared_ptr<Chord> lydianDominantNinth(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantSeventh(note);
        chord->name = "9#11";
        chord->setRoot(note);
        NotePtr n = Interval::majorSecond(note);
        n->octaveUp();
        chord->notes.push_back(n);
        n = Interval::perfectFourth(note);
        n->augment();
        n->octaveUp();
        chord->notes.push_back(n);
        return chord;
    }
    
    
    static shared_ptr<Chord> lydianMajorSeventh(NotePtr note){
        shared_ptr<Chord> chord = Chord::majorSeventh(note);
        chord->name = "M7#11";
        chord->setRoot(note);
        NotePtr n = Interval::perfectFourth(note);
        n->augment();
        chord->notes.push_back(n);
        return chord;
    }
    
    
    /*
     Builds the famous Hendrix chord (7b12)
     Example: 
     hendrix_chord('C')
     ['C', 'E', 'G', 'Bb', 'Eb']
     
     */
    
    static shared_ptr<Chord> hendrixChord(NotePtr note){
        shared_ptr<Chord> chord = Chord::dominantSeventh(note);
        chord->name = "hendrix";
        chord->setRoot(note);
        NotePtr n = Interval::minorThird(note);
        chord->notes.push_back(n);
        return chord;
    }
    
//===================================================================
#pragma mark -		Dropped notes
//===================================================================


    static shared_ptr<Chord> rootless(shared_ptr<Chord> chord){
        if(!chord->isValid()){
            return;
        }
        shared_ptr<Chord> chordCopy = chord->copy();
        chordCopy->notes.pop_front();
        return chordCopy;
    }
    
    
    static shared_ptr<Chord> noThird(shared_ptr<Chord> chord){
        if(!chord->isValid()){
            return;
        }
        shared_ptr<Chord> chordCopy = chord->copy();
        int v = chordCopy->notes.size();
        while(v--){
            if(Interval::measure(chordCopy->getRoot(),chordCopy->notes[v]) == 3 || Interval::measure(chordCopy->getRoot(),chordCopy->notes[v]) == 4){
                //removes major and minor thirds at any octave
                chordCopy->notes.erase(chordCopy->notes.begin()+v);
            }
        }
        return chordCopy;
    }
    
    
    
    static shared_ptr<Chord> noFifth(shared_ptr<Chord> chord){
        if(!chord->isValid()){
            return;
        }
        shared_ptr<Chord> chordCopy = chord->copy();
        int v = chordCopy->notes.size();
        while(v--){
            if(Interval::measure(chordCopy->getRoot(),chordCopy->notes[v]) == 7){
                //removes fifths at any octave
                chordCopy->notes.erase(chordCopy->notes.begin()+v);
            }
        }
        return chordCopy;
    }
    
//===================================================================
#pragma mark -		Chords by harmonic function
//===================================================================
    
    
    
    
    
    /*
     Returns the tonic chord in key.
     Example:
     tonic("C")
     ["C", "E", "G"]
     
     */
    
    static shared_ptr<Chord> tonic(NotePtr key){
        return Chord::triads(key)[0];
    }
    
    
    
    
    /*
     Same as tonic(key), but returns seventh chord instead     */
    
     static shared_ptr<Chord> tonic7(NotePtr key){
         shared_ptr<Chord> chord = Chord::sevenths(key)[0];
         return chord;
     }
    
    

    
    /*
     Returns the supertonic chord in key.
     Example:
     supertonic("C")
     ["D", "F", "A"]
     */
    
     static shared_ptr<Chord> supertonic(NotePtr key){
         return Chord::triads(key)[1];
     }
    
    
    
        
    
    
    /*
     Same as supertonic(key), but returns seventh chord
     */
    
     static shared_ptr<Chord> supertonic7(NotePtr key){
         shared_ptr<Chord> chord = Chord::sevenths(key)[1];
         return chord;
     }
    
    
    
    
    
    /*
     Returns the mediant chord in key.
     Example:
     mediant("C")
     ["E", "G", "B"]
     */
    
    static shared_ptr<Chord> mediant(NotePtr key){
        return Chord::triads(key)[2];
    }
    
    
    
    /*
     Same as mediant(key), but returns seventh chord
     */
    
    static shared_ptr<Chord> mediant7(NotePtr key){
        shared_ptr<Chord> chord = Chord::sevenths(key)[2];
        //chord->name += "7";//added above already
        
        return chord;

    }
    

    
    
    
    /*
     Returns the subdominant chord in key.
     Example:
     subdominant("C")
     ["F", "A", "C"]
     */
    
    static shared_ptr<Chord> subdominant(NotePtr key){
        return Chord::triads(key)[3];
    }
    
    
    
    
    /*
     Same as subdominant(key), but returns seventh chord
     */
    
    static shared_ptr<Chord> subdominant7(NotePtr key){
        shared_ptr<Chord> chord = Chord::sevenths(key)[3];
        //chord->name += "7";
        return chord;

    }
    
    
    
    /*
     Returns the dominant chord in key.
     Example:
     dominant("C")
     ["G", "B", "D"]
     */
    
    static shared_ptr<Chord> dominant(NotePtr key){
        return Chord::triads(key)[4];
    }
    

    /*
     Same as dominant(key), but returns seventh chord
     */
    
    static shared_ptr<Chord> dominant7(NotePtr key){
        shared_ptr<Chord> chord = Chord::sevenths(key)[4];
        //chord->name += "7";
        return chord;

    }
    
    
    
    /*
     Returns the submediant chord in key.
     Example:
     submediant("C")
     ["A", "C", "E"]
     */
    
    static shared_ptr<Chord> submediant(NotePtr key){
        return Chord::triads(key)[5];
    }
    
        
    
    /*
     Same as submediant(key), but returns seventh chord
     */
    
    static shared_ptr<Chord> submediant7(NotePtr key){
        shared_ptr<Chord> chord = Chord::sevenths(key)[5];
        //chord->name += "7";
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
    
    static shared_ptr<Chord> subtonic(NotePtr key){
        NotePtr wholeNoteDown = key->getTransposed(-2);
        shared_ptr<Chord> ch = Chord::majorTriad(wholeNoteDown);
        ch->setName("");
        return ch;
        //return Chord::triads(key)[6];
    }
    
    
    
    /*
     Same as subtonic(key), but returns seventh chord
     */
    
    static shared_ptr<Chord> subtonic7(NotePtr key){
        
        NotePtr wholeNoteDown = key->getTransposed(-2);
        shared_ptr<Chord> ch = Chord::dominantSeventh(wholeNoteDown);
        ch->setName("7");
        return ch;
        
        
        //Chord chord = Chord::sevenths(key)[6];
        //return chord;
    }
    
    static shared_ptr<Chord> leadingtone(NotePtr key){
        return Chord::triads(key)[6];
    }
    
    
    
    /*
     Same as subtonic(key), but returns seventh chord
     */
    
    static shared_ptr<Chord> leadingtone7(NotePtr key){
        shared_ptr<Chord> chord = Chord::sevenths(key)[6];
        return chord;
    }

    
//===================================================================
#pragma mark -		 Aliases
//===================================================================

    
    
    static shared_ptr<Chord> I(NotePtr key){
        return Chord::tonic(key);
    }
    
    /*
    https://www.reddit.com/r/musictheory/comments/1bdws7/i7_in_g_major_uses_f_or_f/
    
     this was
    static shared_ptr<Chord> I7(NotePtr key){
        return Chord::tonic7(key);
    }
    
    for I7 which is confusing in jazz. I7 should be C7 not CM7
    
    
    */
    static shared_ptr<Chord> IM7(NotePtr key){
        return Chord::majorSeventh(key);
    }
    static shared_ptr<Chord> I7(NotePtr key){
        return Chord::dominantSeventh(key);
        //Mingus
        //return Chord::tonic7(key);
    }
    
    static shared_ptr<Chord> ii(NotePtr key){
        return Chord::supertonic(key);
    }
    
    static shared_ptr<Chord> II(NotePtr key){
        return Chord::supertonic(key);
    }
    
    static shared_ptr<Chord> ii7(NotePtr key){
        return Chord::supertonic7(key);
    }
    
    static shared_ptr<Chord> II7(NotePtr key){
        NotePtr transKey = Interval::second(key, key);
        return Chord::dominantSeventh(transKey);
        //Mingus
        //return Chord::supertonic7(key);
    }
    
    static shared_ptr<Chord> iii(NotePtr key){
        return Chord::mediant(key);
    }
    
    static shared_ptr<Chord> III(NotePtr key){
        return Chord::mediant(key);
    }
    
    static shared_ptr<Chord> iii7(NotePtr key){
        NotePtr transKey = Interval::third(key, key);
        return Chord::minorSeventh(transKey);
        //Mingusreturn Chord::mediant7(key);
    }
    
    static shared_ptr<Chord> III7(NotePtr key){
        NotePtr transKey = Interval::third(key, key);
        return Chord::dominantSeventh(transKey);
        //return Chord::mediant7(key);
    }
    
    static shared_ptr<Chord> IV(NotePtr key){
        return Chord::subdominant(key);
    }
    
    static shared_ptr<Chord> IV7(NotePtr key){
        NotePtr transKey = Interval::fourth(key, key);
        return Chord::dominantSeventh(transKey);
        //return Chord::subdominant7(key);
    }
    
    static shared_ptr<Chord> V(NotePtr key){
        return Chord::dominant(key);
    }
    
    static shared_ptr<Chord> V7(NotePtr key){
        return Chord::dominant7(key);
    }
    
    static shared_ptr<Chord> vi(NotePtr key){
        return Chord::submediant(key);
    }
    
    static shared_ptr<Chord> VI(NotePtr key){
        return Chord::submediant(key);
    }
    
    static shared_ptr<Chord> vi7(NotePtr key){
        return Chord::submediant7(key);
    }
    
    static shared_ptr<Chord> VI7(NotePtr key){
        NotePtr transKey = Interval::sixth(key, key);
        return Chord::dominantSeventh(transKey);
        //return Chord::submediant7(key);
    }
    
    static shared_ptr<Chord> bvii(NotePtr key){
        return Chord::subtonic(key);
    }
    
    static shared_ptr<Chord> bVII(NotePtr key){
        return Chord::subtonic(key);
    }
    
    
    static shared_ptr<Chord> vii(NotePtr key){
        return Chord::leadingtone(key);
    }
    
    static shared_ptr<Chord> VII(NotePtr key){
        return Chord::leadingtone(key);
    }
    
    
    static shared_ptr<Chord> bvii7(NotePtr key){
        return Chord::subtonic7(key);//was subtonic in Mingus...why?
    }
    
    static shared_ptr<Chord> bVII7(NotePtr key){
        return Chord::subtonic7(key);
    }
    
    static shared_ptr<Chord> vii7(NotePtr key){
        return Chord::leadingtone7(key);
    }
    
    static shared_ptr<Chord> VII7(NotePtr key){
        NotePtr transKey = Interval::seventh(key, key);
        return Chord::dominantSeventh(transKey);
        //return Chord::leadingtone7(key);
    }

    
    
    
    
    
//===================================================================
#pragma mark -	 	Inversions
//===================================================================

    /*
    Root position
    */
    
    static shared_ptr<Chord> rootPosition(shared_ptr<Chord> chord){
        shared_ptr<Chord> newCopy = Chord::create(chord->getName());
        return newCopy;
    }
    
    shared_ptr<Chord> rootPosition(){
        shared_ptr<Chord> newCopy = Chord::create(getName());
        return newCopy;
    }
    
    /*
    This return a copy
    */
    shared_ptr<Chord> invert(){
        shared_ptr<Chord> c = copy();
        return Chord::invert(c);
    }

    /*
     Inverts a given chord one time
     Returns a copy
     */
    static shared_ptr<Chord> invert(shared_ptr<Chord> chord){
        shared_ptr<Chord> inv = chord->copy();
        Chord::invert(inv->notes);
        return inv;
    }
    
    /*
     This does not return a copy
     */
    static void invert(deque<NotePtr> &notes){
        notes.push_back(notes.front()->getOctaveUp());
        notes.pop_front();
    }
    
    /*
    The first inversion of a chord
    */
    
    static shared_ptr<Chord> firstInversion(shared_ptr<Chord> chord){
        return Chord::invert(chord);
    }
    
    
    
    /*
     The second inversion of a chord
     */
    static shared_ptr<Chord> secondInversion(shared_ptr<Chord> chord){
        return Chord::invert(Chord::invert(chord));
    }
    
    /*
     The third inversion of a chord
     */
    static shared_ptr<Chord> thirdInversion(shared_ptr<Chord> chord){
        return Chord::invert(Chord::secondInversion(chord));
    }
    
    
    /*
     The fourth inversion of a chord
     */
    static shared_ptr<Chord> fourthInversion(shared_ptr<Chord> chord){
        return Chord::invert(Chord::thirdInversion(chord));
    }
    
    
    
    /*
     The fifth inversion of a chord
     */
    static shared_ptr<Chord> fifthInversion(shared_ptr<Chord> chord){
        return Chord::invert(Chord::fourthInversion(chord));
    }
    
    
//===================================================================
#pragma mark -	 	Utils
//===================================================================
   
   /*
   Useful for voice leading, keeping chord progressions dense.
   Iterate through other chord notes to find notes at least distance to middle of this range.
   Returns a copy.
   */
   
   static shared_ptr<Chord> findNearestVoicing(shared_ptr<Chord> thisChord,shared_ptr<Chord> chordToTransform){
        if(!thisChord->isValid() || !chordToTransform->isValid()){
            return chordToTransform;
        }
        int topNote = thisChord->notes.back()->getInt();
        int bottomNote = thisChord->notes.front()->getInt();
        
        
        
        int mid = (bottomNote+topNote)/2;
        
        shared_ptr<Chord>chordCopy = chordToTransform->copy();
        deque<NotePtr>orgNotes = chordCopy->notes;
        chordCopy->notes.clear();
        
        for(NotePtr n:orgNotes){
            int currDist = n->getInt()-mid;
            if(currDist>0){
                //above
                int octDownDist = ABS(n->getOctaveDown()->getInt()-mid);
                bool cont = true;
                while(octDownDist<currDist && cont){
                    currDist = n->getInt()-mid;
                    n->octaveDown();
                    octDownDist = ABS(n->getInt()-mid);
                    if(octDownDist>=currDist){
                        //doesn't make a difference or same
                        n->octaveUp();
                        cont = false;
                   }
                }
            }else{
                //below, pitch up
                int octUpDist = ABS(n->getOctaveUp()->getInt()-mid);
                bool cont = true;
                while(octUpDist<currDist && cont){
                    currDist = n->getInt()-mid;
                    n->octaveUp();
                    octUpDist = ABS(n->getInt()-mid);
                    if(octUpDist>=currDist){
                        //doesn't make a difference or same
                        n->octaveDown();
                        cont = false;
                   }
                }
            }
            
            chordCopy->notes.push_back(n);
            
            Chord::sortNotesOnPitch(chordCopy);
            
        }
        return chordCopy;
    }
    

   /*
   Useful for voice leading, keeping chord progressions dense.
   Iterate through other chord notes to find notes at least distance to this note, ie. 
   centering around this note.
   Returns a copy.
   */
   
   static shared_ptr<Chord> findNearestVoicing(shared_ptr<Chord> chordToTransform,shared_ptr<Note> note){
        if(!chordToTransform->isValid()){
            return chordToTransform;
        }
        int mid = note->getInt();;
        
        shared_ptr<Chord>chordCopy = chordToTransform->copy();
        deque<NotePtr>orgNotes = chordCopy->notes;
        chordCopy->notes.clear();
        
        for(NotePtr n:orgNotes){
            int currDist = n->getInt()-mid;
            if(currDist>0){
                //above
                int octDownDist = ABS(n->getOctaveDown()->getInt()-mid);
                bool cont = true;
                while(octDownDist<currDist && cont){
                    currDist = n->getInt()-mid;
                    n->octaveDown();
                    octDownDist = ABS(n->getInt()-mid);
                    if(octDownDist>=currDist){
                        //doesn't make a difference or same
                        n->octaveUp();
                        cont = false;
                   }
                }
            }else{
                //below, pitch up
                int octUpDist = ABS(n->getOctaveUp()->getInt()-mid);
                bool cont = true;
                while(octUpDist<currDist && cont){
                    currDist = n->getInt()-mid;
                    n->octaveUp();
                    octUpDist = ABS(n->getInt()-mid);
                    if(octUpDist>=currDist){
                        //doesn't make a difference or same
                        n->octaveDown();
                        cont = false;
                   }
                }
            }
            
            chordCopy->notes.push_back(n);
            
            
            Chord::sortNotesOnPitch(chordCopy);
            
        }
        return chordCopy;
    }
    
 
   /*
   Useful for voice leading & chord melodies.
   Iterate through other chord notes to find notes at least distance below this note.
   If okToOverlap will not let any note duplicate, but will pitch down an octave.
   Returns a copy.
    */
   
   static shared_ptr<Chord> findNearestVoicingBelow(shared_ptr<Chord> chordToTransform,shared_ptr<Note> note, bool okToOverlap = true){
         if(!chordToTransform->isValid()){
            return chordToTransform;
        }
        int mid = note->getInt();;
        
        shared_ptr<Chord>chordCopy = chordToTransform->copy();
        deque<NotePtr>orgNotes = chordCopy->notes;
        chordCopy->notes.clear();
        
        for(NotePtr n:orgNotes){
            int currDist = n->getInt()-mid;
            if(currDist>0){
                //above
                while(n->getOctaveDown()->getInt()>=mid){
                    n->octaveDown();
                    
                    if(n->getInt() == mid && !okToOverlap){
                        n->octaveDown();
                   }
                }
            }else if(currDist<0){
                //below, pitch up
                while(n->getOctaveUp()->getInt()<=mid){
                    n->octaveUp();
                    if(n->getInt() == mid && !okToOverlap){
                        n->octaveDown();
                   }
                }
            }else if(!okToOverlap){
                //same note
                n->octaveDown();
            }
            
            chordCopy->notes.push_back(n);
            

            Chord::sortNotesOnPitch(chordCopy);
            
        }
        return chordCopy;
    }
    


   /*
   Useful for voice leading & chord melodies
   Iterate through other chord notes to find notes at least distance above this note.
   If okToOverlap will not let any note duplicate, but will pitch up an octave.
   Returns a copy.
    */
   
   static shared_ptr<Chord> findNearestVoicingAbove(shared_ptr<Chord> chordToTransform,shared_ptr<Note> note, bool okToOverlap = true){
        if(!chordToTransform->isValid()){
            return chordToTransform;
        }
        int mid = note->getInt();
        
        shared_ptr<Chord>chordCopy = chordToTransform->copy();
        deque<NotePtr>orgNotes = chordCopy->notes;
        chordCopy->notes.clear();
        
        for(NotePtr n:orgNotes){
            int currDist = n->getInt()-mid;
            if(currDist>0){
                //above
                while(n->getOctaveDown()->getInt()>=mid){
                    n->octaveDown();
                    
                    if(n->getInt() == mid && !okToOverlap){
                        n->octaveUp();
                   }
                }
            }else if(currDist<0){
                //below, pitch up
                while(n->getOctaveDown()->getInt()<=mid){
                    n->octaveUp();
                    if(n->getInt() == mid && !okToOverlap){
                        n->octaveUp();
                   }
                }
            }else if(!okToOverlap){
                //same note
                n->octaveUp();
            }
            
            chordCopy->notes.push_back(n);
            
            Chord::sortNotesOnPitch(chordCopy);
        }
        return chordCopy;
    }
    

   /*
   Move up/down octaves without changing voicing
   Returns a copy.
   */
   
   static shared_ptr<Chord> findNearestTransposition(shared_ptr<Chord> chordToTransform,shared_ptr<Note> note){
        if(!chordToTransform->isValid()){
            return chordToTransform;
        }
        
        int centre = note->getInt();;
        
        shared_ptr<Chord>chordCopy = chordToTransform->copy();
        
        int mid = (chordCopy->notes.front()->getInt()+chordCopy->notes.back()->getInt())/2;
        
        
        int currDist = mid-centre;
        if(currDist>0){
            //above
            int octDownDist = ABS((mid-12)-centre);
            bool cont = true;
            while(octDownDist<currDist && cont){
                currDist = mid-centre;
                chordCopy->octaveDown();
                mid = (chordCopy->notes.front()->getInt()+chordCopy->notes.back()->getInt())/2;
                octDownDist = ABS(mid-centre);
                if(octDownDist>=currDist){
                    //doesn't make a difference or same
                    chordCopy->octaveUp();
                    cont = false;
                }
            }
        }else{
            //below, pitch up
            int octUpDist = ABS((mid+12)-centre);
            bool cont = true;
            while(octUpDist<currDist && cont){
                currDist = mid-centre;
                chordCopy->octaveUp();
                mid = (chordCopy->notes.front()->getInt()+chordCopy->notes.back()->getInt())/2;
                octUpDist = ABS(mid-centre);
                if(octUpDist>=currDist){
                    //doesn't make a difference or same
                    chordCopy->octaveDown();
                    cont = false;
               }
            }
        }
        
        
        return chordCopy;
    }
    

    static void sortNotesOnPitch(shared_ptr<Chord>chord){
        sort(chord->notes.begin(),chord->notes.end(),Note::comparePtr);
    }



    
//===================================================================
#pragma mark -	 	Other
//===================================================================
    
    
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
    static shared_ptr<Chord> getChordFromString(string shorthand_string){
        return Chord::fromShorthand(shorthand_string);
    }
    //accepts 13 Note("C")
    static shared_ptr<Chord> getChordFromString(string shorthand_string,NotePtr note){
        return Chord::chordFromShorthand(shorthand_string,note);
    }
    
        
        
    //the python version of this is rubbish
    static shared_ptr<Chord> fromShorthand(string shorthand_string){
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
        NotePtr note = Note::create(name);
        string chordSymbol = Chord::getChordSymbol(top);
        
        //this retrives the actual notes

        shared_ptr<Chord> chord = Chord::chordFromShorthand(chordSymbol,note);
        //cout<<"::fromShorthand chordSymbol "<<chordSymbol<<" "<<chord->getName()<<endl;
        //Chord::print(chord->notes);
        chord->name = chordSymbol;
        chord->setRoot(note);
        
        if(poly.size()==2){
            //get polychord
            string name = Chord::getRootNote(poly[1]);
            NotePtr note = NotePtr(new Note(name));
            string chordSymbol = Chord::getChordSymbol(poly[1]);//was top?
            shared_ptr<Chord> chord2 = Chord::chordFromShorthand(chordSymbol,note);
            shared_ptr<Chord> subchord = Chord::create();
            
            subchord->name = chordSymbol;
            subchord->notes = chord->notes;
            subchord->setRoot(note);
            chord->setPolyChord(subchord);
            
            /*
            ofPtr<Chord> subchord(new Chord());
            subchord->name = chordSymbol;
            subchord->notes = chord->notes;
            subchord->setRoot(note);
            
            chord.polychords.push_back(subchord);
            */
            //no longer appending these...get all notes by calling getAllNotes
            //chord->notes.insert(chord->notes.begin(), chord2.notes.begin(),chord2.notes.end());
            
        }else if(slash.size()==2){
            //add bass from slash chord..needs to be checked for format
            NotePtr bass = NotePtr(new Note(slash[1]));
            
            chord->setBass(bass);
            
            /*
            bass.octave = chord->notes[0].getOctaveDown().octave;
            bass.octaveDown();//arbitrary hack ha ha
            chord->notes.push_front(bass);
             */
        }else if(chord->notes.size()){
            //adding bass note here...a bit arbitrarily
           // chord->notes.push_front(chord->notes[0].getOctaveDown());
            
            //chord->setBass(chord->notes[0]);
        }
        
        
        return chord;
        
         
    }
    
    static vector<string> determineDiad(deque<NotePtr> diad, bool shorthand = false, bool allowInversions = true){
       vector<string> inversions;
        if (diad.size() != 2){
            ofLog()<<"warning: raise exception: not a diad";
            return inversions;
        }
        
        string intervalName = Interval::determine(diad[0], diad[1],shorthand);
        
        inversions.push_back(diad[0]->getName()+ intervalName);
        
        if(allowInversions){
            intervalName = Interval::determine(diad[1], diad[0],shorthand);
            inversions.push_back(diad[1]->getName()+ intervalName);
        }
        return inversions;
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
    
   static vector<string> determineTriad(deque<NotePtr> triad, bool shorthand = false, bool allowInversions = true){

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

    
    static vector<string>  determineSeventh(deque<NotePtr> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = true){
        
        
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
    
    
    static vector<string>  determineExtended5Chord(deque<NotePtr> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = true){
	
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

    
    static vector<string>  determineExtended6Chord(deque<NotePtr> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = true){

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
    
        
    static vector<string>  determineExtended7Chord(deque<NotePtr> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = true){
       
        
        
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
    
    
    static vector<string>  determinePolychords(deque<NotePtr> chord, bool shorthand = false, bool allowInversions = true){
	    
  
        //the functions required sorted list I think
       
        
       //sort(chord.begin(),chord.end(),Note::compare);

        
        
        vector<string> inversions;
        Chord::polychordExhauster(chord,  shorthand,1, &inversions, allowInversions);
        return inversions;
    
    }
    
    
        
//===================================================================
#pragma mark -		Chord recognition
//===================================================================
    
    /*
     Names a chord. Can determine almost every chord, from a simple triad to a fourteen note polychord.
     */
    
    static vector<string>  determine(deque<NotePtr> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = false){
        //cout<<"Chord::determine"<<endl;
        vector<string> str;
	
        if (chord.size()<2){
            return str;
        }else if(chord.size()==2){
            return Chord::determineDiad(chord, shorthand, allowInversions);
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
    
    static vector<string>  analyse(deque<NotePtr> chord, bool shorthand = false, bool allowInversions = true, bool allowPolychords = false){
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
        int augs = ofStringTimesInString(chordname, "#");
        int dims = ofStringTimesInString(chordname, "b");
        
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
    
//===================================================================
#pragma mark -		Function hash map
//===================================================================
        /*
         This accepts chord symbols without root note, eg. aug, or dim7
         */
        
        static shared_ptr<Chord> chordFromShorthand(string c,NotePtr note){
            static ChordShorthandFuncLookup  _chordFuncLookup = {
            {"m",&Chord::minorTriad},
            {"M",&Chord::majorTriad},
            {"",&Chord::majorTriad},
            {"dim",&Chord::diminishedTriad},
            
            //Augmented chords
            {"aug",&Chord::augmentedTriad},
            {"+",&Chord::augmentedTriad},
            
            {"m7+5",&Chord::augmentedMinorSeventh},//twin peaks
            {"m7#5",&Chord::augmentedMinorSeventh},
            
            {"M7+",&Chord::augmentedMajorSeventh},
            {"M7+5",&Chord::augmentedMajorSeventh},
            
            {"7+",&Chord::augmentedDominantSeventh},
            {"7#5",&Chord::augmentedDominantSeventh},
            
            {"7#5#9",&Chord::augmentedDominantSharpNinth},
            {"7+#9",&Chord::augmentedDominantSharpNinth},
            {"7#9#5",&Chord::augmentedDominantSharpNinth},
       
        
        
            
            // Suspended chords
            
            {"sus47",&Chord::suspendedSeventh},
            {"sus4",&Chord::suspendedFourthTriad},
            {"sus2",&Chord::suspendedSecondTriad},
            {"sus",&Chord::suspendedTriad},
            {"11",&Chord::eleventh},
            {"sus4b9",&Chord::suspendedFourthNinth},
            {"susb9",&Chord::suspendedFourthNinth},

            
            //Sevenths
            {"m7",&Chord::minorSeventh},
            {"M7",&Chord::majorSeventh},
            {"Maj7",&Chord::majorSeventh},
            {"7",&Chord::dominantSeventh},
            {"dom7",Chord::dominantSeventh},
            {"m7b5",&Chord::minorSeventhFlatFive},
            {"dim7",&Chord::diminishedSeventh},
            {"m/M7",&Chord::minorMajorSeventh},
            {"mM7",Chord::minorMajorSeventh},

            
            
            //Sixths
            {"m6",&Chord::minorSixth},
            {"M6",&Chord::majorSixth},
            {"6",&Chord::majorSixth},
            {"Maj6",&Chord::majorSixth},
            {"6/7",&Chord::dominantSixth},
            {"67",&Chord::dominantSixth},
            {"6/9",&Chord::sixthNith},
            {"69",&Chord::sixthNith},
            
            
            
            //Ninths
            {"9",&Chord::dominantNinth},
            {"7b9",&Chord::dominantFlatNinth},
            {"7#9",&Chord::dominantSharpNinth},
            {"M9",&Chord::majorNinth},
            {"Maj9",&Chord::majorNinth},
            {"m9",&Chord::minorNinth},
            {"m9b5",&Chord::halfDiminishedNinth},
           
            
            //Elevenths
            {"7#11",&Chord::lydianDominantSeventh},//mixolydian raised 4
            {"9#11",&Chord::lydianDominantNinth},
            {"m11",&Chord::minorEleventh},
            {"m11b5",&Chord::halfDiminishedEleventh},
            {"m9/11",&Chord::minorNinthEleventh},
            {"M7#11",&Chord::lydianMajorSeventh},


            
            //Thirteenths
            {"M13",&Chord::majorThirteenth},
            {"Maj13",&Chord::majorThirteenth},
            {"m13",&Chord::minorThirteenth},
            {"13",&Chord::dominantThirteenth},
            
            //Altered Chords
            {"7b5",&Chord::dominantFlatFive},
            {"7#9b13",&Chord::dominantSharpNineFlatThirteen},
            
            {"5",&Chord::powerChord},
            
            //Special
            {"hendrix",&Chord::hendrixChord},
            {"7b12",&Chord::hendrixChord}
            
            };
            
            
            
            if(_chordFuncLookup[c]){
                return _chordFuncLookup[c](note);
            }else{
                ofLog()<<"Warning: Chord abbr "<<note->getName()<<c<<" not recognized"<<endl;
                return Chord::create();
            }

//
                //ofSystemAlertDialog("Chord::chordFromShorthand: Chord "+note->getName()+c+" not recognized");
            
    }
    
//===================================================================
#pragma mark -		Convenience
//===================================================================

    
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
    
    void print(){
        cout <<this;
    }
    

//===================================================================
#pragma mark -		PRIVATE METHODS
//===================================================================
    
    private:
        
    
    /*
     Recursive helper function that runs tries every inversion
     and saves the result.
     */
    
    static void triadInversionExhauster(deque<NotePtr> triad, bool shorthand,int tries, vector<string> *result,bool allowInversions){
        
        
        
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
        
        
        
        int int1 = Interval::measure(triad[0], triad[1]);
        int int2 = Interval::measure(triad[0], triad[2]);

        if (int1 == 2 && int2 == 7){
            result->push_back("sus2,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 4 && int2 == 10){
            result->push_back("dom7,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if( int1 == 4 && int2 == 6){
            result->push_back("7b5,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if( int1 == 4 && int2 == 7){
            result->push_back("M,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 4 && int2 == 8){
            result->push_back("aug,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 4 && int2 == 4){
            result->push_back("M6,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 4 && int2 == 11){
            result->push_back("M7,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 3 && int2 == 6){
            result->push_back("dim,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 3 && int2 == 7){
            result->push_back("m,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 3 && int2 == 9){
            result->push_back("m6,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 3 && int2 == 10){
            result->push_back("m7,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 3 && int2 == 11){
            result->push_back("m/M7,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 5 && int2 == 7){
            result->push_back("sus4,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 7 && int2 == 10){
            result->push_back("m7,"+ofToString(tries)+ ","+ triad[0]->name);
        }else if(int1 == 7 && int2 == 11){
            result->push_back("M7,"+ofToString(tries)+ ","+ triad[0]->name);
        }
 
        
        
         
        if(tries != 3 && allowInversions){
            Chord::invert(triad);
            Chord::triadInversionExhauster(triad, shorthand,tries + 1, result,allowInversions);
        } else{
        
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
    
    
    
    
    
    
    
    
    
    static void seventhInversionExhauster(deque<NotePtr> seventh, bool shorthand,int tries, vector<string> *result,bool allowInversions){
        // Check whether the first three notes of seventh
        //are part of some triad.
        deque<NotePtr> sub;
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
            string intval3 = Interval::determine(Note::create(root), seventh[3]);
            
            
            int int1 = Interval::measure(Note::create(root), seventh[3]);

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
                }else if(intval3 == "diminished seventh" || intval3 == "major sixth"){
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

    
    
    
    
    static void chord5InversionExhauster(deque<NotePtr> chord, bool shorthand,int tries, vector<string> *result,bool allowInversions){
    
    
        deque<NotePtr> sub;
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
            string intval4 = Interval::determine(Note::create(root), chord[4]);
            int int4 = Interval::measure(Note::create(root), chord[4]);
            
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
    
    
    
    
	static void chord6InversionExhauster(deque<NotePtr> chord, bool shorthand,int tries, vector<string> *result,bool allowInversions){
        
        
        deque<NotePtr> sub;
        sub.insert(sub.begin(),chord.begin(),chord.begin()+5);
        vector<string> ch = Chord::determineExtended5Chord(sub, true, allowInversions,false);
        

    
        for(int c=0;c<ch.size();c++){
            string ochStr = ch[c].substr(chord[0]->name.size());

            string chStr =  Chord::getChordSymbol(ch[c]);
            string root = Chord::getRootNote(ch[c]);

            
            //Get the interval between the first and last note
            string intval5 = Interval::determine(Note::create(root), chord[5]);
            int int5 = Interval::measure(Note::create(root), chord[5]);
            
            
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

    
    
    
    
    static void chord7InversionExhauster(deque<NotePtr> chord, bool shorthand,int tries, vector<string> *result,bool allowInversions){
        
        
        deque<NotePtr> sub;
        sub.insert(sub.begin(),chord.begin(),chord.begin()+6);
        vector<string> ch = Chord::determineExtended6Chord(sub, true, true,false);
        
        

        
        for(int c=0;c<ch.size();c++){
            
            
            string chStr =  Chord::getChordSymbol(ch[c]);
            string root = Chord::getRootNote(ch[c]);
               
            //Get the interval between the first and last note
            string intval6 = Interval::determine(Note::create(root), chord[6]);
            int int6 = Interval::measure(Note::create(root), chord[6]);
            
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
    

    
    
    static void polychordExhauster(deque<NotePtr> chord, bool shorthand,int tries, vector<string> *result, bool allowInversions){
    
        
        
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
            deque<NotePtr> chord1;
           // cout<<chord.size() -(3 + f)<<endl;
            chord1.insert(chord1.begin(), chord.begin()+chord.size() -(3 + f),chord.end() );
            
            //explore top inversions
            Chord::subcall(chord1, shorthand, 1, &chord1options, allowInversions);
            
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
                    deque<NotePtr> chord2;
                    
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
    
    static void subcall(deque<NotePtr> chord, bool shorthand,int tries, vector<string> *result, bool allowInversions){
    
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
    friend ostream& operator<<(ostream& os, const shared_ptr<Chord>& n);
};//class
    
   
    
    
    
    //this overloads the cout stream with useful output data
    //corresponding friend function above, note: inside class
    inline ostream& operator<<(ostream& os, Chord& c){
        if(c.notes.size()==0){
            os <<"Chord invalid"<<endl;
        }else{
            //os <<"Chord "<<c.getRoot()->getDiatonicName() <<c.name<<" [ ";
            
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
    
    
    inline ostream& operator<<(ostream& os, shared_ptr<Chord>& c){
        if(!Chord::isValid(c)){
            os <<"Chord invalid"<<endl;
        }else{
            //os <<"Chord "<<c.getRoot()->getDiatonicName() <<c.name<<" [ ";
            
            os <<"Chord "<<c->getName()<<" [ ";
            for(int i=0;i<c->notes.size();i++){
                os<<c->notes[i];
                if(i<c->notes.size()-1){
                    os<<", ";
                }
            }
            os<<" ]"<<endl;
            
        }
        return os;
    }

typedef shared_ptr<Chord> ChordPtr;
        

}//namespace
//how to output to ofLog()<<
        
       // MusicTheory::operator<<(std::ostream&, MusicTheory::Chord const&);
        
    // ofLog& ofLog::operator<< <MusicTheory::Chord>(MusicTheory::Chord const&);
#endif
