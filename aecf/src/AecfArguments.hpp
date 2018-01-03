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

#include "MainArgs.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>

class AecfArguments
{
public:
  AecfArguments( MainArgs mainArgs);

  bool hasOption(std::string optionName) const;

private:
 struct OptionShort
 {
  std::string value;
  bool operator<(const OptionShort & os) const { return value < os.value;}
 };

 struct OptionLong
 {
  std::string value;
  bool operator<(const OptionLong & os) const { return value < os.value;}
 };

 struct OptionExplanation
 {
  std::string value;
 };

 struct ExistingOption
 {
  bool exists;
  OptionLong optionLong;
 };

 ExistingOption optionExists( std::string optionName);

 std::vector<OptionLong> m_options;
 std::map<OptionShort,OptionLong> m_optionsShort2Long;
 std::map<OptionLong,OptionShort> m_optionsLong2Short;
 std::map<OptionLong,OptionExplanation> m_optionsLong2Explanation;

 void addOption( OptionShort optionShort, OptionLong optionLong, OptionExplanation optionExplanation= {});

 std::string m_programName;
 std::vector<std::string> m_commandLine;
 std::set<OptionLong> m_commandLineArguments;
};

