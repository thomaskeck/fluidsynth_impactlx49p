compile:
	g++ -o impact_lx48+ impact_lx48+.cpp modulator_handler.cpp track.cpp record_handler.cpp effect_handler.cpp io.cpp -lfluidsynth -lfmt -std=c++20 