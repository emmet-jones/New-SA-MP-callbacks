/*
 *	New SA-MP callbacks by Emmet
 *	Created on Sunday, January 26, 2014 at 11:58 AM.
 *
 *  Last Update: February 25, 2016 @ 10:00 AM.
*/

// The amount of damage a vehicle must take before being classified as a car crash.
#if !defined POTENTIAL_CRASH_DAMAGE
	#define POTENTIAL_CRASH_DAMAGE (55.0)
#endif

// Maximum amount of shots fired without reloading before OnPlayerAntiReload is called.
#if !defined MAX_ACCUMULATED_SHOTS
	#define MAX_ACCUMULATED_SHOTS (5)
#endif

#define CB_GetPlayerFlag(%0,%1) \
	((g_cPlayerInfo[(%0)][e_cbFlags]) & (%1))

#define CB_EnableFlag(%0,%1) \
	((g_cPlayerInfo[(%0)][e_cbFlags]) |= (%1))

#define CB_DisableFlag(%0,%1) \
	((g_cPlayerInfo[(%0)][e_cbFlags]) &= (~%1))

#define PLAYER_ACTION_NONE      (0)
#define PLAYER_ACTION_SHOOTING  (1)
#define PLAYER_ACTION_SWIMMING  (2)
#define PLAYER_ACTION_SKYDIVING (3)
	
// Player variables
enum e_cbPlayerData {
	e_cbPlayerFlags:e_cbFlags,
	Float:e_cbPacket,
	e_cbHoldingTimer,
  	e_cbFrames,
  	e_cbDrunkLevel,
  	e_cbTarget,
  	e_cbAction,
  	e_cbAnimation,
  	e_cbHoverColor,
  	e_cbShotCounter[12],
  	e_cbLastWeapon,
  	e_cbLastAmmo,
   	e_cbVehicle
};

// Used for storing tick counts (GetTickCount).
enum e_cbPlayerTicks {
	e_cbTickCount,
	e_cbLastUpdate,
	e_cbLastShot,
	e_cbLastJump,
	e_cbBurnTick,
	e_cbVendingTick
};

// Vehicle variables
enum e_cbCarData {
	e_vPanels,
	e_vDoors,
	e_vLights,
	e_vEnterID,
	Float:e_vHealth,
	Float:e_vSpeed
};

// Player flags
enum e_cbPlayerFlags (<<= 1) {
	e_cbUpdate = 1,
	e_cbPaused,
	e_cbHolding,
	e_cbSelection,
	e_cbAnimationSet,
	e_cbReloading,
	e_cbBurning,
	e_cbAiming,
	e_cbPressed,
	e_cbSprinting
};

static const
    g_sCallbackList[][] = {
		{"OnPlayerPause"},          {"OnPlayerResume"},         {"OnPlayerHoldingKey"},
		{"OnPlayerReleaseKey"},     {"OnPlayerFall"},           {"OnPlayerPacketLoss"},
		{"OnPlayerUseVending"},     {"OnPlayerCrashVehicle"},   {"OnPlayerFPSChange"},
		{"OnPlayerJackVehicle"},    {"OnPlayerEmptyWeapon"},    {"OnPlayerFriendlyFire"},
		{"OnPlayerTargetPlayer"},   {"OnPlayerHideCursor"},     {"OnPlayerAntiReload"},
		{"OnPlayerAnimationPlay"},  {"OnPlayerReloadWeapon"},   {"OnPlayerBurn"},
		{"OnPlayerAiming"}, 		{"OnPlayerRamPlayer"},      {"OnPlayerActionChange"},
		{"OnPlayerUseCamera"},      {"OnPlayerSprayAtVehicle"}, {"OnPlayerSprint"},
		{"OnPlayerJump"}
	},

	Float:g_aVendingLocations[62][3] = {
	    {2480.9548, -1958.5217, 13.5830}, {2325.9185, -1645.9553, 14.8270},
		{2352.9932, -1357.1597, 24.3984}, {2153.8535, -1015.7230, 62.8835},
	    {2140.2710, -1161.4998, 23.9922}, {2224.4141, -1153.4441, 1025.7969},
		{2060.1152, -1898.4554, 13.5538}, {1634.0741, -2238.2815, 13.5078},
	    {1729.7947, -1943.8628, 13.5687}, {-16.1516, -90.8512, 1003.5469},
		{-17.5205, -90.9663, 1003.5469},  {1929.5516, -1772.4803, 13.5469},
	    {496.0115, -23.5306, 1000.6797},  {500.6090, -2.1074, 1000.6797},
		{501.7604, -2.2185, 1000.6797},   {-19.1310, -57.0453, 1003.5469},
	    {-36.1302, -57.1351, 1003.5469},  {1153.9125, -1460.9729, 15.7969},
		{1788.3973, -1369.2472, 15.7578}, {2575.7300, -1284.6108, 1060.9844},
	    {662.4797, -551.4141, 16.3359},   {200.2021, -107.5868, 1.5513},
		{1278.5977, 372.2211, 19.5547},   {2271.7053, -77.2283, 26.5810},
	    {-2119.4229, -422.2747, 35.5313}, {-2119.7649, -423.4770, 35.5313},
		{-2068.6082, -397.5225, 35.5313}, {-2039.8768, -397.5218, 35.5313},
	    {-2011.1292, -397.5222, 35.5313}, {-2005.6145, -490.8686, 35.5313},
		{-2034.4011, -490.8754, 35.5313}, {-2063.2300, -490.8705, 35.5313},
	    {-2092.0771, -490.8697, 35.5313}, {-2229.1521, 287.1645, 35.3203},
		{-1981.6025, 142.6991, 27.6875},  {-1349.2971, 492.2820, 11.1953},
	    {-1349.3674, 493.9295, 11.1953},  {-2419.5833, 984.5712, 45.2969},
		{-2419.5457, 986.0020, 45.2969},  {-1455.0173, 2592.4175, 55.8359},
	    {-252.9913, 2599.6885, 62.8582},  {-252.9274, 2597.8921, 62.8582},
		{-862.7581, 1537.4230, 22.5870},  {-75.2812, 1227.9269, 19.7359},
	    {-14.7254, 1176.1750, 19.5634},   {1398.8198, 2223.3601, 11.0234},
		{1659.4318, 1722.0974, 10.8281},  {1519.3350, 1055.2886, 10.8203},
	    {2503.1318, 1244.5094, 10.8203},  {2647.7424, 1128.9138, 11.1797},
		{2845.7429, 1294.2975, 11.3906},  {2320.0618, 2532.0364, 10.8203},
	    {2086.5894, 2071.4111, 11.0579},  {-32.5264, -185.9041, 1003.5469},
		{-33.8699, -186.0213, 1003.5469}, {361.6433, 159.3575, 1008.3828},
	    {374.7823, 188.2360, 1008.3893},  {351.6473, 206.1147, 1008.3828},
		{371.6005, 177.7106, 1019.9844},  {373.8686, -178.9308, 1000.6328},
	    {378.2982, -178.9220, 1000.6328}, {315.9102, -140.5504, 999.6016}
	}
;

static g_cPlayerInfo[MAX_PLAYERS][e_cbPlayerData];
static g_cPlayerTick[MAX_PLAYERS][e_cbPlayerTicks];

static g_cVehicleInfo[MAX_VEHICLES][e_cbCarData];
static g_cCallbackDefined[sizeof(g_sCallbackList)];

// Called when a player pauses their game.
forward OnPlayerPause(playerid);

// Called when a player resumes their game.
forward OnPlayerResume(playerid, time);

// Called when a player starts holding a key.
forward OnPlayerHoldingKey(playerid, keys);

// Called when a player releases a key.
forward OnPlayerReleaseKey(playerid, keys, time);

// Called when a player falls and loses damage.
forward OnPlayerFall(playerid, Float:damage);

// Called when a player experiences packet loss.
forward OnPlayerPacketLoss(playerid, Float:oldloss, Float:newloss);

// Called when a player crashes their vehicle.
forward OnPlayerCrashVehicle(playerid, vehicleid, Float:damage);

// Called when a player uses a vending machine.
forward OnPlayerUseVending(playerid);

// Called when a player's FPS change.
forward OnPlayerFPSChange(playerid, oldfps, newfps);

// Called when a player jacks another player's vehicle.
forward OnPlayerJackVehicle(playerid, targetid, vehicleid);

// Called when a player completely uses up all ammo in their weapon.
forward OnPlayerEmptyWeapon(playerid, weaponid);

// Called when a player shoots another player in their team.
forward OnPlayerFriendlyFire(playerid, targetid, weaponid);

// Called when a player aims at another player.
forward OnPlayerTargetPlayer(playerid, targetid, weaponid);

// Called when a player explicitly disables textdraw selection mode.
forward OnPlayerHideCursor(playerid, hovercolor);

// Called when a player shoots more ammo than their weapon's clip can hold.
forward OnPlayerAntiReload(playerid, weaponid);

// Called when an animation is finished.
forward OnPlayerAnimationPlay(playerid, animlib[], animname[]);

// Called when a player is reloading their weapon.
forward OnPlayerReloadWeapon(playerid, weaponid, ammo);

// Called when a player starts or stops burning from fire.
forward OnPlayerBurn(playerid, status);

// Called when a player starts or stops aiming a weapon.
forward OnPlayerAiming(playerid, weaponid, status);

// Called when a player's action changes.
forward OnPlayerActionChange(playerid, oldaction, newaction);

// Called when a player rams another player.
forward OnPlayerRamPlayer(playerid, driverid, vehicleid, Float:damage);

// Called when a player uses a camera to take a picture.
forward OnPlayerUseCamera(playerid);

// Called when a player sprays a spraycan towards a vehicle.
forward OnPlayerSprayAtVehicle(playerid, vehicleid);

// Called when a player jumps by pressing the jump key.
forward OnPlayerJump(playerid);

// Called when a player starts or stops sprinting.
forward OnPlayerSprint(playerid, status);

stock IsPlayerNearVending(playerid)
{
    for(new i = 0; i < sizeof(g_aVendingLocations); i ++)
	{
        if(IsPlayerInRangeOfPoint(playerid, 3.0, g_aVendingLocations[i][0], g_aVendingLocations[i][1], g_aVendingLocations[i][2]))
		{
            return 1;
		}
    }
	return 0;
}

stock GetConnectedTime(playerid)
{
	return NetStats_GetConnectedTime(playerid);
}

stock IsPlayerPaused(playerid)
{
	return CB_GetPlayerFlag(playerid, e_cbPaused);
}

stock IsPlayerBurning(playerid)
{
	return CB_GetPlayerFlag(playerid, e_cbBurning);
}

stock IsPlayerAiming(playerid)
{
	return CB_GetPlayerFlag(playerid, e_cbAiming);
}

stock IsPlayerSwimming(playerid)
{
    return (1538 <= GetPlayerAnimationIndex(playerid) <= 1544);
}

stock IsPlayerSkydiving(playerid)
{
	return (GetPlayerWeapon(playerid) == 46 && GetPlayerAnimationIndex(playerid) == 1134);
}

stock IsPlayerSprinting(playerid)
{
	new
	    index = GetPlayerAnimationIndex(playerid),
	    keys,
	    ud,
	    lr
	;

	GetPlayerKeys(playerid, keys, ud, lr);
	
	return (keys & KEY_SPRINT) && ((1222 <= index <= 1236) || index == 1196);
}

stock IsPlayerJumping(playerid)
{
	new
	    index = GetPlayerAnimationIndex(playerid),
	    keys,
	    ud,
	    lr
	;

	GetPlayerKeys(playerid, keys, ud, lr);

	return (keys & KEY_JUMP) && (1196 <= index <= 1198);
}

stock GetPlayerFPS(playerid)
{
	return g_cPlayerInfo[playerid][e_cbFrames];
}

stock GetPlayerPausedTime(playerid)
{
	return (GetConnectedTime(playerid) - g_cPlayerTick[playerid][e_cbTickCount]);
}

stock IsPlayerSprayingVehicle(playerid, vehicleid)
{
	if(IsVehicleStreamedIn(vehicleid, playerid) && CB_GetVehicleInFrontOfPlayer(playerid) == vehicleid)
	{
	    new
			index = GetPlayerAnimationIndex(playerid);

		return ((1160 <= index <= 1163) || index == 1167 || index == 640);
	}
	return 0;
}

stock CB_ApplyAnimation(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync = 0)
{
	new
		ret = ApplyAnimation(playerid, animlib, animname, fDelta, loop, lockx, locky, freeze, time, forcesync);

	if(ret)
	{
	 	SetTimerEx("Callback_CheckAnimation", 100, false, "i", playerid);
	}
	return ret;
}

stock CB_SelectTextDraw(playerid, hovercolor)
{
	new
	    ret = SelectTextDraw(playerid, hovercolor);

	if(ret)
	{
		g_cPlayerInfo[playerid][e_cbHoverColor] = hovercolor;

		CB_EnableFlag(playerid, e_cbSelection);
	}
	return ret;
}

stock CB_CancelSelectTextDraw(playerid)
{
	new
	    ret = CancelSelectTextDraw(playerid);

	if(ret)
	{
	    g_cPlayerInfo[playerid][e_cbHoverColor] = 0;

		CB_DisableFlag(playerid, e_cbSelection);
	}
	return ret;
}

stock static Callback_GetDriver(vehicleid)
{
	for(new i = 0; i < MAX_PLAYERS; i ++)
	{
		if(GetPlayerState(i) == PLAYER_STATE_DRIVER && IsPlayerInVehicle(i, vehicleid))
		{
		    return i;
		}
	}
	return INVALID_PLAYER_ID;
}

stock static Callback_GetMaxClip(weaponid)
{
    static const
		s_AmmoCapacity[11] = {34, 17, 7, 1, 4, 7, 100, 30, 30, 50, 100}
	;

	if((WEAPON_COLT45 <= weaponid <= WEAPON_TEC9) && weaponid != WEAPON_SHOTGUN)
	{
	    weaponid -= WEAPON_COLT45;

	    return s_AmmoCapacity[weaponid];
	}

	return 0;
}

stock static Float:Callback_GetSpeed(vehicleid)
{
	// This is only for basic speed calculations. No need to get the real speed.

	new
	    Float:fX,
	    Float:fY,
	    Float:fZ
	;

	GetVehicleVelocity(vehicleid, fX, fY, fZ);

	return (floatsqroot((fX * fX) + (fY * fY) + (fZ * fZ)) * 100);
}

stock static CB_IsVehicleInRangeOfPoint(vehicleid, Float:range, Float:x, Float:y, Float:z)
{
	new
		Float:fX,
		Float:fY,
		Float:fZ
	;

	GetVehiclePos(vehicleid, fX, fY, fZ);

	fX -= x;
	fY -= y;
	fZ -= z;

	return ((fX * fX) + (fY * fY) + (fZ * fZ)) < (range * range);
}

stock static CB_GetVehicleInFrontOfPlayer(playerid)
{
	new
		Float:fX,
		Float:fY,
		Float:fZ,
		Float:fA
	;

	GetPlayerPos(playerid, fX, fY, fZ);
    GetPlayerFacingAngle(playerid, fA);

    fX += (2.0 * floatsin(-fA, degrees));
	fY += (2.0 * floatcos(-fA, degrees));

 	for(new i = 0; i < MAX_VEHICLES; i ++)
  	{
   		if(IsVehicleStreamedIn(i, playerid) && CB_IsVehicleInRangeOfPoint(i, 2.0, fX, fY, fZ))
      	{
     		return i;
  		}
	}
	return INVALID_VEHICLE_ID;
}

stock static Callback_IsDefined(const name[])
{
	for(new i = 0; i < sizeof(g_sCallbackList); i ++)
	{
	    if(g_cCallbackDefined[i] && !strcmp(g_sCallbackList[i], name))
	    {
	        return 1;
		}
	}

	return 0;
}

stock static Callback_Initialize()
{
	for(new i = 0; i < sizeof(g_sCallbackList); i ++)
	{
		if(funcidx(g_sCallbackList[i]) >= 0)
		{
		    g_cCallbackDefined[i] = true;
		}
	}

	if(Callback_IsDefined("OnPlayerPause") || Callback_IsDefined("OnPlayerResume"))
    {
		SetTimer("Callback_TabCheck", 600, true);
	}
}

public OnPlayerDisconnect(playerid, reason)
{
	// Kill any active timers.

	if(CB_GetPlayerFlag(playerid, e_cbHolding))
	{
	    KillTimer(g_cPlayerInfo[playerid][e_cbHoldingTimer]);
	}

	// Reset the tick variables.
    g_cPlayerTick[playerid][e_cbTickCount] = 0;
    g_cPlayerTick[playerid][e_cbLastUpdate] = 0;
	g_cPlayerTick[playerid][e_cbLastShot] = 0;
	g_cPlayerTick[playerid][e_cbLastJump] = 0;
    g_cPlayerTick[playerid][e_cbBurnTick] = 0;
	g_cPlayerTick[playerid][e_cbVendingTick] = 0;

	// Reset the player information.
	g_cPlayerInfo[playerid][e_cbAction] = PLAYER_ACTION_NONE;
	g_cPlayerInfo[playerid][e_cbLastWeapon] = 0;
	g_cPlayerInfo[playerid][e_cbLastAmmo] = 0;
  	g_cPlayerInfo[playerid][e_cbFrames] = 0;
  	g_cPlayerInfo[playerid][e_cbDrunkLevel] = 0;
  	g_cPlayerInfo[playerid][e_cbTarget] = 0;
  	g_cPlayerInfo[playerid][e_cbAnimation] = 0;
  	g_cPlayerInfo[playerid][e_cbHoverColor] = 0;
 	g_cPlayerInfo[playerid][e_cbPacket] = 0.0;
 	g_cPlayerInfo[playerid][e_cbVehicle] = INVALID_VEHICLE_ID;
    g_cPlayerInfo[playerid][e_cbFlags] = e_cbPlayerFlags:0;

    for(new i = 0; i < 12; i ++)
	{
		g_cPlayerInfo[playerid][e_cbShotCounter][i] = 0;
	}

 	#if defined CB_OnPlayerDisconnect
 	    return CB_OnPlayerDisconnect(playerid, reason);
	#else
	    return 1;
	#endif
}

public OnFilterScriptInit()
{
    Callback_Initialize();

	#if defined CB_OnFilterScriptInit
	    return CB_OnFilterScriptInit();
	#else
		return 1;
	#endif
}

public OnGameModeInit()
{
 	Callback_Initialize();

	#if defined CB_OnGameModeInit
	    return CB_OnGameModeInit();
	#else
		return 1;
	#endif
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
    g_cPlayerInfo[playerid][e_cbVehicle] = INVALID_PLAYER_ID;
    
    #if defined CB_OnPlayerExitVehicle
        return CB_OnPlayerExitVehicle(playerid, vehicleid);
	#else
	    return 1;
	#endif
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if(Callback_IsDefined("OnPlayerFriendlyFire"))
	{
	    new teamid = GetPlayerTeam(playerid);

		if((teamid != NO_TEAM && hittype == BULLET_HIT_TYPE_PLAYER) && GetPlayerTeam(hitid) == teamid)
		{
			CallLocalFunction("OnPlayerFriendlyFire", "iii", playerid, hitid, weaponid);
		}
	}
	if(Callback_IsDefined("OnPlayerEmptyWeapon") && GetPlayerWeaponState(playerid) == WEAPONSTATE_LAST_BULLET && GetPlayerAmmo(playerid) == 1)
	{
		CallLocalFunction("OnPlayerEmptyWeapon", "ii", playerid, weaponid);
	}
	if(Callback_IsDefined("OnPlayerAntiReload") && (WEAPON_COLT45 <= weaponid <= WEAPON_TEC9) && weaponid != WEAPON_SHOTGUN)
	{
		new
			clip = Callback_GetMaxClip(weaponid),
			index = weaponid - WEAPON_COLT45;
			
		g_cPlayerInfo[playerid][e_cbShotCounter][index]++;

		if((g_cPlayerInfo[playerid][e_cbShotCounter][index] - clip) >= MAX_ACCUMULATED_SHOTS)
		{
			CallLocalFunction("OnPlayerAntiReload", "ii", playerid, weaponid);
            g_cPlayerInfo[playerid][e_cbShotCounter][index] = 0;
		}
	}
	if(Callback_IsDefined("OnPlayerActionChange"))
	{
		if(g_cPlayerInfo[playerid][e_cbAction] != PLAYER_ACTION_SHOOTING)
		{
	    	CallLocalFunction("OnPlayerActionChange", "iii", playerid, g_cPlayerInfo[playerid][e_cbAction], PLAYER_ACTION_SHOOTING);
	    	g_cPlayerInfo[playerid][e_cbAction] = PLAYER_ACTION_SHOOTING;
		}
		g_cPlayerTick[playerid][e_cbLastShot] = GetConnectedTime(playerid);
	}

	#if defined CB_OnPlayerWeaponShot
	    return CB_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
		return 1;
	#endif
}

public OnPlayerUpdate(playerid)
{
	new
		weaponid = GetPlayerWeapon(playerid),
		level = GetPlayerDrunkLevel(playerid),
		Float:health
	;

 	if(Callback_IsDefined("OnPlayerActionChange"))
	{
		if(IsPlayerSkydiving(playerid) && g_cPlayerInfo[playerid][e_cbAction] != PLAYER_ACTION_SKYDIVING)
 		{
	 		CallLocalFunction("OnPlayerActionChange", "iii", playerid, g_cPlayerInfo[playerid][e_cbAction], PLAYER_ACTION_SKYDIVING);
   			g_cPlayerInfo[playerid][e_cbAction] = PLAYER_ACTION_SKYDIVING;
 		}
 		else if(!IsPlayerSkydiving(playerid) && g_cPlayerInfo[playerid][e_cbAction] == PLAYER_ACTION_SKYDIVING)
 		{
 		    CallLocalFunction("OnPlayerActionChange", "iii", playerid, g_cPlayerInfo[playerid][e_cbAction], PLAYER_ACTION_NONE);
 		    g_cPlayerInfo[playerid][e_cbAction] = PLAYER_ACTION_SKYDIVING;
		}
		else if(IsPlayerSwimming(playerid) && g_cPlayerInfo[playerid][e_cbAction] != PLAYER_ACTION_SWIMMING)
 		{
 		    CallLocalFunction("OnPlayerActionChange", "iii", playerid, g_cPlayerInfo[playerid][e_cbAction], PLAYER_ACTION_SWIMMING);
   			g_cPlayerInfo[playerid][e_cbAction] = PLAYER_ACTION_SWIMMING;
 		}
 		else if(!IsPlayerSwimming(playerid) && g_cPlayerInfo[playerid][e_cbAction] == PLAYER_ACTION_SWIMMING)
 		{
 		    CallLocalFunction("OnPlayerActionChange", "iii", playerid, g_cPlayerInfo[playerid][e_cbAction], PLAYER_ACTION_NONE);
 		    g_cPlayerInfo[playerid][e_cbAction] = PLAYER_ACTION_NONE;
		}
        else if(g_cPlayerInfo[playerid][e_cbAction] == PLAYER_ACTION_SHOOTING && (GetConnectedTime(playerid) - g_cPlayerTick[playerid][e_cbLastShot]) > 1200)
 		{
 		    CallLocalFunction("OnPlayerActionChange", "iii", playerid, g_cPlayerInfo[playerid][e_cbAction], PLAYER_ACTION_NONE);
 		    g_cPlayerInfo[playerid][e_cbAction] = PLAYER_ACTION_NONE;
		}
	}

	if(Callback_IsDefined("OnPlayerBurn") && (GetConnectedTime(playerid) - g_cPlayerTick[playerid][e_cbBurnTick]) >= 1500 && CB_GetPlayerFlag(playerid, e_cbBurning))
	{
		CB_DisableFlag(playerid, e_cbBurning);
 		CallLocalFunction("OnPlayerBurn", "ii", playerid, 0);
	}

	switch(GetPlayerState(playerid))
	{
		case PLAYER_STATE_ONFOOT:
		{
		    if(Callback_IsDefined("OnPlayerSprint"))
		    {
		        if(IsPlayerSprinting(playerid) && !CB_GetPlayerFlag(playerid, e_cbSprinting))
		        {
		            CB_EnableFlag(playerid, e_cbSprinting);
		            CallLocalFunction("OnPlayerSprint", "ii", playerid, 1);
		        }
		        else
		        {
		            CB_DisableFlag(playerid, e_cbSprinting);
		            CallLocalFunction("OnPlayerSprint", "ii", playerid, 0);
		        }
			}
		    if(Callback_IsDefined("OnPlayerTargetPlayer"))
			{
	 			if(!g_cPlayerInfo[playerid][e_cbTarget] && GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID)
				{
					g_cPlayerInfo[playerid][e_cbTarget] = INVALID_PLAYER_ID;
				}
				else if(GetPlayerTargetPlayer(playerid) != g_cPlayerInfo[playerid][e_cbTarget])
				{
	  				CallLocalFunction("OnPlayerTargetPlayer", "iii", playerid, g_cPlayerInfo[playerid][e_cbTarget], GetPlayerWeapon(playerid));
					g_cPlayerInfo[playerid][e_cbTarget] = GetPlayerTargetPlayer(playerid);
				}
			}
			if(Callback_IsDefined("OnPlayerReloadWeapon"))
			{
	  			if(!CB_GetPlayerFlag(playerid, e_cbReloading) && (WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) && GetPlayerWeaponState(playerid) == WEAPONSTATE_RELOADING)
	   			{
				   	CB_EnableFlag(playerid, e_cbReloading);
					CallLocalFunction("OnPlayerReloadWeapon", "iii", playerid, weaponid, GetPlayerAmmo(playerid));
				}
				else if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING)
				{
	   				CB_DisableFlag(playerid, e_cbReloading);
				}
			}
			if(Callback_IsDefined("OnPlayerUseVending") && IsPlayerNearVending(playerid))
			{
				if(GetPlayerAnimationIndex(playerid) == 1660 && (GetConnectedTime(playerid) - g_cPlayerTick[playerid][e_cbVendingTick]) > 3500)
				{
					CallLocalFunction("OnPlayerUseVending", "i", playerid);
					g_cPlayerTick[playerid][e_cbVendingTick] = GetConnectedTime(playerid);
				}
			}
			if(Callback_IsDefined("OnPlayerJump") && (GetConnectedTime(playerid) - g_cPlayerTick[playerid][e_cbLastJump] >= 1000) && IsPlayerJumping(playerid))
			{
   				CallLocalFunction("OnPlayerJump", "i", playerid);
			    g_cPlayerTick[playerid][e_cbLastJump] = GetConnectedTime(playerid);
			}
			
		}
		case PLAYER_STATE_DRIVER:
		{
		    if(Callback_IsDefined("OnPlayerCrashVehicle"))
		    {
		 		new
		   			vehicleid = GetPlayerVehicleID(playerid),
				   	panels,
				   	doors,
				   	lights,
				   	tires
		   		;

				GetVehicleHealth(vehicleid, health);

				if(g_cVehicleInfo[vehicleid][e_vHealth] > health && floatabs(g_cVehicleInfo[vehicleid][e_vHealth] - health) >= POTENTIAL_CRASH_DAMAGE && floatabs(g_cVehicleInfo[vehicleid][e_vSpeed] - Callback_GetSpeed(vehicleid)) >= 5.0)
				{
					GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

				    if(g_cVehicleInfo[vehicleid][e_vPanels] != panels || g_cVehicleInfo[vehicleid][e_vDoors] != doors || g_cVehicleInfo[vehicleid][e_vLights] != lights)
				    {
						CallLocalFunction("OnPlayerCrashVehicle", "iif", playerid, vehicleid, floatsub(g_cVehicleInfo[vehicleid][e_vHealth], health));
					}

					GetVehicleDamageStatus(vehicleid, g_cVehicleInfo[vehicleid][e_vPanels], g_cVehicleInfo[vehicleid][e_vDoors], g_cVehicleInfo[vehicleid][e_vLights], tires);
				}
				g_cVehicleInfo[vehicleid][e_vHealth] = health;
				g_cVehicleInfo[vehicleid][e_vSpeed] = Callback_GetSpeed(vehicleid);
			}
		}
	}

	if(Callback_IsDefined("OnPlayerFPSChange"))
	{
	    new
	        amount = g_cPlayerInfo[playerid][e_cbDrunkLevel] - level;
	
		if(GetPlayerDrunkLevel(playerid) < 100)
		{
    		SetPlayerDrunkLevel(playerid, 2000);
		}
		else if(g_cPlayerInfo[playerid][e_cbDrunkLevel] != level)
		{
		    if((0 <= amount <= 200) && g_cPlayerInfo[playerid][e_cbFrames] != amount)
		    {
		        if(g_cPlayerInfo[playerid][e_cbFrames] < 0)
		        {
		            g_cPlayerInfo[playerid][e_cbFrames] = 0;
		        }
				CallLocalFunction("OnPlayerFPSChange", "iii", playerid, g_cPlayerInfo[playerid][e_cbFrames], amount);
			}
			g_cPlayerInfo[playerid][e_cbFrames] = amount;
			g_cPlayerInfo[playerid][e_cbDrunkLevel] = level;
		}
	}
	if(Callback_IsDefined("OnPlayerPacketLoss"))
	{
		new
			Float:packet = NetStats_PacketLossPercent(playerid);

		if(floatcmp(packet, 0.0) != 0 && floatcmp(packet, g_cPlayerInfo[playerid][e_cbPacket]) != 0)
		{
		    CallLocalFunction("OnPlayerPacketLoss", "iff", playerid, g_cPlayerInfo[playerid][e_cbPacket], packet);
			g_cPlayerInfo[playerid][e_cbPacket] = packet;
		}
	}
	if(Callback_IsDefined("OnPlayerAnimationPlay"))
	{
	    new
     		animname[32],
       		animlib[32];
	
	    if(CB_GetPlayerFlag(playerid, e_cbAnimationSet) && g_cPlayerInfo[playerid][e_cbAnimation] != GetPlayerAnimationIndex(playerid))
	    {
	        CB_DisableFlag(playerid, e_cbAnimationSet);

	        GetAnimationName(g_cPlayerInfo[playerid][e_cbAnimation], animlib, sizeof(animlib), animname, sizeof(animname));
	        CallLocalFunction("OnPlayerAnimationPlay", "iss", playerid, animlib, animname);
	    }
	}
	if(Callback_IsDefined("OnPlayerUseCamera") || Callback_IsDefined("OnPlayerSprayAtVehicle"))
	{
	    new
	        ammo = GetPlayerAmmo(playerid);

	    if(g_cPlayerInfo[playerid][e_cbLastAmmo] != ammo)
	    {
	        if(GetPlayerWeapon(playerid) == WEAPON_CAMERA && g_cPlayerInfo[playerid][e_cbLastAmmo] > ammo)
	        {
	            CallLocalFunction("OnPlayerUseCamera", "i", playerid);
	        }
	        else if(GetPlayerWeapon(playerid) == WEAPON_SPRAYCAN && g_cPlayerInfo[playerid][e_cbLastAmmo] > ammo)
	        {
	            new id = CB_GetVehicleInFrontOfPlayer(playerid);

				if(id != INVALID_VEHICLE_ID && IsPlayerSprayingVehicle(playerid, id))
				{
					CallLocalFunction("OnPlayerSprayAtVehicle", "ii", playerid, id);
				}
			}
	        g_cPlayerInfo[playerid][e_cbLastAmmo] = ammo;
		}
	}
	if(Callback_IsDefined("OnPlayerAntiReload"))
	{
		if(g_cPlayerInfo[playerid][e_cbLastWeapon] != weaponid)
		{
		    new
				lastweapon = g_cPlayerInfo[playerid][e_cbLastWeapon];

    		if((WEAPON_COLT45 <= lastweapon <= WEAPON_TEC9) && lastweapon != WEAPON_SHOTGUN)
			{
				g_cPlayerInfo[playerid][e_cbShotCounter][lastweapon - WEAPON_COLT45] = 0;
			}
			g_cPlayerInfo[playerid][e_cbLastWeapon] = weaponid;
		}
		else
		{
			if((WEAPON_COLT45 <= weaponid <= WEAPON_TEC9) && weaponid != WEAPON_SHOTGUN && GetPlayerWeaponState(playerid) == WEAPONSTATE_RELOADING)
			{
				g_cPlayerInfo[playerid][e_cbShotCounter][weaponid - WEAPON_COLT45] = 0;
			}
		}
	}

	CB_EnableFlag(playerid, e_cbUpdate);

	g_cPlayerTick[playerid][e_cbLastUpdate] = GetConnectedTime(playerid);

	#if defined CB_OnPlayerUpdate
		return CB_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	new vehicleid = GetPlayerVehicleID(playerid);

	if(Callback_IsDefined("OnPlayerCrashVehicle"))
	{
	    if(newstate == PLAYER_STATE_DRIVER)
	    {
	        GetVehicleHealth(vehicleid, g_cVehicleInfo[vehicleid][e_vHealth]);
		}
	}
	if(Callback_IsDefined("OnPlayerJackVehicle"))
	{
		if(newstate == PLAYER_STATE_DRIVER)
		{
			g_cPlayerInfo[playerid][e_cbVehicle] = vehicleid;
		}
		else if(newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
		{
		    new
				driverid = Callback_GetDriver(g_cPlayerInfo[playerid][e_cbVehicle]);

			if(driverid != INVALID_PLAYER_ID)
			{
				CallLocalFunction("OnPlayerJackVehicle", "iii", driverid, playerid, g_cPlayerInfo[playerid][e_cbVehicle]);
			}

			g_cPlayerInfo[playerid][e_cbVehicle] = INVALID_VEHICLE_ID;
		}
	}

	if(newstate == PLAYER_STATE_WASTED)
	{
		g_cPlayerInfo[playerid][e_cbAction] = PLAYER_ACTION_NONE;
	}

	#if defined CB_OnPlayerStateChange
	    return CB_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
		return 1;
	#endif
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	new weaponid = GetPlayerWeapon(playerid);

	if(GetPlayerState(playerid) != PLAYER_STATE_SPECTATING && GetPlayerState(playerid) != PLAYER_STATE_SPAWNED && GetPlayerState(playerid) != PLAYER_STATE_WASTED)
	{
	    if(Callback_IsDefined("OnPlayerHoldingKey"))
		{
			if(!CB_GetPlayerFlag(playerid, e_cbHolding))
			{
			    CB_EnableFlag(playerid, e_cbHolding);

		    	g_cPlayerTick[playerid][e_cbTickCount] = GetConnectedTime(playerid);
				g_cPlayerInfo[playerid][e_cbHoldingTimer] = SetTimerEx("Callback_HoldTimer", 200, true, "ii", playerid, newkeys);
			}
		}
		if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
		{
			if(Callback_IsDefined("OnPlayerAiming") && (WEAPON_COLT45 <= weaponid <= WEAPON_CAMERA) && weaponid != WEAPON_BOMB)
			{
				if(!CB_GetPlayerFlag(playerid, e_cbAiming) && (newkeys & KEY_HANDBRAKE) & KEY_HANDBRAKE)
				{
	                CB_EnableFlag(playerid, e_cbAiming);
		    		CallLocalFunction("OnPlayerAiming", "iii", playerid, weaponid, 1);
				}
				else if((newkeys & KEY_HANDBRAKE) != KEY_HANDBRAKE && (oldkeys & KEY_HANDBRAKE) == KEY_HANDBRAKE)
				{
				    CB_DisableFlag(playerid, e_cbAiming);
				    CallLocalFunction("OnPlayerAiming", "iii", playerid, weaponid, 0);
				}
			}
		}
	}

	#if defined CB_OnPlayerKeyStateChange
	    return CB_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(Callback_IsDefined("OnPlayerHideCursor"))
	{
		if(clickedid == Text:INVALID_TEXT_DRAW && CB_GetPlayerFlag(playerid, e_cbSelection))
		{
	    	CB_DisableFlag(playerid, e_cbSelection);
			CallLocalFunction("OnPlayerHideCursor", "ii", playerid, g_cPlayerInfo[playerid][e_cbHoverColor]);
		}
	}
	
	#if defined CB_OnPlayerClickTextDraw
		return CB_OnPlayerClickTextDraw(playerid, clickedid);
	#else
		return 0;
	#endif
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if(Callback_IsDefined("OnPlayerFall") && weaponid == WEAPON_COLLISION && issuerid == INVALID_PLAYER_ID && amount >= 4.95)
	{
		CallLocalFunction("OnPlayerFall", "if", playerid, amount);
	}
    if(Callback_IsDefined("OnPlayerRamPlayer") && weaponid == WEAPON_VEHICLE && issuerid != INVALID_PLAYER_ID)
	{
	    CallLocalFunction("OnPlayerRamPlayer", "iiif", playerid, issuerid, GetPlayerVehicleID(issuerid), amount);
	}
	if(Callback_IsDefined("OnPlayerBurn") && weaponid == WEAPON_FLAMETHROWER)
	{
	    if(!CB_GetPlayerFlag(playerid, e_cbBurning))
	    {
	        CB_EnableFlag(playerid, e_cbBurning);
        	CallLocalFunction("OnPlayerBurn", "ii", playerid, 1);
		}
		g_cPlayerTick[playerid][e_cbBurnTick] = GetConnectedTime(playerid);
	}

	#if defined CB_OnPlayerTakeDamage
		return CB_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}

forward Callback_CheckAnimation(playerid);
public Callback_CheckAnimation(playerid)
{
    g_cPlayerInfo[playerid][e_cbAnimation] = GetPlayerAnimationIndex(playerid);

	CB_EnableFlag(playerid, e_cbAnimationSet);
}

forward Callback_HoldTimer(playerid, key);
public Callback_HoldTimer(playerid, key)
{
    new keys, ud, lr;

    GetPlayerKeys(playerid, keys, ud, lr);

    if(keys & key)
    {
        if(!CB_GetPlayerFlag(playerid, e_cbPressed))
        {
            CB_EnableFlag(playerid, e_cbPressed);
        }
        CallLocalFunction("OnPlayerHoldingKey", "ii", playerid, key);
    }
    else
    {
        if(Callback_IsDefined("OnPlayerReleaseKey") && CB_GetPlayerFlag(playerid, e_cbPressed))
        {
            CallLocalFunction("OnPlayerReleaseKey", "iii", playerid, key, (GetConnectedTime(playerid) - g_cPlayerTick[playerid][e_cbTickCount]));
        }

       	KillTimer(g_cPlayerInfo[playerid][e_cbHoldingTimer]);

    	CB_DisableFlag(playerid, e_cbPressed);
    	CB_DisableFlag(playerid, e_cbHolding);
    }
}

forward Callback_TabCheck();
public Callback_TabCheck()
{
	for(new i = 0, l = GetMaxPlayers(); i != l; i ++)
	{
	    switch(GetPlayerState(i))
	    {
	        case PLAYER_STATE_ONFOOT, PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
	        {
				if(CB_GetPlayerFlag(i, e_cbUpdate))
				{
		    		if(Callback_IsDefined("OnPlayerResume") && CB_GetPlayerFlag(i, e_cbPaused))
		    		{
						CallLocalFunction("OnPlayerResume", "ii", i, GetConnectedTime(i) - g_cPlayerTick[i][e_cbTickCount]);
					}
		            CB_DisableFlag(i, e_cbUpdate);
		            CB_DisableFlag(i, e_cbPaused);
				}
				else if(Callback_IsDefined("OnPlayerPause"))
				{
					if(!CB_GetPlayerFlag(i, e_cbPaused) && (GetConnectedTime(i) - g_cPlayerTick[i][e_cbLastUpdate]) >= 4000)
					{
				    	g_cPlayerTick[i][e_cbTickCount] = GetConnectedTime(i);

						CB_EnableFlag(i, e_cbPaused);

						CallLocalFunction("OnPlayerPause", "i", i);
					}
				}
			}
		}
	}
}

#if defined _ALS_ApplyAnimation
	#undef ApplyAnimation
#else
	#define _ALS_ApplyAnimation
#endif

#if defined _ALS_SelectTextDraw
	#undef SelectTextDraw
#else
	#define _ALS_SelectTextDraw
#endif

#if defined _ALS_CancelSelectTextDraw
	#undef CancelSelectTextDraw
#else
	#define _ALS_CancelSelectTextDraw
#endif

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#if defined _ALS_OnPlayerExitVehicle
	#undef OnPlayerExitVehicle
#else
	#define _ALS_OnPlayerExitVehicle
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif

#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif

#if defined _ALS_OnPlayerClickTextDraw
	#undef OnPlayerClickTextDraw
#else
	#define _ALS_OnPlayerClickTextDraw
#endif

#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif

#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif

#define ApplyAnimation          CB_ApplyAnimation
#define SelectTextDraw          CB_SelectTextDraw
#define CancelSelectTextDraw    CB_CancelSelectTextDraw
#define OnFilterScriptInit  	CB_OnFilterScriptInit
#define OnGameModeInit 			CB_OnGameModeInit
#define OnPlayerExitVehicle     CB_OnPlayerExitVehicle
#define OnPlayerDisconnect      CB_OnPlayerDisconnect
#define OnPlayerUpdate 			CB_OnPlayerUpdate
#define OnPlayerStateChange  	CB_OnPlayerStateChange
#define OnPlayerKeyStateChange  CB_OnPlayerKeyStateChange
#define OnPlayerWeaponShot      CB_OnPlayerWeaponShot
#define OnPlayerClickTextDraw   CB_OnPlayerClickTextDraw
#define OnPlayerTakeDamage      CB_OnPlayerTakeDamage
#define OnPlayerDeath           CB_OnPlayerDeath

#if defined CB_OnPlayerUpdate
	forward CB_OnPlayerUpdate(playerid);
#endif

#if defined CB_OnFilterScriptInit
	forward CB_OnFilterScriptInit();
#endif

#if defined CB_OnGameModeInit
	forward CB_OnGameModeInit();
#endif

#if defined CB_OnPlayerExitVehicle
	forward CB_OnPlayerExitVehicle(playerid, vehicleid);
#endif

#if defined CB_OnPlayerDisconnect
	forward CB_OnPlayerDisconnect(playerid, reason);
#endif

#if defined CB_OnPlayerStateChange
	forward CB_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

#if defined CB_OnPlayerKeyStateChange
	forward CB_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

#if defined CB_OnPlayerWeaponShot
	forward CB_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

#if defined CB_OnPlayerClickTextDraw
	forward CB_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif

#if defined CB_OnPlayerTakeDamage
	forward CB_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

#if defined CB_OnPlayerDeath
	forward CB_OnPlayerDeath(playerid, killerid, reason);
#endif
 
