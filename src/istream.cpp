//
// This file is part of Condor2Nav file formats translator.
//
// Copyright (C) 2009 Mateusz Pusz
//
// Condor2Nav is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Condor2Nav is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Condor2Nav. If not, see <http://www.gnu.org/licenses/>.
//
// Visit the project webpage (http://sf.net/projects/condor2nav) for more info.
//

/**
 * @file istream.cpp
 *
 * @brief Implements the IStream wrapper class. 
**/

#include "istream.h"
#include "activeSync.h"
#include <fstream>
#include <iostream>


/**
 * @brief Class constructor.
 *
 * condor2nav::CIStream class constructor.
 *
 * @param fileName The name of the file to read.
**/
condor2nav::CIStream::CIStream(const std::string &fileName):
CStream(fileName)
{
  switch(Type()) {
    case TYPE_LOCAL:
      {
        char dirCurr[MAX_PATH];
        std::string file;
        std::string dir;
        FilePathSplit(fileName, dir, file);
        if(!dir.empty()) {
          GetCurrentDirectory(MAX_PATH, dirCurr);
          SetCurrentDirectory(dir.c_str());
        }

        std::ifstream stream(file.c_str());
        if(!stream)
          throw std::runtime_error("ERROR: Couldn't open file '" + fileName + "' for reading!!!");
        Buffer() << stream.rdbuf();

        if(!dir.empty())
          SetCurrentDirectory(dirCurr);
      }
      break;

    case TYPE_ACTIVE_SYNC:
      {
        CActiveSync &activeSync(CActiveSync::Instance());
        activeSync.Read(FileName(), Buffer());
      }
      break;

    default:
      throw std::runtime_error("ERROR: Unknown stream type!!!");
  }
}


/**
 * @brief Class destructor.
 *
 * condor2nav::CIStream class destructor.
**/
condor2nav::CIStream::~CIStream()
{
}
