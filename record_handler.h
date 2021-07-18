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

#pragma once

#include <memory>
#include <fluidsynth.h>

#include "handler.h"
#include "track.h"

class RecordHandler : public Handler {

    public:
        RecordHandler(fluid_sequencer_t *sequencer, int seq_synth_id) : 
            sequencer(sequencer),
            seq_synth_id(seq_synth_id),
            current_track(-1) {}
        void handleEvent(fluid_midi_event_t *event);

    private:
        void addNewTrack();
        void recordStart();
        void recordStop();
        void playStart();
        void playStop();
        void loadPreviousTrack();
        void loadNextTrack();
        void maybeRecordEvent(fluid_midi_event_t *event);

    private:
        fluid_sequencer_t *sequencer;
        int seq_synth_id;
        int current_track;
        std::vector<std::unique_ptr<Track>> tracks;

};