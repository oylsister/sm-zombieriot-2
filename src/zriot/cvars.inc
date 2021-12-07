/**
 * ====================
 *     Zombie Riot
 *   File: cvars.inc
 *   Author: Greyscale
 * ====================
 */

enum struct ZRiotSettings
{
    Handle CVAR_ENABLE;
    Handle CVAR_AMBIENCE;
    Handle CVAR_AMBIENCE_FILE;
    Handle CVAR_AMBIENCE_LENGTH;
    Handle CVAR_AMBIENCE_VOLUME;
    Handle CVAR_HOSTNAME_UPDATE;
    Handle CVAR_ZVISION_REDISPLAY;
    Handle CVAR_ZVISION_ALLOW_DISABLE;
    Handle CVAR_NOBLOCK;
    Handle CVAR_FREEZE;
    Handle CVAR_BOTQUOTA_SILENT;
    Handle CVAR_REGRESSION;
    Handle CVAR_FIRST_RESPAWN;
    Handle CVAR_RESPAWN;
    Handle CVAR_ZOMBIETEAM;
    Handle CVAR_ZOMBIEMAX;
    Handle CVAR_HUD;
    Handle CVAR_TARGETING;
    Handle CVAR_ROUNDFADE;
    Handle CVAR_OVERLAYS;
    Handle CVAR_OVERLAYS_ZOMBIE;
    Handle CVAR_OVERLAYS_HUMAN;
    Handle CVAR_RAGDOLL_REMOVE;
    Handle CVAR_NAPALM;
    Handle CVAR_NAPALM_TIME;
    Handle CVAR_DARK;
    Handle CVAR_DARK_LEVEL;
    Handle CVAR_DARK_SKY;
    Handle CVAR_ZMARKET_BUYZONE;
    Handle CVAR_CASHFILL;
    Handle CVAR_CASHAMOUNT;
    Handle CVAR_TRIGGERMAPVOTE;
    Handle CVAR_MAPVOTEDAYLEFT;
    Handle CVAR_SPAWNPROTECT;
    Handle CVAR_SPAWNPROTECTTIME;
    Handle CVAR_SPAWNPROTECTSPEED;
    Handle CVAR_DYNAMIC_ENABLE;
    Handle CVAR_DYNAMIC_PLAYERRATIO;
}

ZRiotSettings gCvars;

void CreateCvars()
{
    gCvars.CVAR_ENABLE                =    CreateConVar("zriot_enable", "1", "Enable ZombieRiot gameplay (0: Disable)");
    gCvars.CVAR_AMBIENCE              =    CreateConVar("zriot_ambience", "1", "Enable creepy ambience to be played throughout the game (0: Disable)");
    gCvars.CVAR_AMBIENCE_FILE         =    CreateConVar("zriot_ambience_file", "ambient/zr/zr_ambience.mp3", "Path to ambient sound file that will be played throughout the game, when zriot_ambience is 1");
    gCvars.CVAR_AMBIENCE_LENGTH       =    CreateConVar("zriot_ambience_length", "60.0", "The length, in seconds, of the ambient sound file");
    gCvars.CVAR_AMBIENCE_VOLUME       =    CreateConVar("zriot_ambience_volume", "0.6", "Volume of ambient sounds when zriot_ambience is 1 (0.0: Unhearable,  1.0: Max volume)");
    gCvars.CVAR_HOSTNAME_UPDATE       =    CreateConVar("zriot_hostname_update", "1", "Updates the server's hostname to display the current day server is playing (0: Disable)");
    gCvars.CVAR_ZVISION_REDISPLAY     =    CreateConVar("zriot_zvision_redisplay", "0.2", "Frequency, in seconds, to display zvision on the zombies' screen (Never go below 0.1, 0.2 seems safe)");
    gCvars.CVAR_ZVISION_ALLOW_DISABLE =    CreateConVar("zriot_zvision_allow_disable", "1", "Allow users to disable ZVision with their nightvision key (0: Disable)");
    gCvars.CVAR_REGRESSION            =    CreateConVar("zriot_regression", "1", "If the zombies win the round, the game will regress one day (0: Disable)");
    gCvars.CVAR_NOBLOCK               =    CreateConVar("zriot_noblock", "1", "Prevents zombies from getting stuck in each other (0: Disable)");
    gCvars.CVAR_FREEZE                =    CreateConVar("zriot_freeze", "10", "Time, in seconds, to freeze zombies at round start to allow humans to get set up (0: Disable)");
    gCvars.CVAR_BOTQUOTA_SILENT       =    CreateConVar("zriot_botquota_silent", "1", "Blocks join/leave text for bots (0: Disable)");
    gCvars.CVAR_FIRST_RESPAWN         =    CreateConVar("zriot_first_respawn", "10", "Amount of time to wait before spawning a player into the game for the first time (0: Disable)");
    gCvars.CVAR_RESPAWN               =    CreateConVar("zriot_respawn", "30", "Amount of time each human has to wait before they will respawn into the game (0: Disable)");
    gCvars.CVAR_ZOMBIETEAM            =    CreateConVar("zriot_zombieteam", "t", "Which team zombie's will be on (t: Terrorist ct: Counter-Terrorist)");
    gCvars.CVAR_ZOMBIEMAX             =    CreateConVar("zriot_zombiemax", "12", "The max amount of zombies spawned at one time");
    gCvars.CVAR_HUD                   =    CreateConVar("zriot_hud", "1", "Enable persistent display of the HUD which displays day, zombies left, and humans left (0: Disable)");
    gCvars.CVAR_TARGETING             =    CreateConVar("zriot_targeting", "1", "Enables a system that tracks damage done to each zombie, and shows you each one's current health (0: Disable)");
    gCvars.CVAR_ROUNDFADE             =    CreateConVar("zriot_roundfade", "0", "Player's screens fade blue if humans win, red if zombies in, and black in any other case (0: Disable)");
    gCvars.CVAR_OVERLAYS              =    CreateConVar("zriot_overlays", "1", "Enable use of round end overlays to show the winner (0: Disable)");
    gCvars.CVAR_OVERLAYS_ZOMBIE       =    CreateConVar("zriot_overlays_zombie", "overlays/zr/zombies_win", "Path to overlay shown when Zombies win, when zriot_overlays is 1");
    gCvars.CVAR_OVERLAYS_HUMAN        =    CreateConVar("zriot_overlays_human", "overlays/zr/humans_win", "Path to overlay shown when Humans win, when zriot_overlays is 1");
    gCvars.CVAR_RAGDOLL_REMOVE        =    CreateConVar("zriot_ragdoll_remove", "20", "The time, in seconds, before the ragdoll of dead zombies will be deleted (0: Disable)");
    gCvars.CVAR_NAPALM                =    CreateConVar("zriot_napalm", "1", "Turns grenades into napalm grenades that light zombies on fire (0: Disable)");
    gCvars.CVAR_NAPALM_TIME           =    CreateConVar("zriot_napalm_time", "20", "How long the zombie burns when zr_napalm is 1");
    gCvars.CVAR_DARK                  =    CreateConVar("zriot_dark", "0", "Darkens the map (0: Disable)");
    gCvars.CVAR_DARK_LEVEL            =    CreateConVar("zriot_dark_level", "a", "The darkness of the map,  a being the darkest,  z being extremely bright when zriot_dark is 1 (n: Default)");
    gCvars.CVAR_DARK_SKY              =    CreateConVar("zriot_dark_sky", "sky_borealis01", "The sky the map will have when zriot_dark is 1");
    gCvars.CVAR_ZMARKET_BUYZONE       =    CreateConVar("zriot_zmarket_buyzone", "0", "Must be in buyzone to access !zmarket, if Market is installed (0: Can be used anywhere)");
    gCvars.CVAR_CASHFILL              =    CreateConVar("zriot_cashfill", "1", "Enable the mod to set the players cash to zriot_cashamount (0: Disabled)");
    gCvars.CVAR_CASHAMOUNT            =    CreateConVar("zriot_cashamount", "12000", "How much money players will have when they spawn when zriot_cashfill is 1");
    gCvars.CVAR_TRIGGERMAPVOTE        =    CreateConVar("zriot_triggermapvote", "1.0", "Trigger the mapvote before the map end for Zriot mode (0: Disable)");
    gCvars.CVAR_MAPVOTEDAYLEFT        =    CreateConVar("zriot_mapvotedayleft", "2.0", "How many day left before map vote start");
    gCvars.CVAR_SPAWNPROTECT          =    CreateConVar("zriot_spawnprotect", "1.0", "Enable Spawn Protection for human or not (0: Disable)");
    gCvars.CVAR_SPAWNPROTECTTIME      =    CreateConVar("zriot_spawnprotecttime", "7.0", "How long that client will get immune from every damage before back to normal again");
    gCvars.CVAR_SPAWNPROTECTSPEED     =    CreateConVar("zriot_spawnprotectspeed", "400.0", "How much speed that client will get after spawn before back to normal");
    gCvars.CVAR_DYNAMIC_ENABLE        =    CreateConVar("zriot_dynamic_enable", "0.0", "Enable Dyanmic Zombie mode (Difficulty Dynamic) WARNING: This is still unstable.");
    gCvars.CVAR_DYNAMIC_PLAYERRATIO   =    CreateConVar("zriot_dynamic_playerratio", "10.0", "Every n player the zombie will get buffed");
    
    HookConVarChange(gCvars.CVAR_ENABLE, EnableHook);
    
    AutoExecConfig(true, "zombieriot", "sourcemod/zombieriot");
}

void HookCvars()
{
    SetConVarBool(FindConVar("mp_autoteambalance"), false);
    SetConVarInt(FindConVar("mp_limitteams"), 0);
    
    HookConVarChange(FindConVar("mp_autoteambalance"), AutoTeamBalanceHook);
    HookConVarChange(FindConVar("mp_limitteams"), LimitTeamsHook);
    
    HookConVarChange(gCvars.CVAR_ZOMBIETEAM, ZombieTeamHook);
    
    HookConVarChange(FindConVar("mp_restartgame"), RestartGameHook);
}

void UnhookCvars()
{
    UnhookConVarChange(FindConVar("mp_autoteambalance"), AutoTeamBalanceHook);
    UnhookConVarChange(FindConVar("mp_limitteams"), LimitTeamsHook);
    
    UnhookConVarChange(gCvars.CVAR_ZOMBIETEAM, ZombieTeamHook);
    
    UnhookConVarChange(FindConVar("mp_restartgame"), RestartGameHook);
}

public void EnableHook(Handle convar, const char[] oldValue, const char[] newValue)
{
    bool enable = view_as<bool>(StringToInt(newValue));
    
    if (enable)
    {
        FindMapSky();
        
        FindHostname();
        UpdateHostname();
        
        HookEvents();
        HookCvars();
        
        ServerCommand("bot_kick");
        
        gDay = 0;
        
        CS_TerminateRound(3.0, CSRoundEnd_GameStart, true);
    }
    else
    {
        ZRiotEnd();
    }
}

public void AutoTeamBalanceHook(Handle convar, const char[] oldValue, const char[] newValue)
{
    SetConVarBool(convar, false);
}

public void LimitTeamsHook(Handle convar, const char[] oldValue, const char[] newValue)
{
    SetConVarInt(convar, 0);
}

public void ZombieTeamHook(Handle convar, const char[] oldValue, const char[] newValue)
{
    if (StrEqual(newValue, "t", false) || StrEqual(newValue, "ct", false))
    {
        UpdateTeams();
    }
}

public void RestartGameHook(Handle convar, const char[] oldValue, const char[] newValue)
{
    gDay = 0;
    
    ResetZombies(true);
}