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

#include "format_workaround.h"

#include "effect_handler.h"
#include "midi_enums.h"


EffectHandler::EffectHandler(fluid_synth_t *synth) :
    synth(synth),
    mode(EffectControlMode::REVERB) {
        fluid_synth_set_reverb(synth, 0.0, 0.0, 0.0, 0.0);
        fluid_synth_set_chorus(synth, 0, 0.0, 0.3, 0.0, FLUID_CHORUS_MOD_SINE);
}

void EffectHandler::handleEvent(fluid_midi_event_t *event) {
    if (fluid_midi_event_get_type(event) == midi_event_type::CONTROL_CHANGE) {
        switch(fluid_midi_event_get_control(event)) {
            case midi_cc::REVERB_BUTTON:
                return handleReverbButtonEvent(event);
            case midi_cc::CHORUS_BUTTON:
                return handleChorusButtonEvent(event);
            case midi_cc::EFFECT_PARAM1:
                return handleEffectParam1Event(event);
            case midi_cc::EFFECT_PARAM2:
                return handleEffectParam2Event(event);
            case midi_cc::EFFECT_PARAM3:
                return handleEffectParam3Event(event);
            case midi_cc::EFFECT_PARAM4:
                return handleEffectParam4Event(event);
        }
    }
}

void EffectHandler::handleReverbButtonEvent(fluid_midi_event_t *event) {
    mode = EffectControlMode::REVERB;
}

void EffectHandler::handleChorusButtonEvent(fluid_midi_event_t *event) {
    mode = EffectControlMode::CHORUS;
}
  
void EffectHandler::handleEffectParam1Event(fluid_midi_event_t *event) {
  float value = static_cast<float>(fluid_midi_event_get_value(event));
  switch(mode) {
    case EffectControlMode::REVERB:
        value = value / 127.0;
        if (fluid_synth_set_reverb_roomsize(synth, value) == FLUID_FAILED) {
            throw std::runtime_error(std::format(
                "Failed to set reverb roomsize with value {}", value));
        }
        break;
    case EffectControlMode::CHORUS:
        value = 0.1 + 4.9 * value / 127.0;
        if (fluid_synth_set_chorus_speed(synth, value) == FLUID_FAILED) {
            throw std::runtime_error(std::format(
                "Failed to set chorus speed with value {}", value));
        }
        break;
  }
}

void EffectHandler::handleEffectParam2Event(fluid_midi_event_t *event) {
  float value = static_cast<float>(fluid_midi_event_get_value(event));
  switch(mode) {
    case EffectControlMode::REVERB:
        value = value / 127.0;
        if (fluid_synth_set_reverb_level(synth, value) == FLUID_FAILED) {
            throw std::runtime_error(std::format(
                "Failed to set reverb level with value {}", value));
        }
        break;
    case EffectControlMode::CHORUS:
        value = 10.0 * value / 127.0;
        if (fluid_synth_set_chorus_level(synth, value) == FLUID_FAILED) {
            throw std::runtime_error(std::format(
                "Failed to set chorus level with value {}", value));
        }
        break;
  }
}

void EffectHandler::handleEffectParam3Event(fluid_midi_event_t *event) {
  float value = static_cast<float>(fluid_midi_event_get_value(event));
  switch(mode) {
    case EffectControlMode::REVERB:
        value = value / 127.0;
        if (fluid_synth_set_reverb_damp(synth, value) == FLUID_FAILED) {
            throw std::runtime_error(std::format(
                "Failed to set reverb damp with value {}", value));
        }
        break;
    case EffectControlMode::CHORUS:
        value = 21.0 * value / 127.0;
        if (fluid_synth_set_chorus_depth(synth, value) == FLUID_FAILED) {
            throw std::runtime_error(std::format(
                "Failed to set chorus depth with value {}", value));
        }
        break;
  }
}

void EffectHandler::handleEffectParam4Event(fluid_midi_event_t *event) {
  int value = fluid_midi_event_get_value(event);
  float fvalue = static_cast<float>(value);
  switch(mode) {
    case EffectControlMode::REVERB:
        fvalue = 100.0 * fvalue / 127.0;
        if (fluid_synth_set_reverb_width(synth, fvalue) == FLUID_FAILED) {
            throw std::runtime_error(std::format(
                "Failed to set reverb width with value {}", fvalue));
        }
        break;
    case EffectControlMode::CHORUS:
        value = (value > 99) ? 99 : 99; 
        if (fluid_synth_set_chorus_nr(synth, value) == FLUID_FAILED) {
                throw std::runtime_error(std::format(
                    "Failed to set chorus nr with value {}", value));
            }
        break;
  }
}