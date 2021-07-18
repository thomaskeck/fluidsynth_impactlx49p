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

#include <fluidsynth.h>


/**
 * Sets up the default modulators for fluidsynth.
 * 
 * The soundfonts use modulators to modulate the synthesized sounds.
 * Often a soundfont only supports a subset of modulators.
 * For instance, the sustain level is not supported by many soundfonts,
 * and therefore it often seems like this modulator has no effect.
 * 
 * Most modulator midi events are handled automatically by fluidsynth after the
 * default modulators are set up.
 * This class handles the remaining events, for instance the custom filter
 * setup.
 */

#pragma once

#include <fluidsynth.h>
#include "handler.h"

class ModulatorHandler : public Handler {

    public:
        ModulatorHandler(fluid_synth_t *synth);
        void handleEvent(fluid_midi_event_t *event) override;

    private:
        void handleFilterModulatorEvent(fluid_midi_event_t *event);

    private:
        fluid_synth_t *synth;

};
