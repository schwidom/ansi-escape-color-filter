/* 

    Copyright : Frank Schwidom, 2017, schwidom@gmx.net

    This file is part of the ansi-escape-color-filter software.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
    veröffentlichten Version, weiterverbreiten und/oder modifizieren.

    Dieses Programm wird in der Hoffnung, dass es nützlich sein wird, aber
    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License für weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.

*/

#pragma once

#include "AnsiEscapeState.hpp"
#include "Chars.hpp"
#include "Globals.hpp"
#include "Transmitter.hpp"

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cctype>

#include "../src_tools/string.hpp" // outputStringHex, stringAsHex, string2int, int2string
#include "../src_tools/breakpoint.hpp" // BREAK

class Parser
{
private:

 struct AnsiCommand
 {
  std::string command;
 };

 std::vector<std::vector<AnsiCommand>> m_ansi_command_vector;

 AnsiEscapeState m_AnsiEscapeState;

 struct UnwindOnUnexpectedCharacter
 {
 };

 struct UnwindOnAllDone
 {
 };

 bool m_outputInputSequences;
 bool m_invertColors;
 bool m_outputTrueColor;
 bool m_dfg;
 bool m_dbg;
 bool m_lfg;
 bool m_lbg;
 bool m_greyify;
 bool m_forceBW;
 bool m_forceWB;
 bool m_dropFG;
 bool m_dropBG;
 bool m_toggleBW;
 bool m_toggleBWisBlackOnWhite= true;
 bool m_NoBold;
 bool m_NoFaint;
 bool m_NoItalic;
 bool m_NoUnderline;
 bool m_NoSlowBlink;
 bool m_NoRapidBlink;
 
public:

 using char_type = Transmitter::char_type;

 Parser( Transmitter& transmitter);

private:

 uint remainingParameters();

 void handleCurrentLeftBracket() throw( UnwindOnUnexpectedCharacter, UnwindOnAllDone);

public:
 void handleCurrentESC() throw( UnwindOnUnexpectedCharacter, UnwindOnAllDone);

private:
 Transmitter& m_transmitter;
};
