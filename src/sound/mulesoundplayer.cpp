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

#include "sound/mulesoundplayer.h"

MuleSoundPlayer::MuleSoundPlayer() {
	if (init() == false)
		muleexception(10, "Failed to initialize MuleSoundPlayer", true);

}

bool MuleSoundPlayer::init() {
	muledebug("MuleSoundPlayer::init()");
	mcpLocalClass = MuleApplication::getRunningInstance()->getPlatformClass();
	if (!(mcpLocalClass))
		return false;
	if (mcpLocalClass->getSoundBackend() == "lego")
		muleprintf("WARNING!!! It was detected that MuleSoundPlayer is running on LEGO Mindstorms EV3 official firmware. While LEGO EV3 has a sound device, it has very limited capabilities due to the firmware's lack of support for ALSA, PulseAudio or even Open Sound System. As such, many features of MuleSoundPlayer won't work correctly. Use at your own risk and do not expect your wave files to play on this thing!");
	return true;
}

void MuleSoundPlayer::addToPlaylist(MULE_OTHER_STRINGTYPE filename) {
	playlist.push_back(filename);
}

const std::vector<std::string> MuleSoundPlayer::getPlaylist() {
	return playlist;
}

void MuleSoundPlayer::clearPlaylist() {
	playlist.clear();
}

void MuleSoundPlayer::removeFromPlaylist(MULE_OTHER_STRINGTYPE filename) {
	int vsize = (int)(playlist.size());
	for (int i = 0; i < vsize; i++) {
		if (playlist[i] == filename) {
			playlist.erase(playlist.begin() + i);
			return;
		}
	}
	muleexception(12, "Item not found", true);
	return;
}

void MuleSoundPlayer::removeFromPlaylist(int fileindex) {
	if (fileindex > (int)(playlist.size())) 
		muleexception(12, "Item not found", true);
	else
		playlist.erase(playlist.begin() + fileindex);
	return;
}

void MuleSoundPlayer::play() {
	int vsize = (int)(playlist.size());
	for (int i = 0; i < vsize; i++)
		mcpLocalClass->playWaveFile(playlist[i]);
	return;
}

bool MuleSoundPlayer::playSingleFile(MULE_OTHER_STRINGTYPE filename) {
	return (MuleApplication::getRunningInstance()->getPlatformClass())->playWaveFile(filename);
}


void MuleSoundPlayer::stop() {
	mcpLocalClass->stopAllSounds();
	return;
}

MuleSoundPlayer::~MuleSoundPlayer() {
	clearPlaylist();
}

void MuleSoundPlayer::beep() {
	(MuleApplication::getRunningInstance()->getPlatformClass())->doBeep();
}
