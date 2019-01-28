//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef MULESOUNDPLAYER_H
#define MULESOUNDPLAYER_H
#include <iostream>
#include <vector>
#include "core/muleapplication.h"
#include "platformsupport/common/mulecommonplatform.h"
#include "fileio/mulefile.h"

class MuleSoundPlayer {
	public:
	  MuleSoundPlayer();
	  ~MuleSoundPlayer();
	  void addToPlaylist(MULE_OTHER_STRINGTYPE filename);
	  void addToPlaylist(MuleFile file);
	  const std::vector<MuleFile> getPlaylist();
	  void clearPlaylist();
	  void removeFromPlaylist(MULE_OTHER_STRINGTYPE filename);
	  void removeFromPlaylist(int fileindex);
	  void removeFromPlaylist(MuleFile file);
	  void play();
	  static bool playSingleFile(MULE_OTHER_STRINGTYPE filename);
	  static bool playSingleFile(MuleFile file);
	  void stop();
	  static void beep();

	private:
	  bool init();
	  std::vector<MuleFile> playlist;
	  MuleCommonPlatform* mcpLocalClass;
};

#endif
