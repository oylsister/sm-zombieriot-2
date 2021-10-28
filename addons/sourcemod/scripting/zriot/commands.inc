/**
 * ====================
 *     Zombie Riot
 *   File: commands.inc
 *   Author: Greyscale
 * ====================
 */
 
CreateCommands()
{
    RegAdminCmd("zriot_restrict", Command_Restrict, ADMFLAG_GENERIC, "Restrict a specified weapon");
    RegAdminCmd("zriot_unrestrict", Command_UnRestrict, ADMFLAG_GENERIC, "Unrestrict a specified weapon");
    
    RegAdminCmd("zriot_setday", Command_SetDay, ADMFLAG_GENERIC, "Sets the game to a certain day");
    
    RegAdminCmd("zriot_zombie", Command_Zombie, ADMFLAG_GENERIC, "Turns player into zombie");
    RegAdminCmd("zriot_human", Command_Human, ADMFLAG_GENERIC, "Turns player into human");
}
    
public Action:Command_Restrict(client, argc)
{
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (argc < 1 || !enabled)
    {
        return Plugin_Handled;
    }
    
    decl String:arg1[32];
    GetCmdArg(1, arg1, sizeof(arg1));
    
    new WepRestrictQuery:output = RestrictWeapon(arg1);
    
    if (output == Existing)
    {
        ZRiot_ReplyToCommand(client, "Weapon already restricted", arg1);
    }
    
    return Plugin_Handled;
}

public Action:Command_UnRestrict(client, argc)
{
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (argc < 1 || !enabled)
    {
        return Plugin_Handled;
    }
    
    decl String:arg1[32];
    GetCmdArg(1, arg1, sizeof(arg1));
    
    new WepRestrictQuery:output = UnRestrictWeapon(arg1);
    
    if (output == Invalid)
    {
        ZRiot_ReplyToCommand(client, "Weapon invalid", arg1);
    }
    
    return Plugin_Handled;
}

public Action:Command_SetDay(client, argc)
{
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (argc < 1 || !enabled)
    {
        return Plugin_Handled;
    }
    
    decl String:arg1[32];
    GetCmdArg(1, arg1, sizeof(arg1));
    
    new day = StringToInt(arg1) - 1;
    day = (day >= 0) ? day : 0;
    
    gDay = day;
    gDay = (gDay + 1 > dCount) ? dCount - 1 : gDay;
    
    ZRiot_PrintToChat(0, "Skip to day", gDay + 1);
    
    if (tHUD == INVALID_HANDLE)
    {
        return Plugin_Handled;
    }
    
    TerminateRound(3.0, Round_Draw);
    
    return Plugin_Handled;
}

public Action:Command_Zombie(client, argc)
{
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (argc < 1 || !enabled)
    {
        return Plugin_Handled;
    }
    
    decl String:arg1[32];
    GetCmdArg(1, arg1, sizeof(arg1));
    
    decl String:target_name[MAX_TARGET_LENGTH];
    new targets[MAXPLAYERS];
    new bool:tn_is_ml;
    
    new tcount = ProcessTargetString(arg1, client, targets, MAXPLAYERS, COMMAND_FILTER_NO_BOTS, target_name, sizeof(target_name), tn_is_ml);
    if (tcount <= 0)
    {
        ReplyToTargetError(client, tcount);
        return Plugin_Handled;
    }
    
    for (new x = 0; x < tcount; x++)
    {
        ZRiot_Zombie(targets[x]);
    }
    
    if (GetLiveHumanCount() <= 0)
    {
        TerminateRound(5.0, Terrorists_Win);
    }
    
    return Plugin_Handled;
}

public Action:Command_Human(client, argc)
{
    new bool:enabled = GetConVarBool(gCvars[CVAR_ENABLE]);
    if (argc < 1 || !enabled)
    {
        return Plugin_Handled;
    }
    
    decl String:arg1[32];
    GetCmdArg(1, arg1, sizeof(arg1));
    
    decl String:target_name[MAX_TARGET_LENGTH];
    new targets[MAXPLAYERS];
    new bool:tn_is_ml;
    
    new tcount = ProcessTargetString(arg1, client, targets, MAXPLAYERS, COMMAND_FILTER_NO_BOTS, target_name, sizeof(target_name), tn_is_ml);
    if (tcount <= 0)
    {
        ReplyToTargetError(client, tcount);
        return Plugin_Handled;
    }
    
    for (new x = 0; x < tcount; x++)
    {
        ZRiot_Human(targets[x]);
    }
    
    return Plugin_Handled;
}