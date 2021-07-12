/**
 * Fluidsynth for ImpactLX48P
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

#include <vector>
#include <utility>
#include <fluidsynth.h>

#define CALLBACK_TIME 50

class Track {

    public:
        Track(fluid_sequencer_t* sequencer, int seq_synth_id);
        ~Track();
        void recordStart();
        void recordStop();
        void playStart();
        void playStop();
        bool isPlaying() const;
        bool isRecording() const;

        void playNextChunk();
        void maybeRecordMidiEvent(fluid_midi_event_t* event);

    private:
        int getRecordDuration() const;
        int getPlayDuration() const;
        int getScheduledPlayDuration() const;
        int getRemainingPlayDuration() const;

		void scheduleNextCallback();

        fluid_event_t* convertMidiEventToEvent(fluid_midi_event_t* midi_event);

  private:
    fluid_sequencer_t *sequencer;
    int seq_synth_id;
    int seq_client_id;
    bool is_recording;
    bool is_playing;
	int record_start_time;
    int record_stop_time;
	int play_start_time;
    int play_current_time;
    std::vector<std::pair<int, fluid_event_t*>> record;
};
