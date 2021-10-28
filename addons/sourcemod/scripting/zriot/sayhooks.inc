/**
 * ====================
 *     Zombie Riot
 *   File: sayhooks.inc
 *   Author: Greyscale
 * ====================
 */

HookChatCmds()
{
    RegConsoleCmd("say", SayCommand);
    RegConsoleCmd("say_team", SayCommand);
}

public Action:SayCommand(client, argc)
{
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (!client || !enabled)
    {
        return Plugin_Continue;
    }
    
    decl String:args[192];
    
    GetCmdArgString(args, sizeof(args));
    ReplaceString(args, sizeof(args), "\"", "");
    
    if (StrEqual(args, "!market", false))
    {
        Market(client);
        return Plugin_Handled;
    }
    
    return Plugin_Continue;
}

Market(client)
{
    if (!market)
    {
        ZRiot_PrintToChat(client, "Feature is disabled");
        
        return;
    }
    
    new bool:buyzone = GetConVarBool(gCvars[CVAR_ZMARKET_BUYZONE]); 
    if (!IsClientInBuyZone(client) && buyzone)
    {
        ZRiot_PrintCenterText(client, "Market out of buyzone");
        
        return;
    }
    
    SetGlobalTransTarget(client);
    
    decl String:title[64];
    decl String:rebuy[64];
    
    Format(title, sizeof(title), "%t\n ", "Market title");
    Format(rebuy, sizeof(rebuy), "%t\n ", "Market rebuy");
    
    Market_Send(client, title, rebuy);
}

public bool:Market_OnWeaponSelected(client, String:weaponid[])
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
    
    decl String:display[64];
    decl String:weapon[32];
    new price;
    
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

public Market_PostOnWeaponSelected(client, &bool:allowed)
{
    if (!allowed)
    {
        return;
    }
    
    Market(client);
}