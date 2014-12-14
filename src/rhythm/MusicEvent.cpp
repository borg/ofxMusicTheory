#include "MusicEvent.h"

//instantiate statics
using namespace MusicTheory;

 ofEvent <MusicEvent> MusicEvent::NOTE_ON;
 ofEvent <MusicEvent> MusicEvent::NOTE_OFF;
 ofEvent <MusicEvent> MusicEvent::TICK;
 ofEvent <MusicEvent> MusicEvent::BEAT;
 ofEvent <MusicEvent> MusicEvent::TEMPO_CHANGE;
 ofEvent <MusicEvent> MusicEvent::LOOP;
 ofEvent <MusicEvent> MusicEvent::PLAY;
 ofEvent <MusicEvent> MusicEvent::STOP;
 ofEvent <MusicEvent> MusicEvent::SET_POSITION;
 ofEvent <MusicEvent> MusicEvent::CLEAR_TRACK;
 ofEvent <MusicEvent> MusicEvent::TIMER_TICK;
 ofEvent <MusicEvent> MusicEvent::TIMER_BEAT;
