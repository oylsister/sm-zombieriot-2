/**
 * ====================
 *     Zombie Riot
 *   File: weaponrestrict.inc
 *   Author: Greyscale
 * ====================
 */

Handle restrictedWeapons = INVALID_HANDLE;

enum WepRestrictQuery
{
    Successful,  /** Weapon (un)restrict query was successful */
    Invalid,  /** Weapon invalid */
    Existing,  /** Already restricted */
}
    
void InitWeaponRestrict()
{
    RegConsoleCmd("buy", BuyHook);
    
    restrictedWeapons = CreateArray(32, 0);
}

void ClientHookUse(int client)
{
	SDKHook(client, SDKHook_WeaponCanUse, Weapon_CanUse);
}

public Action BuyHook(int client, int argc)
{
    bool enabled = GetConVarBool(gCvars.CVAR_ENABLE);
    if (!enabled)
    {
        return Plugin_Continue;
    }
    
    if (IsPlayerZombie(client))
    {
        ZRiot_PrintToChat(client, "Zombie cant use weapon");
        
        return Plugin_Handled;
    }
    
    char weapon[64];
    GetCmdArg(1, weapon, sizeof(weapon));
    
    ReplaceString(weapon, sizeof(weapon), "weapon_", "");
    
    if (IsWeaponRestricted(weapon))
    {
        ZRiot_PrintToChat(client, "Weapon is restricted", weapon);
        return Plugin_Handled;
    }
    
    return Plugin_Continue;
}

WepRestrictQuery RestrictWeapon(const char[] weapon)
{
    if (IsWeaponGroup(weapon))
    {
        RestrictWeaponGroup(weapon);
        
        ZRiot_PrintToChat(0, "Weapon group has been restricted", weapon);
        
        return Successful;
    }
    
    if (!IsWeaponRestricted(weapon))
    {
        PushArrayString(restrictedWeapons, weapon);
        
        ZRiot_PrintToChat(0, "Weapon has been restricted", weapon);
        
        return Successful;
    }
    
    return Existing;
}

void RestrictWeaponGroup(const char[] group)
{
    if (StrEqual(group, "pistols", false))
    {
        PushArrayString(restrictedWeapons, "glock");
        PushArrayString(restrictedWeapons, "usp");
        PushArrayString(restrictedWeapons, "p228");
        PushArrayString(restrictedWeapons, "deagle");
        PushArrayString(restrictedWeapons, "elite");
        PushArrayString(restrictedWeapons, "fiveseven");
    }
    else if (StrEqual(group, "shotguns", false))
    {
        PushArrayString(restrictedWeapons, "m3");
        PushArrayString(restrictedWeapons, "xm1014");
    }
    else if (StrEqual(group, "smgs", false))
    {
        PushArrayString(restrictedWeapons, "tmp");
        PushArrayString(restrictedWeapons, "mac10");
        PushArrayString(restrictedWeapons, "mp5navy");
        PushArrayString(restrictedWeapons, "ump45");
        PushArrayString(restrictedWeapons, "p90");
    }
    else if (StrEqual(group, "rifles", false))
    {
        PushArrayString(restrictedWeapons, "galil");
        PushArrayString(restrictedWeapons, "famas");
        PushArrayString(restrictedWeapons, "ak47");
        PushArrayString(restrictedWeapons, "m4a1");
        PushArrayString(restrictedWeapons, "sg552");
        PushArrayString(restrictedWeapons, "bullpup");
    }
    else if (StrEqual(group, "snipers", false))
    {
        PushArrayString(restrictedWeapons, "scout");
        PushArrayString(restrictedWeapons, "sg550");
        PushArrayString(restrictedWeapons, "g3sg1");
        PushArrayString(restrictedWeapons, "awp");
    }
}
    
WepRestrictQuery UnRestrictWeapon(const char[] weapon)
{
    if (IsWeaponGroup(weapon))
    {
        UnRestrictWeaponGroup(weapon);
        
        ZRiot_PrintToChat(0, "Weapon group has been unrestricted", weapon);
        
        return Successful;
    }
    
    int index = GetRestrictedWeaponIndex(weapon);
    
    if (index > -1)
    {
        RemoveFromArray(restrictedWeapons, index);
        
        ZRiot_PrintToChat(0, "Weapon has been unrestricted", weapon);
        
        return Successful;
    }

    return Invalid;
}

void UnRestrictWeaponGroup(const char[] group)
{
    if (StrEqual(group, "pistols", false))
    {
        UnRestrictWeapon("glock");
        UnRestrictWeapon("usp");
        UnRestrictWeapon("p228");
        UnRestrictWeapon("deagle");
        UnRestrictWeapon("elite");
        UnRestrictWeapon("fiveseven");
    }
    else if (StrEqual(group, "shotguns", false))
    {
        UnRestrictWeapon("m3");
        UnRestrictWeapon("xm1014");
    }
    else if (StrEqual(group, "smgs", false))
    {
        UnRestrictWeapon("tmp");
        UnRestrictWeapon("mac10");
        UnRestrictWeapon("mp5navy");
        UnRestrictWeapon("ump45");
        UnRestrictWeapon("p90");
    }
    else if (StrEqual(group, "rifles", false))
    {
        UnRestrictWeapon("galil");
        UnRestrictWeapon("famas");
        UnRestrictWeapon("ak47");
        UnRestrictWeapon("m4a1");
        UnRestrictWeapon("sg552");
        UnRestrictWeapon("bullpup");
    }
    else if (StrEqual(group, "snipers", false))
    {
        UnRestrictWeapon("scout");
        UnRestrictWeapon("sg550");
        UnRestrictWeapon("g3sg1");
        UnRestrictWeapon("awp");
    }
}

bool IsWeaponRestricted(const char[] weapon)
{
    for (int x = 0; x < GetArraySize(restrictedWeapons); x++)
    {
        char restrictedweapon[32];
        GetArrayString(restrictedWeapons, x, restrictedweapon, sizeof(restrictedweapon));
        
        if (StrEqual(weapon, restrictedweapon, false))
        {
            return true;
        }
    }
    
    return false;
}

int GetRestrictedWeaponIndex(const char[] weapon)
{
    for (int x = 0; x < GetArraySize(restrictedWeapons); x++)
    {
        char restrictedweapon[32];
        GetArrayString(restrictedWeapons, x, restrictedweapon, sizeof(restrictedweapon));
        ReplaceString(restrictedweapon, sizeof(restrictedweapon), "weapon_", "");
        
        if (StrEqual(weapon, restrictedweapon, false))
        {
            return x;
        }
    }
    
    return -1;
}

bool IsWeaponGroup(const char[] weapon)
{
  return (StrEqual(weapon, "pistols", false) || StrEqual(weapon, "shotguns", false) || StrEqual(weapon, "smgs", false) || StrEqual(weapon, "rifles", false) || StrEqual(weapon, "snipers", false));
}

public Action Weapon_CanUse(int client, int weapon)
{
    bool enabled = GetConVarBool(gCvars.CVAR_ENABLE);
    if (!enabled)
    {
        return Plugin_Continue;
    }
        
    char weaponname[32];
    if (!weapon || !GetEdictClassname(weapon, weaponname, sizeof(weaponname)))
    {
        return Plugin_Continue;
    }
    
    ReplaceString(weaponname, sizeof(weaponname), "weapon_", "");
    
    char model[256];
    GetClientModel(client, model, sizeof(model));
    
    ReplaceString(model, sizeof(model), ".mdl", "");
    
    if (FindStringInArray(adtModels, model) > -1 && !StrEqual(weaponname, "knife"))
    {
        return Plugin_Handled;
    }
    
    if (IsWeaponRestricted(weaponname))
    {
        return Plugin_Handled;
    }
    
    if (IsPlayerZombie(client) && !StrEqual(weaponname, "knife"))
    {
        if (StrEqual(weaponname, "glock") || StrEqual(weaponname, "usp"))
        {
            CreateTimer(0.0, RemoveSpawnWeapon, weapon);
        }
        
        return Plugin_Handled;
    }
    
    return Plugin_Continue;
}

public Action RemoveSpawnWeapon(Handle timer, any weapon)
{
    if (IsValidEdict(weapon))
    {
        RemoveEdict(weapon);
	}
}