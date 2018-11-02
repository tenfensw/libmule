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
	if (mcpLocalClass->fileExists(filename) == false)
		muleexception(11, "File does not exist", true);
	else
		playlist.push_back(MuleFile(filename));
}

void MuleSoundPlayer::addToPlaylist(MuleFile file) {
	playlist.push_back(file);
}

const std::vector<MuleFile> MuleSoundPlayer::getPlaylist() {
	return playlist;
}

void MuleSoundPlayer::clearPlaylist() {
	playlist.clear();
}

void MuleSoundPlayer::removeFromPlaylist(MULE_OTHER_STRINGTYPE filename) {
	int vsize = (int)(playlist.size());
	for (int i = 0; i < vsize; i++) {
		if (playlist[i].getFileName() == filename) {
			playlist.erase(playlist.begin() + i);
			return;
		}
	}
	muleexception(12, "Item not found", true);
	return;
}

void MuleSoundPlayer::removeFromPlaylist(MuleFile file) {
	int vsize = (int)(playlist.size());
	for (int i = 0; i < vsize; i++) {
		if (playlist[i].getFileName() == file.getFileName()) {
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
		mcpLocalClass->playWaveFile(playlist[i].getFileName());
	return;
}

bool MuleSoundPlayer::playSingleFile(MULE_OTHER_STRINGTYPE filename) {
	return MuleSoundPlayer::playSingleFile(MuleFile(filename));
}

bool MuleSoundPlayer::playSingleFile(MuleFile file) {
	if (file.exists() == true)
		return (MuleApplication::getRunningInstance()->getPlatformClass())->playWaveFile(file.getFileName());
	else
		return false;
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
