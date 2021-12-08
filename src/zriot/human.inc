/**
 * ====================
 *     Zombie Riot
 *   File: human.inc
 *   Author: Oylsister
 * ====================
 */

Handle g_hHumanClassCookie = INVALID_HANDLE;

int g_iSelectedClass[MAXPLAYERS+1];

void HumanClassInit()
{
    //RegConsoleCmd("sm_zrclass", Command_SelectHumanClass);

    if(g_hHumanClassCookie == INVALID_HANDLE)
    {
        g_hHumanClassCookie = RegClientCookie("zriot_humanclass", "[ZRiot] Human Class Cookies", CookieAccess_Protected);
    }
}

/*
public Action Command_SelectHumanClass(int client, int args)
{
    SelectClassMenu(client);
    return Plugin_Handled;
}

public void SelectClassMenu(int client)
{
    Menu menu = new Menu(SelectClassMenuHandler, MENU_ACTIONS_ALL);
    menu.SetTitle("%t", "Human Menu");
    for(int x = 0; x < hCount; x++)
    {
        
    }
}
*/

void HumanClassOnCookiesCahced(int client)
{
    char sBuffer[32];
    GetClientCookie(client, g_hHumanClassCookie, sBuffer, sizeof(sBuffer));

    if(sBuffer[0] != '\0')
    {
        g_iSelectedClass[client] = FindHumanIDByName(sBuffer);
    }
    else
    {
        char sDefault[64];
        GetHumanDefaultClass(sDefault, sizeof(sDefault));

        g_iSelectedClass[client] = FindHumanIDByName(sDefault);
        SaveHumanClassCookie(client, g_iSelectedClass[client]);
    }
}

void SaveHumanClassCookie(int client, int humanid)
{
    char sCookie[64];
    FormatEx(sCookie, sizeof(sCookie), "%d", humanid);
    SetClientCookie(client, g_hHumanClassCookie, sCookie);
}

public Action HumanSpawnPost(Handle timer, any client)
{
    MakeHuman(client, g_iSelectedClass[client]);
    return Plugin_Handled;
}

void MakeHuman(int client, int humanid)
{
    ApplyHumanModel(client, humanid);
    ApplyHumanArms(client, humanid);
    ApplyHumanHealth(client, humanid);
    ApplyHumanSpeed(client, humanid);
    ApplyHumanGravity(client, humanid);
    ApplyHumanFOV(client, humanid);
}