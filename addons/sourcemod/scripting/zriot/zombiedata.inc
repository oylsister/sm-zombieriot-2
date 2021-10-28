/**
 * ====================
 *     Zombie Riot
 *   File: zombiedata.inc
 *   Author: Greyscale
 * ==================== 
 */

enum ZRiot_ZombieData
{
    String:data_name[32],
    String:data_model[256],
    String:data_zvision[256],
    bool:data_override_required,
    data_health,
    Float:data_speed,
    Float:data_gravity,
    Float:data_jump,
    data_fov
}

#define MAXZOMBIES 25

new String:modelSuffix[8][16] = {".dx80.vtx", ".dx90.vtx", ".mdl", ".phy", ".sw.vtx", ".vvd", ".xbox", ".xbox.vtx"};

new Handle:kvZombies = INVALID_HANDLE;

new Handle:adtModels = INVALID_HANDLE;

new arrayZombies[MAXZOMBIES][ZRiot_ZombieData];
new Handle:adtZombies = INVALID_HANDLE;
new zCount;

FileLinesToArray(Handle:array, const Handle:file)
{
    ClearArray(array);
    
    decl String:line[128];
    
    while(!IsEndOfFile(file) && ReadFileLine(file, line, sizeof(line)))
    {
        if (StrContains(line, ";") == -1)
        {
            if (StrContains(line, "//") > -1)
            {
                SplitString(line, "//", line, sizeof(line));
            }
            TrimString(line);
            
            if (!StrEqual(line, "", false))
            {
                PushArrayString(array, line);
            }
        }
    }
}
    
LoadZombieData(bool:defaultconfig)
{
    decl String:path[PLATFORM_MAX_PATH];
    Format(path, sizeof(path), "%s/zombies.txt", gMapConfig);
    
    if (!defaultconfig && !FileExists(path))
    {
        return;
    }
    
    if (kvZombies != INVALID_HANDLE)
    {
        CloseHandle(kvZombies);
    }
    
    kvZombies = CreateKeyValues("zombies");
    
    if (!FileToKeyValues(kvZombies, path))
    {
        SetFailState("\"%s\" failed to load", path);
    }
    
    KvRewind(kvZombies);
    if (!KvGotoFirstSubKey(kvZombies))
    {
        SetFailState("No zombie data defined in \"%s\"", path);
    }
    
    decl String:name[64];
    decl String:type[32];
    decl String:model[256];
    decl String:zvision[256];
    
    zCount = 0;
    
    do
    {
        KvGetSectionName(kvZombies, name, sizeof(name));
        strcopy(arrayZombies[zCount][data_name], 32, name);
        
        KvGetString(kvZombies, "type", type, sizeof(type));
        arrayZombies[zCount][data_override_required] = (StrEqual(type, "override_required", false));
        
        KvGetString(kvZombies, "model", model, sizeof(model));
        strcopy(arrayZombies[zCount][data_model], 256, model);
        
        KvGetString(kvZombies, "zvision", zvision, sizeof(zvision));
        strcopy(arrayZombies[zCount][data_zvision], 256, zvision);
        
        arrayZombies[zCount][data_health] = KvGetNum(kvZombies, "health", 500);
        
        arrayZombies[zCount][data_speed] = KvGetFloat(kvZombies, "speed", 300.0);
        
        arrayZombies[zCount][data_gravity] = KvGetFloat(kvZombies, "gravity", 1.0);
        
        arrayZombies[zCount][data_jump] = KvGetFloat(kvZombies, "jump", 1.0);
        
        arrayZombies[zCount][data_fov] = KvGetNum(kvZombies, "fov", 90);
        
        zCount++;
    } while (KvGotoNextKey(kvZombies));
}

LoadModelData()
{
    decl String:path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "configs/zriot/models.txt");
    
    new Handle:fileModels = OpenFile(path, "r");
    
    if (fileModels == INVALID_HANDLE)
    {
        SetFailState("\"%s\" missing from server", path);
    }
    
    if (adtModels != INVALID_HANDLE)
    {
        CloseHandle(adtModels);
    }
    
    adtModels = CreateArray(256, 0);
    
    FileLinesToArray(adtModels, fileModels);
    
    if (!GetArraySize(adtModels))
    {
        SetFailState("No models listed in models.txt, please add some models then restart");
    }
    
    decl String:model[256];
    decl String:modelpath[256];
    
    new modelsize = GetArraySize(adtModels);
    for (new x = 0; x < modelsize; x++)
    {
        for (new y = 0; y < 8; y++)
        {
            GetArrayString(adtModels, x, model, sizeof(model));
            Format(modelpath, sizeof(modelpath), "%s%s", model, modelSuffix[y]);
            
            if (FileExists(modelpath))
            {
                AddFileToDownloadsTable(modelpath);
            }
        }
    }
  
    CloseHandle(fileModels);
}

LoadDownloadData()
{
    decl String:path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "configs/zriot/downloads.txt");
    
    new Handle:fileDownloads = OpenFile(path, "r");
    
    if (fileDownloads == INVALID_HANDLE)
    {
        SetFailState("\"%s\" missing from server", path);
    }
    
    new Handle:arrayDownloads = CreateArray(256, 0);
    
    FileLinesToArray(arrayDownloads, fileDownloads);
    
    decl String:file[256];
    
    new downloadsize = GetArraySize(arrayDownloads);
    for (new x = 0; x < downloadsize; x++)
    {
        GetArrayString(arrayDownloads, x, file, sizeof(file));
        if (FileExists(file))
        {
            AddFileToDownloadsTable(file);
        }
        else
        {
            ZRiot_LogMessage("File load failed", file);
        }
    }
  
    CloseHandle(fileDownloads);
    CloseHandle(arrayDownloads);
}

FindZombieIDByName(const String:name[])
{
    for (new x = 0; x < zCount; x++)
    {
        if (StrEqual(name, arrayZombies[x][data_name], false))
        {
            return x;
        }
    }
    
    return -1;
}

bool:IsValidZombieID(zombieid)
{
    if (zombieid > -1 && zombieid < zCount)
    {
        return true;
    }
    
    return false;
}

bool:IsOverrideRequired(zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        return arrayZombies[zombieid][data_override_required];
    }
    
    return false;
}

ApplyZombieModel(client, zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        decl String:model[256];
        strcopy(model, sizeof(model), arrayZombies[zombieid][data_model]);
        
        PrecacheModel(model);
        SetEntityModel(client, model);
    }
}

GetZombieZVision(zombieid, String:zvision[], maxlen)
{
    if (IsValidZombieID(zombieid))
    {
        strcopy(zvision, maxlen, arrayZombies[zombieid][data_zvision]);
    }
}

ApplyZombieHealth(client, zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        SetEntityHealth(client, arrayZombies[zombieid][data_health]);
    }
}

ApplyZombieSpeed(client, zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        SetPlayerSpeed(client, arrayZombies[zombieid][data_speed]);
    }
}

ApplyZombieGravity(client, zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        SetEntityGravity(client, arrayZombies[zombieid][data_gravity]);
    }
}

Float:GetZombieJump(zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        return arrayZombies[zombieid][data_jump];
    }
    
    return 0.0;
}

ApplyZombieFOV(client, zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        SetPlayerFOV(client, arrayZombies[zombieid][data_fov]);
    }
}