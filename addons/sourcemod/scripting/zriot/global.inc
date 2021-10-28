/**
 * ====================
 *     Zombie Riot
 *   File: global.inc
 *   Author: Greyscale
 * ====================
 */

new Handle:hOnClientZombie = INVALID_HANDLE;
new Handle:hOnClientHuman = INVALID_HANDLE;
new Handle:hOnClientHUDUpdate = INVALID_HANDLE;

CreateGlobals()
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

public Native_IsClientZombie(Handle:plugin, argc)
{
    new client = GetNativeCell(1);
    if (!client)
        ThrowNativeError(SP_ERROR_INDEX, "Client index %d is not in the game", client);
    
    return bZombie[GetNativeCell(1)];
}

public Native_GetZombieTeam(Handle:plugin, argc)
{
    return gZombieTeam;
}

public Native_GetHumanTeam(Handle:plugin, argc)
{
    return gHumanTeam;
}

public Native_Zombie(Handle:plugin, argc)
{
    new client = GetNativeCell(1);
    if (!client)
        ThrowNativeError(SP_ERROR_INDEX, "Client index %d is not in the game", client);
    
    ZRiot_Zombie(client);
}

public Native_Human(Handle:plugin, argc)
{
    new client = GetNativeCell(1);
    if (!client)
        ThrowNativeError(SP_ERROR_INDEX, "Client index %d is not in the game", client);
    
    if (IsFakeClient(client))
        ThrowNativeError(SP_ERROR_INDEX, "Bots cannot be moved to the human team");
    
    ZRiot_Human(client);
}