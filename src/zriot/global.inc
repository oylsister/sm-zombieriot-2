/**
 * ====================
 *     Zombie Riot
 *   File: global.inc
 *   Author: Greyscale
 * ====================
 */

Handle hOnClientZombie = INVALID_HANDLE;
Handle hOnClientHuman = INVALID_HANDLE;
Handle hOnClientHUDUpdate = INVALID_HANDLE;

void CreateGlobals()
{
    CreateNative("ZRiot_IsClientZombie", Native_IsClientZombie);
    CreateNative("ZRiot_Zombie", Native_Zombie);
    CreateNative("ZRiot_Human", Native_Human);
    CreateNative("ZRiot_GetZombieTeam", Native_GetZombieTeam);
    CreateNative("ZRiot_GetHumanTeam", Native_GetHumanTeam);
    
    hOnClientZombie = CreateGlobalForward("ZRiot_OnClientZombie", ET_Ignore, Param_Cell);
    hOnClientHuman = CreateGlobalForward("ZRiot_OnClientHuman", ET_Ignore, Param_Cell);
    hOnClientHUDUpdate = CreateGlobalForward("ZRiot_OnClientHUDUpdate", ET_Ignore, Param_Cell, Param_String);
}

public int Native_IsClientZombie(Handle plugin, int argc)
{
    int client = GetNativeCell(1);
    if (!client)
        ThrowNativeError(SP_ERROR_INDEX, "Client index %d is not in the game", client);
    
    return bZombie[GetNativeCell(1)];
}

public int Native_GetZombieTeam(Handle plugin, int argc)
{
    return gZombieTeam;
}

public int Native_GetHumanTeam(Handle plugin, int argc)
{
    return gHumanTeam;
}

public int Native_Zombie(Handle plugin, int argc)
{
    int client = GetNativeCell(1);
    if (!client)
        ThrowNativeError(SP_ERROR_INDEX, "Client index %d is not in the game", client);
    
    ZRiot_Zombie(client);
}

public int Native_Human(Handle plugin, int argc)
{
    int client = GetNativeCell(1);
    if (!client)
        ThrowNativeError(SP_ERROR_INDEX, "Client index %d is not in the game", client);
    
    if (IsFakeClient(client))
        ThrowNativeError(SP_ERROR_INDEX, "Bots cannot be moved to the human team");
    
    ZRiot_Human(client);
}