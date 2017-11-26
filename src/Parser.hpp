/* 

    Copyright : Frank Schwidom, 2017, schwidom@gmx.net

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

template<class TransmitterType>
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
 
public:

 using char_type = typename TransmitterType::char_type;

 Parser( TransmitterType & transmitter)
  : m_transmitter(transmitter)
 {
  m_outputInputSequences= Globals::instance().hasOption( "--output-input-sequences");
  m_outputTrueColor= Globals::instance().hasOption( "--output-true-color");
  m_invertColors= Globals::instance().hasOption( "--invert-colors");
  m_dfg= Globals::instance().hasOption( "--darker-fg");
  m_dbg= Globals::instance().hasOption( "--darker-bg");
  m_lfg= Globals::instance().hasOption( "--lighter-fg");
  m_lbg= Globals::instance().hasOption( "--lighter-bg");
  m_greyify= Globals::instance().hasOption( "--greyify");
  m_forceBW= Globals::instance().hasOption( "--force-black-white");
  m_forceWB= Globals::instance().hasOption( "--force-white-black");
 }

private:

 uint remainingParameters()
 {
  const auto & current_filling_command = m_ansi_command_vector.back();
  std::string s = current_filling_command.at( 0).command; // at least this exists, the rest is coming
  int is= string2int( s);
  
// std::cerr << "remainingParameters " << s << std::endl;

  if( 1 == s.size()) return 0;


  if( 2 == s.size())
  {
   if( 10 <= is && is <= 29) return 0;
   if( 30 <= is && is <= 37) return 0; // foreground
   if( 39 == is) return 0; // default foreground
   if( 40 <= is && is <= 47) return 0; // background
   if( 49 == is) return 0; // default background
   
   if( is == 38 || is == 48)
   {
    if( 1 == current_filling_command.size()) return 1; // minimum value, gets corrected in the next step
    if( '5' == s.at(1)) return 1; // [38;5;<x>m / [48;5;<x>m 
    if( '2' == s.at(1)) return 3; // [38;2;<r>;<g>;<b>m / [48;2;<r>;<g>;<b>m
   }
  }

  if( 51 <= is && is <= 55) return 0;
  if( 61 <= is && is <= 65) return 0;
  if( 90 <= is && is <= 97) return 0;
  if( 100 <= is && is <= 107) return 0;

//throw std::invalid_argument( "2");

  throw UnwindOnUnexpectedCharacter();
 }

 void handleCurrentLeftBracket() throw( UnwindOnUnexpectedCharacter, UnwindOnAllDone)
 { // NOTE: CSI - Control Sequence Introducer
  
  {
   const char_type c = m_transmitter.getEscapeStateChars().back();
   if( c != '[' ) throw UnwindOnUnexpectedCharacter();
  }

  { // SGR (Select Graphic Rendition) parameters

   uint parametersLeft= 0;
   

   while( true)
   {
    std::string parameter1= ""; // TODO: rename to command or parameter
    char_type separator1=' ';

    if( 0 == parametersLeft)
    {
     m_ansi_command_vector.push_back( {});
    }


    while( true) // fill first parameter with decimals upto m or ;
    {
     const char_type c = m_transmitter.readAhead2EscapeStateChars();

     switch( c)
     {
      case ';':
      case 'm':
       separator1= c;
       break;
      default:
       if( isdigit( c)){ parameter1.push_back( c); continue;}
       throw UnwindOnUnexpectedCharacter();
     }

     break;
    }

    if( "" == parameter1)
    {
     parameter1= "0";
    } 

    // m_ansi_command_vector.at( m_ansi_command_vector.size()- 1).push_back( AnsiCommand{ parameter1});
    m_ansi_command_vector.back().push_back( AnsiCommand{ parameter1});
    
    parametersLeft = remainingParameters();

    {
     if( 'm' == separator1)
     {
      throw UnwindOnAllDone();
     }
     else if( ';' == separator1)
     {
      continue;
      // std::cerr << ";" << std::endl;
      // handleCurrentLeftBracketOrSemiColon();
      // throw UnwindOnUnexpectedCharacter();
     }
    }
   }
  }
  
 }

public:
 void handleCurrentESC() throw( UnwindOnUnexpectedCharacter, UnwindOnAllDone)
 { // NOTE: Escape sequence
 
  if( Chars::str_esc != m_transmitter.getEscapeStateChars()) return;
 
  m_transmitter.readAhead2EscapeStateChars();

  try
  {
   m_ansi_command_vector.clear();
   handleCurrentLeftBracket();
  }catch( UnwindOnUnexpectedCharacter )
  {
   m_transmitter.setRefeedPoint( 1);
  }catch( UnwindOnAllDone)
  {

   // m_transmitter.setEscapeStateChars( Chars::str_esc+ "[30m"); // TODO : erst bei gesichertem 'm' und nicht hier
   // m_transmitter.setEscapeStateChars( Chars::str_esc+ "[m"+ Chars::str_esc+ "[30m");
   // TODO : set colors according to options

   // WEITERBEI
   // vorgehen: Vektorfunktionen:
   // - status ermitteln (AnsiEscapeState)
   // - status modifizieren
   // - vector ausgeben
   // und den code aus dem catch rausbewegen

   // die Filter werden in der Reihenfolge der Kommandozeilenangabe angewendet

   if( m_outputInputSequences)
   {
    std::cerr << "AnsiCommands" << std::endl;

    for( auto val : m_ansi_command_vector)
    {
     std::cerr << "size: " << val.size() << std::endl;
     std::cerr << "command: " << val.at(0).command << std::endl;
    }
   }

   if( m_forceBW) 
   {
    for( auto & val : m_ansi_command_vector)
    {
     if( 2 != val.at( 0).command.size()) continue;
     // if( '9' == val.at( 0).command.at( 1)) continue;
     switch( val.at( 0).command.at( 0))
     {
      case '3' : val= {{"30"}}; break;
      case '4' : val= {{"47"}}; break;
     }
    }
   }

   if( m_forceWB) 
   {
    for( auto & val : m_ansi_command_vector)
    {
     if( 2 != val.at( 0).command.size()) continue;
     // if( '9' == val.at( 0).command.at( 1)) continue;
     switch( val.at( 0).command.at( 0))
     {
      case '3' : val= {{"37"}}; break;
      case '4' : val= {{"40"}}; break;
     }
    }
   }

   if( m_outputTrueColor)
   {
    for( auto & val : m_ansi_command_vector)
    {

     if( 2 != val.at( 0).command.size()) continue;

     char_type c0 = val.at( 0).command.at( 0);
     char_type c1 = val.at( 0).command.at( 1);

     AnsiColor * ac = nullptr;

     switch( c0)
     {
      case '3' : ac = &m_AnsiEscapeState.foreground; break;
      case '4' : ac = &m_AnsiEscapeState.background; break;
     }

     bool is_set = false;

     if( '0' <= c1 && c1 <='7')
     {
      ac->set7classic( c1);
      is_set = true;
     }

     {

      bool darker;
      bool lighter;

      switch( c0)
      {
       case '3' : 
        darker = m_dfg;
        lighter = m_lfg;
        break;
       case '4' :
        darker = m_dbg;
        lighter = m_lbg;
        break;
      }

      if( darker) ac->darker();
      if( lighter) ac->lighter();
     }

     if( m_greyify)
     {
      ac->greyify();
     }

     if( is_set)
     {
      auto rgb = ac->get82truecolor();
      val = { {std::string({ c0, '8'})}, {"2"}, {int2string(rgb.r)}, {int2string(rgb.g)}, {int2string(rgb.b)}};
     }
    }
   }

   if( m_invertColors) 
   {
    for( auto & val : m_ansi_command_vector)
    {
// throw std::invalid_argument( "1");
     if( 2 != val.at( 0).command.size()) continue;
     switch( val.at( 0).command.at( 0))
     {
      case '3' : val.at(0).command.at( 0)= '4'; break;
      case '4' : val.at(0).command.at( 0)= '3'; break;
     }
    }
   }


   {
    assert( ! m_ansi_command_vector.empty());

    std::ostringstream oss;

    oss << Chars::esc << "[";

    bool butFirstCell = false; // [cell;cell;...m
 
    for( auto itAnsiCommand = m_ansi_command_vector.cbegin(); itAnsiCommand != m_ansi_command_vector.cend(); ++itAnsiCommand)
    {
     const auto ansiCommandCell= *itAnsiCommand;
     for( auto itAnsiCommandCell= ansiCommandCell.cbegin(); itAnsiCommandCell != ansiCommandCell.cend(); ++itAnsiCommandCell)
     {
      if( butFirstCell){ oss << ';';} else{ butFirstCell= true;}
      oss << itAnsiCommandCell->command;
     }
    }

    oss << 'm';

    // m_transmitter.setEscapeStateChars( "");
    m_transmitter.setEscapeStateChars( oss.str());
   }
  }

  m_transmitter.state_escape_reset();
 }

private:
 TransmitterType & m_transmitter;
};