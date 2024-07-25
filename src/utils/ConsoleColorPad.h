// SPDX-License-Identifier: MulanPSL-2.0

/*

    commandline color tools

    created on 2022.12.5
    from ToyCompile
*/

#pragma once

#include <iostream>


namespace ConsoleColorPad {


extern bool noColor;



inline void setCoutColor(int red, int green, int blue) {
    if (!noColor) {
        std::cout << "\e[1;38;2;" << red << ";" << green << ";" << blue << "m";
    }
}


inline void setCoutColor() {
    if (!noColor) {
        std::cout << "\e[0m";
    }
}


inline void setClogColor(int red, int green, int blue) {
    if (!noColor) {
        std::clog << "\e[1;38;2;" << red << ";" << green << ";" << blue << "m";
    }
}


inline void setClogColor() {
    if (!noColor) {
        std::clog << "\e[0m";
    }
}


inline void disableColor() {
    noColor = true;
}

inline void enableColor() {
    noColor = false;
}

inline void setNoColor(bool b) {
    noColor = b;
}

} // namespace ConsoleColorPad
