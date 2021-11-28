/**
 * ====================
 *     Zombie Riot
 *   File: spawnprotect.inc
 *   Author: Oylsister
 * ====================
 */

#define DEFAULT_SPEED 300.0

void SpawnProtectOnClientSpawn(int client)
{
    if(!IsPlayerAlive(client))
    {
        return;
    }

    if(IsPlayerZombie(client))
    {
        return;
    }

    if(g_fClientProtectTimer[client] != INVALID_HANDLE)
    {
        KillTimer(g_fClientProtectTimer[client]);
    }

    float g_fProtectTime = GetConVarFloat(gCvars.CVAR_SPAWNPROTECTTIME);
    float g_fSpawnSpeed = GetConVarFloat(gCvars.CVAR_SPAWNPROTECTSPEED);

    g_bClientProtected[client] = true;

    if(g_fClientProtectTimer[client] == INVALID_HANDLE)
    {
        g_fClientProtectTimer[client] = CreateTimer(1.0, Timer_Protect, client, TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
    }

    g_iCountdown[client] = RoundToNearest(g_fProtectTime);
    ZRiot_HudHint(client, "Spawn Protect End in: %i seconds", g_iCountdown[client]);
    ZRiot_PrintToChat(client, "You're now having spawn protection, Running to the safe place!");

    SetPlayerSpeed(client, g_fSpawnSpeed);
}

void SpawnProtectOnClientDeath(int client)
{
    if(g_fClientProtectTimer[client] != INVALID_HANDLE)
    {
        KillTimer(g_fClientProtectTimer[client]);
    }
    g_fClientProtectTimer[client] = INVALID_HANDLE;
}

public Action Timer_Protect(Handle timer, any client)
{
    if(!IsClientInGame(client))
    {
        g_fClientProtectTimer[client] = INVALID_HANDLE;
        return Plugin_Stop;
    }

    if(!IsPlayerHuman(client))
    {
        g_fClientProtectTimer[client] = INVALID_HANDLE;
        return Plugin_Stop;
    }

    g_iCountdown[client]--;

    ZRiot_HudHint(client, "Spawn Protect End in: %i seconds", g_iCountdown[client]);

    if(g_iCountdown[client] <= 0)
    {
        g_bClientProtected[client] = false;
        SetPlayerSpeed(client, DEFAULT_SPEED);

        g_fClientProtectTimer[client] = INVALID_HANDLE;
        ZRiot_HudHint(client, "You're no longer protected.");
        ZRiot_PrintToChat(client, "You're no longer protected.");

        return Plugin_Stop;
    }

    return Plugin_Continue;
}



