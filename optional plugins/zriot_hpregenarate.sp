#include <sourcemod>
#include <zriot>

ConVar g_Cvar_Delay, g_Cvar_Amount, g_Cvar_Frequent;

float g_fDelay;
int g_iAmount;
float g_fFrequent;

Handle g_hClientTimer[MAXPLAYERS] = INVALID_HANDLE;

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
	g_Cvar_Delay = CreateConVar("zriot_regen_delay", "8.0", "Delaying before HP get regenerate", _, true, "0.1", false);
	g_Cvar_Amount = CreateConVar("zriot_regen_amount", "2.0", "Amount HP that will regenerate after delay", _, true, 1.0, false);
	g_Cvar_Frequent = CreateConVar("zriot_regen_frequent", "1.0", "Regenerate frequency", _, true, 0.1, false);
	
	HookEvent("player_hurt", OnPlayerHurt);
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
			g_hClientTimer[client] = INVALID_HANDLE;
		}
		
		g_fDelay = GetConVarFloat(g_Cvar_Delay);
		CreateTimer(g_fDelay, HP_StartRegen, client);
	}
}

public Action HP_StartRegen(Handle timer, any client);
{
	g_iAmount = GetConVarInt(g_Cvar_Amount);
	SetEntityHealth(client, g_iAmount);
	
	if(g_hClientTimer[client] == INVALID_HANDLE);
	{
		g_fFrequent = GetConVarFloat(g_Cvar_Frequent);
		CreateTimer(g_fFrequent, HP_KeepRegen, client, TIMER_REPEAT);
		return Plugin_Handled;
	}
	return Plugin_Handled;
}


