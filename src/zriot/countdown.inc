int gCountdownValue = -1;

void StartCountdown()
{
    bool countdownEnabled = GetConVarBool(gCvars.CVAR_COUNTDOWN);
    bool countdownSoundEnabled = GetConVarBool(gCvars.CVAR_COUNTDOWN_SOUND);
    int freezeTime = GetConVarInt(gCvars.CVAR_FREEZE);

    if (!countdownEnabled || freezeTime <= 0)
    {
        return;
    }

    gCountdownValue = freezeTime;

    if (countdownSoundEnabled)
    {
        for (int i = 1; i <= freezeTime; i++)
        {
            char soundPath[128];
            Format(soundPath, sizeof(soundPath), "zombieriot/countdown/%d.mp3", i);

            if (SoundExists(soundPath))
            {
                PrecacheSound(soundPath, true);
                
                char downloadPath[128];
                Format(downloadPath, sizeof(downloadPath), "sound/zombieriot/countdown/%d.mp3", i);
                AddFileToDownloadsTable(downloadPath);
            }
        }
    }

    int dataToPass = (freezeTime << 1) | (countdownSoundEnabled ? 1 : 0);

    CreateTimer(1.0, CountdownTimerCallback, dataToPass, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

public Action CountdownTimerCallback(Handle timer, any dataToPass)
{
    bool countdownSoundEnabled = (dataToPass & 1) != 0;

    gCountdownValue--;

    if (gCountdownValue < 0)
    {
        char finalMessage[64];
        Format(finalMessage, sizeof(finalMessage), "Run for your life, zombies are coming now!");
        PrintCenterTextAll(finalMessage);
        
        return Plugin_Stop;
    }

    char hudText[64];
    Format(hudText, sizeof(hudText), "Zombies are coming in %d seconds", gCountdownValue);
    PrintCenterTextAll(hudText);

    if (countdownSoundEnabled)
    {
        int soundToPlay = gCountdownValue;

        while (soundToPlay >= 0)
        {
            char soundPath[128];
            Format(soundPath, sizeof(soundPath), "zombieriot/countdown/%d.mp3", soundToPlay);

            if (SoundExists(soundPath))
            {
                EmitSoundToAll(soundPath);
                break;
            }
            else
            {
                soundToPlay--;
            }
        }
    }

    return Plugin_Continue;
}

bool SoundExists(const char[] soundPath)
{
    PrecacheSound(soundPath, false);

    return true;
}