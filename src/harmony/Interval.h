/*
 *  Interval.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 29/01/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 
 */

#ifndef _Interval
#define _Interval

#include "ofMain.h"
#include "Note.h"
#include "Utils.h"
#include "Diatonic.h"
//class Note;
namespace MusicTheory{
   
    
    typedef Note (*IntervalFunctionPointer)(Note);
    typedef map<int,IntervalFunctionPointer> IntervalFunctionLookup;
 
    
    
class Interval {
	
  public:
    //http://www.parashift.com/c++-faq-lite/static-const-with-initializers.html
    /*
     The caveats are that you may do this only with integral or enumeration types, and that the initializer expression must be an expression that can be evaluated at compile-time: it must only contain other constants, possibly combined with built-in operators.
     
     static const int maximum = 42;
     */
    
    
/*
 Diatonic Interval.
 Needs a note and a key.
 
 Take the diatonic second of note in key.
 Examples:
 {{{
 >>>	second("E", "C")
 'F'
 >>> second("E", "D")
 'F#'
 }}}*/
    
    
    static Note unison(Note note, Note key){
        return note;
    }
    
    static Note second(Note note, Note key){
        return Diatonic::interval(key, note, 1);
    }
    
    static Note third(Note note, Note key){
        return Diatonic::interval(key, note, 2);
    }
    static Note fourth(Note note, Note key){
        return Diatonic::interval(key, note, 3);
    }
    static Note fifth(Note note, Note key){
        return Diatonic::interval(key, note, 4);
    }
    static Note sixth(Note note, Note key){
        return Diatonic::interval(key, note, 5);
    }
    static Note seventh(Note note, Note key){
        return Diatonic::interval(key, note, 6);
    }
   
    
    /*
     Absolute Interval
     Needs a note
     */
    
    static Note minorUnison(Note note){
        Note n(note);
        n.diminish();
        return n;
    }
    
    static Note majorUnison(Note note){
        return note;
    }
    
    static Note augmentedUnison(Note note){
         Note n(note);
        n.augment();
        return n;
    }
    
    
    static Note minorSecond(Note note){
        Note n = note;
        n.transpose(1);
        return n;
        
        /*
        Note sec = Interval::second(note.getNatural(), Note("C"));
        return Interval::findInterval(note, sec, 1);
         */
    }
    
    static Note majorSecond(Note note){
        Note n = note;
        n.transpose(2);
        return n;
        
        /*
        Note sec = Interval::second(note.getNatural(), Note("C"));
        return Interval::findInterval(note, sec, 2);
         */
    }
    
    static Note minorThird(Note note){
        Note n = note;
        n.transpose(3);
        return n;
        /*
         Note trd = Interval::third(note.getNatural(), Note("C"));
        return Interval::findInterval(note, trd, 3);
         */
    }
    
    static Note majorThird(Note note){
        Note n = note;
        n.transpose(4);
        return n;
        /*
       Note trd = Interval::third(note.getNatural(), Note("C"));
        return Interval::findInterval(note, trd, 4);
         */
    }
    
    static Note minorFourth(Note note){
        Note n = note;
        n.transpose(4);
        return n;
        
        
        /*
        Note frt = Interval::fourth(note.getNatural(), Note("C"));
        return Interval::findInterval(note, frt, 4);
         */
    }
    
    static Note majorFourth(Note note){
        Note n = note;
        n.transpose(5);
        return n;
        
        /*
        Note frt = Interval::fourth(note.getNatural(), Note("C"));
        return Interval::findInterval(note, frt, 5);
         */
    }
    
    static Note perfectFourth(Note note){
        return Interval::majorFourth( note);
    }
    
    static Note minorFifth(Note note){
        Note n = note;
        n.transpose(6);
        return n;
        /*
        Note fif = Interval::fifth(note.getNatural(), Note("C"));
        return Interval::findInterval(note, fif, 6);
         */
    }
    
    static Note majorFifth(Note note){
        Note n = note;
        n.transpose(7);
        return n;
        /*
        Note fif = Interval::fifth(note.getNatural(), Note("C"));
        return Interval::findInterval(note, fif, 7);
         */
    }
    
    static Note perfectFifth(Note note){
        return Interval::majorFifth(note);
    }
    
    static Note minorSixth(Note note){
        
        Note n = note;
        n.transpose(8);
        return n;
        /*
        Note sth = Interval::sixth(note.getNatural(), Note("C"));
        return Interval::findInterval(note, sth, 8);*/
    }
    
    static Note majorSixth(Note note){
        Note n = note;
        n.transpose(9);
        return n;
        
        //why would I use these??!!
        //Note sth = Interval::sixth(note.getNatural(),Note("C"));
        //return Interval::findInterval(note, sth, 9);
    }
    
    static Note minorSeventh(Note note){
        Note n = note;
        n.transpose(10);
        return n;
        
        /*
        Note sth = Interval::seventh(note.getNatural(), Note("C"));
       return Interval::findInterval(note, sth, 10);
         */
    }
    
    static Note majorSeventh(Note note){
        Note n = note;
        n.transpose(11);
        return n;
        
        /*
        Note sth = Interval::seventh(note.getNatural(), Note("C"));
        return Interval::findInterval(note, sth, 11);
         */
    }
    
    
    
    //(Helper) Functions
    
    
    /*
     Gets the note an interval (in half notes) away from the given note. \
     This will produce mostly theoretical sound results, but you should \
     use the minor and major functions to work around the corner cases.
     */
    
    static Note getInterval(Note note, int interval,string key = "C"){
	
    /*
        Interval = map(lambda x: (Notes::note_to_int(key) + x) % 12,[0, 2, 4, 5, 7, 9, 11]);
        key_notes = diatonic.get_notes(key);
    
	for x in key_notes){
    if x[0] == note[0]){
    result = (Interval[key_Notes::index(x)] + interval) % 12
    
	if result in Interval){
    return key_notes[Interval.index(result)] + note[1:]
	else){
    return Notes::diminish(key_notes[Interval.index((result + 1) % 12)] + note[1:])*/
    }
    

    /*
     Returns an integer in the range of 0-11, determining the half note 
    steps between note1 and note2.
     */
    
    static int measure(Note note1, Note note2){

        int res = note2.toInt(true) - note1.toInt(true);
        if(res < 0){
            return 12 - (res * (-1));
        }else{
            return res;
        }
        
    }
    
    
    /*
     Names the interval between note1 and note2.
     Example:
     {{{
     >>>	determine("C", "E")
     'major third'
     >>> determine("C", "Eb")
     'minor third'
     >>> determine("C", "E#")
     'augmented third'
     >>> determine("C", "Ebb")
     'diminished third'
     
     
     
     This works for all Interval. Note that there are corner cases \
     for 'major' fifths and fourths:
     {{{
     >>> determine("C", "G")
     'perfect fifth'
     >>> determine("C", "F")
     'perfect fourth'
    
    
     */
    static string determine(Note note1, Note note2, bool shorthand = false){
	
    
        //Corner case for unisons ('A' and 'Ab', for instance)
        if(note1.getUnaltered() == note2.getUnaltered()){
            //get num of accidentals
            int augs = Utils::occurenceNum(note1.name,"#");
            int dims = Utils::occurenceNum(note1.name,"b");
            int x = augs-dims;
            
            augs = Utils::occurenceNum(note2.name,"#");
            dims = Utils::occurenceNum(note2.name,"b");
            int y = augs-dims;
        
            if (x == y){
                if(!shorthand){
                    return "major unison";
                }else{
                    return "1";
                }
            }else if(x < y){
                if(!shorthand){
                    return "augmented unison";
                }else{
                    return "#1";
                }
            }else if((x - y) == 1){
                if(!shorthand){
                    return "minor unison";
                }else{
                    return "b1";
                }
                
            }else{
                 if(!shorthand){
                     return "diminished unison";
                }else{
                    return "bb1";
                }
            }
           
        }
        //Other Interval
            
        vector<string> nNames = ofSplitString(fifths,",");
        vector<string>::iterator it = find(nNames.begin(),nNames.end(),note1.getUnaltered());
        int n1 = it-nNames.begin();
            
        it = find(nNames.begin(),nNames.end(),note2.getUnaltered());
        int n2 = it-nNames.begin();

        
        int number_of_fifth_steps = n2 - n1;
        
        if(n2 < n1){
                number_of_fifth_steps = nNames.size() - n1 + n2;
            }
            // [name, shorthand_name, half notes for major version of this interval]
            
            //tell me nicer ways to do simple lists please..not used to C++ arrays
            
            vector< vector<string> > fifth_steps;
            fifth_steps.push_back(ofSplitString("unison,1,0",","));
            fifth_steps.push_back(ofSplitString("fifth,5,7",","));
            fifth_steps.push_back(ofSplitString("second,2,2",","));
            fifth_steps.push_back(ofSplitString("sixth,6,9",","));
            fifth_steps.push_back(ofSplitString("third,3,4",","));
            fifth_steps.push_back(ofSplitString("seventh,7,11",","));
            fifth_steps.push_back(ofSplitString("fourth,4,5",","));
            
            
           
            
            
                //Count half steps between note1 and note2
           int half_notes = Interval::measure(note1, note2);
                //Get the proper list from the number of fifth steps
            
            //maj = number of major steps for this interval
           int maj = ofToInt(fifth_steps[number_of_fifth_steps][2]);
        
            //if maj is equal to the half steps between note1 and note2
            //the interval is major or perfect
            if(maj == half_notes){
        
                //Corner cases for perfect fifths and fourths
                if(fifth_steps[number_of_fifth_steps][0] == "fifth"){
                    if(!shorthand){
                        return "perfect fifth";
                    }
                }else if (fifth_steps[number_of_fifth_steps][0] == "fourth"){
                    if(!shorthand){
                        return "perfect fourth";
                    }
                }
                if(!shorthand){
                    return "major " + fifth_steps[number_of_fifth_steps][0];
                }else{
                    return fifth_steps[number_of_fifth_steps][1];
                }
            //if maj + 1 is equal to half_notes, the interval is augmented.
            }else if(maj + 1 <= half_notes){
                if(!shorthand){
                    return "augmented " + fifth_steps[number_of_fifth_steps][0];
                }else{
                    string str;
                    for(int i=0;i<(half_notes - maj) ;i++){
                        str+="#";
                    }
                    return str + fifth_steps[number_of_fifth_steps][1];
                }
                    //etc.
            }else if( maj - 1 == half_notes){
                if(!shorthand){
                    return "minor " + fifth_steps[number_of_fifth_steps][0];
                }else{
                    return "b" + fifth_steps[number_of_fifth_steps][1];
                }
            }else if( maj - 2 >= half_notes){
                if(!shorthand){
                    return "diminished " + fifth_steps[number_of_fifth_steps][0];
                }else{
                    string str;
                    for(int i=0;i<(maj - half_notes);i++){
                        str+="b";
                    }
                    return str + fifth_steps[number_of_fifth_steps][1];
                }
            }
    
        
            
    }
    
    
    
    
    /*
     Returns the note on interval up or down.
     Example:
     {{{
     >>> from_shorthand("A", "b3")
     'C'
     >>> from_shorthand("D", "-2")
     'C'
     >>> from_shorthand("E", "2", False)
     'D'
     */
    
    static Note fromShorthand(Note note, string interval){
        
        IntervalFunctionLookup lookup = generateLookup();
        
        string last = interval.substr(interval.size()-1);
        int intNum = ofToInt(last);//strips b from b3..shoujld be in range 1-7
        bool down = (bool) (interval.substr(0,1)=="-");
        
        Note n;
        if(down){
            n = (*lookup[-intNum])(note);
        }else{
            n = (*lookup[intNum])(note);
        }
        
        //this makes no sense...you should already have the right note now from the shorthand
        int augs = Utils::occurenceNum(interval,"#");
        int dims = Utils::occurenceNum(interval,"b");
        
        int diff = augs - dims;
        
        n.transpose(diff);
    
        return n;
    }
    
private:
    
    /*
     A helper function for the minor and major functions. \
     You should probably not use this directly.*/
    
    static Note findInterval(Note note1, Note note2, int interval){
        
        
        int cur = measure(note1, note2);
       
        //cout<<note1<<" "<<note2<<endl;
        //optimize pleeeeaze
        //eg note2.diminish(interval);
        while(cur != interval){
            if (cur > interval){
                note2.diminish();
            }else if (cur < interval){
                note2.augment();
            }
            cur = measure(note1, note2);
        }
       
        //cout<<"cur "<<cur<<endl;
        //cout<<"after stupdi "<<note1<<" "<<note2<<endl;
        
        // We are practically done right now, but we need to be able to create
        // the minor seventh of Cb and get Bbb instead of B######### as the result
        int augs = Utils::occurenceNum(note2.name,"#");
        int dims = Utils::occurenceNum(note2.name,"b");
        
        int val = augs-dims;
        
        
        // These are some checks to see if we have generated too many #'s
        // or too many b's. In these cases we need to convert #'s to b's
        // and vice versa.
        if(val > 6){
            val = val % 12;
            val = -12 + val;
        }else if(val < -6){
            val = val % -12;
            val = 12 + val;
        }
        
        // Rebuild the note
        Note result = note2.getNatural();//...need to figure out what's goin on here
        result.octave = note1.octave;
        
        if(val > 0){
            result.augment(val);
        }
        
        if(val < 0){
            result.diminish(val);
        }
        
        
        return result;
    }
    
    
    
    static IntervalFunctionLookup generateLookup(){
        
        
         IntervalFunctionLookup::value_type f[] ={
            make_pair(-7,&Interval::minorSecond),
            make_pair(-6,&Interval::minorThird),
            make_pair(-5,&Interval::majorFourth),
            make_pair(-4,&Interval::majorFifth),
            make_pair(-3,&Interval::minorSixth),
            make_pair(-2,&Interval::minorSeventh),
            make_pair(-1,&Interval::majorUnison),
            make_pair(1,&Interval::majorUnison),
            make_pair(2,&Interval::majorSecond),
            make_pair(3,&Interval::majorThird),
            make_pair(4,&Interval::majorFourth),
            make_pair(5,&Interval::majorFifth),
            make_pair(6,&Interval::majorSixth),
            make_pair(7,&Interval::majorSeventh)
        };
        
       
        
        
        IntervalFunctionLookup IntervalLookup(f, f + sizeof f / sizeof f[0]);
        return IntervalLookup;
     
        
    }
    
};//class
    
    /*
     
     
     ["1", major_unison, major_unison],
     ["2", major_second, minor_seventh],
     ["3", major_third, minor_sixth],
     ["4", major_fourth, major_fifth],
     ["5", major_fifth, major_fourth],
     ["6", major_sixth, minor_third],
     ["7", major_seventh, minor_second]
     

     */
    
    
    

    
    
}//namespace

#endif
