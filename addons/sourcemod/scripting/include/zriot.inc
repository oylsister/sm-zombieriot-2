/**
 * ====================
 *    Zombie Riot
 *   File: zriot.inc
 *   Author: Greyscale
 * ==================== 
 */

/**
 * Used to check if a player is a zombie.
 * @param client   Client index.
 * @return   True if the player is a zombie, and false if human.
 */   
native bool ZRiot_IsClientZombie(int client);

/**
 * Retrieves the team index of the zombies
 * @note   Remember these are based off cvars, 0 will be returned if called before the cvar is set
 */
native void ZRiot_GetZombieTeam();

/**
 * Retrieves the team index of the humans
 * @note   Remember these are based off cvars, 0 will be returned if called before the cvar is set
 */
native void ZRiot_GetHumanTeam();

/**
 * Turns a human into a zombie
 * @param client   Client index.
 */
native void ZRiot_Zombie(int client);

/**
 * Called when a player turns into a zombie.  This is not called at round end.
 * @param client   Client index.
 */
forward void ZRiot_OnClientZombie(int client);

/**
 * Turns a zombie into a human (will not work for bots)
 * @param client   Client index.
 */
native void ZRiot_Human(int client);

/**
 * Called when a player turns into a human.  This is not called at round end.
 * @param client   Client index.
 */
forward void ZRiot_OnClientHuman(int client);

/**
 * Called when the HUD is being updated on a client (not called for bots)
 * @param client   Client index.
 * @param hinttext   The text string being sent to the usermsg "HintText"
 * @note   If hinttext is modified the new value will be sent to the client
 */
forward void ZRiot_OnClientHUDUpdate(int client, char[] hinttext);