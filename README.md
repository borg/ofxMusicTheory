ofxMusicTheory
==============

![image](https://github.com/borg/ofxMusicTheory/blob/master/ofxaddons_thumbnail.png)

A music theory [openFrameworks](http://www.openframeworks.cc/) addon originally based on the [Python Mingus](https://code.google.com/p/mingus/) library

Created by Andreas Borg on 28/01/2013

[crea.tion.to](http://crea.tion.to)


Change log:

    Major rewrite July 2016
    / Changed all objects into shared_ptr. Most functions should return copies, but be careful when passing chords around as they may share pointers to same notes unless first copied.
    + Added factory methods for Note, Chord, Scale (eg. NotePtr n = Note::create("Bb"))
    + Copy returns unique deep copies
    + Updated to C++11 and cleaned up a lot
    + Fixed plenty of bugs
    + Adapted octaves to Ableton midi register where C-3 = 60 (default before was C-4 = 48)
    + Added a range of chords and scales
    - Removed orphan code (eg. melody generator).
    - Removed rhythm & time classes. They were never particularly good.


**Example usage:**

```

    NotePtr C = Note::create("C");
    cout<<C<<endl;
    
    NotePtr C2 = Note::fromInt(60);
    cout<<C2<<endl;
    
    C2->transpose(7);
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
    
    
    ScalePtr mixo = Scale::getMixolydian(Note::create("C",3));
    cout<<mixo<<endl;
    
    ScalePtr prygian = Scale::getPhrygian(Note::create("C",3));
    cout<<prygian<<endl;

    
    ScalePtr flamenco = Scale::getFlamenco(Note::create("E",3));
    cout<<flamenco<<endl;
    
    ScalePtr Zen =  Scale::getInSen(Note::create("C",3));
    
    
    
    
    cout<<"Scales for 7b9b5"<<endl;
    vector<string>scales = Scale::getScalesForChord("7b9b5");
    Scale::print(scales);
    
    
    NotePtr nn  = Interval::fromShorthand(Note::create("C",4), "b3");
    cout<<nn<<endl;
    
    melMin = Scale::getScaleFromString(scales[0], Note::create("C",4));
    cout<<melMin<<endl;
    
    
    cout<<"Progressions"<<endl;
    
    
    
    
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
    
```
Output:

```
Note C-3 (C-60)
Note C-3 (C-60)
Note G-3 (G-67)
Aug Note C#-3 (C#-61)
Dim Note C-3 (C-60)
Dim Note B-2 (Cb-59)
Note E-2 (C####-52)
Diatonic name: E
Oct down: Note E-1 (C####-40)
Get diatones of: Note C-3 (C-60)
Note C-3 (C-60)
Note D-3 (D-62)
Note E-3 (E-64)
Note F-3 (F-65)
Note G-3 (G-67)
Note A-3 (A-69)
Note B-3 (B-71)
Absolute sixth of Note C-3 (C-60) is Note A-3 (A-69)
Min second of Note C-3 (C-60) is Note C#-3 (C#-61)
Maj second of Note C-3 (C-60) is Note D-3 (D-62)
Min third of Note C-3 (C-60) is Note D#-3 (D#-63)
Maj third of Note C-3 (C-60) is Note E-3 (E-64)
Min fourth of Note C-3 (C-60) is Note E-3 (E-64)
Perfect fourth of Note C-3 (C-60) is Note F-3 (F-65)
Maj fourth of Note C-3 (C-60) is Note F-3 (F-65)
Min fifth of Note C-3 (C-60) is Note F#-3 (F#-66)
Perfect fifth of Note C-3 (C-60) is Note G-3 (G-67)
Min sixth of Note C-3 (C-60) is Note G#-3 (G#-68)
Maj sixth of Note C-3 (C-60) is Note A-3 (A-69)
Min seven of Note C-3 (C-60) is Note A#-3 (A#-70)
Maj seven of Note C-3 (C-60) is Note B-3 (B-71)
 minor/major seventh This one not found:Chord not found
Chord Cm7b5 [ Note C-3 (C-60), Note D#-3 (D#-63), Note F#-3 (F#-66), Note A#-3 (A#-70) ]

Subtonic to Note C-3 (C-60) is Chord C7 [ Note C-3 (C-60), Note E-3 (E-64), Note G-3 (G-67), Note B-3 (B-71) ]

Subdominant to Note C-3 (C-60) is Chord F/C [ Note C-3 (C-60), Note F-3 (F-65), Note A-3 (A-69) ]

Subdominant first inversion is Chord F [ Note F-3 (F-65), Note A-3 (A-69), Note C-4 (C-72) ]

Ext fifth eg.Chord CM9 [ Note C-3 (C-60), Note E-3 (E-64), Note G-3 (G-67), Note B-3 (B-71), Note D-4 (D-74) ]

Triad analysis 
[ Note F-3 (F-65), Note G-3 (G-67), Note C-3 (C-60) ]
Opt 0: Fsus2
Opt 1: Csus4

Seventh analysis 
[ Note C-3 (C-60), Note E-3 (E-64), Note G-3 (G-67), Note A-3 (A-69) ]
Opt 0: CM6
Opt 1: Am7
Opt 2: Am7|CM

Extended  fifth analysis 
[ Note C-3 (C-60), Note E-3 (E-64), Note G-3 (G-67), Note B-3 (B-71), Note D-3 (D-62) ]
Opt 0: CM9
Opt 1: GM|CM
Opt 2: GM|CM7
Opt 3: Em7|CM
Opt 4: GM6|CM
Opt 5: Em7|CM7
Opt 6: GM6|CM7

Extended  sixth analysis 
[ Note C-3 (C-60), Note E-3 (E-64), Note G-3 (G-67), Note B-3 (B-71), Note D-3 (D-62), Note A-3 (A-69) ]
Opt 0: CM13
Opt 1: Am11
Opt 2: Bm7|CM
Opt 3: Bm7|CM7
Opt 4: Bm7|CM9
Opt 5: Em11|CM
Opt 6: G6/9|CM
Opt 7: Em11|CM7
Opt 8: G6/9|CM7
Opt 9: Em11|CM9
Opt 10: G6/9|CM9

Extended  seventh analysis 
[ Note C-3 (C-60), Note E-3 (E-64), Note G-3 (G-67), Note B-3 (B-71), Note D-4 (D-74), Note F-3 (F-65), Note A-3 (A-69) ]
Opt 0: CM13
Opt 1: G13
Opt 2: Dm13
Opt 3: Dm|CM
Opt 4: Dm|CM7
Opt 5: Dm|CM9
Opt 6: Dm|CM11
Opt 7: Bm7b5|CM
Opt 8: Dm6|CM
Opt 9: Bm7b5|CM7
Opt 10: Dm6|CM7
Opt 11: Bm7b5|CM9
Opt 12: Dm6|CM9
Opt 13: Bm7b5|CM11
Opt 14: Dm6|CM11
Opt 15: G9|CM
Opt 16: G9|CM7
Opt 17: G9|CM9
Opt 18: G9|CM11
Opt 19: G13|CM
Opt 20: G13|CM7
Opt 21: G13|CM9
Opt 22: G13|CM11

Polychord analysis
[ Note C-3 (C-60), Note E-3 (E-64), Note G-3 (G-67), Note F#-3 (F#-66), Note A-3 (A-69), Note C#-4 (C#-73) ]
Opt 0: F#m|CM
Opt 1: A13|CM

Scales
Scale F-3 lydian [ Note F-3 (F-65), Note G-3 (G-67), Note A-3 (A-69), Note B-3 (B-71), Note C-4 (C-72), Note D-4 (D-74), Note E-4 (E-76) ]

Scale C-3 melodicMinor [ Note C-3 (B#-60), Note D-3 (C##-62), Note D#-3 (D#-63), Note F-3 (E#-65), Note G-3 (F##-67), Note A-3 (G##-69), Note B-3 (A##-71) ]

Scale C-3 pentatonicMajor [ Note C-3 (C-60), Note D-3 (D-62), Note E-3 (E-64), Note G-3 (G-67), Note A-3 (A-69) ]

Scale C-3 hirajoshi [ Note C-3 (C-60), Note C#-3 (C#-61), Note F-3 (F-65), Note G-3 (G-67), Note G#-3 (G#-68) ]

Scale C-3 mixolydian [ Note C-3 (C-60), Note D-3 (D-62), Note E-3 (E-64), Note F-3 (F-65), Note G-3 (G-67), Note A-3 (A-69), Note Bb-3 (Bb-70) ]

Scale C-3 phrygian [ Note C-3 (B#-60), Note C#-3 (C#-61), Note D#-3 (D#-63), Note F-3 (E#-65), Note G-3 (F##-67), Note G#-3 (G#-68), Note A#-3 (A#-70) ]

Scale E-3 flamenco [ Note E-3 (E-64), Note F-3 (F-65), Note G-3 (G-67), Note G#-3 (G#-68), Note A-3 (A-69), Note B-3 (B-71), Note C-4 (C-72), Note D-4 (D-74) ]

Scales for 7b9b5
[ melodicMinorVII ]
Note D#-4 (D#-75)
Scale C-4 melodicMinorVII [ Note C-4 (B#-72), Note C#-4 (C#-73), Note D#-4 (D#-75), Note E-4 (E-76), Note F#-4 (F#-78), Note G#-4 (G#-80), Note A#-4 (A#-82) ]

Progressions
V7 to Note C-3 (C-60) is Chord G7/D [ Note D-3 (D-62), Note F-3 (F-65), Note G-3 (G-67), Note B-3 (B-71) ]

VIdim7 to Note C-3 (C-60) is Chord Adim7 [ Note A-3 (A-69), Note C-4 (C-72), Note D#-4 (D#-75), Note Gb-4 (Gb-78) ]

bIIdim7,VM7 where I is Note C-3 (C-60)
[ Chord C#dim7 [ Note C#-3 (C#-61), Note E-3 (E-64), Note G-3 (G-67), Note Bb-3 (Bb-70) ]
, Chord GM7 [ Note G-3 (G-67), Note B-3 (B-71), Note D-4 (D-74), Note F#-4 (F#-78) ]
 ]

Harmonic substitution of IV7 [ II7, VI7 ]

Minor for major substitution of VIm7 [ IM7 ]

Major for minor substitution of VM7 [ IIIm7 ]

Dim for dim substitution of VII [ IIdim, IVdim, bVIdim ]

Dim for dom substitution of VIIdim [ Vdom7, bVIIdom7, bIIdom7, IVdom7 ]
Chord C [ Note C-3 (C-60), Note E-3 (E-64), Note G-3 (G-67) ]

Chord D [ Note D-3 (D-62), Note F-3 (F-65), Note A-3 (A-69) ]

Chord Fdim [ Note F-3 (F-65), Note G#-3 (G#-68), Note B-3 (B-71) ]

Chord G7/D [ Note D-3 (D-62), Note F-3 (F-65), Note G-3 (G-67), Note B-3 (B-71) ]
```