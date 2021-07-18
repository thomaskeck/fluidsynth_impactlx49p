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

#include "handler.h"

/**
 * Enum defining which effect is currently controlled by the received midi events.
 */
enum EffectControlMode {
    CHORUS = 1,
    REVERB = 2,
};


/**
 * Handles effect midi events.
 * 
 * The same set of controllers is used for several effects.
 * All control events are assigned to the currently selected effect.
 * In order to select another effect, the corresponding effect button must be
 * pressed.
 */
class EffectHandler : public Handler {

    public:
        EffectHandler(fluid_synth_t *synth);
        void handleEvent(fluid_midi_event_t *event) override;

    private:
        void handleReverbButtonEvent(fluid_midi_event_t *event);
        void handleChorusButtonEvent(fluid_midi_event_t *event);
        void handleEffectParam1Event(fluid_midi_event_t *event);
        void handleEffectParam2Event(fluid_midi_event_t *event);
        void handleEffectParam3Event(fluid_midi_event_t *event);
        void handleEffectParam4Event(fluid_midi_event_t *event);

    private:
        fluid_synth_t *synth;
        EffectControlMode mode;

};