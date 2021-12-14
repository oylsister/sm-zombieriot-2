/**
 * ====================
 *     Zombie Riot
 *   File: ambience.inc
 *   Author: Greyscale
 * ====================
 */

bool soundValid = false;

Handle tAmbience = INVALID_HANDLE;

void LoadAmbienceData()
{
    bool ambience = GetConVarBool(gCvars.CVAR_AMBIENCE);
    if (!ambience)
    {
        return;
    }
    
    char sound[64];
    GetConVarString(gCvars.CVAR_AMBIENCE_FILE, sound, sizeof(sound));
    Format(sound, sizeof(sound), "sound/%s", sound);
    
    soundValid = FileExists(sound, true);
    
    if (soundValid)
    {
        AddFileToDownloadsTable(sound);
    }
    else
    {
        ZRiot_LogMessage("Ambient sound load failed", sound);
    }
}

void RestartAmbience()
{
    if (tAmbience != INVALID_HANDLE)
    {
        CloseHandle(tAmbience);
    }
    
    CreateTimer(0.0, AmbienceLoop, _, TIMER_FLAG_NO_MAPCHANGE);
}

public Action AmbienceLoop(Handle timer)
{
    bool ambience = GetConVarBool(gCvars.CVAR_AMBIENCE);
    
    if (!ambience || !soundValid)
    {
        return;
    }
    
    char sound[64];
    GetConVarString(gCvars.CVAR_AMBIENCE_FILE, sound, sizeof(sound));
    
    EmitAmbience(sound);
    
    float delay = GetConVarFloat(gCvars.CVAR_AMBIENCE_LENGTH);
    tAmbience = CreateTimer(delay, AmbienceLoop, _, TIMER_FLAG_NO_MAPCHANGE);
}

void StopAmbience()
{
    bool ambience = GetConVarBool(gCvars.CVAR_AMBIENCE);
    
    if (!ambience)
    {
        return;
    }
    
    char sound[64];
    GetConVarString(gCvars.CVAR_AMBIENCE_FILE, sound, sizeof(sound));
    
    for (int x = 1; x <= MaxClients; x++)
    {
        if (!IsClientInGame(x))
        {
            continue;
        }
        
        StopSound(x, SNDCHAN_AUTO, sound);
    }
}

void EmitAmbience(const char[] sound)
{
    PrecacheSound(sound);
    
    StopAmbience();
    
    //float volume = GetConVarFloat(gCvars.CVAR_AMBIENCE_VOLUME);
    
    for (int i = 1; i <= MaxClients; i++)
    {
        if(g_fAmbienceVolume[i] != 0.0)
        {
            float realvolume = g_fAmbienceVolume[i] / 100.0;
            EmitSoundToClient(i, sound, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, realvolume, SNDPITCH_NORMAL, -1, NULL_VECTOR, NULL_VECTOR, true, 0.0);
        }
    }
}