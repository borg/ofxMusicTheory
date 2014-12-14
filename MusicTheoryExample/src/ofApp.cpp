#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    
   
    
    Note a("Bb#");
   

    output<<a<<endl;
    
  
     Note b = a.getAugmented();
    
    
    
    
    //output<<"Get aug "<<b<<endl;
    a.augment();
    output<<"Aug "<<a<<endl;
    
    
    a.diminish();
     output<<"Dim "<<a<<endl;
    
    a.diminish();
    output<<"Dim "<<a<<endl;
    
    
    b.changeOctave(-1);
    b.augment();
    b.augment();
    b.augment();
    output<<b<<endl;
    
    
    output<<"Diatonic name: "<<b.getDiatonicName()<<endl;
    
    Note c = b.getOctaveDown();
    output<<"Oct down: "<<c<<endl;
    
    Note r("C");
   //r.augment();
    output<<"Get diatones of: "<<r<<endl;
    
    deque<Note> scale = Diatonic::getNotes(r);
     
    for(int i=0;i<scale.size();i++){
        output<<scale[i]<<endl;
    }

    
    
    

    
    Note root("C");
    
    
    
    
    Note abs6 = Interval::sixth(root,root);
    output<<"Absolute sixth of "<<root<<" is "<<abs6<<endl;
    
    
    
    
   Note abs4 = Interval::sixth(root,root);
    
    
        
    Note m2 = Interval::minorSecond(root);
    output<<"Min second of "<<root<<" is "<<m2<<endl;
    
    Note M2 = Interval::majorSecond(root);
    output<<"Maj second of "<<root<<" is "<<M2<<endl;
    
    
    Note m3 = Interval::minorThird(root);
    output<<"Min third of "<<root<<" is "<<m3<<endl;
    
    Note M3 = Interval::majorThird(root);
    output<<"Maj third of "<<root<<" is "<<M3<<endl;
    
    
    Note m4 = Interval::minorFourth(root);
    output<<"Min fourth of "<<root<<" is "<<m4<<endl;
    
    Note P4 = Interval::perfectFourth(root);
    output<<"Perfect fourth of "<<root<<" is "<<P4<<endl;
    
    Note M4 = Interval::majorFourth(root);
    output<<"Maj fourth of "<<root<<" is "<<M4<<endl;
    
    
    Note m5 = Interval::minorFifth(root);
    output<<"Min fifth of "<<root<<" is "<<m5<<endl;
    
    Note P5 = Interval::perfectFifth(root);
    output<<"Perfect fifth of "<<root<<" is "<<P5<<endl;
    
    Note M5 = Interval::majorFifth(root);
    output<<"Maj fifth of "<<root<<" is "<<M5<<endl;
    
    Note m6 = Interval::minorSixth(root);
    output<<"Min sixth of "<<root<<" is "<<m6<<endl;
    
    Note M6 = Interval::majorSixth(root);
    output<<"Maj sixth of "<<root<<" is "<<M6<<endl;
    
    Note m7 = Interval::minorSeventh(root);
    output<<"Min seven of "<<root<<" is "<<m7<<endl;

    Note M7 = Interval::majorSeventh(root);
    output<<"Maj seven of "<<root<<" is "<<M7<<endl;
    
    
    
   // output<<ChordLookup["7#11"]<<endl;
    
    string m = Chord::getFullName("mM7");
    
    output<<Chord::getFullName("mM7")<<" This one not found:" <<Chord::getFullName("oddname")<<endl;
    //ofLog()<<a.name<<endl;
    
    
    Chord ch = Chord::halfDiminishedSeventh(root);
    output<<ch<<endl;;
    
    
    Chord st = Chord::tonic7(root);
    output<<"Subtonic to "<<root<<" is "<<st<<endl;
    
    
    Chord subdom = Chord::subdominant(root);
    output<<"Subdominant to "<<root<<" is "<<subdom<<endl;
    
    
    Chord subdomInv = Chord::firstInversion(subdom);
    output<<"Subdominant first inversion is "<<subdomInv<<endl;
    
    
    /*
     Extended fifth chords are made of built of 5 notes at third dist
     */
    
    Chord ext5 = Chord::majorNinth(root);
    output<<"Ext fifth eg."<<ext5<<endl;
    
    

    bool useShorthand = true;
    bool allowInvensions = true;
    bool allowPolychords = true;
    
    deque<Note> triad;
    triad.push_back(Note("F"));
    triad.push_back(Note("G"));
    triad.push_back(Note("C"));
    output<<"Triad analysis "<<endl;
    Chord::print(triad);
    vector<string> analyse = Chord::analyse(triad, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        output<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    
    
    deque<Note> seventh;
    seventh.push_back(Note("C"));
    seventh.push_back(Note("E"));
    seventh.push_back(Note("G"));
    seventh.push_back(Note("A"));
    output<<endl<<"Seventh analysis "<<endl;
    
    Chord::print(seventh);
    
    analyse = Chord::analyse(seventh, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        output<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    
    deque<Note> ext;
    ext.push_back(Note("C"));
    ext.push_back(Note("E"));
    ext.push_back(Note("G"));
    ext.push_back(Note("B"));
    ext.push_back(Note("D"));
    output<<endl<<"Extended  fifth analysis "<<endl;
    Chord::print(ext);
    analyse = Chord::analyse(ext, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        output<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    
    output<<endl<<"Extended  sixth analysis "<<endl;
    ext.push_back(Note("A"));
    Chord::print(ext);
    analyse = Chord::analyse(ext, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        output<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    

    
    
    output<<endl<<"Extended  seventh analysis "<<endl;
    
    Chord ext7 = Chord::majorNinth(root);
    
    //add 11 and 13 to the 9th
    ext7.notes.push_back(Interval::fourth(root, root));
    ext7.notes.push_back(Interval::sixth(root, root));
    
    //output<<"Ext seventh eg."<<ext7<<endl;
    
    //turn vector into deque...(using deque since internally we need to shuffle aruond to test different inversions
    deque<Note> ch13;
    ch13.insert(ch13.begin(),ext7.notes.begin(),ext7.notes.end());
    
    Chord::print(ch13);
    
    analyse = Chord::analyse(ch13, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        output<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }

    
    
    
    allowPolychords = true;
    
    
    output<<endl<<"Polychord analysis"<<endl;
    
    deque<Note> poly = Chord::triad(Note("C"),Note("C")).notes;
    
    deque<Note> chord2= Chord::minorTriad(Note("F#")).notes;
    
    poly.insert(poly.end(), chord2.begin(),chord2.end());
    
    
    Chord::print(poly);
    
    analyse = Chord::analyse(poly, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        output<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    
    
    output<<endl<<"Scales"<<endl;
    Scale lyd = Scale::getLydian(Note("F"));
    output<<lyd<<endl;
    


    Scale melMin = Scale::getMelodicMinor(Note("C"));
    output<<melMin<<endl;
    
    Scale pentaMaj = Scale::getPentatonicMajor(Note("C"));
    output<<pentaMaj<<endl;
    
    
    Scale hirajoshi = Scale::getHirajoshi(Note("C"));
    output<<hirajoshi<<endl;
    
    

    
    
    output<<"V7 to "<<root<<" is ";
    Chord dom7= Progression::getChordFromRoman("V7",root);
    output<<dom7<<endl;
    
    output<<"VIdim7 to "<<root<<" is ";
  
    
    
    
    Chord pchord = Progression::getChordfromChordFunction("VIdim7",root);//add dim etc
    output<<pchord<<endl;
    
    
    string progStr = "bIIdim7,VM7";
    deque<Chord> prog = Progression::fromString(progStr,root);
    
    output<<progStr<<" where I is "<<root<<endl;
    Progression::print(prog);
    
    
    string subtituteMe = "IV7";
    output<<endl<<"Harmonic substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteHarmonic(subtituteMe));
    
    
    subtituteMe = "VIm7";
    output<<endl<<"Minor for major substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteMinorForMajor(subtituteMe));
    
    
    subtituteMe = "VM7";
    output<<endl<<"Major for minor substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteMajorForMinor(subtituteMe));

    subtituteMe = "VII";
    output<<endl<<"Dim for dim substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteDiminishedForDiminished(subtituteMe));

    
    subtituteMe = "VIIdim";
    output<<endl<<"Dim for dom substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteDiminishedForDominant(subtituteMe));
    
    
  
    //Note root("E",7);
    
    
    
    Chord I = Progression::getChordfromChordFunction("I",root);//add dim etc
    output<<I<<endl;
    
    Chord II = Progression::getChordfromChordFunction("II",root);//add dim etc
    output<<II<<endl;
    
    
    Chord IV = Progression::getChordfromChordFunction("IVdim",root);//add dim etc
    output<<IV<<endl;

    Chord V7 = Progression::getChordfromChordFunction("V7",root);//add dim etc
    output<<V7<<endl;
    
    output<<root.name<<" ----------------------------"<<endl;
    
    Scale mixo = Scale::getMixolydian(Note("C",6));
    output<<mixo<<endl;
    
    Scale prygian = Scale::getPhrygian(Note("C",6));
    output<<prygian<<endl;

    
    Scale flamenco = Scale::getFlamenco(Note("E",6));
    output<<flamenco<<endl;
    
    Scale Zen =  Scale::getInSen(Note("C",6));
    
    output<<"Scales for 7b9b5"<<endl;
    vector<string>scales = Scale::getScalesForChord("7b9b5");
    Scale::print(scales);
    
    
    Note nn  = Interval::fromShorthand(Note("C",6), "b3");
    output<<nn<<endl;
    
    melMin = Scale::getScaleFromString(scales[0], Note("C",6));
    output<<melMin<<endl;
    
    ofAddListener(MusicEvent::NOTE_ON,this,&ofApp::onNoteOn);
    ofAddListener(MusicEvent::NOTE_OFF,this,&ofApp::onNoteOff);
    
    Track * track = new Track();
    track->name = "Borgs Chords";
    
    
    
    
    
    /*
    Bar * bar = new Bar();
    bar->setTimeSignature(5,4);
    
    bar->addChord(I,Time(1));
    
    bar->addNote(Zen.getNote(0),Time(1));
    bar->addNote(Zen.getNote(1),Time(2));
    bar->addNote(Zen.getNote(2),Time(3));
    bar->addNote(Zen.getNote(3),Time(4));
    
    track->addBar(*bar);
    
    
    
    bar = new Bar();
    bar->addChord(I,Time(1));
    
    
    bar->addNote(Zen.getNote(4),Time(1));
    bar->addNote(Zen.getNote(5),Time(2));
    bar->addNote(Zen.getNote(6),Time(3));
    bar->addNote(Zen.getNote(7),Time(4));
    
    
    
    track->addBar(*bar);
    track->addBar(*bar);
    track->addBar(*bar);
    
    
    
    bar = new Bar();
    bar->addChord(II,Time(1));
    bar->addChord(V7,Time(2));
    bar->addChord(I,Time(3));
    
    
    bar->addNote(Zen.getNote(-1),Time(1));
    bar->addNote(Zen.getNote(-2),Time(2));
    bar->addNote(Zen.getNote(-3),Time(3));
    bar->addNote(Zen.getNote(0),Time(4));
    
    
    track->addBar(*bar);
   
    
    Bar * bar = new Bar();
    bar->setTimeSignature(4,4);
    output<<flamenco.getNote(0).getDiatonicName()<<endl;
    bar->addNote(flamenco.getNote(0),NoteTime(1,NOTE_8_3));
    bar->addNote(flamenco.getNote(0),NoteTime(1,NOTE_8_3, NOTE_8_3*1.0f));
    bar->addNote(flamenco.getNote(0),NoteTime(1,NOTE_8_3, NOTE_8_3*2.0f));
    
    //bar->addNote(flamenco.getNote(1),NoteTime(2,NOTE_2));
    //bar->addNote(flamenco.getNote(1),NoteTime(3,NOTE_2));
    //bar->addNote(flamenco.getNote(1),NoteTime(4,NOTE_2));
    
    bar->addChord(I,NoteTime(1,NOTE_8));

    bar->addChord(IV,NoteTime(3,NOTE_8));
     
     
   // track->addBar(*bar);
    
    
    
    for(int i=0;i<4;i++){
        Bar * bar = new Bar();
        //bar->setTimeSignature(5,4);
        bar->addNote(Note("C",6),NoteTime(1, NOTE_8));
        bar->addNote(Note("C",6),NoteTime(2, NOTE_8));
        bar->addNote(Note("C",6),NoteTime(3, NOTE_1));
        bar->addNote(Note("C",6),NoteTime(4, NOTE_1));
        
        bar->addChord(I,NoteTime(1,NOTE_8));
        
        bar->addChord(IV,NoteTime(3,NOTE_8));
        track->addBar(*bar);
    }
    
    
    bar = new Bar();
    bar->setTimeSignature(4,4);
    
    bar->addNote(flamenco.getNote(13),NoteTime(1,NOTE_8_3));
    bar->addNote(flamenco.getNote(10),NoteTime(1,NOTE_8_3, NOTE_8_3*1.0f));
    bar->addNote(flamenco.getNote(9),NoteTime(1,NOTE_8_3, NOTE_8_3*2.0f));
    
    
    bar->addNote(flamenco.getNote(6),NoteTime(1,NOTE_8_3, NOTE_8_3*1.0f));
    bar->addNote(flamenco.getNote(7),NoteTime(1,NOTE_8_3, NOTE_8_3*2.0f));
    bar->addNote(flamenco.getNote(9),NoteTime(2,NOTE_8_3));
    
    
    bar->addNote(flamenco.getNote(13),NoteTime(3,NOTE_8_3));
    bar->addNote(flamenco.getNote(10),NoteTime(3,NOTE_8_3, NOTE_8_3*1.0f));
    bar->addNote(flamenco.getNote(9),NoteTime(3,NOTE_8_3, NOTE_8_3*2.0f));
    
    
    bar->addNote(flamenco.getNote(6),NoteTime(4,NOTE_8_3, NOTE_8_3*1.0f));
    bar->addNote(flamenco.getNote(7),NoteTime(4,NOTE_8_3, NOTE_8_3*2.0f));
    bar->addNote(flamenco.getNote(9),NoteTime(4,NOTE_8_3));
     */
    //track->addBar(*bar);
    
    //bar = new Bar();
    /*bar->addNote(flamenco.getNote(8),NoteTime(1,NOTE_8));
    bar->addNote(flamenco.getNote(9),NoteTime(2,NOTE_8));
    bar->addNote(flamenco.getNote(10),NoteTime(3,NOTE_16));
    bar->addNote(flamenco.getNote(11),NoteTime(4,NOTE_32));
    bar->addNote(flamenco.getNote(10),NoteTime(4,NOTE_32,NOTE_32));
    */
    /*
    bar->addNote(flamenco.getNote(9),NoteTime(4,NOTE_32,NOTE_32*2));
    bar->addNote(flamenco.getNote(8),NoteTime(4,NOTE_32,NOTE_32*3));
    bar->addNote(flamenco.getNote(7),NoteTime(4,NOTE_32,NOTE_32*4));
    bar->addNote(flamenco.getNote(6),NoteTime(4,NOTE_32,NOTE_32*5));
    bar->addNote(flamenco.getNote(5),NoteTime(4,NOTE_32,NOTE_32*6));
    bar->addNote(flamenco.getNote(4),NoteTime(4,NOTE_32,NOTE_32*7));
    bar->addNote(flamenco.getNote(3),NoteTime(4,NOTE_32,NOTE_32*8));
    
    bar->addNote(flamenco.getNote(10),NoteTime(4,NOTE_32,NOTE_32*2));
    bar->addNote(flamenco.getNote(11),NoteTime(4,NOTE_32,NOTE_32*3));
    bar->addNote(flamenco.getNote(12),NoteTime(4,NOTE_32,NOTE_32*4));
    bar->addNote(flamenco.getNote(13),NoteTime(4,NOTE_32,NOTE_32*5));
    bar->addNote(flamenco.getNote(14),NoteTime(4,NOTE_32,NOTE_32*6));
    bar->addNote(flamenco.getNote(15),NoteTime(4,NOTE_32,NOTE_32*7));
    bar->addNote(flamenco.getNote(16),NoteTime(4,NOTE_32,NOTE_32*8));
    
    track->addBar(*bar);
    */
    /*
    bar = new Bar();
    bar->addNote(flamenco.getNote(-3),NoteTime(1,NOTE_8,0,NOTE_8,1));
    bar->addNote(flamenco.getNote(-2),NoteTime(1,NOTE_8,1,NOTE_8,1));
    bar->addNote(flamenco.getNote(-1),NoteTime(1,NOTE_8,2,NOTE_8,1));
    bar->addNote(flamenco.getNote(0),NoteTime(1,NOTE_8,3,NOTE_8,1));
    track->addBar(*bar);
    
   
    //track->addBar(*bar);
    //track->addBar(*bar);
    
    comp.addTrack(*track);
    comp.setLoop(true);
    comp.setBPM(120.000f);
    
    
    
    
    editor.setup();
    editor.noteRangeBegin = 50;
    editor.noteRangeEnd = 130;
    
    editor.h = 600;
    editor.x = 10;
    editor.y = 100;
    editor.w = ofGetWidth()-20;
    
     */
    comp.play();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofBackground(255,255,255);
    
    ofSetColor(0);
    ofDrawBitmapString(output.str(),10,50);
    
    
    //editor.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key ==' '){
        if(comp.isPlaying){
            comp.pause();
        }else{
            comp.play();
        }
    }
    
   
    
	
}

void ofApp::onNoteOn(MusicEvent &e) {
   
}


void ofApp::onNoteOff(MusicEvent &e) {

    
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
       
    comp.setPosition(0);
    comp.play();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::exit(){
    comp.stop();

};