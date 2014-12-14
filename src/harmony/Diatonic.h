/*
 *  Diatonic.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 29/01/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 
 ===============================================================================
 
 The diatonic module provides a simple interface for dealing with diatonic
 keys. The function get_notes(key) for instance returns the notes in a
 given key; even for extremely exotic notations ("C#####" or "Gbbbb").
 
 ================================================================================
 
 *
 */

#ifndef _Diatonic
#define _Diatonic

#include "ofMain.h"
#include "Note.h"



namespace MusicTheory{
    
    static const string fifths = "F,C,G,D,A,E,B";
    static map<string, deque<Note> > _keyCache;
    
class Diatonic {
	
  public:
	             
     
    
    
    /*
    Returns the note found at the interval starting from start_note 
    in the given key. For example interval('C', 'D', 1) will return 'E'. 
    Will raise a !KeyError if the start_note is not a valid note.
    */
    
    
    static Note interval(Note key, Note startNote,int interval){
	
	    
        deque<Note> notesInKey = getNotes(key);
    
        for(int n=0;n<notesInKey.size();n++){
            if(notesInKey[n].name == startNote.name){
                return notesInKey[(n + interval) % 7];
            }
            
        }
   
      
    }

    
    
    /*
     Returns an ordered list of the notes in this key. \
     For example: if the key is set to 'F', this function will return \
     `['F', 'G', 'A', 'Bb', 'C', 'D', 'E']`. \
     Exotic or ridiculous keys like 'C####' or even 'Gbb##bb#b##' will work; \
     Note however that the latter example will also get cleaned up to 'G'. \
     This function will raise an !NoteFormatError if the key isn't recognised
     */
    static deque<Note> getNotes(Note key){
        //check cache
 
        if (_keyCache[key.name+ofToString(key.octave)].size()>0){
            return _keyCache[key.name+ofToString(key.octave)];
        }
        
        /*
	if not (notes.is_valid_note(key)):
    raise NoteFormatError, "Unrecognised format for key '%s'" % key
    */
        
        
        //root note
        string root = key.name.substr(0,1);
        
        
        //fifth_index = 0 is a special case. It's the key of F and needs
        //Bb instead of B included in the result.
        
        vector<string> fifthArr = ofSplitString(fifths,",");
        vector<string>::iterator it = find(fifthArr.begin(),fifthArr.end(),root);
        int fifthIndex = it-fifthArr.begin();// Note::getNoteId(fifthArr,root);?
        
       
        
       deque<Note> result;
       // =keyCache[key.name];
        string keyAccidentals = key.name.substr(1);
        
        if(fifthIndex != 0){
           // cout<<"NOT FIFTH"<<endl;
       
            //add 
            string startNote = fifthArr[(fifthIndex - 1) % 7] + keyAccidentals;
            result.push_back(Note(startNote,key.octave));
        
            for (int x=fifthIndex; x< fifthArr.size();x++){
                result.push_back(Note(fifthArr[x]  +  keyAccidentals,key.octave));
            }
            
            for (int x=0; x< fifthIndex-1;x++){
                result.push_back(Note(fifthArr[x]  + keyAccidentals+ "#",key.octave));
            }
            
        }else{
            //cout<<" FIFTH"<<endl;
            for (int x=0; x< 6;x++){
                result.push_back(Note(fifthArr[x] + keyAccidentals,key.octave));
            }
            result.push_back(Note("Bb" + keyAccidentals,key.octave));
        }
        
        
        //this sorts all but doesn't start on key
        sort(result.begin(),result.end(),Note::compare);

        
       // vector<Note>::iterator it = find(result.begin(),result.end(),root);
        int tonic = Note::getNoteId(result,key);
        
       // cout<<"tonic "<<tonic<<" "<<key<<" "<<result.size()<<endl;
        
        
        deque<Note> keySorted;
        keySorted.insert(keySorted.begin(),result.begin()+tonic, result.end());
        
        //now in next octave
        for(int i=0;i<tonic;i++){
            result[i].changeOctave(1);
            keySorted.push_back(result[i]);
        }
        
       // keySorted.insert(keySorted.end(),result.begin(), result.begin()+tonic);
       
    
	//result = result[tonic:] + result[:tonic]
    
        //Save result to cache

        
        _keyCache[key.name+ofToString(key.octave)] = keySorted;
        return keySorted;
    }
    
    
    
    
    
		
};//class

}//namespace
#endif
