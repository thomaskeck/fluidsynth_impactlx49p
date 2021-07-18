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


/**
 * The format standard library defined in C++-20 is not yet supported by g++.
 * This workaround injects libfmt (on which the new standard is based) into the
 * std namespace as a drop in replacement.
 * 
 * Once g++ supports C++-20 this workaround should be removed.
 */
#if __has_include(<format>)
    #include <format>
#else
    #include <fmt/core.h>
    namespace std {
        using fmt::format;
    }
#endif