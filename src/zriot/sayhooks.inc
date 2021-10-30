/**
 * ====================
 *     Zombie Riot
 *   File: sayhooks.inc
 *   Author: Greyscale
 * ====================
 */

void HookChatCmds()
{
    RegConsoleCmd("say", SayCommand);
    RegConsoleCmd("say_team", SayCommand);
}

public Action SayCommand(int client, int argc)
{
    bool enabled = GetConVarBool(gCvars.CVAR_ENABLE);
    if (!client || !enabled)
    {
        return Plugin_Continue;
    }
    
    char args[192];
    
    GetCmdArgString(args, sizeof(args));
    ReplaceString(args, sizeof(args), "\"", "");
    
    if (StrEqual(args, "!market", false))
    {
        Market(client);
        return Plugin_Handled;
    }
    
    return Plugin_Continue;
}

void Market(int client)
{
    if (!market)
    {
        ZRiot_PrintToChat(client, "Feature is disabled");
        
        return;
    }
    
    bool buyzone = GetConVarBool(gCvars.CVAR_ZMARKET_BUYZONE); 
    if (!IsClientInBuyZone(client) && buyzone)
    {
        ZRiot_PrintCenterText(client, "Market out of buyzone");
        
        return;
    }
    
    SetGlobalTransTarget(client);
    
    char title[64];
    char rebuy[64];
    
    Format(title, sizeof(title), "%t\n ", "Market title");
    Format(rebuy, sizeof(rebuy), "%t\n ", "Market rebuy");
    
    Market_Send(client, title, rebuy);
}

public bool Market_OnWeaponSelected(int client, char[] weaponid)
{
    if (!weaponid[0] || !IsPlayerAlive(client))
    {
        return false;
    }
    
    if (IsPlayerZombie(client))
    {
        ZRiot_PrintToChat(client, "Zombie cant use weapon");
        
        return false;
    }
    
    if (StrEqual(weaponid, "rebuy"))
    {
        return true;
    }
    
    char display[64];
    char weapon[32];
    int price;
    
    if (!Market_GetWeaponIDInfo(weaponid, display, weapon, price))
    {
        return false;
    }
    
    ReplaceString(weapon, sizeof(weapon), "weapon_", "");
    
    if (IsWeaponRestricted(weapon))
    {
        ZRiot_PrintToChat(client, "Weapon is restricted", weapon);
        
        return false;
    }
        
    return true;
}

public int Market_PostOnWeaponSelected(int client, bool &allowed)
{
    if (!allowed)
    {
        return;
    }
    
    Market(client);
}