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

void InitTeamControl()
{
    RegConsoleCmd("jointeam", Command_JoinTeam);
    RegConsoleCmd("kill", Command_Kill);
    RegConsoleCmd("spectate", Command_Spectate);
}

public Action Command_JoinTeam(int client, int argc)
{
    if (!client)
    {
        return Plugin_Continue;
    }
    
    bool enabled = GetConVarBool(gCvars.CVAR_ENABLE);
    if (!enabled)
    {
        return Plugin_Continue;
    }
    
    char args[8];
    GetCmdArgString(args, sizeof(args));
    
    int jointeam = StringToInt(args);
    
    int team = GetClientTeam(client);
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

public Action Command_Kill(int client, int argc)
{
    if (!client)
    {
        return Plugin_Continue;
    }
    
    bool enabled = GetConVarBool(gCvars.CVAR_ENABLE);
    if (!enabled)
    {
        return Plugin_Continue;
    }
    
    ZRiot_ReplyToCommand(client, "Suicide text");
    
    return Plugin_Handled;
}

public Action Command_Spectate(int client, int argc)
{
    if (!client || !IsPlayerAlive(client))
    {
        return Plugin_Continue;
    }
    
    bool enabled = GetConVarBool(gCvars.CVAR_ENABLE);
    if (!enabled)
    {
        return Plugin_Continue;
    }
    
    ChangeClientDeathCount(client, -1);
    
    return Plugin_Continue;
}

void UpdateTeams()
{
    char zombieteam[8];
    GetConVarString(gCvars.CVAR_ZOMBIETEAM, zombieteam, sizeof(zombieteam));
    
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

void ResetZombies(bool switchteam)
{
    for (int x = 1; x <= MaxClients; x++)
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

public Action AssignTeamTimer(Handle timer)
{
    AssignTeamAll(false);
}

void AssignTeam(int[] clients, int numClients, bool spawn)
{
    for (int x = 0; x < numClients; x++)
    {
        if (!IsClientInGame(clients[x]))
        {
            continue;
        }
        
        int team = GetClientTeam(clients[x]);
        
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

stock void AssignTeamClient(int client, bool spawn)
{
    if (!IsClientInGame(client))
    {
        return;
    }
    
    int clients[1];
    clients[0] = client;
    
    AssignTeam(clients, 1, spawn);
}

stock void AssignTeamAll(bool spawn)
{
    int clients[64];
    int count = 0;
    
    for (int x = 1; x <= MaxClients; x++)
    {
        if (!IsClientInGame(x) || GetClientTeam(x) < CS_TEAM_T)
        {
            continue;
        }
        
        clients[count++] = x;
    }
    
    AssignTeam(clients, count, spawn);
}