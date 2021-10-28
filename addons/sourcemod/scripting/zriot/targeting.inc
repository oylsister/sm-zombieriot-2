/**
 * ====================
 *     Zombie Riot
 *   File: targeting.inc
 *   Author: Greyscale
 * ====================
 */

new gTarget[MAXPLAYERS+1];

new bool:bTargeted[MAXPLAYERS+1][MAXPLAYERS+1];

TargetPlayer(attacker, client)
{
    if (!IsClientInGame(attacker) || !IsClientInGame(client))
    {
        return;
    }
    
    gTarget[attacker] = client;
    
    bTargeted[client][attacker] = true;
}

GetClientTarget(client)
{
    if (gTarget[client] == -1 || !IsClientInGame(gTarget[client]))
    {
        return -1;
    }
    
    return gTarget[client];
}

GetClientTargeters(client, clients[], maxClients)
{
    new count = 0;
    for (new x = 1; x <= maxClients; x++)
    {
        if (!IsClientInGame(x) || !bTargeted[client][x])
        {
            continue;
        }
        
        clients[count++] = x;
    }
    
    return count;
}

FindClientNextTarget(client)
{
    new maxplayers = GetMaxClients();
    for (new x = 1; x <= maxplayers; x++)
    {
        if (!IsClientInGame(x))
        {
            continue;
        }
        
        if (bTargeted[x][client])
        {
            return x;
        }
    }
    
    return -1;
}

RemoveTargeters(client)
{
    new maxplayers = GetMaxClients();
    for (new x = 1; x <= maxplayers; x++)
    {
        bTargeted[client][x] = false;
        
        if (gTarget[x] == client)
        {
            gTarget[x] = FindClientNextTarget(x);
        }
    }
}