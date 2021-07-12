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


#define MIDI_BUTTON_THRESHOLD 64

/**
 * Enum containing the midi event types.
 * 
 * Fluidsynth already defines those, but the constants are not part of the
 * public API, hence I copy&pasted the once that I use here from
 * https://github.com/FluidSynth/fluidsynth/blob/a0ec2c59326b311a7cbaeb638c2002cf2c8c82db/src/midi/fluid_midi.h
 */
enum midi_event_type {
    // channel messages
    NOTE_OFF = 0x80,
    NOTE_ON = 0x90,
    KEY_PRESSURE = 0xa0,
    CONTROL_CHANGE = 0xb0,
    PROGRAM_CHANGE = 0xc0,
    CHANNEL_PRESSURE = 0xd0,
    PITCH_BEND = 0xe0,
};

/**
 * Eum containing the midi control change events.
 * 
 * This defines how control change events received from the midi keyboard
 * are interpreted.
 * There are two ways to change the meaning of a controller on the midi keyboard.
 * Either one assignes the controller a new identifier via the "Control Assign"
 * option in the setup menu of the keyboard,
 * or by changing the identifier in this enum to the preset of the midi keyboard.
 */
enum midi_cc {
    // Chorus and reverb controller.
    CHORUS_BUTTON = 35,
    REVERB_BUTTON = 36,
    EFFECT_PARAM1 = 65,
    EFFECT_PARAM2 = 66,
    EFFECT_PARAM3 = 67,
    EFFECT_PARAM4 = 68,
    // Lowpass and highpass filter controller.
    IIR_FILTER_BUTTON = 37,
    IIR_FILTER_CUTOFF = 60,
    IRR_FILTER_Q = 61,
    // ADSR (attack, decay, sustain, release) controller.
    MODENVATTACK = 38,
    MODENVDECAY = 39,
    MODENVSUSTAIN = 40,
    MODENVRELEASE = 41,
    VOLENVATTACK = 42,
    VOLENVDECAY = 43,
    VOLENVSUSTAIN = 44,
    VOLENVRELEASE = 45,
    // Volume controller.
    // Note: controller 7 is automatically recognized by fluidsynth as
    // the controller that modifies the volume. 
    // If this value is changed to something else the GEN_ATTENUATION
    // modulator needs to be changed as well, see modulators.cpp
    ATTENUATION = 7,
    // Midi recording controller.
    RECORD = 107,
    PLAY = 106,
    STOP = 105,
    FORWARD = 104,
    BACKWARD = 103,
    LOOP = 102,
};