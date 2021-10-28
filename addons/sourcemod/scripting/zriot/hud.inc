/**
 * ====================
 *     Zombie Riot
 *   File: hud.inc
 *   Author: Greyscale
 * ====================
 */

public Action:HUD(Handle:timer)
{
    UpdateHUDAll();
}

UpdateHUD(clients[], numClients)
{
    new bool:hud = GetConVarBool(gCvars[CVAR_HUD]);
    
    if (tHUD == INVALID_HANDLE || !hud)
    {
        return;
    }
    
    new bool:targeting = GetConVarBool(gCvars[CVAR_TARGETING]);
    
    new livezombies = GetLiveZombieCount();
    new livehumans = GetLiveHumanCount();
    
    decl String:display[32];
    decl String:targetname[64];
    
    GetDayDisplay(gDay, display, sizeof(display));
    
    for (new x = 0; x < numClients; x++)
    {
        if (!IsClientInGame(clients[x]) || IsFakeClient(clients[x]) || GetClientTeam(clients[x]) == CS_TEAM_NONE)
        {
            continue;
        }
        
        new target = GetClientTarget(clients[x]);
        if (targeting && target != -1 && IsPlayerZombie(target) && GetClientTeam(clients[x]) != CS_TEAM_SPECTATOR)
        {
            GetClientName(target, targetname, sizeof(targetname));
            
            new health = GetClientHealth(target);
            health = (health >= 0) ? health : 0;
            
            ZRiot_HudHint(clients[x], "HUD target", gDay + 1, dCount, display, livezombies, livehumans, target, health);
        }
        else
        {
            ZRiot_HudHint(clients[x], "HUD", gDay + 1, dCount, display, livezombies, livehumans);
        }
    }
}

stock UpdateHUDClient(client)
{
    if (!IsClientInGame(client) || GetClientTeam(x) == CS_TEAM_NONE)
    {
        return;
    }
    
    new clients[1];
	clients[0] = client;
	
    UpdateHUD(clients, 1);
}

stock UpdateHUDAll()
{
    new maxplayers = GetMaxClients();
    
    new clients[maxplayers];
    new count = 0;
    
    for (new x = 1; x <= maxplayers; x++)
    {
        if (!IsClientInGame(x) || GetClientTeam(x) == CS_TEAM_NONE)
        {
            continue;
        }
        
        clients[count++] = x;
    }
    
    UpdateHUD(clients, count);
}
        
GetLiveHumanCount()
{
    new humansleft = 0;
    new maxplayers = GetMaxClients();
    for (new x = 1; x <= maxplayers; x++)
    {
        if (!IsClientInGame(x) || !IsPlayerAlive(x) || !IsPlayerHuman(x))
        {
            continue;
        }
        
        humansleft++;
    }
    
    return humansleft;
}

GetLiveZombieCount()
{
    return GetDayCount(gDay) - gZombiesKilled;
}