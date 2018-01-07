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

#include "../../src/AecfArgumentMap.hpp"
#include "../../src/AecfArguments.hpp"

#include "../../src_tools/FollowedBy.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include <stdexcept> // runtime_error

#include <cassert>

static std::string convert( std::string toReplace) {

  static const std::regex startFrom {
    "^--"
  };
  static const std::string startTo {
    "m_"
  };

  static const std::regex underscoreFrom {
    "-"
  };
  static const std::string underscoreTo {
    "_"
  };

  std::string res = std::regex_replace( toReplace, startFrom, startTo);
  std::string ret = std::regex_replace( res, underscoreFrom, underscoreTo);

  return ret;
}

static const std::string base="aecf/src_generated/";

int main( int argc, char** argv)
{

  {
    std::string tagsFile { "tags2/2j36v4onzk/2j36v4onzk.txt"};
    std::ifstream ifs { tagsFile};
    if( !ifs.is_open()) {
      throw std::runtime_error( std::string(__func__) +
                                "gvylm7bgk2 tags file not found : " + tagsFile);
    }
  }

  AecfArgumentMap aecfArgumentMap;

  {
    std::ofstream ofs { base + "AecfArgConfig.fpp"};

    // key

    ofs << "#pragma once" << std::endl;
    ofs << "// i7erfvpp0r" << std::endl;

    // creating structure forward declaration

    ofs << "struct AecfArgConfig; " << std::endl;
  }

  {
    std::ofstream ofs { base + "AecfArgConfig.hpp"};

    // key

    ofs << "#pragma once" << std::endl;
    ofs << "// frtdruzb20" << std::endl;
    ofs << "#include \"../src/AecfArguments.hpp\"" << std::endl;

    // creating structure

    ofs << "struct AecfArgConfig {" << std::endl;
    for( const auto & value : aecfArgumentMap.getOptions())
    {
      ofs << "  bool " << convert( value.value) << ";" << std::endl;
    }
    ofs << "};" << std::endl;
    ofs << std::endl;
    ofs << "AecfArgConfig createAecfArgConfig(const AecfArguments & aecfArguments);" << std::endl;
  }


  // filling structure

  {
    std::ofstream ofs { base + "AecfArgConfig.cpp"};

    ofs << "// 2z3wzi9qjn" << std::endl;
    ofs << "#include \"AecfArgConfig.hpp\"" << std::endl;
    ofs << "AecfArgConfig createAecfArgConfig(const AecfArguments & aecfArguments){" << std::endl;
    ofs << "return" << std::endl;

    FollowedBy fby( "", ",");

    ofs << "AecfArgConfig{" << std::endl;
    for( const auto & value : aecfArgumentMap.getOptions())
    {
      ofs << fby.next() << "aecfArguments.hasOption(\"" << value.value << "\")" << std::endl;
    }
    ofs << "};" << std::endl;
    ofs << "}" << std::endl;

  }

  return 0;
}

