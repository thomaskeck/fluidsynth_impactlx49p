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

#include <vector>
#include <utility>

#include <fluidsynth.h>

#include "handler.h"

class SplitHandler : public Handler {

    public:
        SplitHandler(int number_of_splits);
        void handleEvent(fluid_midi_event_t *event) override;

    private:
        void handleControlEvent(fluid_midi_event_t *event);
        void handleNoteEvent(fluid_midi_event_t *event);
    private:
        int number_of_splits;
        std::vector<bool> is_frozen;
        std::vector<int> channels;
        std::vector<std::pair<int, int>> split_bounds;


};