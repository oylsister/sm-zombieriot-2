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
native bool:ZRiot_IsClientZombie(client);

/**
 * Retrieves the team index of the zombies
 * @note   Remember these are based off cvars, 0 will be returned if called before the cvar is set
 */
native ZRiot_GetZombieTeam();

/**
 * Retrieves the team index of the humans
 * @note   Remember these are based off cvars, 0 will be returned if called before the cvar is set
 */
native ZRiot_GetHumanTeam();

/**
 * Turns a human into a zombie
 * @param client   Client index.
 */
native ZRiot_Zombie(client);

/**
 * Called when a player turns into a zombie.  This is not called at round end.
 * @param client   Client index.
 */
forward ZRiot_OnClientZombie(client);

/**
 * Turns a zombie into a human (will not work for bots)
 * @param client   Client index.
 */
native ZRiot_Human(client);

/**
 * Called when a player turns into a human.  This is not called at round end.
 * @param client   Client index.
 */
forward ZRiot_OnClientHuman(client);

/**
 * Called when the HUD is being updated on a client (not called for bots)
 * @param client   Client index.
 * @param hinttext   The text string being sent to the usermsg "HintText"
 * @note   If hinttext is modified the new value will be sent to the client
 */
forward ZRiot_OnClientHUDUpdate(client, String:hinttext[]);