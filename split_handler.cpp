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

#include <stdexcept>

#include "split_handler.h"
#include "midi_enums.h"


SplitHandler::SplitHandler(int number_of_splits) : number_of_splits(number_of_splits) {
    
    if (number_of_splits != 4) {
        throw std::runtime_error("Only 4 splits are supported currently.");
    }
    split_bounds.push_back(std::make_pair(0, 36));
    split_bounds.push_back(std::make_pair(36, 60));
    split_bounds.push_back(std::make_pair(60, 84));
    split_bounds.push_back(std::make_pair(84, 128));

    for(unsigned int split = 0; split < number_of_splits; ++split) {
        is_frozen.push_back(false);
        channels.push_back(1);
    }
}

void SplitHandler::handleControlEvent(fluid_midi_event_t *event) {
    int split = 0;
    switch(fluid_midi_event_get_control(event)) {
        case midi_cc::SPLIT1_BUTTON:
            split = 0;
            break;
        case midi_cc::SPLIT2_BUTTON:
            split = 1;
            break;
        case midi_cc::SPLIT3_BUTTON:
            split = 2;
            break;
        case midi_cc::SPLIT4_BUTTON:
            split = 3;
            break;
        default:
            return;
    }

    is_frozen[split] = fluid_midi_event_get_value(event) > MIDI_BUTTON_THRESHOLD;
    channels[split] = fluid_midi_event_get_channel(event);
}

void SplitHandler::handleNoteEvent(fluid_midi_event_t *event) {
    int key = fluid_midi_event_get_key(event);
    for (unsigned int split = 0; split < number_of_splits; ++split) {
        if (is_frozen[split] && 
            key >= split_bounds[split].first &&
            key < split_bounds[split].second) {
            fluid_midi_event_set_channel(event, channels[split]);
        }
    }
}

void SplitHandler::handleEvent(fluid_midi_event_t *event) {

    switch(fluid_midi_event_get_type(event)) {
        case midi_event_type::CONTROL_CHANGE:
            return handleControlEvent(event);
        case midi_event_type::NOTE_OFF:
        case midi_event_type::NOTE_ON:
            return handleNoteEvent(event);
    }
}

