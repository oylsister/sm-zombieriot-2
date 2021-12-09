/**
 * ====================
 *     Zombie Riot
 *   File: human.inc
 *   Author: Oylsister
 * ====================
 */

void HumanClassInit()
{
    RegConsoleCmd("zriot_class", Command_SelectHumanClass);

    if(g_hHumanClassCookie == INVALID_HANDLE)
    {
        g_hHumanClassCookie = RegClientCookie("zriot_humanclass", "[ZRiot] Human Class Cookies", CookieAccess_Protected);
    }
}

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
        char choice[256];
        Format(choice, sizeof(choice), "%s\n    %s", arrayHumans[x].human_name, arrayHumans[x].human_description);
        menu.AddItem(arrayHumans[x].human_name, choice);
    }

    menu.ExitBackButton = true;
    menu.ExitButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int SelectClassMenuHandler(Menu menu, MenuAction action, int param1, int param2)
{
    switch(action)
    {
        case MenuAction_DrawItem:
        {
            char info[256];
            menu.GetItem(param2, info, sizeof(info));

            for (int x = 0; x < hCount; x++)
            {
                if(StrEqual(info, arrayHumans[x].human_name, false))
                {
                    int humanid = FindHumanIDByName(arrayHumans[x].human_name);
                    if(g_iSelectedClass[param1] == humanid)
                    {
                        return ITEMDRAW_DISABLED;
                    }

                    char sm_flags[64];
                    sm_flags[0] = 0;
                    GetHumanAdminFlag(humanid, sm_flags, sizeof(sm_flags));

                    if(strlen(sm_flags))
                    {
                        int flags = ReadFlagString(sm_flags);

                        if(GetUserFlagBits(param1) & flags != flags)
                        {
                            return ITEMDRAW_DISABLED;
                        }
                    }
                }
            }
        }
        case MenuAction_Select:
        {
            char info[64];
            menu.GetItem(param2, info, sizeof(info));

            for (int x = 0; x < hCount; x++)
            {
                if(StrEqual(info, arrayHumans[x].human_name, false))
                {
                    int humanid = FindHumanIDByName(arrayHumans[x].human_name);
                    SaveHumanClassCookie(param1, humanid);
                    ZRiot_PrintToChat(param1, "Will apply on next spawn");
                }
            }
        }
        case MenuAction_End:
        {
            delete menu;
        }
    }
    return 0;
}

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
    CheckAdminFlag(client);
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

void CheckAdminFlag(int client)
{
    char sm_flags[64];
    sm_flags[0] = 0;
    GetHumanAdminFlag(g_iSelectedClass[client], sm_flags, sizeof(sm_flags));

    if(strlen(sm_flags))
    {
        int flags = ReadFlagString(sm_flags);

        if(GetUserFlagBits(client) & flags != flags)
        {
            ResetClassToDefault(client);
        }
    }
}

void ResetClassToDefault(int client)
{
    char sDefault[64];
    GetHumanDefaultClass(sDefault, sizeof(sDefault));

    g_iSelectedClass[client] = FindHumanIDByName(sDefault);
    SaveHumanClassCookie(client, g_iSelectedClass[client]);

    ZRiot_PrintToChat(client, "Class Reset To Default");
}