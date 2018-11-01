#ifndef MULESOUNDPLAYER_H
#define MULESOUNDPLAYER_H
#include <iostream>
#include <vector>
#include "core/muleapplication.h"
#include "platformsupport/common/mulecurrentplatform.h"
#include "fileio/mulefile.h"

class MuleSoundPlayer {
	public:
	  MuleSoundPlayer();
	  ~MuleSoundPlayer();
	  void addToPlaylist(MULE_OTHER_STRINGTYPE filename);
	  void addToPlaylist(const MuleFile& file);
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
	  MuleCurrentPlatform* mcpLocalClass;
};

#endif
