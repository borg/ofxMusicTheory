/*
 *  Utils.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 02/02/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Utils
#define _Utils

#include "ofMain.h"
//#include "Note.h"


namespace MusicTheory{
    
//forward declaration
class Note;
    
    
class Utils {
	
  public:
	
   static int occurenceNum(string const & str, string const & word ){
        int count(0);
        string::size_type word_pos( 0 );
        while ( word_pos!=string::npos){
            word_pos = str.find(word, word_pos );
            if ( word_pos != string::npos ){
                ++count;
                // start next search after this word
                word_pos += word.length();
            }
        }
        return count;
    }
    
    /*
     If strict only exact same name..not done
     
    static int getNoteId(vector<Note> const & list, Note const & n , bool strict=false){
    
       
       //this is soo annoying with C++ I need to pass objects aorund and get circular include issues
        for(int i=0;i<list.size();i++){
            if(n.name==list[i].name){
                return i;
            }
        }
        
        return -1;
    }*/
};
}
#endif
