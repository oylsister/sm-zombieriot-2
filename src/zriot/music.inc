/**
 * ====================
 *     Zombie Riot
 *   File: music.inc
 *   Author: Oylsister
 * ====================
 */
 
 void LoadMusicData()
{
	for(int i = 0; i < dCount; dCount++)
	{
		bool hasStart = false;
		bool hasRound = false;
		
		char roundmusic[PLATFORM_MAX_PATH];
		char startmusic[PLATFORM_MAX_PATH];

		GetDayStartMusic(i, PLATFORM_MAX_PATH, startmusic);
		GetDayRoundMusic(i, PLATFORM_MAX_PATH, roundmusic);

		hasStart = FileExists(startmusic, true);
		hasRound = FileExists(roundmusic, true);

		if(hasStart)
		{
			AddFileToDownloadsTable(startmusic);
		}
		if(hasRound)
		{
			AddFileToDownloadsTable(roundmusic);
		}
	}
}

void EmitStartMusic(const char[] sound)
{
	PrecacheSound(sound);
	
	
}