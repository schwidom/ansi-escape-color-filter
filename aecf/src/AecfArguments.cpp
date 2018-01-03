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
#include <iomanip>
#include <iostream>
#include <utility>

#include <stdexcept> // invalid_argument

AecfArguments::AecfArguments(MainArgs mainArgs)
{
  addOption( {"-h"}, {"--help"}, {"shows the options"});
  addOption( {"-ois"}, {"--output-input-sequences"}, {"outputs the received sequences to stderr"});
  addOption( {"-ic"}, {"--invert-colors"}, {"inverts colors"});
  addOption( {"-otc"}, {"--output-true-color"}, {"outputs true color escapes"});
  addOption( {"-dfg"}, {"--darker-fg"}, {""});
  addOption( {"-dbg"}, {"--darker-bg"}, {""});
  addOption( {"-lfg"}, {"--lighter-fg"}, {""});
  addOption( {"-lbg"}, {"--lighter-bg"}, {""});
  addOption( {"-nb"}, {"--no-bold"}, {"drops CSI 1 m"});
  addOption( {"-nf"}, {"--no-faint"}, {"drops CSI 2 m"});
  addOption( {"-ni"}, {"--no-italic"}, {"drops CSI 3 m"});
  addOption( {"-nu"}, {"--no-underline"}, {"drops CSI 4 m"});
  addOption( {"-nsb"}, {"--no-slow-blink"}, {"drops CSI 5 m"});
  addOption( {"-nrb"}, {"--no-rapid-blink"}, {"drops CSI 6 m"});
  addOption( {"-nbl"}, {"--no-blink"}, {"drops CSI 5 m and CSI 6  m"});
  addOption( {"-gg"}, {"--greyify"}, {"only with --output-true-color"});
  addOption( {"-fbw"}, {"--force-black-white"}, {""});
  addOption( {"-fwb"}, {"--force-white-black"}, {""});
  addOption( {"-dfc"}, {"--drop-foreground-color"}, {""});
  addOption( {"-dbc"}, {"--drop-background-color"}, {""});
  addOption( {"-tbw"}, {"--toggle-black-white"}, {"toggles black white per colorchange (experimental)"});

  m_programName= mainArgs.argv[ 0];

  std::copy(mainArgs.argv + 1, mainArgs.argv + mainArgs.argc, std::back_inserter(m_commandLine));

  for( const auto & value : m_commandLine)
    {

      auto existingOption = optionExists( value);

      if( ! existingOption.exists)
        {
          throw std::invalid_argument( std::string(__func__) + " : option not registered : >" + value + "<");
        }

      m_commandLineArguments.insert(existingOption.optionLong);
    }

  if( m_commandLineArguments.find( {"--help"}) != m_commandLineArguments.end() )
    {
      std::cerr << "" << std::endl;
      std::cerr << "The Ansi Escape Color Filter (aecf), Frank Schwidom, 2017, schwidom@gmx.net " << std::endl;
      std::cerr << "" << std::endl;
      std::cerr << "usage: " << std::endl;
      std::cerr << m_programName << " -h " << std::endl;
      std::cerr << m_programName << " --help " << std::endl;
      std::cerr << " <cursestool> | " << m_programName << " <options> # to change the colored output " << std::endl;
      std::cerr << std::endl;
      std::cerr << m_programName << ", the \"Ansi Escape Color Filter\", has the following options: " << std::endl;

      size_t w1= 0, w2= 0, w3= 0;

      for( const auto kvp : m_optionsShort2Long)
        {
          const auto & explanation = m_optionsLong2Explanation.at(kvp.second);
          w1 = std::max( w1, 0 + kvp.first.value.size());
          w2 = std::max( w2, 1 + kvp.second.value.size());
          w3 = std::max( w3, 1 + explanation.value.size());
        }

      for( const auto option : m_options)
        {
          const auto & kvp = *m_optionsLong2Short.find( option);
          const auto & explanation = m_optionsLong2Explanation.at( option);
          std::cout << std::setw( w1) << kvp.second.value << " |" ;
          std::cout << std::setw( w2) << kvp.first.value << " |" ;
          std::cout << std::setw( w3) << explanation.value << std::endl;
        }

      exit( 0);
    }
}

bool AecfArguments::hasOption(std::string optionName) const
{
  return m_commandLineArguments.find({optionName}) != m_commandLineArguments.end();
}

AecfArguments::ExistingOption AecfArguments::optionExists(std::string optionName)
{

  ExistingOption ret{ false, {optionName}};

  {
    auto it = m_optionsShort2Long.find( OptionShort{ optionName});
    if( it != m_optionsShort2Long.end() )
      {
        ret.exists= true;
        ret.optionLong= it->second;
      }
  }

  if( !ret.exists)
    {
      auto it = m_optionsLong2Short.find( OptionLong{ optionName});
      if( it != m_optionsLong2Short.end() )
        {
          ret.exists= true;
        }
    }

  return ret;
}

void AecfArguments::addOption(AecfArguments::OptionShort optionShort, AecfArguments::OptionLong optionLong, AecfArguments::OptionExplanation optionExplanation)
{

  if( m_optionsShort2Long.find(optionShort) != m_optionsShort2Long.end())
    {
      throw std::invalid_argument( std::string(__func__) + " : short option already registered : >" + optionShort.value + "<");
    }

  if( m_optionsLong2Short.find(optionLong) != m_optionsLong2Short.end())
    {
      throw std::invalid_argument( std::string(__func__) + " : long option already registered : >" + optionLong.value + "<");
    }

  m_options.push_back( optionLong);
  m_optionsShort2Long.insert(std::make_pair( optionShort, optionLong));
  m_optionsLong2Short.insert(std::make_pair( optionLong, optionShort));
  m_optionsLong2Explanation.insert(std::make_pair( optionLong, optionExplanation));
}
