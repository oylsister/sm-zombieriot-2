/**
 * ====================
 *     Zombie Riot
 *   File: translation.inc
 *   Author: Greyscale
 * ====================
 */

void FormatTextString(char[] text, int maxlen)
{
    Format(text, maxlen, " @green[%t] @default%s", "ZRiot", text);
    
    ReplaceString(text, maxlen, "@default","\x01");
    ReplaceString(text, maxlen, "@lgreen","\x03");
    ReplaceString(text, maxlen, "@green","\x04");
}

stock void ZRiot_PrintToChat(int client, any ...)
{
    char phrase[192];
    
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
        
        for (int x = 1; x <= MaxClients; x++)
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

stock void ZRiot_PrintCenterText(int client, any ...)
{
    SetGlobalTransTarget(client);
    
    char phrase[192];
    
    VFormat(phrase, sizeof(phrase), "%t", 2);
    
    PrintCenterText(client, "%s", phrase);
}

stock void ZRiot_HudHint(int client, any ...)
{
    SetGlobalTransTarget(client);
    
    char phrase[192];
    
    VFormat(phrase, sizeof(phrase), "%t", 2);
	
    if(!csgo)
    {
        Handle hHintText = StartMessageOne("HintText", client);
		
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
    else
    {
        Call_StartForward(hOnClientHUDUpdate);
        Call_PushCell(client);
        Call_PushStringEx(phrase, sizeof(phrase), SM_PARAM_STRING_COPY, SM_PARAM_COPYBACK);
        Call_Finish();
        
        PrintHintText(client, "%s", phrase);
    }
}

stock void ZRiot_PrintToServer(any ...)
{
    SetGlobalTransTarget(LANG_SERVER);
    
    char phrase[192];
    char buffer[192];
    
    VFormat(phrase, sizeof(phrase), "%t", 1);
    Format(buffer, sizeof(buffer), "[%t] %s", "ZRiot", phrase);
    
    PrintToServer(buffer);
}

stock void ZRiot_LogMessage(any ...)
{
    SetGlobalTransTarget(LANG_SERVER);
    
    char phrase[192];
    
    VFormat(phrase, sizeof(phrase), "%t", 1);
    
    LogMessage(phrase);
}

stock void ZRiot_ReplyToCommand(int client, any ...)
{
    char phrase[192];
    
    SetGlobalTransTarget(client);
        
    VFormat(phrase, sizeof(phrase), "%t", 2);
    FormatTextString(phrase, sizeof(phrase));
    
    ReplyToCommand(client, phrase);
}