// =============================================================================
//
// ofxMusicTheory.h
// MusicTheory addon for openFrameworks originally based on the Mingus Python library
//
// Created by Andreas Borg on 28/01/2013
//
// Copyright (c) 2015-2016 Andreas Borg <http://crea.tion.to>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================



/*
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

 */

#ifndef _ofxMusicTheory
#define _ofxMusicTheory

#include "ofMain.h"
#include "Note.h"
#include "Interval.h"
#include "Intervals.h"
#include "Chord.h"
#include "Diatonic.h"
#include "Scale.h"
#include "Progression.h"

#endif
