#include <sourcemod>
#include <zriot>

ConVar g_Cvar_Delay, g_Cvar_Amount, g_Cvar_Frequent;

float g_fDelay;
int g_iAmount;
float g_fFrequent;

Handle g_hClientTimer[MAXPLAYERS+1] = INVALID_HANDLE;

public Plugin myinfo =
{
	name = " [ZRiot] HP Regenerate for Human", 
	author = "Oylsister", 
	description = "Regen HP Back after get damaged", 
	version = "1.0", 
	url = ""
};

public void OnPluginStart()
{
	g_Cvar_Delay = CreateConVar("zriot_regen_delay", "8.0", "Delaying before HP get regenerate", _, true, 0.1, false);
	g_Cvar_Amount = CreateConVar("zriot_regen_amount", "2.0", "Amount HP that will regenerate after delay", _, true, 1.0, false);
	g_Cvar_Frequent = CreateConVar("zriot_regen_frequent", "1.0", "Regenerate frequency", _, true, 0.1, false);
	
	HookEvent("player_hurt", OnPlayerHurt);
	HookEvent("player_spawn", OnPlayerSpawn);
}

public void OnPlayerSpawn(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	
	if(g_hClientTimer[client] != INVALID_HANDLE)
	{
		KillTimer(g_hClientTimer[client]);
	}
	g_hClientTimer[client] = INVALID_HANDLE;
}

public void OnPlayerHurt(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
	
	if(ZRiot_IsClientZombie(client))
		return;
		
	else 
	{
		if(g_hClientTimer[client] != INVALID_HANDLE)
		{
			KillTimer(g_hClientTimer[client]);
		}
		g_hClientTimer[client] = INVALID_HANDLE;
		
		g_fDelay = GetConVarFloat(g_Cvar_Delay);
		CreateTimer(g_fDelay, HP_StartRegen, client);
	}
}

public Action HP_StartRegen(Handle timer, any client)
{
	if(!IsClientInGame(client))
		return Plugin_Handled;
		
	if(!IsPlayerAlive(client))
		return Plugin_Handled;
	
	g_iAmount = GetConVarInt(g_Cvar_Amount);
	
	int iHP = GetEntProp(client, Prop_Send, "m_iHealth");
	
	if(100 - iHP >= g_iAmount)
		SetEntityHealth(client, iHP + g_iAmount);
		
	else
	{
		SetEntityHealth(client, 100);
		return Plugin_Handled;
	}
	
	if(g_hClientTimer[client] != INVALID_HANDLE)
	{
		KillTimer(g_hClientTimer[client]);
	}
	
	g_fFrequent = GetConVarFloat(g_Cvar_Frequent);
	g_hClientTimer[client] = CreateTimer(g_fFrequent, HP_KeepRegen, client, TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
	return Plugin_Handled;
}

public Action HP_KeepRegen(Handle timer, any client)
{
	if(!IsClientInGame(client))
		return Plugin_Stop;
		
	if(!IsPlayerAlive(client))
		return Plugin_Handled;
		
	int iHP = GetEntProp(client, Prop_Send, "m_iHealth");
	
	if(100 - iHP >= g_iAmount)
		SetEntityHealth(client, iHP + g_iAmount);
		
	else
	{
		SetEntityHealth(client, 100);
		KillTimer(g_hClientTimer[client]);
		g_hClientTimer[client] = INVALID_HANDLE;
		
		return Plugin_Handled;
	}
	
	return Plugin_Continue;
}


