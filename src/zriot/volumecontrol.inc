/**
 * ====================
 *     Zombie Riot
 *   File: volumecontrol.inc
 *   Author: Oylsister
 * ====================
 */

enum SoundType
{
    Type_Zombie = 0,
    Type_Ambience = 1
}

void VolumeControlInit()
{
    RegConsoleCmd("sm_zvolume", Command_Volume);
    RegConsoleCmd("sm_zsounds", Command_Volume);
    RegConsoleCmd("sm_zsound", Command_Volume);

    RegConsoleCmd("sm_ambient_volume", Ambient_Volume);
    RegConsoleCmd("sm_zombie_volume", Zombie_Volume);

    if(g_hZombieVolume == INVALID_HANDLE)
    {
        g_hZombieVolume = RegClientCookie("zriot_zombievolume", "[ZRiot] Zombie Volume Cookies", CookieAccess_Protected);
    }
    if(g_hAmbienceVolume == INVALID_HANDLE)
    {
        g_hAmbienceVolume = RegClientCookie("zriot_ambiencevolume", "[ZRiot] Ambience Volume Cookies", CookieAccess_Protected);
    }
}

void VolumeOnCookiesCached(int client)
{
    char sBuffer1[32];
    char sBuffer2[32];

    GetClientCookie(client, g_hZombieVolume, sBuffer1, sizeof(sBuffer1));
    GetClientCookie(client, g_hAmbienceVolume, sBuffer2, sizeof(sBuffer2));

    if(sBuffer1[0] != '\0')
    {
        g_fZombieVolume[client] = StringToFloat(sBuffer1);
    }

    if(sBuffer2[0] != '\0')
    {
        g_fAmbienceVolume[client] = StringToFloat(sBuffer2);
    }

    else
    {
        g_fZombieVolume[client] = 100.0;
        g_fAmbienceVolume[client] = 100.0;

        SaveVolumeCookie(client, Type_Zombie, g_fZombieVolume[client]);
        SaveVolumeCookie(client, Type_Ambience, g_fAmbienceVolume[client]);
    }
}

void SaveVolumeCookie(int client, SoundType type, float volume)
{
    char sCookie[32];
    FormatEx(sCookie, sizeof(sCookie), "%f", volume);
    if(type == Type_Zombie)
    {
        g_fZombieVolume[client] = volume;
        SetClientCookie(client, g_hZombieVolume, sCookie);
    }
    else
    {
        g_fAmbienceVolume[client] = volume;
        SetClientCookie(client, g_hAmbienceVolume, sCookie);
    }
}

public Action Command_Volume(int client, int args)
{
    VolumeTypeMenu(client);
    return Plugin_Handled;
}

public void VolumeTypeMenu(int client)
{
    Menu menu = new Menu(VolumeTypeMenuHandler, MENU_ACTIONS_ALL);
    menu.SetTitle("%t", "Volume Menu");
    
    char zombie[64];
    char ambient[64];

    Format(zombie, sizeof(zombie), "%t", "Choice Zombie Volume");
    Format(ambient, sizeof(ambient), "%t", "Choice Ambient Volume");

    menu.AddItem("zombie", zombie);
    menu.AddItem("ambient", ambient);

    menu.ExitButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int VolumeTypeMenuHandler(Menu menu, MenuAction action, int param1, int param2)
{
    switch(action)
    {
        case MenuAction_Select:
        {
            char info[64];
            menu.GetItem(param2, info, sizeof(info));

            if(StrEqual(info, "zombie", false))
            {
                ZombieVolumeMenu(param1);
            }
            else
            {
                AmbientVolumeMenu(param1);
            }
        }
        case MenuAction_End:
        {
            delete menu;
        }
    }
    return 0;
}

public Action Ambient_Volume(int client, int args)
{
    if(args == 0)
    {
        AmbientVolumeMenu(client);
        return Plugin_Handled;
    }

    if(args > 1)
    {
        AmbientVolumeMenu(client);
        return Plugin_Handled;
    }

    char sArgs[32];
    GetCmdArg(1, sArgs, sizeof(sArgs));

    int volume = StringToInt(sArgs);

    if(volume < 0)
    {
        SaveVolumeCookie(client, Type_Ambience, 0.0);
        ZRiot_ReplyToCommand(client, "Set Ambient Volume", 0);
        return Plugin_Handled;
    }
    else if(volume > 100)
    {
        SaveVolumeCookie(client, Type_Ambience, 100.0);
        ZRiot_ReplyToCommand(client, "Set Ambient Volume", 100);
        return Plugin_Handled;
    }
    else
    {
        SaveVolumeCookie(client, Type_Ambience, float(volume));
        ZRiot_ReplyToCommand(client, "Set Ambient Volume", volume);
        return Plugin_Handled;
    }
}

public void AmbientVolumeMenu(int client)
{
    Menu menu = new Menu(AmbientVolumeMenuHandler, MENU_ACTIONS_ALL);

    char sTitle[64];
    Format(sTitle, sizeof(sTitle), "%t", "Ambient Volume Menu", RoundToNearest(g_fAmbienceVolume[client]));
    menu.SetTitle("%s", sTitle);

    menu.AddItem("100", "100%");
    menu.AddItem("80", "80%");
    menu.AddItem("60", "60%");
    menu.AddItem("40", "40%");
    menu.AddItem("20", "20%");
    menu.AddItem("0", "0%");

    menu.ExitBackButton = true;
    menu.ExitButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int AmbientVolumeMenuHandler(Menu menu, MenuAction action, int param1, int param2)
{
    switch(action)
    {
        case MenuAction_Select:
        {
            char info[64];
            menu.GetItem(param2, info, sizeof(info));

            if(StrEqual(info, "100", false))
            {
                SaveVolumeCookie(param1, Type_Ambience, 100.0);
                ZRiot_PrintToChat(param1, "Set Ambient Volume", 100);
            }
            else if(StrEqual(info, "80", false))
            {
                SaveVolumeCookie(param1, Type_Ambience, 80.0);
                ZRiot_PrintToChat(param1, "Set Ambient Volume", 80);
            }
            else if(StrEqual(info, "60", false))
            {
                SaveVolumeCookie(param1, Type_Ambience, 60.0);
                ZRiot_PrintToChat(param1, "Set Ambient Volume", 60);
            }
            else if(StrEqual(info, "40", false))
            {
                SaveVolumeCookie(param1, Type_Ambience, 40.0);
                ZRiot_PrintToChat(param1, "Set Ambient Volume", 40);
            }
            else if(StrEqual(info, "20", false))
            {
                SaveVolumeCookie(param1, Type_Ambience, 20.0);
                ZRiot_PrintToChat(param1, "Set Ambient Volume", 20);
            }
            else
            {
                SaveVolumeCookie(param1, Type_Ambience, 0.0);
                ZRiot_PrintToChat(param1, "Set Ambient Volume", 0);
            }
            AmbientVolumeMenu(param1);
        }
        case MenuAction_Cancel:
        {
            VolumeTypeMenu(param1);
        }
        case MenuAction_End:
        {
            delete menu;
        }
    }
    return 0;
}

public Action Zombie_Volume(int client, int args)
{
    if(args == 0)
    {
        ZombieVolumeMenu(client);
        return Plugin_Handled;
    }

    if(args > 1)
    {
        ZombieVolumeMenu(client);
        return Plugin_Handled;
    }

    char sArgs[32];
    GetCmdArg(1, sArgs, sizeof(sArgs));

    int volume = StringToInt(sArgs);

    if(volume < 0)
    {
        SaveVolumeCookie(client, Type_Zombie, 0.0);
        ZRiot_ReplyToCommand(client, "Set Zombie Volume", 0);
        return Plugin_Handled;
    }
    else if(volume > 100)
    {
        SaveVolumeCookie(client, Type_Zombie, 100.0);
        ZRiot_ReplyToCommand(client, "Set Zombie Volume", 100);
        return Plugin_Handled;
    }
    else
    {
        SaveVolumeCookie(client, Type_Zombie, float(volume));
        ZRiot_ReplyToCommand(client, "Set Zombie Volume", volume);
        return Plugin_Handled;
    }
}

public void ZombieVolumeMenu(int client)
{
    Menu menu = new Menu(ZombieVolumeMenuHandler, MENU_ACTIONS_ALL);

    char sTitle[64];
    Format(sTitle, sizeof(sTitle), "%t", "Zombie Volume Menu", RoundToNearest(g_fZombieVolume[client]));
    menu.SetTitle("%s", sTitle);

    menu.AddItem("100", "100%");
    menu.AddItem("80", "80%");
    menu.AddItem("60", "60%");
    menu.AddItem("40", "40%");
    menu.AddItem("20", "20%");
    menu.AddItem("0", "0%");

    menu.ExitBackButton = true;
    menu.ExitButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int ZombieVolumeMenuHandler(Menu menu, MenuAction action, int param1, int param2)
{
    switch(action)
    {
        case MenuAction_Select:
        {
            char info[64];
            menu.GetItem(param2, info, sizeof(info));

            if(StrEqual(info, "100", false))
            {
                SaveVolumeCookie(param1, Type_Zombie, 100.0);
                ZRiot_PrintToChat(param1, "Set Zombie Volume", 100);
            }
            else if(StrEqual(info, "80", false))
            {
                SaveVolumeCookie(param1, Type_Zombie, 80.0);
                ZRiot_PrintToChat(param1, "Set Zombie Volume", 80);
            }
            else if(StrEqual(info, "60", false))
            {
                SaveVolumeCookie(param1, Type_Zombie, 60.0);
                ZRiot_PrintToChat(param1, "Set Zombie Volume", 60);
            }
            else if(StrEqual(info, "40", false))
            {
                SaveVolumeCookie(param1, Type_Zombie, 40.0);
                ZRiot_PrintToChat(param1, "Set Zombie Volume", 40);
            }
            else if(StrEqual(info, "20", false))
            {
                SaveVolumeCookie(param1, Type_Zombie, 20.0);
                ZRiot_PrintToChat(param1, "Set Zombie Volume", 20);
            }
            else
            {
                SaveVolumeCookie(param1, Type_Zombie, 0.0);
                ZRiot_PrintToChat(param1, "Set Zombie Volume", 0);
            }
            ZombieVolumeMenu(param1);
        }
        case MenuAction_Cancel:
        {
            VolumeTypeMenu(param1);
        }
        case MenuAction_End:
        {
            delete menu;
        }
    }
    return 0;
}
