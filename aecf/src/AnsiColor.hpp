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

#include <string>

#include <stdexcept> // invalid_argument

#include <cstdint> // ::uint8_t

#include "../src_tools/string.hpp"

class AnsiColor
{
private:

 // TODO : class AnsiColorChannel
 uint8_t m_r= 0;
 uint8_t m_g= 0;
 uint8_t m_b= 0;
 
 void correctBounds();

 // uint8_t multiplicator_normal = 205;
 uint8_t multiplicator_normal = 255;
 uint8_t multiplicator_bright = 255;

 void setRGB( uint8_t r, uint8_t g, uint8_t b)
 {
  m_r=r * multiplicator_normal;
  m_g=g * multiplicator_normal;
  m_b=b * multiplicator_normal;
 }

 struct RGB_1 { uint8_t r; uint8_t g; uint8_t b;}; // TODO : use bool maybe (beware of bright modi)

 RGB_1 getRGB();

public:
 void set7classic( char c);

 char get7classic();
 
 struct RGB_256 { uint8_t r; uint8_t g; uint8_t b;}; // TODO : beware of bright modi

 RGB_256 get82truecolor();

 void darker()
 {
  m_r = static_cast<uint8_t>( 0.5 * m_r);
  m_g = static_cast<uint8_t>( 0.5 * m_g);
  m_b = static_cast<uint8_t>( 0.5 * m_b);
 }

 void lighter()
 {
  m_r = 255 - static_cast<uint8_t>( 0.5 * (255 - m_r));
  m_g = 255 - static_cast<uint8_t>( 0.5 * (255 - m_g));
  m_b = 255 - static_cast<uint8_t>( 0.5 * (255 - m_b));
 }
 
/*
 void darkenDarker()
 {
  auto avg = ( m_r + m_g + m_b / 3);
  if( avg
 }
*/

 void greyify();

};
