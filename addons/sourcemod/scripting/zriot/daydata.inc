/**
 * ====================
 *     Zombie Riot
 *   File: daydata.inc
 *   Author: Greyscale
 * ====================
 */

enum ZRiot_DayData
{
    String:data_display[32],
    String:data_zombieoverride[32*MAXZOMBIES],
    String:data_storyline[192],
    data_count,
    data_healthboost,
    bool:data_respawn,
    data_deaths_before_zombie,
    Float:data_fademin,
    Float:data_fademax
}

#define MAXDAYS 25

new Handle:kvDays = INVALID_HANDLE;

new arrayDays[MAXDAYS][ZRiot_DayData];
new dCount;

LoadDayData(bool:defaultconfig)
{
    decl String:path[PLATFORM_MAX_PATH];
    Format(path, sizeof(path), "%s/days.txt", gMapConfig);
    
    if (!defaultconfig && !FileExists(path))
    {
        return;
    }
    
    if (kvDays != INVALID_HANDLE)
    {
        CloseHandle(kvDays);
    }
    
    kvDays = CreateKeyValues("days");
    KvSetEscapeSequences(kvDays, true);
    
    if (!FileToKeyValues(kvDays, path))
    {
        SetFailState("\"%s\" failed to load", path);
    }
    
    KvRewind(kvDays);
    if (!KvGotoFirstSubKey(kvDays))
    {
        SetFailState("No day data defined in \"%s\"", path);
    }
    
    decl String:display[32];
    decl String:zombieoverride[32*MAXZOMBIES];
    decl String:storyline[192];
    
    dCount = 0;
    do
    {
        KvGetSectionName(kvDays, display, sizeof(display));
        strcopy(arrayDays[dCount][data_display], 32, display);
        
        KvGetString(kvDays, "zombieoverride", zombieoverride, sizeof(zombieoverride));
        strcopy(arrayDays[dCount][data_zombieoverride], 32*MAXZOMBIES, zombieoverride);
        
        KvGetString(kvDays, "storyline", storyline, sizeof(storyline));
        strcopy(arrayDays[dCount][data_storyline], 192, storyline);
        
        arrayDays[dCount][data_count] = KvGetNum(kvDays, "count", 25);
        arrayDays[dCount][data_healthboost] = KvGetNum(kvDays, "healthboost");
        arrayDays[dCount][data_respawn] = bool:KvGetNum(kvDays, "respawn");
        arrayDays[dCount][data_deaths_before_zombie] = KvGetNum(kvDays, "deaths_before_zombie");
        arrayDays[dCount][data_fademin] = KvGetFloat(kvDays, "fademin");
        arrayDays[dCount][data_fademax] = KvGetFloat(kvDays, "fademax");
        
        dCount++;
    } while (KvGotoNextKey(kvDays));
}

GetDayDisplay(day, String:display[], len)
{
    strcopy(display, len, arrayDays[day][data_display]);
}

bool:ExplodeZombieOverrides(day)
{
    if (adtZombies != INVALID_HANDLE)
    {
        CloseHandle(adtZombies);
        adtZombies = INVALID_HANDLE;
    }
    
    decl String:zombieoverride[32*MAXZOMBIES];
    GetDayZombieOverride(day, zombieoverride, sizeof(zombieoverride));
    
    if (zombieoverride[0])
    {
        adtZombies = CreateArray();
        
        new String:sZombies[MAXZOMBIES][64];
        ExplodeString(zombieoverride, ",", sZombies, MAXZOMBIES, 64);
        
        for (new x = 0; x < MAXZOMBIES; x++)
        {
            if (!sZombies[x][0])
                continue;
            
            TrimString(sZombies[x]);
            new zombieid = FindZombieIDByName(sZombies[x]);
            
            if (zombieid == -1)
                continue;
            
            PushArrayCell(adtZombies, zombieid);
        }
        
        return true;
    }
    
    return false;
}

GetDayZombieOverride(day, String:zombieoverride[], len)
{
    strcopy(zombieoverride, len, arrayDays[day][data_zombieoverride]);
}

GetDayStoryLine(day, String:storyline[], len)
{
    strcopy(storyline, len, arrayDays[day][data_storyline]);
}

GetDayCount(day)
{
    return arrayDays[day][data_count];
}

GetDayHealthBoost(day)
{
    return arrayDays[day][data_healthboost];
}

bool:GetDayRespawn(day)
{
    return arrayDays[day][data_respawn];
}

GetDayDeathsBeforeZombie(day)
{
    return arrayDays[day][data_deaths_before_zombie];
}

Float:GetDayMinFade(day)
{
    return arrayDays[day][data_fademin];
}

Float:GetDayMaxFade(day)
{
    return arrayDays[day][data_fademax];
}

BeginDay()
{
    gZombiesKilled = 0;
    
    new zombiecount = GetDayCount(gDay);
    new zombiemax = GetConVarInt(gCvars[CVAR_ZOMBIEMAX]);
    
    new spawncount;
    new bool:respawn = GetDayRespawn(gDay);
    
    if (respawn)
    {
        spawncount = zombiemax;
    }
    else
    {
        spawncount = (zombiecount < zombiemax) ? zombiecount : zombiemax;
    }
    
    ServerCommand("bot_quota %d", spawncount);
    
    decl String:display[32];
    GetDayDisplay(gDay, display, sizeof(display));
    
    new bool:override = ExplodeZombieOverrides(gDay);
    
    new maxplayers = GetMaxClients();
    for (new x = 1; x <= maxplayers; x++)
    {
        if (!IsClientInGame(x))
        {
            continue;
        }
        
        ChangeClientDeathCount(x, 0);
        
        if (!IsPlayerZombie(x))
        {
            continue;
        }
        
        if (override)
        {
            new size = GetArraySize(adtZombies);
            if (size)
            {
                new zombieid = GetRandomInt(0, size - 1);
                            
                Zombify(x, GetArrayCell(adtZombies, zombieid));
            }
        }
    }
}
