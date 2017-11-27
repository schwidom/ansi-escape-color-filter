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

#include "Chars.hpp"

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <cassert>
#include <csignal>

extern "C" {
#include <unistd.h>
}

template<class Tin, class Tout,
 class = 
 typename 
 std::enable_if< 
  std::is_same< 
   typename Tin::char_type, 
   typename Tout::char_type 
  >::value 
 >::type
>
class Transmitter
{
private:
 bool debug= false;

public:

 using char_type = typename Tin::char_type;

 Transmitter( Tin & istream, Tout & ostream)
  : m_Istream(istream)
  , m_Ostream(ostream)
  , m_state_escape(false)
  , m_refeedCharsEOF(true)
 {
 }

 void flushEscapeStateChars()
 {

  if( m_state_escape)
  { // yq8lk8yap9 
   throw std::invalid_argument( std::string(__func__) + " : Transmitter is in escape state");
  }

  for( char_type c : m_escapeStateChars)
  {
   m_Ostream.put( c); // syjotvd1ve 
  }

  m_escapeStateChars.clear();
 }

 void transmitUpToESC()
 {

  if( m_state_escape)
  { // yq8lk8yap9 
   throw std::invalid_argument( std::string(__func__) + " : Transmitter is in escape state");
  }

  flushEscapeStateChars();

  while( true)
  {

   const char_type c = get_next_char();

   if( eof())
   {
    break;
   }
  
   if( Chars::esc == c)
   {
    m_state_escape= true;
    m_escapeStateChars.push_back( c); // hdhqmq3yog
    break;
   }

   m_Ostream.put( c); // syjotvd1ve
  }
  
  return;
 }

 bool eof() { return m_Istream.eof() && m_refeedCharsEOF;}

 bool state_escape() { return m_state_escape;}

 void state_escape_reset() // string ist ggf. modifiziert
 { // equals: state_escape_reset( 0)

  m_state_escape = false;
 }

 void setRefeedPoint( unsigned long refeedStartPos, bool fromBeginning= false) // string ist ggf. modifiziert
 { 

  if( refeedStartPos > m_escapeStateChars.size())
  {
   throw std::invalid_argument( std::string(__func__) + 
    " : refeedStartPos > m_escapeStateChars.size()");
  }

  ulong escapeStateCharsPointCut= 0; 

  {
   if( fromBeginning)
   {
    escapeStateCharsPointCut= refeedStartPos;
   }
   else // !fromBeginning
   {
    escapeStateCharsPointCut= m_escapeStateChars.size() - refeedStartPos;
   }
  }

  // kill( getpid(), SIGINT); // geht

  if( debug) std::cout << "escapeStateCharsPointCut " << escapeStateCharsPointCut << std::endl;
  if( debug) std::cout << "m_refeedChars " << m_refeedChars << std::endl;
  if( debug) std::cout << "m_escapeStateChars " << m_escapeStateChars << std::endl;

  m_refeedChars= m_escapeStateChars.substr( escapeStateCharsPointCut) + m_refeedChars;
  m_refeedCharsEOF= ( 0 == m_refeedChars.size()); // 8gzynmxt6j

  m_escapeStateChars.erase( escapeStateCharsPointCut);

  if( debug) std::cout << "m_refeedChars " << m_refeedChars << std::endl;
  if( debug) std::cout << "m_escapeStateChars " << m_escapeStateChars << std::endl;
 }

 const char_type readAhead2EscapeStateChars() 
 {
  if( ! m_state_escape)
  {
   throw std::invalid_argument( std::string(__func__) + " : Transmitter is not in escape state");
  }

  Transmitter::char_type ret= get_next_char();

  if( ! eof())
  {
   m_escapeStateChars.push_back( ret);
  }

  return ret;
 }

 const std::basic_string<char_type> & getEscapeStateChars() const
 {
  return m_escapeStateChars;
 }

 void setEscapeStateChars(std::basic_string<char_type> escapeStateChars)
 {
  m_escapeStateChars= escapeStateChars;
 }

 const std::basic_string<char_type> & getRefeedChars() const // 4 debug purposes
 {
  return m_refeedChars;
 }

private:
 
 char_type get_next_char() 
 {
  char_type ret;

  m_refeedCharsEOF= ( 0 == m_refeedChars.size()); // 8gzynmxt6j

  if( ! m_refeedCharsEOF)
  {
   ret = m_refeedChars.at( 0);
   m_refeedChars.erase( m_refeedChars.begin());
  }
  else
  {
   ret= m_Istream.get();
  }

  if( debug) std::cout << "get_next_char " << ret << std::endl;

  return ret;
 }

 Tin& m_Istream;
 Tout& m_Ostream;

 bool m_state_escape;

 std::basic_string<char_type>m_escapeStateChars;
 std::basic_string<char_type>m_refeedChars; // TODO : prove possible usage of ostringstream 4v1d02nrpn
 bool m_refeedCharsEOF; // TODO : prove possible usage of ostringstream 4v1d02nrpn 
};
