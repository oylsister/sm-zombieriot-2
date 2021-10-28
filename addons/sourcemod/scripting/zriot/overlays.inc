/**
 * ====================
 *     Zombie Riot
 *   File: overlays.inc
 *   Author: Greyscale
 * ==================== 
 */

ShowOverlays(Float:time, winner)
{
    decl String:overlay[64];
    if (winner == gZombieTeam)
    {
        GetConVarString(gCvars[CVAR_OVERLAYS_ZOMBIE], overlay, sizeof(overlay));
    }
    else if (winner == gHumanTeam)
    {
        GetConVarString(gCvars[CVAR_OVERLAYS_HUMAN], overlay, sizeof(overlay));
    }
        
    new maxplayers = GetMaxClients();
    for (new x = 1; x <= maxplayers; x++)
    {
        if (!IsClientInGame(x))
            continue;
        
        DisplayClientOverlay(x, overlay);
    }
    
    CreateTimer(time, KillOverlays);
}

public Action:KillOverlays(Handle:timer)
{
    new maxplayers = GetMaxClients();
    for (new x = 1; x <= maxplayers; x++)
    {
        if (!IsClientInGame(x))
            continue;
        
        ClientCommand(x, "r_screenoverlay \"\"");
    }
}