/**
 * Fluidsynth for ImpactLX49+
 * 
 * Copyright (C) 2021 Thomas Keck
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "record_handler.h"
#include "midi_enums.h"



void RecordHandler::handleEvent(fluid_midi_event_t *event) {
    if (fluid_midi_event_get_type(event) == midi_event_type::CONTROL_CHANGE) {
        switch(fluid_midi_event_get_control(event)) {
            case midi_cc::RECORD:
                recordStart();
                return;
            case midi_cc::PLAY:
                playStart();
                return;
            case midi_cc::STOP:
                recordStop();
                playStop();
                return;
            case midi_cc::FORWARD:
                loadNextTrack();
                return;
            case midi_cc::BACKWARD:
                loadPreviousTrack();
                return;
        }
    }
    maybeRecordEvent(event);
}


void RecordHandler::addNewTrack() {
    current_track = tracks.size();
    tracks.push_back(std::make_unique<Track>(sequencer, seq_synth_id));
}

void RecordHandler::recordStart() {
    // Add new track if necessary
    if (current_track == -1) {
        addNewTrack();
    }
    tracks[current_track]->recordStart();
}

void RecordHandler::recordStop() {
    if (current_track >= 0) {
        tracks[current_track]->recordStop();
    }
}

void RecordHandler::playStart() {
    if (current_track >= 0) {
        tracks[current_track]->playStart();
    }
}

void RecordHandler::playStop() {
    if (current_track >= 0) {
        tracks[current_track]->playStop();
    }
}

void RecordHandler::loadPreviousTrack() {
    if (current_track > 0) {
        tracks[current_track]->recordStop();
        current_track--;
    }
}
    
void RecordHandler::loadNextTrack() {
    if (current_track >= 0) {
        tracks[current_track]->recordStop();
    }
    if (current_track + 1 < tracks.size()) {
        current_track++;
    } else {
        addNewTrack();
    }
}

void RecordHandler::maybeRecordEvent(fluid_midi_event_t *event) {
    if (current_track >= 0) {
        tracks[current_track]->maybeRecordMidiEvent(event);
    }
}