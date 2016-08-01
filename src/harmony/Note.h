/*
 *  Note.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 28/01/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Note
#define _Note

#include "ofMain.h"
#include <map>

namespace MusicTheory{
    
    
    
    
typedef struct Dynamics{
    int velocity = 100;
    int channel=1;
    int volume=1;
    
}Dynamics;

    static const  string DiatonicAugNames = "C,C#,D,D#,E,F,F#,G,G#,A,A#,B";
    static const  string DiatonicDimNames = "C,Db,D,Eb,E,F,Gb,G,Ab,A,Bb,B";
    static const  string NoteDictionary = "C,,D,,E,F,,G,,A,,B";

    
    
    
    //static const  string DiatonicAugNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    //static const  string DiatonicDimNames[] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
    
    

    
class Note : public enable_shared_from_this<Note>{
	
  public:
  
    
    string name;
    //https://forum.ableton.com/viewtopic.php?f=1&t=148636
    //midi octave ranges are not standardised. I've adjusted to Ableton which begins on -2 = 0
    //middle C = 60 = C3 on Ableton
    int octave = 3;
    Dynamics dynamics;

    
      
    
    /*
     Name can be eg. C, C-1, 12
    [[deprecated("Use Note::create(...)")]]
     */

	Note(string _name = "C",int _oct=3,Dynamics _dyn=Dynamics()){
        _name = ofToUpper(_name.substr(0,1))+_name.substr(1);
        set(_name, _oct, _dyn);
        
 
    };
    


    void set(string _name = "C",int _oct=3,Dynamics _dyn=Dynamics()){
        if (!isValidNote(name)){
            ofLog()<< "The string "<<_name<<" is not a valid representation of a note"<<endl;
            return;
        }
        
        _name = ofToUpper(_name.substr(0,1))+_name.substr(1);
        
        //extract embedded oct eg. D#6
        if(ofIsStringInString(_name, "#") && _name.size()>2){
            vector<string> div = ofSplitString(_name,"#");
            if(isdigit(div[1][0])){
                _oct = ofToInt(div[1]);
            }
        }else if(ofIsStringInString(_name, "b") && _name.size()>2){
            vector<string> div = ofSplitString(_name,"b");
            if(isdigit(div[1][0])){
                _oct = ofToInt(div[1]);
            }
        }else if(_name.size()>1){
            if(isdigit(_name[_name.size()-1])){
                _oct = ofToInt(_name.substr(1,_name.size()-1));
            }
        
        }

        name = _name;
        octave = _oct;
        dynamics = _dyn;
	}
    


    void set(int note, Dynamics _dyn=Dynamics(),bool absolute = true){
        if (!isValidNote(name)){
            ofLog()<< "The int "<<note<<" is not a valid representation of a note"<<endl;
            return;
        }
        
         string n[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        
        if(absolute){
            name = n[note % 12];
            octave = (note / 12) - 2;
        }else{
            if(note<0 || note>11){
                ofLog()<<note<<" outside note range 0-11"<<endl;
                return;
            }else{
                name = n[note];
                octave = 3;
            }
            
        }   
        
       
        
        
        
        dynamics = _dyn;
    }
    
    
    
    void empty(){
        name = "";
        octave = 0;
        dynamics = Dynamics();
    }
    /*
     
     Augments the note itself.
     
     This produces double and triple etc. augmentations on repetition, eg. A###, which is hard to read.
     Theorists claim it is the correct way of notating scales to avoid clashing with other non-altered notes
     in the same scale, eg. writing A### as C would clash with the natural C.
     
     Use getDiatonicName to output A### as C.
     */
	void augment(int i=1){
        string last;
        if(name.size()==1){
            last = name;
        }else{
            last = name.substr(name.size()-1,1);
        }
        if(last == "b"){
            //if already dim, just remove b
            name = name.substr(0,name.size()-1);
           
        }else{
            for(int ii=0;ii<i;ii++){
                name +="#";
            }
        }
	}
    
    //TODO:Why argument not used here?!
    shared_ptr<Note> getAugmented(int i=1){
        shared_ptr<Note> n = copy();
        n->augment();
        return n;
    }
    
	void diminish(int i=1){
        string last;
        if(name.size()==1){
            last = name;
        }else{
            last = name.at(name.size()-1);
        }
        
        if(last == "#"){
            name = name.substr(0,name.size()-1);
            
        }else{
            for(int ii=0;ii<i;ii++){
                name +="b";
            }
        }
 
    }
    shared_ptr<Note> getDiminished(int i=1){
        //NotePtr n = *this;
        shared_ptr<Note> n = shared_ptr<Note>(new Note(*this));
        n->diminish();
        return n;
    }
    
    /*
     Removes any accidentals on this note, eg. Bb to B
     */
    void naturalise(){
        set(this->getUnaltered(),octave,dynamics);
    }
    /*
     Returns a natural copy
     */
    shared_ptr<Note> getNatural(){
        shared_ptr<Note> n = shared_ptr<Note>(new Note(*this));
        n->naturalise();
        return n;
    }


	void changeOctave(int diff, bool limit = false){
        octave += diff;
        
        if(limit){
            octave = ofClamp(octave,-2,8);
        }
        
    }
    
    void setOctave(int oct, bool limit = false){
        //Ableton starts at -2
        octave = oct;
        
        if(limit){
            octave = ofClamp(octave,-2,8);
        }
    }
    
    int getOctave(){
        return octave;
    }
    
	void octaveUp(){
        changeOctave(1);
    }
    
    shared_ptr<Note> getOctaveUp(){
        shared_ptr<Note> n = copy();
        n->changeOctave(1);
        return n;
    }
    
	void octaveDown(){
        changeOctave(-1);
    }

    shared_ptr<Note> getOctaveDown(){
        shared_ptr<Note> n = copy();
        n->changeOctave(-1);
        return n;
    }
    
	void toMinor(){
        //name = Notes::toMinor(name);
        
        //sth = Interval.h::sixth(note[0], 'C')
       //sth = diatonic.interval(key, note, 5)
        
        //return augment_or_diminish_until_the_interval_is_right(note, sth, 9)

    }
    
    
	void toMajor(){
       // name = Notes::toMajor(name);
    }
    
	void removeRedundantAccidentals(){
       // name = Notes::removeRedundantAccidentals(name);
    }
    /*
     Transposes the note up or down the interval.
     >>> a = Note("A")
     >>> a.transpose(3)
     >>> a
     'C#-5'
     >>> a.transpose(-3)
     >>> a
     'A-4'
     
     Note: Removes accidentals (but adds # if necessary)
     */
    
	void transpose(int interval){
        int dif = toInt()+interval;        
        set(dif,dynamics);
    }
    
    
    shared_ptr<Note> getTransposed(int interval){
        shared_ptr<Note> n = copy();
        n->transpose(interval);
        return n;
    }
    /*
     Returns the number of semitones between this Note and the other.
    {{{
        >>> Note("C").measure(Note("D"))
        2
        >>> Note("D").measure(Note("C"))
        -2
    }}}
     */
    
    int measure(Note other){
        return other.toInt() - toInt();
        
        
    }
    /*
     If relative, returns value from 0-11 where C = 0.
     Else considers octave.
     */
    int  toInt(bool relative=false) const{
       // 
        
        if(!&name){
            cout<<"Warning: No name"<<endl;
            return 0;
        }
        //root note
        string root = name.substr(0,1);
        vector<string> nNames = ofSplitString(NoteDictionary,",");
        vector<string>::iterator it = find(nNames.begin(),nNames.end(),root);
        int uid = it-nNames.begin();
        
        int augs = ofStringTimesInString(name, "#");
        int dims = ofStringTimesInString(name, "b");
        
        if(relative){
            int val = uid+augs-dims;
            return val % 12;
        }else{
            return (octave+2)*12 +uid+augs-dims;
        }
        
    }
    
    int  getInt() const{
        return toInt();
        
    }
    

    //ableton
    
    
    //F5//89 = 8
    //E5//88 = 7
    //D#5//87 = 6#
    //D5//86 = 6
    //C#5//85 = 5#
    //C5//84 = 5
    //B4//83 = 4
    //A#4//82 = 3#
    //A4//81 = 3
    //G#4//80 = 2#
    //G4//79 = 2
    //(Gb4//78 = 2b)
    //F#4//78 = 1#
    //F4//77 = 1
    //E4//76 = 0
    //(Db4//75 = 0b)
    //D#4//75 = -1#
    //D4//74 = -1
    //(Db4//73 = -1b)
    //C#4//73 = -2#
    //C4//72 = -2
    
    //B3//71 = -3
    
    map<int,ofVec2f> midToClef;
    
    
    ofVec2f toTrebleClef(){
        if(!midToClef.size()){
            midToClef[11] = ofVec2f(4,0);//B
            midToClef[10] = ofVec2f(3,1);//A#
            midToClef[9] = ofVec2f(3,0);//A
            midToClef[8] = ofVec2f(2,1);//G#
            midToClef[7] = ofVec2f(2,0);//G
            midToClef[6] = ofVec2f(1,1);//F#
            midToClef[5] = ofVec2f(1,0);//F
            midToClef[4] = ofVec2f(0,0);//E
            midToClef[3] = ofVec2f(-1,1);//D#
            midToClef[2] = ofVec2f(-1,0);//D
            midToClef[1] = ofVec2f(-2,1);//C#
            midToClef[0] = ofVec2f(-2,0);//C
        }

        int mid = getInt();
        int oct = floor(mid/12);
        int trans = oct-5;//C3 is 5th oct cause it starts on -2
        int nn = mid%12;
        ofVec2f line = midToClef[nn];
        line.x += 7*trans;
        return line;
    
    }
    
    //Todo
    ofVec2f toBassClef(){
    
    }
    
    static shared_ptr<Note> fromInt(int val){
       
        int relVal = val % 12;
        int oct = floor(val/12)-2;
        
        string n[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        
        shared_ptr<Note> note = Note::create(n[relVal]);
        note->setOctave(oct);
        return note;
        
    }
    //not sure this is used...from Mingus
    string intToNote(int note){
        if(note<0 || note>11){
            ofLog()<<note<<" outside note range 0-11"<<endl;
            return "";
        }
        
        string n[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        return n[note];
        
    }
    

    
    
    /*
     Returns the Note in Hz. The `standard_pitch` argument can be used \
    to set the pitch of A-4, from which the rest is calculated.
     Not done.
     */
	float toHertz(int standardPitch = 440){
        float diff = toInt() - 57.0;
        return pow(2,(diff / 12.0)) * 440;
    }
    
    
    /*
     Sets the Note name and pitch, calculated from the `hertz` value. \
     The `standard_pitch` argument can be used to set the pitch of A-4, from \
     which the rest is calculated.*/
    
	int fromHertz(float hertz, int standardPitch = 440){
        /*value = log(float(hertz) / standardPitch, 2) * 12 + Note("A").toInt();
        name = notes.int_to_note(int(value) % 12);
        octave = int(value / 12) + 4;*/
        return 0;
    }
    
    
    /*
     
     Eg. C#-5
     
     */
    string getShorthand(){
        
        int adjustedOct = floor(getInt()/12)-2;
        string str = getDiatonicName()+"-"+ofToString(getAbsoluteOctave());
        return str;
    }
    
    /*
    getDiatonicName will return C for B#, which is in next octave from B#
    hence, should take octave from int intead
    */
    int getAbsoluteOctave(){
        int diatonicOct = floor(getInt()/12)-2;
        return diatonicOct;
    }
    
    string getName(){
        return name;
    }
    
    string getDiatonicName(){
      
        if(name.size()==1){
            return name;
        }
         //translate C### to D#
       
        //get num of accidentals
        int augs = ofStringTimesInString(name, "#");
        int dims = ofStringTimesInString(name, "b");
        
        int diff = augs-dims;
        
        
        //root note
        string root = name.substr(0,1);
        
        
        if(diff>0){
           vector<string> augNames = ofSplitString(DiatonicAugNames,",");
            vector<string>::iterator it = find(augNames.begin(),augNames.end(),root);
            int uid = it-augNames.begin();
            int skip = (diff+uid) % 12;//this returns correct id in note sequence
            return augNames[skip];
        }else if(diff<0){
            vector<string> dimNames = ofSplitString(DiatonicDimNames,",");
            vector<string>::iterator it = find(dimNames.begin(),dimNames.end(),root);
            int uid = it-dimNames.begin();
            //find root and go down the number of b's
            int skip = (uid+12+diff) % 12;//this returns correct id in note sequence
            return dimNames[skip];
        }else{
            //#b equalled out
            return root;
        }
    }
    
    
    //Some string functions
    /*
     Returns eg. ## from B##
     */
    string getAccidentals(){
        return name.substr(1,name.size()-1);
    }
    /*
     Returns natural string name eg. B from B##
     */
    string getUnaltered(){
        return name.substr(0,1);
    }
    
    /*
    Gives the traditional Helmhotz pitch notation.\
         {{{
         >>> Note("C-4").to_shorthand()
         "c'"
         >>> Note("C-3").to_shorthand()
         'c'
         >>> Note("C-2").to_shorthand()
         'C'
         >>> Note("C-1").to_shorthand()
         'C,'
         }}}"""
    */
    string toShorthand(){
        string res="";
       /* if(octave < 3){
            res = name;
        }else{
            res = str.lower(self.name)
        }
        
        int o = octave - 3;
        while (o < -1){
            res += ",";
            o += 1;
        }
        while(o > 0){
            res += "'";
         */
        return res;
    }
    
    /*
    Convert from traditional Helmhotz pitch notation.\
    {{{
    >>> Note().from_shorthand("C,,")
    'C-0'
    >>> Note().from_shorthand("C")
    'C-2'
    >>> Note().from_shorthand("c'")
    'C-4'
     }}}
     */

    void fromShorthand(string shorthand){
        /*  name = ""
        octave = 0
    for x in shorthand:
    if x in ['a', 'b', 'c', 'd', 'e', 'f', 'g']:
    name = str.upper(x)
    octave = 3
    elif x in ['A', 'B', 'C', 'D', 'E', 'F', 'G']:
    name = x
    octave = 2
    elif x in ["#", "b"]:
    name += x
    elif x == ',':
    octave -= 1
    elif x == "'":
    octave += 1
    return self.set_note(name, octave, {})*/
    }
  
    
    //Todo
    bool isValidNote(string name){
        return true;
    }
    
    static int getNoteId(deque<Note> const & list, Note const & n , bool strict=false){
        
        for(int i=0;i<list.size();i++){
            if(n.name==list[i].name){
                return i;
            }
        }
        
        return -1;
    }

    static int getNoteId(deque<shared_ptr<Note> > list, shared_ptr<Note> n , bool strict=false){
        
        for(int i=0;i<list.size();i++){
            if(n->name==list[i]->name){
                return i;
            }
        }
        
        return -1;
    }

    //factory methods
    shared_ptr<Note> copy(){
        return shared_ptr<Note>(new Note(*this));//copy
    }

    static shared_ptr<Note> create(string _name = "C",int _oct=3,Dynamics _dyn=Dynamics()){
        return shared_ptr<Note>(new Note(_name,_oct,_dyn));//new
    }
    
    //http://stackoverflow.com/questions/6039567/const-member-function
    static bool compare(const Note& a, const Note& b) {
       // cout<<a.toInt() <<" "<< b.toInt()<<endl;
        return (a.toInt() < b.toInt());
    }
    
    static bool comparePtr(const shared_ptr<Note> &a,const shared_ptr<Note> & b) {
        return (a->toInt() < b->toInt());
    }
    
    //give access to private parts
    friend ostream& operator<<(ostream& os, const Note& n);
    
    friend ostream& operator<<(ostream& os, const shared_ptr<Note> &n);
    
};//class

    
    
    
    
    
    
typedef shared_ptr<Note> NotePtr;
    
typedef NotePtr N;//short enough?
        
//this overloads the cout stream with useful output data
//corresponding friend function above, note: inside class
inline ostream& operator<<(ostream& os, Note& n){
    //name contains accidentals info getDiatonicName() doesn't
     os <<"Note "<< n.getShorthand()<<" ("<<n.name<<"-"<<n.getInt()<<")";
    //os <<"Note "<< n.getShorthand() << " (vel: " << n.dynamics.velocity<<")";
    return os;
}

inline ostream& operator<<(ostream& os, NotePtr& n){
    //name contains accidentals info getDiatonicName() doesn't
     os <<"Note "<< n->getShorthand()<<" ("<<n->name<<"-"<<n->getInt()<<")";
    //os <<"Note "<< n.getShorthand() << " (vel: " << n.dynamics.velocity<<")";
    return os;
}
    

    
}//namespace
#endif

