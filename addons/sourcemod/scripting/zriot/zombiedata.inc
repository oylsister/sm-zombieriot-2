/**
 * ====================
 *     Zombie Riot
 *   File: zombiedata.inc
 *   Author: Greyscale
 * ==================== 
 */

enum struct ZRiot_ZombieData
{
    char data_name[32];
    char data_model[256];
    char data_zvision[256];
    bool data_override_required;
    int data_health;
    float data_speed;
    float data_gravity;
    float data_jump;
    int data_fov;
}

#define MAXZOMBIES 25

char modelSuffix[8][16] = {".dx80.vtx", ".dx90.vtx", ".mdl", ".phy", ".sw.vtx", ".vvd", ".xbox", ".xbox.vtx"};

Handle kvZombies = INVALID_HANDLE;

Handle adtModels = INVALID_HANDLE;

ZRiot_ZombieData arrayZombies[MAXZOMBIES];
Handle adtZombies = INVALID_HANDLE;
int zCount;

void FileLinesToArray(Handle array, const Handle files)
{
    ClearArray(array);
    
    char line[128];
    
    while(!IsEndOfFile(files) && ReadFileLine(files, line, sizeof(line)))
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
    
void LoadZombieData(bool defaultconfig)
{
    char path[PLATFORM_MAX_PATH];
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
    
    char name[64];
    char type[32];
    char model[256];
    char zvision[256];
    
    zCount = 0;
    
    do
    {
        KvGetSectionName(kvZombies, name, sizeof(name));
        strcopy(arrayZombies[zCount].data_name, 32, name);
        
        KvGetString(kvZombies, "type", type, sizeof(type));
        arrayZombies[zCount].data_override_required = (StrEqual(type, "override_required", false));
        
        KvGetString(kvZombies, "model", model, sizeof(model));
        strcopy(arrayZombies[zCount].data_model, 256, model);
        
        KvGetString(kvZombies, "zvision", zvision, sizeof(zvision));
        strcopy(arrayZombies[zCount].data_zvision, 256, zvision);
        
        arrayZombies[zCount].data_health = KvGetNum(kvZombies, "health", 500);
        
        arrayZombies[zCount].data_speed = KvGetFloat(kvZombies, "speed", 300.0);
        
        arrayZombies[zCount].data_gravity = KvGetFloat(kvZombies, "gravity", 1.0);
        
        arrayZombies[zCount].data_jump = KvGetFloat(kvZombies, "jump", 1.0);
        
        arrayZombies[zCount].data_fov = KvGetNum(kvZombies, "fov", 90);
        
        zCount++;
    } while (KvGotoNextKey(kvZombies));
}

void LoadModelData()
{
    char path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "configs/zriot/models.txt");
    
    Handle fileModels = OpenFile(path, "r");
    
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
    
    char model[256];
    char modelpath[256];
    
    int modelsize = GetArraySize(adtModels);
    for (int x = 0; x < modelsize; x++)
    {
        for (int y = 0; y < 8; y++)
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

void LoadDownloadData()
{
    char path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "configs/zriot/downloads.txt");
    
    Handle fileDownloads = OpenFile(path, "r");
    
    if (fileDownloads == INVALID_HANDLE)
    {
        SetFailState("\"%s\" missing from server", path);
    }
    
    Handle arrayDownloads = CreateArray(256, 0);
    
    FileLinesToArray(arrayDownloads, fileDownloads);
    
    char files[256];
    
    int downloadsize = GetArraySize(arrayDownloads);
    for (int x = 0; x < downloadsize; x++)
    {
        GetArrayString(arrayDownloads, x, files, sizeof(files));
        if (FileExists(files))
        {
            AddFileToDownloadsTable(files);
        }
        else
        {
            ZRiot_LogMessage("File load failed", files);
        }
    }
  
    CloseHandle(fileDownloads);
    CloseHandle(arrayDownloads);
}

int FindZombieIDByName(const char[] name)
{
    for (int x = 0; x < zCount; x++)
    {
        if (StrEqual(name, arrayZombies[x].data_name, false))
        {
            return x;
        }
    }
    
    return -1;
}

bool IsValidZombieID(int zombieid)
{
    if (zombieid > -1 && zombieid < zCount)
    {
        return true;
    }
    
    return false;
}

bool IsOverrideRequired(int zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        return arrayZombies[zombieid].data_override_required;
    }
    
    return false;
}

void ApplyZombieModel(int client, int zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        char model[256];
        strcopy(model, sizeof(model), arrayZombies[zombieid].data_model);
        
        PrecacheModel(model);
        SetEntityModel(client, model);
    }
}

void GetZombieZVision(int zombieid, char[] zvision, int maxlen)
{
    if (IsValidZombieID(zombieid))
    {
        strcopy(zvision, maxlen, arrayZombies[zombieid].data_zvision);
    }
}

void ApplyZombieHealth(int client, int zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        SetEntityHealth(client, arrayZombies[zombieid].data_health);
    }
}

void ApplyZombieSpeed(int client, int zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        SetPlayerSpeed(client, arrayZombies[zombieid].data_speed);
    }
}

void ApplyZombieGravity(int client, int zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        SetEntityGravity(client, arrayZombies[zombieid].data_gravity);
    }
}

float GetZombieJump(int zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        return arrayZombies[zombieid].data_jump;
    }
    
    return 0.0;
}

void ApplyZombieFOV(int client, int zombieid)
{
    if (IsValidZombieID(zombieid))
    {
        SetPlayerFOV(client, arrayZombies[zombieid].data_fov);
    }
}