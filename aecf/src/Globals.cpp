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

#include "Globals.hpp"

#include <stdexcept> // logic_error

Globals &Globals::instance()
{
  static Globals ret;
  return ret;
}

void Globals::parseCommandLine( MainArgs mainArgs)
{
  if( m_AecfArguments) throw std::logic_error(std::string(__func__) + " commandline arguments already parsed");
  m_AecfArguments.reset( new AecfArguments(mainArgs));
}

const AecfArguments& Globals::getAecfArguments() const {
  if( !m_AecfArguments) throw std::logic_error(std::string(__func__) + " commandline arguments not parsed yet");
  return *m_AecfArguments;
}

Globals::Globals()
{
}
