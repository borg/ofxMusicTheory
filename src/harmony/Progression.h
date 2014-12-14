/*
 *  Progression.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 07/02/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Progression
#define _Progression

#include "ofMain.h"
#include "Chord.h"
#include "Utils.h"

#include "Poco/RegularExpression.h"
using Poco::RegularExpression;

namespace MusicTheory{

    static string ROMAN[] = {"I","II","III","IV","V","VI","VII"};
    static int numeral_intervals[] = {0, 2, 4, 5, 7, 9, 11};
    
    
    typedef map<string, int> IntLookup;
    
    const IntLookup::value_type xx[] = {
        make_pair("I",0),
		make_pair("II",1),
		make_pair("III",2),
		make_pair("IV",3),
        make_pair("V",4),
        make_pair("VI",5),
        make_pair("VII",6),
    };
        
    const IntLookup RomanLookup(xx, xx + sizeof xx / sizeof xx[0]);
    
    
    
    
    typedef Chord (*ChordFunctionPointer)(Note);
    typedef map<string,ChordFunctionPointer> ChordFunctionLookup;

    //TODO: Add other b # functions if there are like more subtonic/leadingtone
    
    const ChordFunctionLookup::value_type f[] ={
        make_pair("I",&Chord::I),
        make_pair("I7",&Chord::I7),
        make_pair("II",&Chord::II),
        make_pair("II7",&Chord::II7),
        make_pair("III",&Chord::III),
        make_pair("III7",&Chord::III7),
        make_pair("IV",&Chord::IV),
        make_pair("IV7",&Chord::IV7),
        make_pair("V",&Chord::V),
        make_pair("V7",&Chord::V7),
        make_pair("VI",&Chord::VI),
        make_pair("VI7",&Chord::VI7),
        make_pair("bVII",&Chord::bVII),
        make_pair("VII",&Chord::VII),
        make_pair("bVII7",&Chord::bVII7),
        make_pair("VII7",&Chord::VII7)
    };
    
    
    
    const ChordFunctionLookup ChordFunctions(f, f + sizeof f / sizeof f[0]);
    
       

    
    
    
    typedef struct ChordTuple{
        string roman;//eg. V
        //string bass;
        //string poly;
        int numeral;//roman converted
        int accidentals;//eg. # = +1
        string cleanedAccidentals;//reduced ##b to #
        string suffix;//eg. dim7
    }ChordTuple;
    
class Progression {
	
  public:
	
	Progression(){};
	    
    
    
    
    
    

   // numeralInterval = [0, 2, 4, 5, 7, 9, 11]
    
    
    
    /*
     Converts a list of chord functions (eg `['I', 'V7']`) or 
     a string (eg. 'I7') to a list of chords. 
     Any number of accidentals can be used as prefix to augment or diminish; 
     for example: bIV or #I. All the chord abbreviations in the chord module 
     can be used as suffixes; for example: Im7, IVdim7, etc. 
     You can combine prefixes and suffixes to manage complex progressions: 
     #vii7, #iidim7, iii7, etc. 
     Using 7 as suffix is ambiguous, since it is classicly used to denote 
     the seventh chord when talking about progressions instead of _just_ the 
     dominanth seventh chord. We have taken the classic route; I7 
     will get you a major seventh chord. If you specifically want a dominanth 
     seventh, use Idom7.
     */
    static deque<Chord> fromString(string progression, Note key = Note("C")){
        vector<string> prog = ofSplitString(progression,",");
    
        deque<Chord> result;
        
        Chord chord;
        
        for (int c=0;c<prog.size();c++){
            string chordStr = prog[c];
            chord = Progression::getChordfromChordFunction(chordStr,key);
            result.push_back(chord);
        }
        
        return result;
    }
	
    
    static Chord getChordfromChordFunction(string chordFunction, Note key = Note("C")){
        //strip preceding accidentals from the string
        
        
      
        
        vector<string> slash = ofSplitString(chordFunction,"/");
        vector<string> poly = ofSplitString(chordFunction,"| ");
        
        string chordTop;
        string bass="";
        string chordBottom="";
        
        if(slash.size()==2){
            chordTop = slash[0];
            bass = slash[1];
        }else if(poly.size()==2){
            chordTop = poly[0];
            chordBottom = poly[1];
        }else{
            chordTop = chordFunction;
            
        }
        
        
        
        ChordTuple tuple = Progression::parse(chordTop);
        
        
        Chord chord;
        
        chord = Progression::tupleToChord(tuple,key);
        
        
            
        if(slash.size()==2){
            ChordTuple bassTuple = Progression::parse(bass);
            Chord bassNote;
            bassNote = Progression::tupleToChord(bassTuple,key);
            chord.setBass(bassNote.notes[0]);
            
        }else if(poly.size()==2){
            ChordTuple polyTuple = Progression::parse(chordBottom);
            Chord polyChord;
            polyChord = Progression::tupleToChord(polyTuple,key);
            //I don't know if anything is missing here...for subchord symbols etc
            /*
            Chord subchord;
            
            subchord.name = chordSymbol;
            subchord.notes = chord.notes;
            subchord.setRoot(note);*/
            
            chord.setPolyChord(polyChord);
        }
        
        return chord;
        //These suffixes don't need any post processing
       
    }
    
    /*
     Doesn't work?
     */
    static vector<string> getFunctionFromChord(Chord chord, Note key = Note("C")){
        
        
        vector<string> prog = Progression::determine(chord.notes,key,true,true,true);//not sure about trues here when only returning one...unnecessary?
        
        
        return prog;
        
    }
    
    
    /*
     Cm13 in of G returns IVm13 as the first option...use that by default
     */
    static vector<string> getFunctionFromChordString(string chordString, Note key = Note("C")){
        
        
        vector<string> prog = Progression::determine(chordString,key,true,false,false);//not sure about trues here when only returning one...unnecessary?
        
        return prog;
    }
    
    static Chord tupleToChord(ChordTuple tuple, Note key){
        Chord chord;
        Note transKey = key;
        
        
        if(tuple.accidentals!=0){
            transKey = key.getTransposed(tuple.accidentals);
        }
        
        
        if(tuple.suffix == "7" || tuple.suffix == ""){
            tuple.roman += tuple.suffix;
            chord = Progression::getChordFromRoman(tuple.roman, transKey);
        }else{
            chord = Progression::getChordFromRoman(tuple.roman, transKey);
            chord = Chord::chordFromShorthand(tuple.suffix,chord.notes[0]);//add dim etc
        }
        
        //Let the accidentals do their work
         int acc = tuple.accidentals;
        
        //needed?
         //for(int i=0;i<chord.notes.size();i++){
         //  chord.notes[i].transpose(acc);
         //}
        
        return chord;
        
    }
    
    /*
     Returns a tuple (roman numeral, accidentals, chord suffix).
     {{{
     >>> progressions.parse_string("I")
     ('I', 0, '')
     >>> progressions.parse_string("bIM7")
     ('I', -1, 'M7')
     */
    
    
    static ChordTuple parse(string progression){
        

     
        string roman_numeral = "";

        
        ChordTuple tuple;
        
        
        //http://www.codeproject.com/Articles/9099/The-30-Minute-Regex-Tutorial
        
        RegularExpression::Match match;
        
        RegularExpression accEx("[#b]*(?=[iIvV])");//find accidentals
        string acc="";
        if(accEx.match(progression, match) != 0) {
            acc = progression.substr(0,match.length);
        }
        
        
        //any sharper regex head please improve this
        int augs = Utils::occurenceNum(acc,"#");
        int dims = Utils::occurenceNum(acc,"b");
        
        
        /*
        
        RegularExpression augEx("[#]*(?=[#biIvV])");//find augmented
        
        int augs=0;
        if(augEx.match(acc, match) != 0) {
            augs = match.length;
        }
        
        RegularExpression dimEx("[b]*(?=[#biIvV])");//dims
        
        int dims=0;
        if(dimEx.match(acc, match) != 0) {
            dims = match.length;
        }
        cout<<progression<<" augs: "<<augs<<" dims "<<dims<<endl;
         
        */
        tuple.accidentals = augs-dims;
        

        string roman="";
        roman = progression.substr((augs+dims));//remove accidentals
        
        RegularExpression romanEx("(?<!d)[iIvV]*");//find all romans not prefixed by d, thus exclude i in dim..have I missed some?
        
        
        
        if(romanEx.match(roman, match) != 0) {
            roman = roman.substr(match.offset,match.length);
        }
        
        tuple.roman = ofToUpper(roman);
       // cout<<progression<<" tuple.accidentals "<<tuple.accidentals <<" tuple.roman "<<tuple.roman<<endl;
        
               
        
        string suffix = progression;
        tuple.suffix = progression.substr(roman.size()+augs+dims);//get suffix if any
        
        
        tuple.cleanedAccidentals = Progression::cleanAccidentals(tuple.accidentals);
        
        //fix for the subtonic/leadingtone mixup
        
        if((tuple.roman=="VII" && tuple.cleanedAccidentals=="b") || (tuple.roman=="VI" && tuple.cleanedAccidentals=="#") ){
            tuple.roman="bVII";
            tuple.cleanedAccidentals=="";
            tuple.accidentals=0;
        }
        
        return tuple;
   }
    

    
    static Chord getChordFromRoman(string romanSymbol,Note key){
        ChordFunctionLookup cf = ChordFunctions;
        Chord cc;
        if(cf[romanSymbol]){
            cc= cf[romanSymbol](key);
            //cout<<"getChordFromRoman "<<romanSymbol<<endl;
        }else{
            ofLog()<<"Progression::getChordFromRoman error not found: "<<romanSymbol<<endl;
        }
        return cc;
    }
    
    /*
     Does simple harmonic substitutions. Returns a 
     list of possible substitions for `progression[substitute_index]`. 
     If `ignore_suffix` is set to True the suffix of the chord being substituted 
     will be ignored. Otherwise only progressions without a suffix, or with suffix '7' 
     will be substituted. The following table is used to convert progressions:
     || I || III ||
     || I || VI ||
     || IV || II ||
     || IV || VI ||
     || V || VII ||
     */
    
    
    static vector<string> substituteHarmonic(string chordFunction, bool ignore_suffix = false){

        vector<string> result;
    //turn into map to speed up map<string,string[]>
        string simple_substitutions[5][2] = {
            {"I", "III"},
            {"I", "VI"},
            {"IV", "II"},
            {"IV", "VI"},
            {"V", "VII"}};
                            
	       
        ChordTuple tuple = Progression::parse(chordFunction);
        
        string substituteRoman;
        Chord chord;
        
        if(tuple.suffix == "" || tuple.suffix == "7" || ignore_suffix){
            for (int s = 0;s<5;s++){
                substituteRoman="";
                
                if(tuple.roman==simple_substitutions[s][0]){
                    substituteRoman = simple_substitutions[s][1];//swap one for the other whichever side its found
                }else if(tuple.roman==simple_substitutions[s][1]){
                    substituteRoman = simple_substitutions[s][0];
                }
                
                if(substituteRoman!=""){
                    string chStr = tuple.cleanedAccidentals+substituteRoman+tuple.suffix;//rebuild chord
                    result.push_back(chStr);
                }
                
                
                
            }
        }
        
        return result;
    
    }
    
    
    /*
     Substitutes minor chords for its major equivalent. Recognizes 'm' and 'm7' suffixes \
     and ['II', 'III', 'VI'] if there is no suffix.
     {{{
     >>> progressions.substitute_minor_for_major(["VI"], 0)
     ["I"]
     >>> progressions.substitute_minor_for_major(["Vm"], 0)
     ["bVIIM"]
     >>> progressions.substitute_minor_for_major(["VIm7"], 0)
     ["IM7"]
     }}}
     */
    
    
    static vector<string> substituteMinorForMajor(string chordFunction, bool ignore_suffix = false){

    
        ChordTuple tuple = Progression::parse(chordFunction);
        vector<string> result;
    
	
        string newRoman,newAcc;
        int i,a;
        IntLookup Lookilook = RomanLookup;//instance...not sure why needed
        //Minor to major substitution
        if(tuple.suffix == "m" || tuple.suffix == "m7" || (tuple.suffix == "" && (tuple.roman == "II" ||tuple.roman ==  "III" || tuple.roman ==  "VI")) || ignore_suffix){
            
            i = Lookilook[tuple.roman];
            newRoman = ROMAN[(i+2) % 7];
            a = Progression::intervalDiff(tuple.roman,newRoman, 3) + tuple.accidentals;
            
            if(tuple.suffix =="m" || ignore_suffix){
                newAcc = Progression::cleanAccidentals(a);
                result.push_back(newAcc+newRoman+"M");
            }
            
            if(tuple.suffix =="m7" || ignore_suffix){
                newAcc = Progression::cleanAccidentals(a);
                result.push_back(newAcc+newRoman+"M7");
            }
            
            if(tuple.suffix =="" || ignore_suffix){
                newAcc = Progression::cleanAccidentals(a);
                result.push_back(newAcc+newRoman);
            }

        }
        
        return result;

    }
    
    
    /*
     Substitutes major chords for their minor equivalent. Recognizes 'M' and 'M7' suffixes \
     and ['I', 'IV', 'V'] if there is no suffix.
     {{{
     >>> progressions.substitute_major_for_minor(["I"], 0)
     ["VI"]
     >>> progressions.substitute_major_for_minor(["VM7"], 0)
     ["IIIm7"]
     */
    
    
    static vector<string> substituteMajorForMinor(string chordFunction, bool ignore_suffix = false){
        ChordTuple tuple = Progression::parse(chordFunction);
        vector<string> result;

        string newRoman,newAcc;
        int i,a;
        IntLookup Lookilook = RomanLookup;
            
        //Major to minor substitution
        if(tuple.suffix == "M" || tuple.suffix == "M7" || (tuple.suffix == "" && (tuple.roman == "I" || tuple.roman == "IV"|| tuple.roman == "V")) || ignore_suffix){
            
            i = Lookilook[tuple.roman];
            newRoman = ROMAN[(i+5) % 7];
            a = Progression::intervalDiff(tuple.roman,newRoman, 9) + tuple.accidentals;
            
            
            if(tuple.suffix =="M" || ignore_suffix){
                newAcc = Progression::cleanAccidentals(a);
                result.push_back(newAcc+newRoman+"m");
            }
            
            if(tuple.suffix =="M7" || ignore_suffix){
                newAcc = Progression::cleanAccidentals(a);
                result.push_back(newAcc+newRoman+"m7");
            }
            
            if(tuple.suffix =="" || ignore_suffix){
                newAcc = Progression::cleanAccidentals(a);
                result.push_back(newAcc+newRoman);
            }
            
        }
        return result;
        
    }
		  
    
    /*
     Substitutes a diminished chord for another diminished chord. Recognizes the 'dim' and 'dim7' \
     suffix and "VI" if there is no suffix.
     {{{
     >>> progressions.substitute_diminished_for_diminished(["VII"], 0)
     ["IIdim", "bIVdim", "bbVIdim"]
     */
    
    
    
    static vector<string>  substituteDiminishedForDiminished(string chordFunction, bool ignore_suffix = false){
        ChordTuple tuple = Progression::parse(chordFunction);
        vector<string> result;
        
        string newRoman,newAcc;
         
        int i,a = tuple.accidentals;
        IntLookup Lookilook = RomanLookup;
        
        
        
        //Diminished progressions
        if(tuple.suffix == "dim7" || tuple.suffix == "dim" || (tuple.suffix == "" && tuple.roman== "VII") || ignore_suffix){
  
            if(tuple.suffix == ""){
                tuple.suffix = "dim";
            }
            
            //Add diminished chord
            string last = tuple.roman;
            for(int x=0;x<3;x++){
                i = Lookilook[last];
                newRoman = ROMAN[(i+2) % 7];
                a += Progression::intervalDiff(last, newRoman, 3);
                newAcc = Progression::cleanAccidentals(a);
                result.push_back(newAcc+newRoman+tuple.suffix);
                last = newRoman;
            }
        }
        return result;
    }
    
    
    
    
    
    
    static vector<string>  substituteDiminishedForDominant(string chordFunction, bool ignore_suffix = false){
        ChordTuple tuple = Progression::parse(chordFunction);
        vector<string> result;
        
        string newRoman,newAcc,domRoman;
        
        int i,a = tuple.accidentals;
        IntLookup Lookilook = RomanLookup;

        
        
        
        //Diminished progressions
        if(tuple.suffix == "dim7" || tuple.suffix == "dim" || (tuple.suffix == "" && tuple.roman == "VII") || ignore_suffix){
	
            if(tuple.suffix == ""){
                tuple.suffix = "dim";
            }
            //Add diminished chord
            string last = tuple.roman;
            for(int x=0;x<4;x++){
                i = Lookilook[last];
                newRoman = ROMAN[(i+2) % 7];
                domRoman = ROMAN[(i+5) % 7];
                a = Progression::intervalDiff(last, domRoman, 8)+tuple.accidentals;
                newAcc = Progression::cleanAccidentals(a);
                result.push_back(newAcc+domRoman+"dom7");
                last = newRoman;
            }
        }
        return result;
    }
    
    
    /*
     Gives a list of possible substitutions for 
     `progression[substitute_index]`. If depth > 0 the substitutions 
     of each result will be recursively added as well.
     {{{
     >>> progressions.substitute(["I", "IV", "V", "I"], 0)
     ["III", "VI", etc.
     */
    
    static vector<string>  substitute(string chordFunction, int depth=0){
        
        ChordTuple tuple = Progression::parse(chordFunction);
        vector<string> result;
        
         string simple_substitutions[9][2] = {
             {"I", "III"},
             {"I", "VI"},
             {"IV", "II"},
             {"IV", "VI"},
             {"V", "VII"},
             {"V", "VIIdim7"},
             {"V", "IIdim7"},
             {"V", "IVdim7"},
             {"V", "bVIIdim7"}};
                                
         

         
        //Do the simple harmonic substitutions
        if(tuple.suffix == "" || tuple.suffix == "7"){
            
            
        }
        
        /*
         for subs in simple_substitutions:
         r = None
         if roman == subs[0]:
         r = subs[1]
         elif roman == subs[1]:
         r = subs[0]
         if r != None:
         res.append(tuple_to_string((r, acc, '')))
             //Add seventh or triad depending on r
         if r[-1] != "7":
         res.append(tuple_to_string((r, acc, '7')))
         else:
         res.append(tuple_to_string((r[:-1], acc, '')))
         
         
             //Add natural seventh
         if suff == '' or suff == 'M' or suff == 'm':
         res.append(tuple_to_string((roman, acc, suff + '7')))
         
         
             //Minor to major substitution
         if suff == 'm' or suff == 'm7':
         n = skip(roman, 2)
         a = interval_diff(roman, n, 3) + acc
         res.append(tuple_to_string((n, a, 'M')))
         res.append(tuple_to_string((n, a, 'M7')))
         
             //Major to minor substitution
         if suff == 'M' or suff == 'M7':
         n = skip(roman, 5)
         a = interval_diff(roman, n, 9) + acc
         res.append(tuple_to_string((n, a, 'm')))
         res.append(tuple_to_string((n, a, 'm7')))
         
         
             //Diminished progressions
         if suff == 'dim7' or suff == 'dim':
         
             //Add the corresponding dominant seventh
         res.append(tuple_to_string((skip(roman, 5), acc, 'dom7')))
         
         
             //Add chromatic dominant seventh
         n = skip(roman, 1)
         res.append(tuple_to_string((n, acc + interval_diff(roman, n, 1), 'dom7')))
         
             //Add diminished chord
         last = roman
         for x in range(4):
         next = skip(last, 2)
         acc += interval_diff(last, next, 3)
         res.append(tuple_to_string((next , acc, suff)))
         last = next
         
         res2 = []
         if depth > 0:
         for x in res:
         new_progr = progression
         new_progr[substitute_index] = x
         res2 += substitute(new_progr, substitute_index, depth - 1)
             return res + res2;
         */
    
        return result;
    }
    
    
    /*
     
     Determines the harmonic function of chord in key. This function can also deal with lists of chords.
     {{{
     >>> determine(["C", "E", "G"], "C")
     ['tonic']
     >>> determine(["G", "B", "D"], "C")
     ['dominant']
     >>> determine(["G", "B", "D", "F"], "C", True)
     ['V7']
     >>> determine([["C", "E", "G"], ["G", "B", "D"]], "C", True)
     [['I'], ['V']]

     */
    
    
    static vector< vector<string> > determine(vector< deque<Note> > notes, Note key, bool shorthand = false){
        vector< vector<string> > result;
        
        for(int i=0;i<notes.size();i++){
            result.push_back(Progression::determine(notes[i],key,shorthand));
        }
        
        return result;
        
    }
    
    
    
    
    static vector< vector<string> > determine(vector<Chord> chords, Note key, bool shorthand = false){
        vector< vector<string> > result;
        
        for(int i=0;i<chords.size();i++){
            result.push_back(Progression::determine(chords[i].notes,key,shorthand));
        }
        
        return result;
        
    }
    /*
     <Cm7,Adim,E7b9>
     */
    static vector< vector<string> > determine(vector<string> chordNames, Note key, bool shorthand = false, bool useInversions = true, bool usePoly = true){
        vector< vector<string> > result;
        
        for(int i=0;i<chordNames.size();i++){
            result.push_back(Progression::determine(Chord::getChordFromString(chordNames[i]).notes,key,shorthand,useInversions,usePoly));
        }
        
        return result;
        
    }
    
    
    static vector<string> determine(string chordName, Note key, bool shorthand = false, bool useInversions = true, bool usePoly = true){
        
        
        
        //check for poly and slash chords
        vector<string> slash = ofSplitString(chordName, "/");//different bass
        vector<string> poly = ofSplitString(chordName, "|");//combined chords
        
        
        
        string func = "";
        if(poly.size()==2 && poly[0]!=poly[1]){
            string top = Progression::getFunctionInRoman(poly[0],key,shorthand);
            string bottom = Progression::getFunctionInRoman(poly[1],key,shorthand);
            func = top+"|"+bottom;
        }else if(slash.size()==2){
            string top = Progression::getFunctionInRoman(slash[0],key,shorthand);
            string bass = Progression::getFunctionInRoman(slash[1],key,shorthand);
            func = top+"/"+bass;
        }else{
            func = Progression::getFunctionInRoman(chordName,key,shorthand);
        }
        
        //this ensures that the first one is the one as it is written
        vector<string> result;
        result.push_back(func);
        
        if(useInversions || usePoly){
            Chord chord = Chord::getChordFromString(chordName);
            vector<string> permutations = Progression::determine(chord.notes,key,shorthand,useInversions,usePoly);
            for(int i=0;i<permutations.size();i++){
                if(permutations[i]!=func){
                    result.push_back(permutations[i]);
                }
            }
        }

        return result;
    }
    
    
    static vector<string> determine(deque<Note> notes, Note key, bool shorthand = true, bool useInversions = true, bool usePoly = true){
        vector<string> result;
        if(notes.size()==0){
            ofLog()<<"Warning: Progression::determin empty notes"<<endl;
            return result;
        }
        
        
        //cout<<"Progression::determine"<<endl;
        Chord::print(notes);
        
        
        vector<string> type_of_chord = Chord::determine(notes, true, useInversions, usePoly);//shorthand,inversion,poly
        
        //cout<<"type_of_chord.size  "<<type_of_chord.size()<<endl;
        
        
        for(int i=0;i<type_of_chord.size();i++){
            string chordStr = type_of_chord[i];
            //cout<<"chordStr "<<chordStr<<endl;
            
            //check for poly and slash chords
            vector<string> slash = ofSplitString(chordStr, "/");//different bass
            vector<string> poly = ofSplitString(chordStr, "|");//combined chords
            
            
            
            string func = "";
            if(poly.size()==2 && poly[0]!=poly[1]){
                string top = Progression::getFunctionInRoman(poly[0],key,shorthand);
                string bottom = Progression::getFunctionInRoman(poly[1],key,shorthand);
                func = top+"|"+bottom;
            }else if(slash.size()==2){
                string top = Progression::getFunctionInRoman(slash[0],key,shorthand);
                string bass = Progression::getFunctionInRoman(slash[1],key,shorthand);
                func = top+"/"+bass;
            }else{
                func = Progression::getFunctionInRoman(chordStr,key,shorthand);
            }
            
            // Add to results
            result.push_back(func);
        }
        
        return result;
    }
    
                                                                            
    static string getFunctionInRoman(string chordStr,Note key, bool shorthand = true){
        
        //this only affects shorthand false
        map<string, string> func_dict;
        func_dict["I"] ="tonic";
        func_dict["II"] ="supertonic";//was lower
        func_dict["III"] ="mediant";//was lower
        func_dict["IV"] ="subdominant";
        func_dict["V"] ="dominant";
        func_dict["VI"] ="submediant";//was lower
        func_dict["#VI"] ="subtonic";
        func_dict["bVII"] ="subtonic";//was lower//wa
        func_dict["VII"] ="leadingtone";//was lower//wa
        
        map<string, vector<string> > expected_chord;
        vector<string> M;
        M.push_back("M");
        M.push_back("M7");
        expected_chord["I"]  = M;
        expected_chord["IV"]  = M;
        
        vector<string> m;
        m.push_back("m");
        m.push_back("m7");
        expected_chord["II"]  = m;
        expected_chord["III"]  = m;
        expected_chord["VI"]  = m;
        expected_chord["VII"]  = m;
        
        vector<string> M7;
        M7.push_back("M");
        M7.push_back("7");
        expected_chord["V"]  = M7;
        
        vector<string> dim;
        dim.push_back("dim");
        dim.push_back("m7b5");
        expected_chord["VII"]  = dim;
        
        
        
        
        Chord chord = Chord::getChordFromString(chordStr);
        cout<<"Determine function for "<<chordStr<<" "<<chord.name<<endl;
        Note root = chord.getRoot();
        
        //string chord_type = chord.getChordSymbol();
        
        string chord_type = chord.name;
        
        //Determine chord function
        
        string intv = Interval::determine(key, root);
        vector<string> intvArr = ofSplitString(intv," ");
        
        //is this always size 2? These python string functions aren't very nice at all
        //write a Interval::getRoman
        string interval_type = intvArr[0];
        string interval = intvArr[1];
        string func="";
        
        if(interval == "unison"){
            func = "I";
        }else if(interval == "second"){
            func = "II";//was lower, but I don't think it makes sense since a song can be said to be in F lydian and then ii is not minor but G mixo. Also just harder to read since ii seems to indicate minor, but isn't necesarily, eg. Im7 iiM7
        }else if(interval == "third"){
            func = "III";//was lower
        }else if(interval == "fourth"){
            func = "IV";
        }else if(interval == "fifth"){
            func = "V";
        }else if(interval == "sixth"){
            func = "VI";//was lower
        }else if(interval == "seventh"){
            func = "VII";//was lower
        }
        

        //Check whether the chord is altered or not
        typedef map<string, vector<string> >::iterator it_type;
        for(it_type iterator = expected_chord.begin(); iterator != expected_chord.end(); iterator++) {
            
            
            if(iterator->first == func){
                
                //Triads
                if(chord_type == iterator->second[0]){
                    if (!shorthand){
                        func = func_dict[func];
                    }
                    //Sevenths
                }else if(chord_type == iterator->second[1]){
                    if (shorthand){
                        func += "7";
                    }else{
                        func = func_dict[func] + " seventh";
                    }
                    //Other
                }else{
                    if(shorthand){
                        func += chord_type;
                    }else{
                        func = func_dict[func] + Chord::getFullName(chord_type);
                    }
                }
            }
        }
        //Handle b's and #'s (for instance Dbm in key C is bII)
        if (shorthand){
            if (interval_type == "minor"){
               
                func = "b" + func;
            }else if( interval_type == "augmented"){
                func = "#" + func;
            }else if( interval_type == "diminished"){
                func = "bb" + func;
            }
        }else{
            if (interval_type == "minor"){
                func = "minor " + func;
            }else if( interval_type == "augmented"){
                func = "augmented " + func;
            }else if( interval_type == "diminished"){
                func = "diminished " + func;
            }
        }
        
        
        cout<<"Interval: "<<interval_type<<" "<<interval<<" key: "<<key<<" root: "<<root<<" function: "<<func<<" symbol: " <<chord_type<<endl;
        
        return func;
    }
    
    /*
     Convenience
     */
    
    static void print(deque<Chord> chords){
        
        cout <<"[ ";
        for(int i = 0;i<chords.size();i++){
            cout<<chords[i];
            if(i<chords.size()-1){
                cout<<", ";
            }
        }
        cout<<" ]"<<endl;
        
        
    }
    
    static void print(vector<string> substitutes){
        
        cout <<"[ ";
        for(int i = 0;i<substitutes.size();i++){
            cout<<substitutes[i];
            if(i<substitutes.size()-1){
                cout<<", ";
            }
        }
        cout<<" ]"<<endl;
        
        
    }
    
    static void print(vector< vector<string> > prog){
        
        cout <<"Progression [ ";
        for(int i = 0;i<prog.size();i++){
            cout <<"[";
            for(int ii = 0;ii<prog[i].size();ii++){
                
                cout<<prog[i][ii];
                if(ii<prog[i].size()-1){
                    cout<<", ";
                }
            }
            cout <<"]";
            if(i<prog.size()-1){
                cout<<", ";
            }
        }
        cout<<" ]"<<endl;
        
        
    }
    
    
private:
    
    /*
     It just wacks the accidentals back unto the string as normal b or # symbols
     */
    static string cleanAccidentals(int accidentals){
        string acc="";
        
        if( accidentals > 6){
            accidentals = - accidentals % 6;
        }else if(accidentals < -6){
            accidentals = accidentals % 6;
        }
        
            
        if(accidentals>0){
            string aug = "##############################################";
            acc = aug.substr(0,accidentals);//this must be quicker than a for loop?
        }
        
        if(accidentals<0){
            string dims = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
            acc = dims.substr(0,abs(accidentals));//this must be quicker than a for loop?
        }
        
        return acc;
    }
	
    /*
     Returns the number of half steps progression2 needs to be \
     diminished or augmented until the interval between `progression1` \
     and `progression2` is `interval`
     */
    static int intervalDiff(string progression1, string progression2,int interval){
        IntLookup Lookilook = RomanLookup;
   
        int i = numeral_intervals[Lookilook[progression1]];
        int j = numeral_intervals[Lookilook[progression2]];
        
        int acc = 0;
        if(j < i){
            j += 12;
        }
        while((j - i) > interval){
            acc--;
            j--;
        }
        while((j - i) < interval){
            acc++;
            j++;
        }
        return acc;
     }
    
};//class
};//namespace

#endif
