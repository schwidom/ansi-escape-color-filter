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

#include "TestTransmitter.hpp"




void TestTransmitter::testStringComplete(std::string s, ulong readAheadCount, ulong pointCutFromEnd, int expectedESChandlings)
{
  std::ostringstream oss;
  std::istringstream iss( s);

  Transmitter<decltype( iss), decltype( oss)> transmitter( iss, oss);

  if( debug) std::cout << "### start test ###" << std::endl;
  if( debug) std::cout << s << std::endl;

  int actualESChandlings= 0;

  // die parameter readAheadCount, pointCutFromEnd
  // gelten nur für den ersten Durchlauf, daher Kontrolle per "loops"

  // while( ! transmitter.eof())
  for( int loops= 0; true; ++loops)
    {

      if( 0 != loops)
        {
          readAheadCount= 0;
          pointCutFromEnd= 0;
        }

      transmitter.transmitUpToESC();
      if( transmitter.eof()) break;
      ++actualESChandlings;
      for( ulong i= 0; i < readAheadCount; i++)
        {
          if( debug) std::cout << "1 " << transmitter.getEscapeStateChars() << std::endl;
          transmitter.readAhead2EscapeStateChars();
          if( transmitter.eof()) break;
          if( debug) std::cout << "2 " << transmitter.getEscapeStateChars() << std::endl;
          assert( ( i+2) == transmitter.getEscapeStateChars().size());
        }
      transmitter.setRefeedPoint(pointCutFromEnd); // von rechts des vorausgelesenen Strings
      transmitter.state_escape_reset();
      if( debug) std::cout << "3 " << transmitter.getRefeedChars() << std::endl;
      if( debug) std::cout << "3 " << transmitter.getEscapeStateChars() << std::endl;
      transmitter.flushEscapeStateChars();
    }


  std::string soss = oss.str();
  assert( s == soss);

  if( -1 != expectedESChandlings)
    {
      assert( actualESChandlings == expectedESChandlings);
    }
}

void TestTransmitter::test01()
{

  std::string s( "abc\033de");
  std::ostringstream oss;
  std::istringstream iss( s);

  Transmitter<decltype( iss), decltype( oss)> transmitter( iss, oss);

  transmitter.transmitUpToESC();

  transmitter.readAhead2EscapeStateChars();
  transmitter.readAhead2EscapeStateChars();
  assert( ! transmitter.eof()); // d7tc0411tx 1
  assert( "\033de" == transmitter.getEscapeStateChars());
  transmitter.readAhead2EscapeStateChars();
  assert( transmitter.eof()); // d7tc0411tx 2
  assert( "\033de" == transmitter.getEscapeStateChars());
  assert( "" == transmitter.getRefeedChars());

  transmitter.setRefeedPoint(0, true);
  transmitter.state_escape_reset();
  assert( ! transmitter.eof());
  transmitter.transmitUpToESC();


  assert( ! transmitter.eof());
  assert( "\033" == transmitter.getEscapeStateChars());
  assert( "de" == transmitter.getRefeedChars());
  transmitter.readAhead2EscapeStateChars();
  assert( !transmitter.eof());
  assert( "\033d" == transmitter.getEscapeStateChars());
  assert( "e" == transmitter.getRefeedChars());
  transmitter.readAhead2EscapeStateChars();
  assert( ! transmitter.eof()); // d7tc0411tx 1
  assert( "\033de" == transmitter.getEscapeStateChars());
  assert( "" == transmitter.getRefeedChars());
  transmitter.readAhead2EscapeStateChars();
  assert( transmitter.eof()); // d7tc0411tx  2
  assert( "\033de" == transmitter.getEscapeStateChars());
  assert( "" == transmitter.getRefeedChars());

  transmitter.setRefeedPoint(0, true);
  transmitter.state_escape_reset();
  assert( ! transmitter.eof());
  transmitter.transmitUpToESC();

  assert( ! transmitter.eof());
  assert( "\033" == transmitter.getEscapeStateChars());
  assert( "de" == transmitter.getRefeedChars());
  transmitter.readAhead2EscapeStateChars();
  assert( !transmitter.eof());
  assert( "\033d" == transmitter.getEscapeStateChars());
  assert( "e" == transmitter.getRefeedChars());
  transmitter.readAhead2EscapeStateChars();
  assert( ! transmitter.eof()); // d7tc0411tx 1
  assert( "\033de" == transmitter.getEscapeStateChars());
  assert( "" == transmitter.getRefeedChars());
  transmitter.readAhead2EscapeStateChars();
  assert( transmitter.eof()); // d7tc0411tx  2
  assert( "\033de" == transmitter.getEscapeStateChars());
  assert( "" == transmitter.getRefeedChars());

  transmitter.state_escape_reset();

  transmitter.transmitUpToESC();

  assert( s == oss.str());
}

void TestTransmitter::runTest()
{
  std::cout << __FILE__ << std::endl;

  assert( "\033"[0] == 0x1b);
  assert( "\x1b"[0] == 0x1b);

  testStringComplete( "", 0, 0, 0);
  testStringComplete( "abc", 0, 0, 0);
  testStringComplete( "abc\033de", 0, 0, 1);
  testStringComplete( "abc\033de", 1, 0, 1);
  testStringComplete( "abc\033de", 1, 1, 1);
  // testStringComplete( "abc\033de", 1, 2, 1); // setRefeedPoint : 0 == m_escapeStateCharsPointCut
  // testStringComplete( "abc\033de", 1, 3, 1); // setRefeedPoint : refeedStartPos > m_escapeStateChars.size()
  testStringComplete( "abc\033de", 2, 0, 1);
  testStringComplete( "abc\033de", 2, 1, 1);
  testStringComplete( "abc\033de", 2, 2, 1);
  // testStringComplete( "abc\033de", 3, 2, 1); // transmitUpToESC : Transmitter is in eof state

  testStringComplete( "abc\033\033z", 2, 0, 1);
  testStringComplete( "abc\033\033z", 2, 1, 1);
  testStringComplete( "abc\033\033z", 2, 2, 2);
  // testStringComplete( "abc\033\033z", 2, 3, 2); // setRefeedPoint : 0 == escapeStateCharsPointCut
  // testStringComplete( "abc\033\033z", 2, 4, 2); // setRefeedPoint : refeedStartPos > m_escapeStateChars.size()

  test01();
}
