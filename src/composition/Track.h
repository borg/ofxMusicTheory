/*
 *  Track.h
 *  MusicTheory
 *
 *  Created by Andreas Borg on 11/02/2013
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Track
#define _Track

#include "ofMain.h"
#include "Bar.h"
#include "MusicEvent.h"
#include "ofxXmlSettings.h"

/*
 The tracks should be the time listeners and manage
 all note events.
 
 To optimize the number of iterations required per tick we want to 
 shorten the event loop and only iterate through current
 beat. (Similar to a quadtree approach).
 
 Thus note events need to be parsed when bar is added to track,
 yet still belong to the bars for editing purposes.
 
 
 How to keep all events in sync? 
 
 */

namespace MusicTheory{

    
    typedef struct TrackEvent{
        vector<UniqueNote> notes;
        bool isOnEvent;//else off
        double time;//millis after beat
        bool fired;
        //same note can be fired many times...keep track of which when overlapping
        string noteName;
        string chordName;
    }TrackEvent;
    
    
    
    //class Playable;//forward
    
class Track {
	
  public:
    string name;
    string uid;//unique identifier
    
    
    
    int currBar;
    int currBeat;
    int barCurrBeat;
    int currTimeSignature;//top half of 4/4
    int currDenominator;//bottom
    double currBarDuration;
    double beatInMilli;//the beat is the duration of the current denominator
    
    
    int totalBeats;
    double beginTime;
    
    double bpm;
    
    int noteCounter;
    
   // Playable *playable;
    
    Bar *currBarPtr;
    
    map<int,vector<TrackEvent> > events;//subdivide events per beat
    
    map<int,Bar> timeline;//timeline starts on 1 to be compliant with normal sequencers. Used for playback
    
    vector<Bar> bars;//used for generating voicing, reharmonisation and phrases
    
	Track(){
        name = "Untitled track";
        currBeat = 1;
        barCurrBeat=1;
        currBar = 0;
        totalBeats = 0;
        currBarPtr = 0;
        bpm = 120;
        beatInMilli = 60000.0f/bpm;
        
        noteCounter=0;
 
    };
    
    ~Track(){
        //TODO: Sort out GC
        removeListeners();
        for(int i=0;i<bars.size();i++){
           // bars[i]->removeListeners();
           // delete bars[i];
        }
        bars.clear();
        
    };

    
    void addListeners(){
        
//        playable = owner;
       // ofAddListener(MusicEvent::LOOP,this,&Track::onPlay);
       // ofAddListener(MusicEvent::LOOP,this,&Track::onStop);
        //ofAddListener(MusicEvent::LOOP,this,&Track::onLoop);
        /*ofAddListener(MusicEvent::TEMPO_CHANGE,this,&Track::onTempoChange);
        ofAddListener(MusicEvent::TICK,this,&Track::onTick);
        ofAddListener(MusicEvent::BEAT,this,&Track::onBeat);
        ofAddListener(MusicEvent::SET_POSITION,this,&Track::onSetPosition);*/
    }
    void removeListeners(){
        //TODO: By God reinstate
        /*
        ofRemoveListener(MusicEvent::LOOP,this,&Track::onPlay);
        ofRemoveListener(MusicEvent::LOOP,this,&Track::onStop);
        ofRemoveListener(MusicEvent::LOOP,this,&Track::onLoop);
        ofRemoveListener(MusicEvent::TEMPO_CHANGE,this,&Track::onTempoChange);
        ofRemoveListener(MusicEvent::TICK,this,&Track::onTick);
        ofRemoveListener(MusicEvent::BEAT,this,&Track::onBeat);
        ofRemoveListener(MusicEvent::SET_POSITION,this,&Track::onSetPosition);*/
    }
      	

    

    //this is probably inconsistent...better just use pointers or smart pointers
    void addBar(Bar &bar){
        
        
        
        //a whole note is always 4 quarters...so in a 6/8 it would be longer than the bar, namely
        //2 beats longer
           
        double wholeNoteInMilli = beatInMilli*(double)bar.denominator;
        
        BarEvent b;
        double begin,duration,end;
        for(int i=0;i<bar.notes.size();i++){
            b = bar.notes[i];
            begin = b.time.getBegin()*wholeNoteInMilli;
            duration = b.time.getDuration()*wholeNoteInMilli;
            
            UniqueNote un;
            un.note = b.note;
            un.uid = ofToString(noteCounter);
            noteCounter++;
            
            //NOTE ON EVENT
            TrackEvent te;
            te.notes.push_back(un);
            te.isOnEvent = true;//else off
            te.fired = false;
            te.noteName = b.note.getDiatonicName();
            
            
            //is the start beat to be parsed later than the current beat?
            //ie. is the delay extending beyond this beat (b.time.beat)?
            int noteEventDelay = floor(begin/beatInMilli);//0 - n number of beats
            
            int startBeat =  totalBeats+b.time.beat+noteEventDelay;
            
            te.time = begin - (double)noteEventDelay*beatInMilli;//mill
            events[startBeat].push_back(te);//copy?
            
            
            //NOTE OFF EVENT
            te.isOnEvent = false;
            
            //is this longer than this beat?
            //we want all events to slot in at the right beat where they actually happen
            int noteEventEnd = floor((begin+duration)/beatInMilli);
            int endBeat =  totalBeats+b.time.beat+noteEventEnd;
            
            
            
            te.time = begin+duration-beatInMilli*noteEventEnd;//mill
                       
            events[endBeat].push_back(te);//copy?
            
        }
        
        for(int i=0;i<bar.chords.size();i++){
            b = bar.chords[i];
            
            //chords have the same duration
            begin = b.time.getBegin()*wholeNoteInMilli;
            duration = b.time.getDuration()*wholeNoteInMilli;
            
            
            vector<UniqueNote> chordNotes;
            for(int ii=0;ii<b.chord.getAllNotes().size();ii++){
                UniqueNote un;
                un.note = b.chord.getAllNotes()[ii];
                un.uid = ofToString(noteCounter);
                chordNotes.push_back(un);
                noteCounter++;
            }
            
                
                TrackEvent te;
                te.notes.insert(te.notes.begin(), chordNotes.begin(), chordNotes.end());
                te.isOnEvent = true;//else off
                te.fired = false;
               
                te.chordName = b.chord.getName();
            
            
                //is the start beat to be parsed later than the current beat?
                //ie. is the delay extending beyond this beat (b.time.beat)?
                int noteEventDelay = floor(begin/beatInMilli);//0 - n number of beats
                
                int startBeat =  totalBeats+b.time.beat+noteEventDelay;
                
                te.time = begin-(double)noteEventDelay*beatInMilli;//mill
                events[startBeat].push_back(te);//copy?
            
            
                //NOTE OFF EVENT
                te.isOnEvent = false;
                
            
                //is this longer than this beat?
                int noteEventEnd = floor((begin+duration)/beatInMilli);
                int endBeat =  totalBeats+b.time.beat+noteEventEnd;
                te.time = begin+duration-beatInMilli*noteEventEnd;//mill

                events[endBeat].push_back(te);//copy?
            //}
            
        }
        
        //add bar info to each beat of the track timeline
        
        for(int i =1;i<=bar.beats;i++){
            timeline[totalBeats+1] = bar;
            totalBeats++;
        }
        
        //
        bars.push_back(bar);
        
    };
    
    void addBars(deque<Bar> _bars){
        //bars.insert(bars.end(),_bars.begin(),_bars.end());
    };
        
    void generateTimeline(){
        
    }
    
    
    void onSetPosition(MusicEvent &e){
        currBeat = e.value+1;
        
        resetEvents(currBeat);
        
    }
    
    
    void onTick(MusicEvent &e){
        
         //cout<<"trsack on tick"<<endl;
        if(!(beginTime>0)){
            return;
        }
        
        double currTime = ofGetElapsedTimeMillis() - beginTime;
        
        //a whole note is always 4 quarters...so in a 6/8 it would be longer than the bar, namely
        //2 beats longer

        double wholeNoteInMilli = beatInMilli*(double)currDenominator;
        
        //double beatInMilli = wholeNoteInMilli/(float)currDenominator;
        
       //cout<<currBeat<<"  Curr time "<<currTime<<" beatInMilli "<<beatInMilli<<" currTimeSignature "<<currTimeSignature<<" currDenominator "<<currDenominator<<endl;
        
        deque<UniqueNote> on_notes;
        deque<UniqueNote> off_notes;
        string meta="";
        
        
        
        TrackEvent *te;
        //cout<<"currBeat "<<currBeat<<endl;
        for(int i =0; i<events[currBeat].size();i++){
            
            te = &events[currBeat][i];
            double eventTime = te->time;
   
            
            if(eventTime<=currTime && te->isOnEvent && !te->fired){
                
                on_notes.insert(on_notes.end(), te->notes.begin(),te->notes.end());
                te->fired = true;
                if(te->chordName.size()>0){
                    meta+=te->chordName;
                }
            }else if(eventTime<=currTime && !te->isOnEvent && !te->fired){
                
                off_notes.insert(off_notes.end(), te->notes.begin(),te->notes.end());
                te->fired = true;
            }            
            
            
        }
        
        if(on_notes.size()>0){
            MusicEvent mEvent;
            mEvent.notes = on_notes;
            mEvent.meta = meta;
            ofNotifyEvent(MusicEvent::NOTE_ON,mEvent);
        }
        
        if(off_notes.size()>0){
            MusicEvent mEvent;
            mEvent.notes = off_notes;
            ofNotifyEvent(MusicEvent::NOTE_OFF,mEvent);
        }
        

    
    };
    
    void onLoop(MusicEvent &e) {
       
        resetEvents();
        
        currBeat=1;
        
    }
    /*
     Needed for rewind
     */
    void resetEvents(int from=1){
        typedef map<int,vector<TrackEvent> >::iterator it_type;
        for(it_type iterator = events.begin(); iterator != events.end(); iterator++) {
            // iterator->first = key
            for(int i=0; i<iterator->second.size() && iterator->first>=from; i++) {
                // iterator->second = value
                iterator->second[i].fired = false;//reset fired events
            }
        }
    }
    
    void onPlay(MusicEvent &e) {
        //cout<<"track got on play"<<endl;
        currBeat=1;
        
        currBarPtr = &timeline[currBeat];
    }
    
    void onStop(MusicEvent &e) {
        
    }
    
    void onBeat(MusicEvent &e) {
       // cout<<"trsack on beat"<<endl;
        
        if(totalBeats>e.position){

            
            //currBarPtr->syncBeat(barCurrBeat);
            //cout<<name<<" got music event Pos: "<<e.position<<", currBeat: "<<currBeat<<", totalBeats: "<<totalBeats<<endl;

            /*
             Beats start on 1 position on 0
             */
            currBeat=(e.position+1);//total track beats
            /*
            cout<<"barCurrBeat "<<barCurrBeat<<endl;
            cout<<"currBarPtr "<<currBarPtr<<endl;
            cout<<"currBeat "<<currBeat<<endl;*/
            
            if(!currBarPtr){
                
                currBarPtr = &timeline[currBeat];
                
            }
            
                if(barCurrBeat >= currBarPtr->beats || currBeat==1){
                    barCurrBeat=1;
                }else{
                    barCurrBeat++;//inside bar
                }
            
            
            
            
            currDenominator = currBarPtr->denominator;
            currTimeSignature = currBarPtr->beats;
                 
            beginTime = ofGetElapsedTimeMillis();
            
            
            
            }

           
        

    }
    
    
    void onTempoChange(MusicEvent &e) {
            //bpm change..this one is between beat...desired?
            bpm = e.value;
            beatInMilli = 60000.0f/bpm;
    }
 
	void load(string file){
        
        ofxXmlSettings xml;
        xml.loadFile(file);
        xml.pushTag("data");
        //assuming only one track
        xml.pushTag("track");
        
        Track::xmlToTrack(xml,*this);
        
        xml.popTag();
        xml.popTag();
    }
    void save(string file){
        
        ofxXmlSettings xml;
        //save to xml
        xml.addTag("data");
        xml.pushTag("data");
        
        
        
        //go to right pos in xml
        Track::appendTrackToXML(xml,*this);
        
        
        xml.popTag();
        
        xml.saveFile(file);
    
    }
    
    /*
     Because of the awkward state nature of ofxXmlSettings you need to push to the right
     spot in the document before calling this funtion.
     */
    static void appendTrackToXML(ofxXmlSettings &xml,Track &track){
        xml.addTag("track");
        int trackNum = xml.getNumTags("track");
        xml.pushTag("track",trackNum-1);
        
        for(int i=0;i<track.bars.size();i++){
            xml.addTag("bar");
            
            xml.setAttribute("bar", "beats",track.bars[i].beats, i);
            xml.setAttribute("bar", "denominator",track.bars[i].denominator, i);
            
            
            xml.pushTag("bar",i);
            //add all notes
            for(int ii=0;ii<track.bars[i].notes.size();ii++){
                xml.addTag("note");
                // xml.pushTag("note",ii);
                xml.setAttribute("note", "name",track.bars[i].notes[ii].note.getDiatonicName(), ii);
                xml.setAttribute("note", "int",track.bars[i].notes[ii].note.getInt(), ii);
                xml.setAttribute("note", "octave",track.bars[i].notes[ii].note.getOctave(), ii);
                
                xml.setAttribute("note", "beat",track.bars[i].notes[ii].time.getBeat(), ii);
                xml.setAttribute("note", "begin",track.bars[i].notes[ii].time.getBegin(), ii);
                xml.setAttribute("note", "duration",track.bars[i].notes[ii].time.getDuration(), ii);
                
                //xml.popTag();
            }
            
            
            //chords
            for(int ii=0;ii<track.bars[i].chords.size();ii++){
                xml.addTag("chord");
                // xml.pushTag("note",ii);
                xml.setAttribute("chord", "name",track.bars[i].chords[ii].chord.getName(), ii);
                xml.setAttribute("chord", "root",track.bars[i].chords[ii].chord.getRoot().getName(), ii);
                
                xml.setAttribute("chord", "beat",track.bars[i].chords[ii].time.getBeat(), ii);
                xml.setAttribute("chord", "begin",track.bars[i].chords[ii].time.getBegin(), ii);
                xml.setAttribute("chord", "duration",track.bars[i].chords[ii].time.getDuration(), ii);
            }
            xml.popTag();
        }
        //go back out
        xml.popTag();
    }
    
    static void xmlToTrack(ofxXmlSettings &xml,Track &track){
        
        
        
        int num = xml.getNumTags("bar");
        
        Bar * bar;
        
        for(int i=0;i<num;i++){
            
            bar = new Bar();
            
            
            int b= xml.getAttribute("bar", "beats",4, i);
            int d = xml.getAttribute("bar", "denominator",4, i);
            bar->setTimeSignature(b, d);
            
            xml.pushTag("bar",i);
            
            int noteNum = xml.getNumTags("note");
            for(int ii=0;ii<noteNum;ii++){
                
                int nInt = xml.getAttribute("note", "int",0, ii);
                Note note = Note::fromInt(nInt);
                
                int beat = xml.getAttribute("note", "beat",1, ii);
                double begin = xml.getAttribute("note", "begin",0.0f, ii);
                double duration = xml.getAttribute("note", "duration",0.0f, ii);
                NoteTime nt(beat,duration,begin);
                
                bar->addNote(note,nt);
                
            }
            
            //TODO: Same with chords
            
            xml.popTag();
            
            
            track.addBar(*bar);
        }
        

    }
};
};

#endif
