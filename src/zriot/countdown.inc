// Global variable to track countdown value
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

    gCountdownValue = freezeTime;  // Set the countdown to the freeze time

    if (countdownSoundEnabled)
    {
        for (int i = 1; i <= freezeTime; i++)
        {
            char soundPath[128];
            Format(soundPath, sizeof(soundPath), "zombieriot/countdown/%d.mp3", i); // Playing path: zombieriot/...
            PrecacheSound(soundPath, true);
            
            // Add to downloads with the correct path: sound/zombieriot/...
            char downloadPath[128];
            Format(downloadPath, sizeof(downloadPath), "sound/zombieriot/countdown/%d.mp3", i);
            AddFileToDownloadsTable(downloadPath);
        }
    }

    int dataToPass = (freezeTime << 1) | (countdownSoundEnabled ? 1 : 0);

    // Create the countdown timer
    CreateTimer(1.0, CountdownTimerCallback, dataToPass, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

public Action CountdownTimerCallback(Handle timer, any dataToPass)
{
    bool countdownSoundEnabled = (dataToPass & 1) != 0;

    // Decrement countdown value
    gCountdownValue--;

    if (gCountdownValue < 0)
    {
        // When countdown reaches 0, display the "run for your life" message and stop the timer
        char finalMessage[64];
        Format(finalMessage, sizeof(finalMessage), "Run for your life, zombies are coming now!");
        PrintCenterTextAll(finalMessage);
        
        return Plugin_Stop;
    }

    // Update HUD text with the countdown value
    char hudText[64];
    Format(hudText, sizeof(hudText), "Zombies are coming in %d seconds", gCountdownValue);
    PrintCenterTextAll(hudText);

    // Play countdown sound if enabled
    if (countdownSoundEnabled)
    {
        int soundToPlay = gCountdownValue;

        while (soundToPlay >= 0)
        {
            char soundPath[128];
            Format(soundPath, sizeof(soundPath), "zombieriot/countdown/%d.mp3", soundToPlay); // Playing path: zombieriot/...

            if (SoundExists(soundPath))
            {
                // Emit sound to all clients
                EmitSoundToAll(soundPath);
                break;  // Play the sound and break out of the loop
            }
            else
            {
                soundToPlay--;  // Try the next lower sound if current one does not exist
            }
        }
    }

    return Plugin_Continue;
}

bool SoundExists(const char[] soundPath)
{
    // Precache the sound
    PrecacheSound(soundPath, false);

    // Return true if the sound exists
    return true;  // As long as the sound is precached, it's valid
}