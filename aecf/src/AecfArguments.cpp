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

#include "AecfArguments.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>

#include <stdexcept> // invalid_argument

AecfArguments::AecfArguments(MainArgs mainArgs)
{
  m_programName= mainArgs.argv[ 0];

  std::copy(mainArgs.argv + 1, mainArgs.argv + mainArgs.argc, std::back_inserter(m_argv));

  for( const auto & value : m_argv)
  {

    auto existingOption = aecfArgumentMap.optionExists( value);

    if( ! existingOption.exists)
    {
      throw std::invalid_argument( std::string(__func__) + " : option not registered : >" + value + "<");
    }

    m_options.push_back(existingOption.optionLong);
    m_optionSet.insert(existingOption.optionLong);
  }
}

bool AecfArguments::hasOption(std::string optionName) const
{
  return m_optionSet.find({optionName}) != m_optionSet.end();
}

std::string AecfArguments::getHelp() const
{

  std::ostringstream oss;

  oss << "" << std::endl;
  oss << "The Ansi Escape Color Filter (aecf), Frank Schwidom, 2017, schwidom@gmx.net " << std::endl;
  oss << "" << std::endl;
  oss << "usage: " << std::endl;
  oss << m_programName << " -h " << std::endl;
  oss << m_programName << " --help " << std::endl;
  oss << " <cursestool> | " << m_programName << " <options> # to change the colored output " << std::endl;
  oss << std::endl;
  oss << m_programName << ", the \"Ansi Escape Color Filter\", has the following options: " << std::endl;
  oss << std::endl;

  oss << aecfArgumentMap.getHelp();

  auto ret = oss.str();
  return ret;
}

const std::vector<AecfArgumentMap::OptionLong> AecfArguments::getOptions() const
{
  return m_options;
}




