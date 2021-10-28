/**
 * ====================
 *     Zombie Riot
 *   File: translation.inc
 *   Author: Greyscale
 * ====================
 */

FormatTextString(String:text[], maxlen)
{
    Format(text, maxlen, "@green[%t] @default%s", "ZRiot", text);
    
    ReplaceString(text, maxlen, "@default","\x01");
    ReplaceString(text, maxlen, "@lgreen","\x03");
    ReplaceString(text, maxlen, "@green","\x04");
}

stock ZRiot_PrintToChat(client, any:...)
{
    decl String:phrase[192];
    
    if (client)
    {
        SetGlobalTransTarget(client);
        
        VFormat(phrase, sizeof(phrase), "%t", 2);
        FormatTextString(phrase, sizeof(phrase));
        
        PrintToChat(client, phrase);
    }
    else
    {
        SetGlobalTransTarget(client);
        
        VFormat(phrase, sizeof(phrase), "%t", 2);
        FormatTextString(phrase, sizeof(phrase));
        
        PrintToServer(phrase);
        
        new maxplayers = GetMaxClients();
        for (new x = 1; x <= maxplayers; x++)
        {
            if (IsClientInGame(x))
            {
                SetGlobalTransTarget(x);
                
                VFormat(phrase, sizeof(phrase), "%t", 2);
                FormatTextString(phrase, sizeof(phrase));
                
                PrintToChat(x, phrase);
            }
        }
    }
}

stock ZRiot_PrintCenterText(client, any:...)
{
    SetGlobalTransTarget(client);
    
    decl String:phrase[192];
    
    VFormat(phrase, sizeof(phrase), "%t", 2);
    
    PrintCenterText(client, phrase);
}

stock ZRiot_HudHint(client, any:...)
{
    SetGlobalTransTarget(client);
    
    decl String:phrase[192];
    
    VFormat(phrase, sizeof(phrase), "%t", 2);
    
    new Handle:hHintText = StartMessageOne("HintText", client);
    if (hHintText != INVALID_HANDLE)
    {
        BfWriteByte(hHintText, -1);
        
        Call_StartForward(hOnClientHUDUpdate);
        Call_PushCell(client);
        Call_PushStringEx(phrase, sizeof(phrase), SM_PARAM_STRING_COPY, SM_PARAM_COPYBACK);
        Call_Finish();
        
        BfWriteString(hHintText, phrase);
        EndMessage();
    }
}

stock ZRiot_PrintToServer(any:...)
{
    SetGlobalTransTarget(LANG_SERVER);
    
    decl String:phrase[192];
    decl String:buffer[192];
    
    VFormat(phrase, sizeof(phrase), "%t", 1);
    Format(buffer, sizeof(buffer), "[%t] %s", "ZRiot", phrase);
    
    PrintToServer(buffer);
}

stock ZRiot_LogMessage(any:...)
{
    SetGlobalTransTarget(LANG_SERVER);
    
    decl String:phrase[192];
    
    VFormat(phrase, sizeof(phrase), "%t", 1);
    
    LogMessage(phrase);
}

stock ZRiot_ReplyToCommand(client, any:...)
{
    decl String:phrase[192];
    
    SetGlobalTransTarget(client);
        
    VFormat(phrase, sizeof(phrase), "%t", 2);
    FormatTextString(phrase, sizeof(phrase));
    
    ReplyToCommand(client, phrase);
}