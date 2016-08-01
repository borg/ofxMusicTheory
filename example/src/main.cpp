#include "ofMain.h"
#include "ofxMusicTheory.h"

using namespace MusicTheory;
//--------------------------------------------------------------
int main(){
    NotePtr C = Note::create("C");
    cout<<C<<endl;
    
    NotePtr C2 = Note::fromInt(60);
    cout<<C2<<endl;
    
    NotePtr b = C->getAugmented();
    
    C->augment();
    cout<<"Aug "<<C<<endl;
    
    C->diminish();
    cout<<"Dim "<<C<<endl;
    
    C->diminish();
    cout<<"Dim "<<C<<endl;
    
    
    b->changeOctave(-1);
    b->augment();
    b->augment();
    b->augment();
    cout<<b<<endl;
    
    
    cout<<"Diatonic name: "<<b->getDiatonicName()<<endl;
    
    NotePtr c = b->getOctaveDown();
    cout<<"Oct down: "<<c<<endl;
    
    NotePtr r = Note::create("C");
   
    cout<<"Get diatones of: "<<r<<endl;
    
    deque<NotePtr> scale = Diatonic::getNotes(r);
     
    for(int i=0;i<scale.size();i++){
        cout<<scale[i]<<endl;
    }

    
    NotePtr root = Note::create("C");
    
    
    NotePtr abs6 = Interval::sixth(root,root);
    cout<<"Absolute sixth of "<<root<<" is "<<abs6<<endl;
    
    
    
    NotePtr abs4 = Interval::sixth(root,root);
    
        
    NotePtr m2 = Interval::minorSecond(root);
    cout<<"Min second of "<<root<<" is "<<m2<<endl;
    
    NotePtr M2 = Interval::majorSecond(root);
    cout<<"Maj second of "<<root<<" is "<<M2<<endl;
    
    
    NotePtr m3 = Interval::minorThird(root);
    cout<<"Min third of "<<root<<" is "<<m3<<endl;
    
    NotePtr M3 = Interval::majorThird(root);
    cout<<"Maj third of "<<root<<" is "<<M3<<endl;
    
    
    NotePtr m4 = Interval::minorFourth(root);
    cout<<"Min fourth of "<<root<<" is "<<m4<<endl;
    
    NotePtr P4 = Interval::perfectFourth(root);
    cout<<"Perfect fourth of "<<root<<" is "<<P4<<endl;
    
    NotePtr M4 = Interval::majorFourth(root);
    cout<<"Maj fourth of "<<root<<" is "<<M4<<endl;
    
    
    NotePtr m5 = Interval::minorFifth(root);
    cout<<"Min fifth of "<<root<<" is "<<m5<<endl;
    
    NotePtr P5 = Interval::perfectFifth(root);
    cout<<"Perfect fifth of "<<root<<" is "<<P5<<endl;
    
    NotePtr m6 = Interval::minorSixth(root);
    cout<<"Min sixth of "<<root<<" is "<<m6<<endl;
    
    NotePtr M6 = Interval::majorSixth(root);
    cout<<"Maj sixth of "<<root<<" is "<<M6<<endl;
    
    NotePtr m7 = Interval::minorSeventh(root);
    cout<<"Min seven of "<<root<<" is "<<m7<<endl;

    NotePtr M7 = Interval::majorSeventh(root);
    cout<<"Maj seven of "<<root<<" is "<<M7<<endl;
    
    
    
    string m = Chord::getFullName("mM7");
    
    cout<<Chord::getFullName("mM7")<<" This one not found:" <<Chord::getFullName("oddname")<<endl;
    //ofLog()<<a->name<<endl;
    
    
    ChordPtr ch = Chord::halfDiminishedSeventh(root);
    cout<<ch<<endl;;
    
    
    ChordPtr st = Chord::tonic7(root);
    cout<<"Subtonic to "<<root<<" is "<<st<<endl;
    
    
    ChordPtr subdom = Chord::subdominant(root);
    cout<<"Subdominant to "<<root<<" is "<<subdom<<endl;
    
    
    ChordPtr subdomInv = Chord::firstInversion(subdom);
    cout<<"Subdominant first inversion is "<<subdomInv<<endl;
    
    
    /*
     Extended fifth chords are made of built of 5 notes at third dist
     */
    
    ChordPtr ext5 = Chord::majorNinth(root);
    cout<<"Ext fifth eg."<<ext5<<endl;
    
    

    bool useShorthand = true;
    bool allowInvensions = true;
    bool allowPolychords = true;
    
    deque<NotePtr> triad = {
        {Note::create("F")},
        {Note::create("G")},
        {Note::create("C")}
    };
    
    cout<<"Triad analysis "<<endl;
    Chord::print(triad);
    vector<string> analyse = Chord::analyse(triad, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        cout<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    
    
    deque<NotePtr> seventh = {
        {Note::create("C")},
        {Note::create("E")},
        {Note::create("G")},
        {Note::create("A")}
    };
    
    cout<<endl<<"Seventh analysis "<<endl;
    
    Chord::print(seventh);
    
    analyse = Chord::analyse(seventh, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        cout<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    
    deque<NotePtr> ext = {
        {Note::create("C")},
        {Note::create("E")},
        {Note::create("G")},
        {Note::create("B")},
        {Note::create("D")}
    };

    cout<<endl<<"Extended  fifth analysis "<<endl;
    Chord::print(ext);
    analyse = Chord::analyse(ext, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        cout<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    
    cout<<endl<<"Extended  sixth analysis "<<endl;
    ext.push_back(Note::create("A"));
    Chord::print(ext);
    analyse = Chord::analyse(ext, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        cout<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    

    
    
    cout<<endl<<"Extended  seventh analysis "<<endl;
    
    ChordPtr ext7 = Chord::majorNinth(root);
    
    //add 11 and 13 to the 9th
    ext7->notes.push_back(Interval::fourth(root, root));
    ext7->notes.push_back(Interval::sixth(root, root));
    
    //cout<<"Ext seventh eg."<<ext7<<endl;
    
    //turn vector into deque...(using deque since internally we need to shuffle aruond to test different inversions
    deque<NotePtr> ch13;
    ch13.insert(ch13.begin(),ext7->notes.begin(),ext7->notes.end());
    
    Chord::print(ch13);
    
    analyse = Chord::analyse(ch13, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        cout<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }

    
    
    
    allowPolychords = true;
    
    
    cout<<endl<<"Polychord analysis"<<endl;
    
    deque<NotePtr> poly = Chord::triad(Note::create("C"),Note::create("C"))->notes;
    
    deque<NotePtr> chord2= Chord::minorTriad(Note::create("F#"))->notes;
    
    poly.insert(poly.end(), chord2.begin(),chord2.end());
    
    
    Chord::print(poly);
    
    analyse = Chord::analyse(poly, useShorthand,allowInvensions,allowPolychords);
    for(int i = 0;i<analyse.size();i++){
        cout<<"Opt "<<i<<": "<<analyse[i]<<endl;
    }
    
    
    
    cout<<endl<<"Scales"<<endl;
    ScalePtr lyd = Scale::getLydian(Note::create("F"));
    cout<<lyd<<endl;
    


    ScalePtr melMin = Scale::getMelodicMinor(Note::create("C"));
    cout<<melMin<<endl;
    
    ScalePtr pentaMaj = Scale::getPentatonicMajor(Note::create("C"));
    cout<<pentaMaj<<endl;
    
    
    ScalePtr hirajoshi = Scale::getHirajoshi(Note::create("C"));
    cout<<hirajoshi<<endl;
    
    
    cout<<"V7 to "<<root<<" is ";
    ChordPtr dom7= Progression::getChordFromRoman("V7",root);
    cout<<dom7<<endl;
    
    cout<<"VIdim7 to "<<root<<" is ";
  
    
    
    
    ChordPtr pchord = Progression::getChordfromChordFunction("VIdim7",root);//add dim etc
    cout<<pchord<<endl;
    
    
    string progStr = "bIIdim7,VM7";
    deque<ChordPtr> prog = Progression::fromString(progStr,root);
    
    cout<<progStr<<" where I is "<<root<<endl;
    Progression::print(prog);
    
    
    string subtituteMe = "IV7";
    cout<<endl<<"Harmonic substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteHarmonic(subtituteMe));
    
    
    subtituteMe = "VIm7";
    cout<<endl<<"Minor for major substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteMinorForMajor(subtituteMe));
    
    
    subtituteMe = "VM7";
    cout<<endl<<"Major for minor substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteMajorForMinor(subtituteMe));

    subtituteMe = "VII";
    cout<<endl<<"Dim for dim substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteDiminishedForDiminished(subtituteMe));

    
    subtituteMe = "VIIdim";
    cout<<endl<<"Dim for dom substitution of "<<subtituteMe<<" ";
    Progression::print(Progression::substituteDiminishedForDominant(subtituteMe));
    
    
  
    //NotePtr root("E",7);
    
    
    
    ChordPtr I = Progression::getChordfromChordFunction("I",root);
    cout<<I<<endl;
    
    ChordPtr II = Progression::getChordfromChordFunction("II",root);
    cout<<II<<endl;
    
    
    ChordPtr IV = Progression::getChordfromChordFunction("IVdim",root);
    cout<<IV<<endl;

    ChordPtr V7 = Progression::getChordfromChordFunction("V7",root);
    cout<<V7<<endl;
    
    cout<<root->name<<" ----------------------------"<<endl;
    
    ScalePtr mixo = Scale::getMixolydian(Note::create("C",6));
    cout<<mixo<<endl;
    
    ScalePtr prygian = Scale::getPhrygian(Note::create("C",6));
    cout<<prygian<<endl;

    
    ScalePtr flamenco = Scale::getFlamenco(Note::create("E",6));
    cout<<flamenco<<endl;
    
    ScalePtr Zen =  Scale::getInSen(Note::create("C",6));
    
    cout<<"Scales for 7b9b5"<<endl;
    vector<string>scales = Scale::getScalesForChord("7b9b5");
    Scale::print(scales);
    
    
    NotePtr nn  = Interval::fromShorthand(Note::create("C",6), "b3");
    cout<<nn<<endl;
    
    melMin = Scale::getScaleFromString(scales[0], Note::create("C",6));
    cout<<melMin<<endl;
    

    
}
