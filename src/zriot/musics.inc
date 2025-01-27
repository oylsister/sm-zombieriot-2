#define MAX_MUSICS 100
#define MAX_PATH_LENGTH PLATFORM_MAX_PATH
#define MAX_CLIENTS 64

Handle kvMusics = INVALID_HANDLE;

char arrayMusics[MAX_MUSICS][MAX_NAME_LENGTH];
char arrayMusicPaths[MAX_MUSICS][MAX_PATH_LENGTH];
int mCount = 0;
int sequentialIndex = 0;

void MusicCommandInit()
{
    RegConsoleCmd("stopmusic", Command_StopMusic);
    RegConsoleCmd("music", Command_PlayMusic);
}

void LoadMusicData(bool defaultconfig)
{
    char path[PLATFORM_MAX_PATH];
    Format(path, sizeof(path), "%s/musics.txt", gMapConfig);

    if (!defaultconfig && !FileExists(path))
    {
        return;
    }

    if (kvMusics != INVALID_HANDLE)
    {
        CloseHandle(kvMusics);
    }

    kvMusics = CreateKeyValues("musics");
    KvSetEscapeSequences(kvMusics, true);

    if (!FileToKeyValues(kvMusics, path))
    {
        SetFailState("\"%s\" failed to load", path);
    }

    KvRewind(kvMusics);
    if (!KvGotoFirstSubKey(kvMusics))
    {
        SetFailState("No music data defined in \"%s\"", path);
    }

    mCount = 0;
    do
    {
        char name[MAX_NAME_LENGTH];
        char musicpath[MAX_PATH_LENGTH];

        KvGetString(kvMusics, "name", name, sizeof(name));
        strcopy(arrayMusics[mCount], MAX_NAME_LENGTH, name);

        KvGetString(kvMusics, "path", musicpath, sizeof(musicpath));
        strcopy(arrayMusicPaths[mCount], MAX_PATH_LENGTH, musicpath);

        // Precache the sound directly from the specified path
        PrecacheSound(musicpath, true);

        char downloadPath[MAX_PATH_LENGTH];
        Format(downloadPath, sizeof(downloadPath), "sound/%s", musicpath);
        AddFileToDownloadsTable(downloadPath);

        mCount++;
        if (mCount >= MAX_MUSICS)
        {
            SetFailState("Too many music entries, increase MAX_MUSICS.");
            break;
        }

    } while (KvGotoNextKey(kvMusics));
}

void PlayMusic()
{
    if (GetConVarInt(gCvars.CVAR_MUSICS) != 1 || mCount <= 0)
    {
        return;
    }

    char currentMusicName[MAX_NAME_LENGTH];
    char currentMusicPath[MAX_PATH_LENGTH];

    int randomIndex = -1;

    if (GetConVarInt(gCvars.CVAR_MUSICS_MODE) == 1)
    {
        randomIndex = GetRandomInt(0, mCount - 1);
    }
    else
    {
        randomIndex = sequentialIndex;
        sequentialIndex++;
        if (sequentialIndex >= mCount)
        {
            sequentialIndex = 0;
        }
    }

    strcopy(currentMusicName, MAX_NAME_LENGTH, arrayMusics[randomIndex]);
    strcopy(currentMusicPath, MAX_PATH_LENGTH, arrayMusicPaths[randomIndex]);

    PrecacheSound(currentMusicPath, true);

    char downloadPath[MAX_PATH_LENGTH];
    Format(downloadPath, sizeof(downloadPath), "sound/%s", currentMusicPath);
    AddFileToDownloadsTable(downloadPath);

    // Play the music to all clients who haven't stopped it
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && !IsFakeClient(i) && !g_bStopHearingMusic[i])
        {
            EmitSoundToClient(i, currentMusicPath);
            ZRiot_PrintToChat(i, "Currently playing: %s", currentMusicName);
        }
        else if (IsClientInGame(i) && !IsFakeClient(i) && g_bStopHearingMusic[i])
        {
            ZRiot_PrintToChat(i, "Music is currently stopped for you.");
        }
    }
}

void StopMusic()
{
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && !IsFakeClient(i))
        {
            EmitSoundToClient(i, ""); // Stop any music currently playing for the client
            PrintToChat(i, "Music has been stopped.");
        }
    }
}

public Action Command_StopMusic(int client, int args)
{
    if (IsClientInGame(client) && !IsFakeClient(client))
    {
        if (!g_bStopHearingMusic[client])
        {
            // Stop music for this client
            g_bStopHearingMusic[client] = true;
            EmitSoundToClient(client, ""); // Stop any currently playing sound for the client
            ZRiot_PrintToChat(client, "Music has been stopped for you.");
        }
        else
        {
            // Re-enable music for this client
            g_bStopHearingMusic[client] = false;
            ZRiot_PrintToChat(client, "Music is now enabled for you.");
        }
    }

    return Plugin_Handled;
}

public Action Command_PlayMusic(int client, int args)
{
    if (IsClientInGame(client) && !IsFakeClient(client) && !g_bStopHearingMusic[client])
    {
        PlayMusic();
        ZRiot_PrintToChat(client, "Now Playing");
    }
    else if (g_bStopHearingMusic[client])
    {
        ZRiot_PrintToChat(client, "Disabled Music");
    }

    return Plugin_Handled;
}

void ResetMusic()
{
    for (int i = 1; i <= MaxClients; i++)
    {
        g_bStopHearingMusic[i] = false;
    }
}
