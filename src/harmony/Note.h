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
#include "Utils.h"
#include "Constants.h"
#include "NoteTime.h"
#include <map>

namespace MusicTheory{
    
    
typedef struct Dynamics{
    int velocity;
    int channel;
    int volume;
    
}Dynamics;

    static const  string DiatonicAugNames = "C,C#,D,D#,E,F,F#,G,G#,A,A#,B";
    static const  string DiatonicDimNames = "C,Db,D,Eb,E,F,Gb,G,Ab,A,Bb,B";
    static const  string NoteDictionary = "C,,D,,E,F,,G,,A,,B";

    
    
    
    //static const  string DiatonicAugNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    //static const  string DiatonicDimNames[] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
    
    

    
class Note {
	
  public:
   
    static const float WHOLE = NOTE_1;
    static const float HALF = NOTE_2;
    static const float FOURTH = NOTE_4;
    static const float FOURTH_DOTTED = NOTE_4_DOT;
    static const float FOURTH_TRIPLET = NOTE_4_3;
    static const float FOURTH_QUINTUPLET = NOTE_4_5;
    static const float FOURTH_SEPTUPLET = NOTE_4_7;
    
    static const float EIGHT = NOTE_8;
    static const float EIGHT_DOTTED = NOTE_8_DOT;
    //a triplet eighth note is a third of a quarter note.
    static const float EIGHT_TRIPLET = NOTE_8_3;
    static const float EIGHT_QUINTUPLET = NOTE_8_5;
    static const float EIGHT_SEPTUPLET = NOTE_8_7;
    
    
    
    static const float SIXTEENTH = NOTE_16;
    static const float SIXTEENTH_DOTTED = NOTE_16_DOT;
    static const float SIXTEENTH_TRIPLET = NOTE_16_3;
    static const float SIXTEENTH_QUINTUPLET = NOTE_16_5;
    static const float SIXTEENTH_SEPTUPLET = NOTE_16_7;

  
    static const float THIRTYSECOND = NOTE_32;
    static const float THIRTYSECOND_DOTTED = NOTE_32_DOT;
    static const float THIRTYSECOND_TRIPLET = NOTE_32_3;
    static const float THIRTYSECOND_QUINTUPLET = NOTE_32_5;
    static const float THIRTYSECOND_SEPTUPLET = NOTE_32_7;
    
    string name;
    int octave;
    Dynamics dynamics;

    
      
    
    /*
     Name can be eg. C, C-1, 12
     */
	Note(string _name = "C",int _oct=4,Dynamics _dyn=Dynamics()){
        _name = ofToUpper(_name.substr(0,1))+_name.substr(1);
        set(_name, _oct, _dyn);
        
 
    };
    
    /*
     //super important
     //http://en.wikipedia.org/wiki/Copy_constructor
     
     
    Note(Note &note){
        set(note.name,note.octave, note.dynamics);
    };
    */

    
    void set(string _name = "C",int _oct=4,Dynamics _dyn=Dynamics()){
        if (!isValidNote(name)){
            ofLog()<< "The string "<<_name<<" is not a valid representation of a note in mingus"<<endl;
            return;
        }
        
        _name = ofToUpper(_name.substr(0,1))+_name.substr(1);
        
        int len = _name.size();
       // if(len ==1){
            name = _name;
            octave = _oct;
            dynamics = _dyn;
       // }else if(len==3){
           // set(_name,_dyn);
       // }
	}
    
    
    /*
    void set(string _name = "C-1",Dynamics _dyn=Dynamics()){
        if (!isValidNote(name)){
            ofLog()<< "The string "<<_name<<" is not a valid representation of a note in mingus"<<endl;
            return;
        }
        
        int len = _name.size();
        if(len ==1){
            ofLog()<< "The string "<<_name<<" lacks octave info"<<endl;
            return;
        }else if(len==3){
            vector<string> str = ofSplitString(_name,"-");
            set(str[0],ofToInt(str[1]),_dyn);
        }
        
    }*/

    void set(int note, Dynamics _dyn=Dynamics(),bool absolute = true){
        if (!isValidNote(name)){
            ofLog()<< "The int "<<note<<" is not a valid representation of a note in mingus"<<endl;
            return;
        }
        
         string n[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        
        if(absolute){
            name = n[note % 12];
            octave = note / 12;
        }else{
            if(note<0 || note>11){
                ofLog()<<note<<" outside note range 0-11"<<endl;
                return;
            }else{
                name = n[note];
                octave = 4;
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
    Note getAugmented(int i=1){
        Note n = *this;
        n.augment();
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
    Note getDiminished(int i=1){
        Note n = *this;
        n.diminish();
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
    Note getNatural(){
        Note n = *this;
        n.naturalise();
        return n;
    }

    
	void changeOctave(int diff){
        octave += diff;
        if(octave < 0){
            octave = 0;
        }
    }
    
    void setOctave(int oct){
        octave = oct;
    }
    
    int getOctave(){
        return octave;
    }
    
	void octaveUp(){
        changeOctave(1);
    }
    
    Note getOctaveUp(){
        Note n = *this;//copy
        n.changeOctave(1);
        return n;
    }
    
	void octaveDown(){
        changeOctave(-1);
    }

    Note getOctaveDown(){
        Note n = *this;//copy
        n.changeOctave(-1);
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
    
    
    Note getTransposed(int interval){
        Note n = *this;//copy
        n.transpose(interval);
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
            return;
        }
        //root note
        string root = name.substr(0,1);
        vector<string> nNames = ofSplitString(NoteDictionary,",");
        vector<string>::iterator it = find(nNames.begin(),nNames.end(),root);
        int uid = it-nNames.begin();
        
        int augs = Utils::occurenceNum(name,"#");
        int dims = Utils::occurenceNum(name,"b");
        
        if(relative){
           int val = uid+augs-dims;
            return val % 12;
        }else{
            return octave*12 +uid+augs-dims;
        }
        
    }
    
    int  getInt() const{
        return toInt();
        
    }

    
    static Note fromInt(int val){
       
        int relVal = val % 12;
        int oct = floor(val/12);
        
        string n[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
        
        Note note(n[relVal]);
        note.setOctave(oct);
        return note;
        
    }
    //not sure this is used...from Mingus
    string intToNote(int note){
        if(note<0 || note>11){
            ofLog()<<note<<" outside note range 0-11"<<endl;
            return;
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
        string str = getDiatonicName()+"-"+ofToString(octave);
        return str;
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
        int augs = Utils::occurenceNum(name,"#");
        int dims = Utils::occurenceNum(name,"b");
        
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

    
    
    //http://stackoverflow.com/questions/6039567/const-member-function
    static bool compare(const Note& a, const Note& b) {
       // cout<<a.toInt() <<" "<< b.toInt()<<endl;
        return (a.toInt() < b.toInt());
    }
    
    //give access to private parts
    friend ostream& operator<<(ostream& os, const Note& n);
    
};//class

    
    
    
    
    
    
    
    
    
        
//this overloads the cout stream with useful output data
//corresponding friend function above, note: inside class
inline ostream& operator<<(ostream& os, Note& n){
     os <<"Note "<< n.getShorthand()<<" ("<<n.name<<"-"<<n.toInt()<<")";
    //os <<"Note "<< n.getShorthand() << " (vel: " << n.dynamics.velocity<<")";
    return os;
}
    

    
}//namespace
#endif

