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


#include "track.h"
#include "midi_enums.h"


void track_callback(unsigned int time, fluid_event_t* event, fluid_sequencer_t* seq, void* data) {
  Track *track = reinterpret_cast<Track*>(data);
  track->playNextChunk();
}


Track::Track(fluid_sequencer_t* sequencer, int seq_synth_id) : 
    sequencer(sequencer),
    seq_synth_id(seq_synth_id),
    is_recording(false),
    is_playing(false) {
        seq_client_id = fluid_sequencer_register_client(sequencer, "track_callback", track_callback, this);
}
    
Track::~Track() {
    fluid_sequencer_unregister_client(sequencer, seq_client_id);
    for (auto &pair : record) {
        delete_fluid_event(pair.second);
    }
}

void Track::recordStart() {
    if (not isRecording()) {
    is_recording = true;
    record_start_time = fluid_sequencer_get_tick(sequencer);
    record_stop_time = record_start_time;
    }
}

void Track::recordStop() {
    if (isRecording()) {
    is_recording = false;
    record_stop_time = fluid_sequencer_get_tick(sequencer);
    }
}

void Track::playStart() {
    if (not isPlaying()) {
    is_playing = true;
    play_start_time = fluid_sequencer_get_tick(sequencer);
    scheduleNextCallback();
    }
}
    
void Track::playStop() {
    if(isPlaying()) {
    is_playing = false;
    }
}

bool Track::isPlaying() const {
    return is_playing;
}

bool Track::isRecording() const {
    return is_recording;
}

int Track::getRecordDuration() const {
    return record_stop_time - record_start_time;  
}

int Track::getPlayDuration() const {
    int current_time = fluid_sequencer_get_tick(sequencer);
    return current_time - play_start_time;
}

int Track::getScheduledPlayDuration() const {
    return play_current_time - play_start_time;

}

int Track::getRemainingPlayDuration() const {
    return getRecordDuration() - getPlayDuration();
}

void Track::scheduleNextCallback() {
    fluid_event_t *evt = new_fluid_event();
    fluid_event_set_source(evt, -1);
    fluid_event_set_dest(evt, seq_client_id);
    fluid_event_timer(evt, NULL);
    fluid_sequencer_send_at(sequencer, evt, CALLBACK_TIME, false);
    delete_fluid_event(evt);
}

#include <iostream>

void Track::playNextChunk() {
    int current_time = fluid_sequencer_get_tick(sequencer);
    if (isPlaying()) {
        int remaining_play_duration = getRemainingPlayDuration();
        if (remaining_play_duration < 0) {
            std::cout << "restart " << remaining_play_duration << std::endl;
            // We pretend to have started at the right time in between the callbacks.
            play_start_time = current_time + remaining_play_duration;
            play_current_time = current_time;
        }
        
        int played_until = play_current_time;
        for (auto &pair : record) {
            // Considers scheduling recorded event if it will be due to play in twice the callback time.
            if (pair.first < getPlayDuration() + 2 * CALLBACK_TIME) {
            int play_time = play_start_time + pair.first;
            // Play event if we haven't already played it.
            if (play_time > play_current_time) {
                fluid_sequencer_send_at(sequencer, pair.second, play_time, 1);
            }
            // Remembers until which time mark we already played.
            if (play_time > played_until) {
                played_until = play_time;
            }
            }
        }
        play_current_time = played_until;

        // Schedules next callback so the track keeps on playing.
        scheduleNextCallback();
    }
}

void Track::maybeRecordMidiEvent(fluid_midi_event_t* event) {
    if (isRecording()) {
        int time = fluid_sequencer_get_tick(sequencer) - record_start_time;
        record.push_back(std::make_pair(time, convertMidiEventToEvent(event)));
    }
}

fluid_event_t* Track::convertMidiEventToEvent(fluid_midi_event_t* midi_event) {
    fluid_event_t *event = new_fluid_event();
    fluid_event_set_dest(event, seq_synth_id);
    
    // See https://github.com/FluidSynth/fluidsynth/blob/
    // 883ea24960f7af117747eb99c257022b1e3de750/src/midi/fluid_seqbind.c#L371
    int type = fluid_midi_event_get_type(midi_event);
    int channel = fluid_midi_event_get_channel(midi_event);
    int key = fluid_midi_event_get_key(midi_event);
    int control = fluid_midi_event_get_control(midi_event);
    int value = fluid_midi_event_get_value(midi_event);
    int velocity = fluid_midi_event_get_velocity(midi_event);
    switch(type) {
    case midi_event_type::NOTE_OFF:
        fluid_event_noteoff(event, channel, key);
        break;
    case midi_event_type::NOTE_ON:
        fluid_event_noteon(event, channel, key, velocity);
        break;
    case midi_event_type::CONTROL_CHANGE:
        fluid_event_control_change(event, channel, control, value);
        break;
    }	
    return event;
}