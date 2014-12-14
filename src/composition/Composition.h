/*
 *  Composition.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 10/02/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Composition
#define _Composition

#include "ofMain.h"


#include "Playable.h"

namespace MusicTheory{

    class Composition : public Playable{
	
  public:
    string name;
    string uid;//unique identifier

    //double bpm;//tempo
     Composition(){
               
    };
    
        
	
};
};//namespace

#endif
