/**
 * ====================
 *     Zombie Riot
 *   File: zombieriot.inc
 *   Author: Greyscale
 * ==================== 
 */

#define Target_Bombed							0		// Target Successfully Bombed!
#define VIP_Escaped								1		// The VIP has escaped!
#define VIP_Assassinated						2		// VIP has been assassinated!
#define Terrorists_Escaped						3		// The terrorists have escaped!
#define CTs_PreventEscape						4		// The CT's have prevented most of the terrorists from escaping!
#define Escaping_Terrorists_Neutralized			5		// Escaping terrorists have all been neutralized!
#define Bomb_Defused							6		// The bomb has been defused!
#define CTs_Win									7		// Counter-Terrorists Win!
#define Terrorists_Win							8		// Terrorists Win!
#define Round_Draw								9		// Round Draw!
#define All_Hostages_Rescued					10		// All Hostages have been rescued!
#define Target_Saved							11		// Target has been saved!
#define Hostages_Not_Rescued					12		// Hostages have not been rescued!
#define Terrorists_Not_Escaped					13		// Terrorists have not escaped!
#define VIP_Not_Escaped							14		// VIP has not escaped!
#define Game_Commencing							15		// Game Commencing!

#define DXLEVEL_MIN 90

#define DEFAULT_FOV 90
#define DEFAULT_GRAVITY 1.0

bool market;

char gMapConfig[PLATFORM_MAX_PATH];

int gDay;

int gZombieTeam;
int gHumanTeam;

int gZombiesKilled;

int dxLevel[MAXPLAYERS+1];

bool bZombie[MAXPLAYERS+1];
Handle trieDeaths = INVALID_HANDLE;

int gRespawnTime[MAXPLAYERS+1];
Handle tRespawn[MAXPLAYERS+1];
Handle tZVision[MAXPLAYERS+1];

bool bZVision[MAXPLAYERS+1];

Handle tHUD = INVALID_HANDLE;
Handle tFreeze = INVALID_HANDLE;

QueryCookie mat_dxlevel;

int g_iDayleft;
int g_iDayMapvote;

bool g_bTriggerMapVote;
bool g_bAlreadyVoted;

int g_iCountdown[MAXPLAYERS+1];
bool g_bClientProtected[MAXPLAYERS+1] = false;
Handle g_fClientProtectTimer[MAXPLAYERS+1] = INVALID_HANDLE;

void FindClientDXLevel(int client)
{
    if (IsFakeClient(client))
    {
        return;
    }
    
    mat_dxlevel = QueryClientConVar(client, "mat_dxlevel", DXLevelClientQuery);
}

public void DXLevelClientQuery(QueryCookie cookie, int client, ConVarQueryResult result, const char[] cvarName, const char[] cvarValue)
{
    if (cookie != mat_dxlevel)
    {
        return;
    }
    
    dxLevel[client] = 0;
    
    if (result != ConVarQuery_Okay)
    {
        return;
    }
    
    dxLevel[client] = StringToInt(cvarValue);
}

void DisplayClientOverlay(int client, const char[] overlay)
{
    if (!dxLevel[client])
    {
        FindClientDXLevel(client);
        
        return;
    }
    
    if (dxLevel[client] >= DXLEVEL_MIN)
    {
        ClientCommand(client, "r_screenoverlay \"%s\"", overlay);
    }
    else
    {
        ZRiot_PrintCenterText(client, "DX90 not supported", dxLevel[client], DXLEVEL_MIN);
    }
}

stock void GotoNextMap()
{
    Handle timelimit = FindConVar("mp_timelimit");
    
    if (timelimit == INVALID_HANDLE)
    {
        return;
    }
    
    int flags = GetConVarFlags(timelimit) & FCVAR_NOTIFY;
    SetConVarFlags(timelimit, flags);
    
    SetConVarInt(timelimit, 1);
}