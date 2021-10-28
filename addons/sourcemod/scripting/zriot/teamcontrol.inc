/**
 * ====================
 *     Zombie Riot
 *   File: teamcontrol.inc
 *   Author: Greyscale
 * ====================
 */

#define JOINTEAM_AUTOASSIGN 0
#define JOINTEAM_SPECTATOR 1
#define JOINTEAM_T 2
#define JOINTEAM_CT 3

InitTeamControl()
{
    RegConsoleCmd("jointeam", Command_JoinTeam);
    RegConsoleCmd("kill", Command_Kill);
    RegConsoleCmd("spectate", Command_Spectate);
}

public Action:Command_JoinTeam(client, argc)
{
    if (!client)
    {
        return Plugin_Continue;
    }
    
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (!enabled)
    {
        return Plugin_Continue;
    }
    
    decl String:args[8];
    GetCmdArgString(args, sizeof(args));
    
    new jointeam = StringToInt(args);
    
    new team = GetClientTeam(client);
    if (team == CS_TEAM_T || team == CS_TEAM_CT)
    {
        if (jointeam != JOINTEAM_SPECTATOR)
        {
            return Plugin_Handled;
        }
        else if (IsPlayerAlive(client))
        {
            ChangeClientDeathCount(client, -1);
        }
    }
    
    return Plugin_Continue;
}

public Action:Command_Kill(client, argc)
{
    if (!client)
    {
        return Plugin_Continue;
    }
    
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (!enabled)
    {
        return Plugin_Continue;
    }
    
    ZRiot_ReplyToCommand(client, "Suicide text");
    
    return Plugin_Handled;
}

public Action:Command_Spectate(client, argc)
{
    if (!client || !IsPlayerAlive(client))
    {
        return Plugin_Continue;
    }
    
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (!enabled)
    {
        return Plugin_Continue;
    }
    
    ChangeClientDeathCount(client, -1);
    
    return Plugin_Continue;
}

UpdateTeams()
{
    decl String:zombieteam[8];
    GetConVarString(gCvars[CVAR_ZOMBIETEAM], zombieteam, sizeof(zombieteam));
    
    if (StrEqual(zombieteam, "t", false))
    {
        gZombieTeam = CS_TEAM_T;
        gHumanTeam = CS_TEAM_CT;
    }
    else if (StrEqual(zombieteam, "ct", false))
    {
        gZombieTeam = CS_TEAM_CT;
        gHumanTeam = CS_TEAM_T;
    }
    else
    {
        SetFailState("Invalid value for cvar zriot_zombieteam, see config file");
        return;
    }
    
    AssignTeamAll(true);
}

ResetZombies(bool:switchteam)
{
    new maxplayers = GetMaxClients();
    for (new x = 1; x <= maxplayers; x++)
    {
        if (!IsClientInGame(x))
        {
            continue;
        }
        
        bZombie[x] = IsFakeClient(x);
    }
    
    if (switchteam)
    {
        AssignTeamAll(false);
    }
}

public Action:AssignTeamTimer(Handle:timer)
{
    AssignTeamAll(false);
}

AssignTeam(clients[], numClients, bool:spawn)
{
    for (new x = 0; x < numClients; x++)
    {
        if (!IsClientInGame(clients[x]))
        {
            continue;
        }
        
        new team = GetClientTeam(clients[x]);
        
        if (IsPlayerZombie(clients[x]))
        {
            CS_SwitchTeam(clients[x], gZombieTeam);
        
            if (spawn && team != gZombieTeam)
            {
                CS_RespawnPlayer(clients[x]);
            }
        }
        else
        {
            CS_SwitchTeam(clients[x], gHumanTeam);
        
            if (spawn && team != gHumanTeam)
            {
                CS_RespawnPlayer(clients[x]);
            }
        }
    }
}

stock AssignTeamClient(client, bool:spawn)
{
    if (!IsClientInGame(client))
    {
        return;
    }
    
    new clients[1];
    clients[0] = client;
    
    AssignTeam(clients, 1, spawn);
}

stock AssignTeamAll(bool:spawn)
{
    new maxplayers = GetMaxClients();
    
    new clients[maxplayers];
    new count = 0;
    
    for (new x = 1; x <= maxplayers; x++)
    {
        if (!IsClientInGame(x) || GetClientTeam(x) < CS_TEAM_T)
        {
            continue;
        }
        
        clients[count++] = x;
    }
    
    AssignTeam(clients, count, spawn);
}