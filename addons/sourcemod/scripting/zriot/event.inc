/**
 * ====================
 *     Zombie Riot
 *   File: events.inc
 *   Author: Greyscale
 * ====================
 */

HookEvents()
{
    HookEvent("player_connect", PlayerConnect, EventHookMode_Pre);
    HookEvent("player_disconnect", PlayerDisconnect, EventHookMode_Pre);
    HookEvent("round_start", RoundStart);
    HookEvent("round_freeze_end", RoundFreezeEnd);
    HookEvent("round_end", RoundEnd);
    HookEvent("player_team", PlayerTeam_Pre, EventHookMode_Pre);
    HookEvent("player_team", PlayerTeam_Post, EventHookMode_Post);
    HookEvent("player_spawn", PlayerSpawn);
    HookEvent("player_hurt", PlayerHurt);
    HookEvent("player_death", PlayerDeath);
    HookEvent("player_jump", PlayerJump);
}

UnhookEvents()
{
    UnhookEvent("player_connect", PlayerConnect, EventHookMode_Pre);
    UnhookEvent("player_disconnect", PlayerDisconnect, EventHookMode_Pre);
    UnhookEvent("round_start", RoundStart);
    UnhookEvent("round_freeze_end", RoundFreezeEnd);
    UnhookEvent("round_end", RoundEnd);
    UnhookEvent("player_team", PlayerTeam_Pre, EventHookMode_Pre);
    UnhookEvent("player_team", PlayerTeam_Post, EventHookMode_Post);
    UnhookEvent("player_spawn", PlayerSpawn);
    UnhookEvent("player_hurt", PlayerHurt);
    UnhookEvent("player_death", PlayerDeath);
    UnhookEvent("player_jump", PlayerJump);
}

new bool:pcFire = true;
public Action:PlayerConnect(Handle:event, const String:name[], bool:dontBroadcast)
{
    new bool:botquota_silent = GetConVarBool(gCvars[CVAR_BOTQUOTA_SILENT]);
    if (!botquota_silent)
        return Plugin_Continue;
    
    decl String:address[64];
    GetEventString(event, "address", address, sizeof(address));
    
    if (pcFire && StrEqual(address, "none"))
    {
        decl String:pname[64];
        decl String:networkid[64];
        
        GetEventString(event, "name", pname, sizeof(pname));
        GetEventString(event, "networkid", networkid, sizeof(networkid));
        new Handle:hPlayerConnect = CreateEvent("player_connect", true);
        
        SetEventString(hPlayerConnect, "name", pname);
        SetEventInt(hPlayerConnect, "index", GetEventInt(event, "index"));
        SetEventInt(hPlayerConnect, "userid", GetEventInt(event, "userid"));
        SetEventString(hPlayerConnect, "networkid", networkid);
        SetEventString(hPlayerConnect, "address", address);
        
        pcFire = false;
        FireEvent(hPlayerConnect, true);
        pcFire = true;
        
        return Plugin_Handled;
    }
    
    return Plugin_Continue;
}

new bool:pdFire = true;
public Action:PlayerDisconnect(Handle:event, const String:name[], bool:dontBroadcast)
{
    new bool:botquota_silent = GetConVarBool(gCvars[CVAR_BOTQUOTA_SILENT]);
    if (!botquota_silent)
        return Plugin_Continue;
    
    new userid = GetEventInt(event, "userid");
    new index = GetClientOfUserId(userid);
    
    if (!index || !IsClientInGame(index))
        return Plugin_Continue;
    
    if (pdFire && IsFakeClient(index))
    {
        decl String:reason[192];
        decl String:pname[64];
        decl String:networkid[64];
        
        GetEventString(event, "reason", reason, sizeof(reason));
        GetEventString(event, "name", pname, sizeof(pname));
        GetEventString(event, "networkid", networkid, sizeof(networkid));
        
        new Handle:hPlayerDisconnect = CreateEvent("player_disconnect", true);
        
        SetEventInt(hPlayerDisconnect, "userid", userid);
        SetEventString(hPlayerDisconnect, "reason", reason);
        SetEventString(hPlayerDisconnect, "name", pname);
        SetEventString(hPlayerDisconnect, "networkid", networkid);
        
        pdFire = false;
        FireEvent(hPlayerDisconnect, true);
        pdFire = true;
        
        return Plugin_Handled;
    }
    
    return Plugin_Continue;
}

public Action:RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
    UpdateHostname();
    
    ChangeLightStyle();
    
    RestartAmbience();
    
    ServerCommand("bot_knives_only");
    
    ZRiot_PrintToChat(0, "Round objective");
    
    decl String:storyline[192];
    GetDayStoryLine(gDay, storyline, sizeof(storyline));
    
    if (storyline[0])
    {
        FormatTextString(storyline, sizeof(storyline));
    
        PrintToChatAll(storyline);
    }
    
    BeginDay();
    
    if (tHUD != INVALID_HANDLE)
    {
        CloseHandle(tHUD);
        tHUD = INVALID_HANDLE;
    }
    
    new bool:hud = GetConVarBool(gCvars[CVAR_HUD]);
    if (hud)
    {
        tHUD = CreateTimer(5.0, HUD, _, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
        
        UpdateHUDAll();
    }
    
    if (tFreeze != INVALID_HANDLE)
    {
        CloseHandle(tFreeze);
        tFreeze = INVALID_HANDLE;
    }
}

public Action:RoundFreezeEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
    RemoveObjectives();
    
    if (tFreeze != INVALID_HANDLE)
    {
        CloseHandle(tFreeze);
        tFreeze = INVALID_HANDLE;
    }
    
    new Float:freeze = GetConVarFloat(gCvars[CVAR_FREEZE]);
    if (freeze > 0)
    {
        FreezeZombies();
        
        tFreeze = CreateTimer(freeze, UnfreezeZombies);
    }   
}

public Action:RoundEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
    ResetZombies(false);
    ClearTrie(trieDeaths);
    
    CreateTimer(0.0, AssignTeamTimer);
    
    new reason = GetEventInt(event, "reason");
    
    if (reason == CTs_Win || reason == Terrorists_Win)
    {
        new winner = GetEventInt(event, "winner");
        
        if (winner == gZombieTeam)
        {
            ZombiesWin();
        }
        else if (winner == gHumanTeam)
        {
            HumansWin();
        }
    }
    else
    {
        RoundDraw();
    }

    if (tHUD != INVALID_HANDLE)
    {
        CloseHandle(tHUD);
        tHUD = INVALID_HANDLE;
    }
    
    if (tFreeze != INVALID_HANDLE)
    {
        CloseHandle(tFreeze);
        tFreeze = INVALID_HANDLE;
    }
}

new bool:ptFire = true;
public Action:PlayerTeam_Pre(Handle:event, const String:name[], bool:dontBroadcast)
{
    if (ptFire)
    {
        new Handle:hPlayerTeam = CreateEvent("player_team", true);
        
        SetEventInt(hPlayerTeam, "userid", GetEventInt(event, "userid"));
        SetEventInt(hPlayerTeam, "team", GetEventInt(event, "team"));
        SetEventInt(hPlayerTeam, "oldteam", GetEventInt(event, "oldteam"));
        SetEventBool(hPlayerTeam, "disconnect", GetEventBool(event, "disconnect"));
        
        ptFire = false;
        FireEvent(hPlayerTeam, true);
        ptFire = true;
        
        return Plugin_Handled;
    }
    
    return Plugin_Continue;
}

public Action:PlayerTeam_Post(Handle:event, const String:name[], bool:dontBroadcast)
{
    new bool:disconnect = GetEventBool(event, "disconnect");
    if (disconnect)
    {
        return;
    }
    
    new index = GetClientOfUserId(GetEventInt(event, "userid"));
    new oldteam = GetEventInt(event, "oldteam");
    new team = GetEventInt(event, "team");
    
    if (team != CS_TEAM_SPECTATOR && oldteam == CS_TEAM_NONE || oldteam == CS_TEAM_SPECTATOR)
    {
        CreateTimer(0.0, CheckTeam, index);
    }
    
    if (team == gHumanTeam)
    {
        StopZVision(index);
        SetPlayerFOV(index, DEFAULT_FOV);
        SetEntityGravity(index, DEFAULT_GRAVITY);
        
        if (IsPlayerAlive(index) || tRespawn[index] != INVALID_HANDLE || tHUD == INVALID_HANDLE)
        {
            return;
        }
        
        StartRespawnTimer(index, true);
    }
}

public Action:CheckTeam(Handle:timer, any:index)
{
    AssignTeamClient(index, IsPlayerAlive(index));
}

public Action:PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
    new index = GetClientOfUserId(GetEventInt(event, "userid"));
    
    new team = GetClientTeam(index);
    if (team != CS_TEAM_T && team != CS_TEAM_CT)
    {
        return;
    }
    
    gZombieID[index] = -1;
    
    if (IsPlayerZombie(index))
    {
        RemoveTargeters(index);
        
        InitClientDeathCount(index);
        
        SetPlayerMoney(index, 0);
        
        new bool:noblock = GetConVarBool(gCvars[CVAR_NOBLOCK]);
        if (noblock)
        {
            NoCollide(index, true);
        }
        
        decl String:zombieoverride[4];
        GetDayZombieOverride(gDay, zombieoverride, sizeof(zombieoverride));
        
        new zombieid;
        
        if (adtZombies != INVALID_HANDLE && zombieoverride[0])
        {
            new size = GetArraySize(adtZombies);
            if (size)
            {
                zombieid = GetRandomInt(0, size - 1);
                
                Zombify(index, GetArrayCell(adtZombies, zombieid));
            }
        }
        else
        {
            do
            {
                zombieid = GetRandomInt(0, zCount - 1);
            } while(IsOverrideRequired(zombieid));
            
            Zombify(index, zombieid);
        }
        
        new health = GetClientHealth(index);
        SetEntityHealth(index, health + GetDayHealthBoost(gDay));
        
        new Float:fademin = GetDayMinFade(gDay);
        new Float:fademax = GetDayMaxFade(gDay);
        
        SetPlayerMinDist(index, fademin);
        SetPlayerMaxDist(index, fademax);
        
        new knife = GetPlayerWeaponSlot(index, 2);
        
        if (knife != -1)
        {
            SetEntityRenderMode(knife, RENDER_TRANSALPHA);
            SetEntityRenderColor(knife, 255, 255, 255, 0);
        }
    }
    else
    {
        if (market)
        {
            ZRiot_PrintToChat(index, "!market reminder");
        }
        
        new bool:noblock = GetConVarBool(gCvars[CVAR_NOBLOCK]);
        if (noblock)
        {
            NoCollide(index, false);
        }
        
        SetPlayerMinDist(index, 0.0);
        SetPlayerMaxDist(index, 0.0);
        
        SetPlayerFOV(index, DEFAULT_FOV);
        SetEntityGravity(index, DEFAULT_GRAVITY);
        
        new bool:cashfill = GetConVarBool(gCvars[CVAR_CASHFILL]);
        if (cashfill)
        {
            new cash = GetConVarInt(gCvars[CVAR_CASHAMOUNT]);
            SetPlayerMoney(index, cash);
        }
        
        if (tZVision[index] != INVALID_HANDLE)
        {
            CloseHandle(tZVision[index]);
            tZVision[index] = INVALID_HANDLE;
        }
        
        ClientCommand(index, "r_screenoverlay \"\"");
        
        RemoveTargeters(index);
        UpdateHUDAll();
    }
    
    if (tRespawn[index] != INVALID_HANDLE)
    {
        CloseHandle(tRespawn[index]);
        tRespawn[index] = INVALID_HANDLE;
    }
}

public Action:PlayerHurt(Handle:event, const String:name[], bool:dontBroadcast)
{
    new index = GetClientOfUserId(GetEventInt(event, "userid"));
    new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
    
    decl String:weapon[32];
    GetEventString(event, "weapon", weapon, sizeof(weapon));
    
    if (!IsPlayerZombie(index))
    {
        return;
    }
    
    if (attacker)
    {
        TargetPlayer(attacker, index);
    }
    
    new maxplayers = GetMaxClients();
    new clients[maxplayers];
    new numClients = GetClientTargeters(index, clients, maxplayers);
    
    UpdateHUD(clients, numClients);
    
    if (GetRandomInt(1, 5) == 1)
    {
        decl String:sound[64];
        new randsound = GetRandomInt(1, 6);
        
        Format(sound, sizeof(sound), "npc/zombie/zombie_pain%d.wav", randsound);
        
        PrecacheSound(sound);
        EmitSoundToAll(sound, index);
    }
    
    new bool:napalm = GetConVarBool(gCvars[CVAR_NAPALM]);
    
    if (napalm && StrEqual(weapon, "hegrenade", false))
    {
        new Float:burntime = GetConVarFloat(gCvars[CVAR_NAPALM_TIME]);
        IgniteEntity(index, burntime);
    }
}

public Action:PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
    new index = GetClientOfUserId(GetEventInt(event, "userid"));
    
    if (tHUD == INVALID_HANDLE)
    {
        return;
    }
    
    new bool:respawn = GetDayRespawn(gDay);
    
    if (IsPlayerZombie(index))
    {
        ExtinguishEntity(index);
        
        SetEntProp(index, Prop_Data, "m_takedamage", 2, 1);
        SetEntProp(index, Prop_Data, "m_fFlags", FL_ONGROUND);
        
        decl String:sound[64];
        
        new randsound = GetRandomInt(1, 3);
        Format(sound, sizeof(sound), "npc/zombie/zombie_die%d.wav", randsound);
        
        PrecacheSound(sound);
        EmitSoundToAll(sound, index);
        
        new zombiecount = GetLiveZombieCount();
        new zombiemax = GetConVarInt(gCvars[CVAR_ZOMBIEMAX]);
        
        if (respawn || zombiecount > zombiemax)
        {
            CreateTimer(0.5, ZombieRespawn, index, TIMER_FLAG_NO_MAPCHANGE);
        }
        
        gZombiesKilled++;
        
        RemoveTargeters(index);
        UpdateHUDAll();
    
        if (gZombiesKilled >= GetDayCount(gDay) && respawn)
        {
            TerminateRound(5.0, CTs_Win);
        }
    }
    else
    {
        ChangeClientDeathCount(index, 1);
        new deaths_before_zombie = GetDayDeathsBeforeZombie(gDay);
        
        if (deaths_before_zombie > 0 && GetClientDeathCount(index) >= deaths_before_zombie && GetLiveHumanCount() > 0)
        {
            ZRiot_PrintToChat(index, "You are now a zombie");
            
            CreateTimer(0.5, JoinZombie, index);
        }
        else
        {
            StartRespawnTimer(index, false);
        }
        
        RemoveTargeters(index);
        UpdateHUDAll();
        
        if (GetLiveHumanCount() <= 0 && respawn)
        {
            TerminateRound(5.0, Terrorists_Win);
        }
    }
    
    new Float:delay = GetConVarFloat(gCvars[CVAR_RAGDOLL_REMOVE]);
    if (delay > 0.0)
    {
        new ragdoll = GetEntPropEnt(index, Prop_Send, "m_hRagdoll");
        
        CreateTimer(delay, RemoveRagdoll, ragdoll);
    }
        
    if (tZVision[index] != INVALID_HANDLE)
    {
        CloseHandle(tZVision[index]);
        tZVision[index] = INVALID_HANDLE;
    }
}

public Action:PlayerJump(Handle:event, const String:name[], bool:dontBroadcast)
{
    new index = GetClientOfUserId(GetEventInt(event, "userid"));
    
    if (!IsPlayerZombie(index))
    {
        return;
    }
    
    new Float:vel[3] = {0.0, 0.0, 0.0};
    vel[2] = GetZombieJump(gZombieID[index]);
    
    SetPlayerVelocity(index, vel);
}