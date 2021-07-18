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

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <utility>

#include <unistd.h>

#include <fluidsynth.h>

#include "modulator_handler.h"
#include "effect_handler.h"
#include "record_handler.h"
#include "midi_enums.h"
#include "io.h"


int handle_midi_event(void* data, fluid_midi_event_t* fluid_event);


class MidiKeyboard {

    public:
        MidiKeyboard() {
            settings = new_fluid_settings();
            fluid_settings_setnum(settings, "synth.gain", 2.0);
            fluid_settings_setnum(settings, "synth.sample-rate", 48000.0);
            synth = new_fluid_synth(settings);
            sequencer =  new_fluid_sequencer2(1);
            int seq_synth_id = fluid_sequencer_register_fluidsynth(sequencer, synth);
            sfont = loadSfont("fluidr3.sf2");
            adriver = new_fluid_audio_driver(settings, synth);
            mdriver = new_fluid_midi_driver(settings, handle_midi_event, this);
            handlers.push_back(std::make_unique<EffectHandler>(synth));
            handlers.push_back(std::make_unique<ModulatorHandler>(synth));
            handlers.push_back(std::make_unique<RecordHandler>(sequencer, seq_synth_id));
        }

        fluid_sfont_t* loadSfont(const std::string &path) {
            int sfont_id = fluid_synth_sfload(synth, path.c_str(), 1);
            return fluid_synth_get_sfont_by_id(synth, sfont_id);
        }

        void handleMidiEvent(fluid_midi_event_t* event) {
            for(auto &handler : handlers) {
              handler->handleEvent(event);
            }
            fluid_sequencer_add_midi_event_to_buffer(sequencer, event);
        }
      

        ~MidiKeyboard() {
            // Remove all handlers first, because they contain pointers to
            // fluid synth objects that we delete here.
            handlers.clear();
            delete_fluid_midi_driver(mdriver);
            delete_fluid_sequencer(sequencer);
            delete_fluid_audio_driver(adriver);
            delete_fluid_synth(synth);
            delete_fluid_settings(settings);
        }



  public:
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    fluid_sequencer_t *sequencer;
    fluid_audio_driver_t *adriver;
    fluid_midi_driver_t *mdriver;
    fluid_sfont_t *sfont;
    std::vector<std::unique_ptr<Handler>> handlers;

};


int handle_midi_event(void* data, fluid_midi_event_t* event) {

  MidiKeyboard *keyboard = reinterpret_cast<MidiKeyboard*>(data);
  std::cout << event << std::endl;
  keyboard->handleMidiEvent(event);
  return 0;
}


int main(int argc, char **argv) {
  MidiKeyboard keyboard;
  while(true) sleep(1);
  return 0;
}
