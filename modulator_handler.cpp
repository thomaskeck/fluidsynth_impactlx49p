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


#include <stdexcept>

#include "format_workaround.h"
#include "modulator_handler.h"
#include "midi_enums.h"

/**
 * Struct used internally to define the default modulators.
 */
struct _ModSetting {
    int source;
    int flags;
    float amount;
};

void set_custom_filter(fluid_synth_t *synth, int type) {
    if (fluid_synth_set_custom_filter(synth, type, FLUID_IIR_Q_ZERO_OFF) == FLUID_FAILED) {
        throw std::runtime_error(std::format("Failed to set custom filter with type {}", type));
    }
}

ModulatorHandler::ModulatorHandler(fluid_synth_t *synth) : synth(synth) {

    // Defines all the default modulators with suitable flags and amounts.
    std::vector<_ModSetting> default_modulator_settings = {
        // It is unclear to me what these modulators control (GEN_MODENV*).
        // I've never found a soundfont which is influenced by them.
        {midi_cc::MODENVATTACK, GEN_MODENVATTACK | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 20000.0f},
        {midi_cc::MODENVDECAY, GEN_MODENVDECAY | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 20000.0f},
        {midi_cc::MODENVSUSTAIN, GEN_MODENVSUSTAIN | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 1440.0f},
        {midi_cc::MODENVRELEASE, GEN_MODENVRELEASE | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 20000.0f},
        // These are the standard ADSR modulators that many soundfonts support.
        {midi_cc::VOLENVATTACK, GEN_VOLENVATTACK | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 20000.0f},
        {midi_cc::VOLENVDECAY, GEN_VOLENVDECAY | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 20000.0f},
        {midi_cc::VOLENVSUSTAIN, GEN_VOLENVSUSTAIN | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_NEGATIVE, 1440.0f},
        {midi_cc::VOLENVRELEASE, GEN_VOLENVRELEASE | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 20000.0f},
        // Lowpass and highpass filter.
        {midi_cc::IIR_FILTER_CUTOFF, GEN_CUSTOM_FILTERFC | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 12000.0f},
        {midi_cc::IRR_FILTER_Q, GEN_CUSTOM_FILTERQ | FLUID_MOD_CONCAVE | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 960.0f},
        // Note: volume control is automatically setup up by fluidsynth for CC 007, hence we don't need to do this here.
        // {midi_cc::ATTENUATION, GEN_ATTENUATION | FLUID_MOD_LINEAR | FLUID_MOD_UNIPOLAR | FLUID_MOD_POSITIVE, 960.0f},
    };

    for(auto &setting : default_modulator_settings) {
        fluid_mod_t *mod = new_fluid_mod();
        fluid_mod_set_source1(mod, setting.source, FLUID_MOD_CC);
        fluid_mod_set_source2(mod, 0, 0);
        fluid_mod_set_dest(mod, setting.flags);
        fluid_mod_set_amount(mod, setting.amount);
        if (fluid_synth_add_default_mod(synth, mod, FLUID_SYNTH_OVERWRITE) == FLUID_FAILED) {
            throw std::runtime_error(std::format(
                "Failed to add default mod for source {} with flags {} and amount {}",
                setting.source, setting.flags, setting.amount));
        }
        // fluid_synth_add_default_mod copies the mod, hence it can be freed.
        delete_fluid_mod(mod);
    }

    // the filter modulator only take effect once we also set up a filter.
    set_custom_filter(synth, FLUID_IIR_LOWPASS);
}

void ModulatorHandler::handleFilterModulatorEvent(fluid_midi_event_t *event) {
    if (fluid_midi_event_get_value(event) > MIDI_BUTTON_THRESHOLD) {
        set_custom_filter(synth, FLUID_IIR_HIGHPASS);
    } else {
        set_custom_filter(synth, FLUID_IIR_LOWPASS);
    }
}

void ModulatorHandler::handleEvent(fluid_midi_event_t *event) {
    if (fluid_midi_event_get_type(event) == midi_event_type::CONTROL_CHANGE) {
        switch(fluid_midi_event_get_control(event)) {
            case midi_cc::IIR_FILTER_BUTTON: 
                return handleFilterModulatorEvent(event);
        }
    }
}