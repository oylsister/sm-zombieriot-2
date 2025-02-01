#define MAX_MUSICS 100
#define MAX_PATH_LENGTH PLATFORM_MAX_PATH
#define MAX_CLIENTS 64
#define MAX_NAME_LENGTH 64

Handle kvMusics = INVALID_HANDLE;

char arrayMusics[MAX_MUSICS][MAX_NAME_LENGTH];
char arrayMusicPaths[MAX_MUSICS][MAX_PATH_LENGTH];
int mCount = 0;
int sequentialIndex = 0;
char g_sCurrentMusicName[MAX_NAME_LENGTH]

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
    PrintToServer("PlayMusic() called. g_bMusicPlaying: %d, CVAR_MUSICS: %d, mCount: %d", g_bMusicPlaying, GetConVarInt(gCvars.CVAR_MUSICS), mCount);

    if (g_bMusicPlaying || GetConVarInt(gCvars.CVAR_MUSICS) != 1 || mCount <= 0)
    {
        PrintToServer("PlayMusic() skipped: g_bMusicPlaying: %d, CVAR_MUSICS: %d, mCount: %d", g_bMusicPlaying, GetConVarInt(gCvars.CVAR_MUSICS), mCount);
        return;
    }

    g_bMusicPlaying = true;

    int randomIndex = (GetConVarInt(gCvars.CVAR_MUSICS_MODE) == 1) ? GetRandomInt(0, mCount - 1) : sequentialIndex++;
    if (sequentialIndex >= mCount) sequentialIndex = 0;

    char currentMusicPath[MAX_PATH_LENGTH];
    strcopy(currentMusicPath, MAX_PATH_LENGTH, arrayMusicPaths[randomIndex]);
    strcopy(g_sCurrentMusicName, MAX_NAME_LENGTH, arrayMusics[randomIndex]); // Store the current music name
    PrecacheSound(currentMusicPath, true);

    PrintToServer("Playing music: %s", g_sCurrentMusicName);

    // Play the music for all eligible clients
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && !IsFakeClient(i) && g_bMusicEnabled[i])
        {
            EmitSoundToClient(i, currentMusicPath);
            CPrintToChat(i, "{fullred}[ZombieRiot] Now playing: {green}%s", g_sCurrentMusicName); // Notify the client
        }
    }
}

public Action Command_PlayMusic(int client, int args)
{
    if (IsClientInGame(client) && !IsFakeClient(client))
    {
        if (GetConVarInt(gCvars.CVAR_MUSICS) == 0)
        {
            CPrintToChat(client, "{fullred}[ZombieRiot] Music is disabled for this server.");
            return Plugin_Handled;
        }

        if (!g_bMusicEnabled[client])
        {
            g_bMusicEnabled[client] = true;
            CPrintToChat(client, "{fullred}[ZombieRiot] Music has been enabled for you.");
            if (g_bMusicPlaying)
            {
                CPrintToChat(client, "{fullred}[ZombieRiot] Now playing: {green}%s", g_sCurrentMusicName); // Show the current music
                PlayMusicForClient(client);
            }
        }
        else
        {
            CPrintToChat(client, "{fullred}[ZombieRiot] Music is already enabled for you.");
            if (g_bMusicPlaying)
            {
                CPrintToChat(client, "{fullred}[ZombieRiot] Now playing: {green}%s", g_sCurrentMusicName); // Show the current music
            }
        }
    }
    return Plugin_Handled;
}

void PlayMusicForClient(int client)
{
    if (g_bMusicPlaying && IsClientInGame(client) && !IsFakeClient(client) && g_bMusicEnabled[client])
    {
        char currentMusicPath[MAX_PATH_LENGTH];
        strcopy(currentMusicPath, MAX_PATH_LENGTH, arrayMusicPaths[sequentialIndex]);
        EmitSoundToClient(client, currentMusicPath);
    }
}

public Action Command_StopMusic(int client, int args)
{
    if (IsClientInGame(client) && !IsFakeClient(client))
    {
        g_bMusicEnabled[client] = false;
        StopMusicForClient(client);
        CPrintToChat(client, "{fullred}[ZombieRiot] Music has been stopped for you.");
    }
    return Plugin_Handled;
}

void StopMusicForClient(int client)
{
    if (IsClientInGame(client) && !IsFakeClient(client))
    {
        // Stop the sound immediately
        EmitSoundToClient(client, "", SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NONE, SND_STOP, 0.0);
    }
}

void StopMusicForAll()
{
    g_bMusicPlaying = false;
    for (int i = 1; i <= MaxClients; i++)
    {
        if (IsClientInGame(i) && !IsFakeClient(i))
        {
            EmitSoundToClient(i, "", SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NONE, SND_STOP, 0.0);
        }
    }
}

void ResetMusic()
{
    g_bMusicPlaying = false;
    for (int i = 1; i <= MaxClients; i++)
    {
        g_bMusicEnabled[i] = true; // Enable music by default for everyone
    }
}

public Action Timer_PlayMusicOnJoin(Handle timer, any client)
{
    if (IsClientInGame(client) && !IsFakeClient(client) && g_bMusicEnabled[client])
    {
        PlayMusicForClient(client);
    }
    return Plugin_Stop;
}