/**
 * ====================
 *     Zombie Riot
 *   File: hud.inc
 *   Author: Greyscale
 * ====================
 */

public Action HUD(Handle timer)
{
    UpdateHUDAll();
}

void UpdateHUD(int[] clients, int numClients)
{
    bool hud = GetConVarBool(gCvars.CVAR_HUD);
    
    if (tHUD == INVALID_HANDLE || !hud)
    {
        return;
    }
    
    bool targeting = GetConVarBool(gCvars.CVAR_TARGETING);
    
    int livezombies = GetLiveZombieCount();
    int livehumans = GetLiveHumanCount();
    
    char display[32];
    char targetname[64];
    
    GetDayDisplay(gDay, display, sizeof(display));
    
    for (int x = 0; x < numClients; x++)
    {
        if (!IsClientInGame(clients[x]) || IsFakeClient(clients[x]) || GetClientTeam(clients[x]) == CS_TEAM_NONE)
        {
            continue;
        }
        
        int target = GetClientTarget(clients[x]);
        if (targeting && target != -1 && IsPlayerZombie(target) && GetClientTeam(clients[x]) != CS_TEAM_SPECTATOR)
        {
            GetClientName(target, targetname, sizeof(targetname));
            
            int health = GetClientHealth(target);
            health = (health >= 0) ? health : 0;
            
            ZRiot_HudHint(clients[x], "HUD target", gDay + 1, dCount, display, livezombies, livehumans, target, health);
        }
        else
        {
            ZRiot_HudHint(clients[x], "HUD", gDay + 1, dCount, display, livezombies, livehumans);
        }
    }
}

stock void UpdateHUDClient(int client)
{
    if (!IsClientInGame(client) || GetClientTeam(x) == CS_TEAM_NONE)
    {
        return;
    }
    
    int clients[1];
	clients[0] = client;
	
    UpdateHUD(clients, 1);
}

stock void UpdateHUDAll()
{
    int clients[64];
    int count = 0;
    
    for (int x = 1; x <= MaxClients; x++)
    {
        if (!IsClientInGame(x) || GetClientTeam(x) == CS_TEAM_NONE)
        {
            continue;
        }
        
        clients[count++] = x;
    }
    
    UpdateHUD(clients, count);
}
        
int GetLiveHumanCount()
{
    int humansleft = 0;

    for (int x = 1; x <= MaxClients; x++)
    {
        if (!IsClientInGame(x) || !IsPlayerAlive(x) || !IsPlayerHuman(x))
        {
            continue;
        }
        
        humansleft++;
    }
    
    return humansleft;
}

int GetLiveZombieCount()
{
    return GetDayCount(gDay) - gZombiesKilled;
}