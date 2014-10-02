/*
	New SA-MP callbacks by Emmet
	Created on Sunday, January 26, 2014 at 11:58 AM.
	
	Total callbacks (since October 2, 2014): 20
*/

#if !defined POTENTIAL_CRASH_DAMAGE
	#define POTENTIAL_CRASH_DAMAGE (55.0)
#endif

#if !defined POTENTIAL_SPEED_DROP
	#define POTENTIAL_SPEED_DROP (5.0)
#endif

// Player variables
enum e_cbPlayerData {
	e_cbPlayerFlags:e_cbFlags,
	e_cbTime,
	e_cbTimer,
	e_cbPresses,
	e_cbLast,
 	e_cbVending,
  	e_cbFrames,
  	e_cbDrunk,
  	e_cbTarget,
  	e_cbAnimation,
  	e_cbSelectColor,
  	e_cbLastVehicle,
  	e_cbShotCounter[4],
  	e_cbLastWeapon,
  	e_cbLastBurn,
  	e_cbLastAmmo,
	Float:e_cbHealth,
	Float:e_cbPacket,
	Float:e_cbPosition[3]
};

// Vehicle variables
enum e_cbCarData {
	e_cbPanelData,
	e_cbDoorData,
	e_cbLightData,
	Float:e_cbCarHealth,
	Float:e_cbCarSpeed
};

// Player flags
enum e_cbPlayerFlags (<<= 1) {
	e_cbInitialized = 1,
	e_cbUpdate,
	e_cbPaused,
	e_cbHolding,
	e_cbSelection,
	e_cbSetAnim,
	e_cbReloading,
	e_cbBurning,
	e_cbAiming
};

// Callback flags
enum e_cbCallbackFlags (<<= 1) {
 	CB_OnPlayerPause = 1,
	CB_OnPlayerResume,
	CB_OnPlayerHoldingKey,
	CB_OnPlayerReleaseKey,
	CB_OnPlayerFall,
	CB_OnPlayerPacketLoss,
	CB_OnPlayerUseVending,
	CB_OnPlayerCrashVehicle,
	CB_OnPlayerFPSChange,
	CB_OnPlayerCarJack,
	CB_OnPlayerWeaponEmpty,
	CB_OnPlayerFriendlyFire,
	CB_OnPlayerTargetPlayer,
	CB_OnPlayerDisableCursor,
	CB_OnPlayerAntiReload,
	CB_OnPlayerAnimationFinish,
	CB_OnPlayerReloadWeapon,
	CB_OnPlayerBurning,
	CB_OnPlayerAiming,
	CB_OnPlayerTakePicture
};

static
    e_cbCallbackFlags: g_bCallbacks,
	g_cbPlayers[MAX_PLAYERS][e_cbPlayerData],
	g_cbVehicles[MAX_VEHICLES][e_cbCarData];

static const
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
	},
	g_aMaximumClip[11] = {34, 17, 7, 1, 4, 7, 100, 30, 30, 50, 100},
	g_aClipIndex[11] = {0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 2};

// Called when a player pauses their game.
forward OnPlayerPause(playerid);

// Called when a player returns to the game.
forward OnPlayerResume(playerid, time);

// Called when a player starts holding a key.
forward OnPlayerHoldingKey(playerid, keys);

// Called when a player releases a key.
forward OnPlayerReleaseKey(playerid, keys, time);

// Called when a player falls from a tall height.
forward OnPlayerFall(playerid, Float:height, Float:damage);

// Called when a player loses packets.
forward OnPlayerPacketLoss(playerid, Float:oldloss, Float:newloss);

// Called when a player crashes their vehicle.
forward OnPlayerCrashVehicle(playerid, vehicleid, Float:damage);

// Called when a player uses a vending machine.
forward OnPlayerUseVending(playerid);

// Called when a player's FPS change.
forward OnPlayerFPSChange(playerid, oldfps, newfps);

// Called when a player jacks another player's vehicle.
forward OnPlayerCarJack(playerid, targetid, vehicleid);

// Called when a player completely uses up all ammo in their weapon.
forward OnPlayerWeaponEmpty(playerid, weaponid);

// Called when a player shoots another player in their team.
forward OnPlayerFriendlyFire(playerid, targetid, weaponid);

// Called when a player aims at another player.
forward OnPlayerTargetPlayer(playerid, targetid, weaponid);

// Called when a player explicitly disables textdraw selection mode.
forward OnPlayerDisableCursor(playerid, hovercolor);

// Called when a player shoots more ammo than their weapon's clip can hold.
forward OnPlayerAntiReload(playerid, weaponid);

// Called when an animation is finished.
forward OnPlayerAnimationFinish(playerid, animlib[], animname[]);

// Called when a player is reloading their weapon.
forward OnPlayerReloadWeapon(playerid, weaponid, ammo);

// Called when a player is burning from fire.
forward OnPlayerBurning(playerid, status);

// Called when a player aims a weapon.
forward OnPlayerAiming(playerid, weaponid, status);

// Called when a player takes a picture with their camera.
forward OnPlayerTakePicture(playerid);

stock IsPlayerNearVending(playerid)
{
    for (new i = 0; i < sizeof(g_aVendingLocations); i ++)
	{
        if (GetPlayerDistanceFromPoint(playerid, g_aVendingLocations[i][0], g_aVendingLocations[i][1], 

g_aVendingLocations[i][2]) > 3.0)
            continue;

		else
		    return i;
    }
	return 0;
}

stock IsPlayerPaused(playerid)
{
	return g_cbPlayers[playerid][e_cbFlags] & e_cbPaused;
}

stock GetPlayerFPS(playerid)
{
	return g_cbPlayers[playerid][e_cbFrames];
}

stock GetPlayerPausedTime(playerid)
{
	return g_cbPlayers[playerid][e_cbTime];
}

stock CB_ApplyAnimation(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync 

= 0)
{
	if (ApplyAnimation(playerid, animlib, animname, fDelta, loop, lockx, locky, freeze, time, forcesync))
	{
	 	SetTimerEx("CB_UpdateAnim", 100, false, "d", playerid);
	 	return 1;
	}
	return 0;
}

stock CB_SelectTextDraw(playerid, hovercolor)
{
	g_cbPlayers[playerid][e_cbSelectColor] = hovercolor;
	g_cbPlayers[playerid][e_cbFlags] |= e_cbSelection;

	return SelectTextDraw(playerid, hovercolor);
}

stock CB_CancelSelectTextDraw(playerid)
{
    g_cbPlayers[playerid][e_cbSelectColor] = 0;
	g_cbPlayers[playerid][e_cbFlags] &= ~e_cbSelection;

	return CancelSelectTextDraw(playerid);
}

stock static CB_GetVehicleDriver(vehicleid)
{
	for (new i = 0; i < MAX_PLAYERS; i ++)
	{
		if (GetPlayerState(i) == PLAYER_STATE_DRIVER && IsPlayerInVehicle(i, vehicleid))
		{
		    return i;
		}
	}
	return INVALID_PLAYER_ID;
}

stock static Float:CB_GetVehicleSpeed(vehicleid)
{
	new
	    Float:fX,
	    Float:fY,
	    Float:fZ;

	GetVehicleVelocity(vehicleid, fX, fY, fZ);
	return (floatsqroot((fX * fX) + (fY * fY) + (fZ * fZ)) * 100);
}

stock static CB_Initialize()
{
 	enum e_cbListInfo
	{
	    e_cbName[28 char],
	    e_cbCallbackFlags:e_cbFlag
	};
	static const g_aCallbackList[][e_cbListInfo] = {
	    {!"OnPlayerPause", 		  	 CB_OnPlayerPause},
		{!"OnPlayerResume", 	  	 CB_OnPlayerResume},
		{!"OnPlayerHoldingKey",   	 CB_OnPlayerHoldingKey},
		{!"OnPlayerReleaseKey",   	 CB_OnPlayerReleaseKey},
	    {!"OnPlayerFall", 		  	 CB_OnPlayerFall},
		{!"OnPlayerPacketLoss",   	 CB_OnPlayerPacketLoss},
		{!"OnPlayerUseVending",   	 CB_OnPlayerUseVending},
		{!"OnPlayerCrashVehicle", 	 CB_OnPlayerCrashVehicle},
	    {!"OnPlayerFPSChange", 	  	 CB_OnPlayerFPSChange},
		{!"OnPlayerCarJack",      	 CB_OnPlayerCarJack},
		{!"OnPlayerWeaponEmpty",   	 CB_OnPlayerWeaponEmpty},
		{!"OnPlayerFriendlyFire",    CB_OnPlayerFriendlyFire},
	    {!"OnPlayerTargetPlayer",    CB_OnPlayerTargetPlayer},
		{!"OnPlayerDisableCursor",   CB_OnPlayerDisableCursor},
		{!"OnPlayerAntiReload",      CB_OnPlayerAntiReload},
		{!"OnPlayerAnimationFinish", CB_OnPlayerAnimationFinish},
		{!"OnPlayerReloadWeapon",    CB_OnPlayerReloadWeapon},
		{!"OnPlayerBurning",         CB_OnPlayerBurning},
		{!"OnPlayerAiming",          CB_OnPlayerAiming},
		{!"OnPlayerTakePicture",     CB_OnPlayerTakePicture}
	};
	for (new i = 0; i != sizeof(g_aCallbackList); i ++)
	{
		if (funcidx(g_aCallbackList[i][e_cbName]) == -1)
		    continue;

		g_bCallbacks |= g_aCallbackList[i][e_cbFlag];
	}
    if (g_bCallbacks & CB_OnPlayerPause || g_bCallbacks & CB_OnPlayerResume)
    {
		SetTimer("CB_TabTimer", 600, true);
	}
}

public OnPlayerDisconnect(playerid, reason)
{
	if (g_cbPlayers[playerid][e_cbFlags] & e_cbHolding)
	{
	    KillTimer(g_cbPlayers[playerid][e_cbTimer]);
	}
	g_cbPlayers[playerid][e_cbFlags] = e_cbPlayerFlags:0;
    g_cbPlayers[playerid][e_cbTime] = 0;
	g_cbPlayers[playerid][e_cbPresses] = 0;
	g_cbPlayers[playerid][e_cbLast] = 0;
 	g_cbPlayers[playerid][e_cbVending] = 0;
  	g_cbPlayers[playerid][e_cbFrames] = 0;
  	g_cbPlayers[playerid][e_cbDrunk] = 0;
  	g_cbPlayers[playerid][e_cbTarget] = 0;
  	g_cbPlayers[playerid][e_cbAnimation] = 0;
  	g_cbPlayers[playerid][e_cbSelectColor] = 0;
 	g_cbPlayers[playerid][e_cbHealth] = 100.0;
 	g_cbPlayers[playerid][e_cbPacket] = 0.0;
 	g_cbPlayers[playerid][e_cbLastWeapon] = 0;
	g_cbPlayers[playerid][e_cbLastAmmo] = 0;

	for (new i = 0; i < 4; i ++)
	{
		g_cbPlayers[playerid][e_cbShotCounter][i] = 0;
	}
 	#if defined CB_OnPlayerDisconnect
 	    return CB_OnPlayerDisconnect(playerid, reason);
	#else
	    return 1;
	#endif
}

public OnFilterScriptInit()
{
    CB_Initialize();

	#if defined CB_OnFilterScriptInit
	    return CB_OnFilterScriptInit();
	#else
		return 1;
	#endif
}

public OnGameModeInit()
{
 	CB_Initialize();

	#if defined CB_OnGameModeInit
	    return CB_OnGameModeInit();
	#else
		return 1;
	#endif
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (g_bCallbacks & CB_OnPlayerFriendlyFire)
	{
	    new teamid = GetPlayerTeam(playerid);

		if (teamid != NO_TEAM && hittype == BULLET_HIT_TYPE_PLAYER && GetPlayerTeam(hitid) == teamid)
		{
			CallLocalFunction("OnPlayerFriendlyFire", "ddd", playerid, hitid, weaponid);
		}
	}
	if (g_bCallbacks & CB_OnPlayerWeaponEmpty)
	{
		if (GetPlayerWeaponState(playerid) == WEAPONSTATE_LAST_BULLET && GetPlayerAmmo(playerid) == 1)
		{
			CallLocalFunction("OnPlayerWeaponEmpty", "dd", playerid, weaponid);
		}
	}
	if (g_bCallbacks & CB_OnPlayerAntiReload && (22 <= weaponid <= 32) && weaponid != 25)
	{
		new clip = g_aMaximumClip[weaponid - 22];
		new slot = g_aClipIndex[weaponid - 22];

		if (++ g_cbPlayers[playerid][e_cbShotCounter][slot] > clip && (g_cbPlayers[playerid]

[e_cbShotCounter][slot] - clip) >= 5)
		{
			CallLocalFunction("OnPlayerAntiReload", "dd", playerid, weaponid);
            g_cbPlayers[playerid][e_cbShotCounter][slot] = 0;
		}
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
		level = 0,
		Float:health;

	if (g_bCallbacks & CB_OnPlayerBurning)
	{
		if (g_cbPlayers[playerid][e_cbFlags] & e_cbBurning)
		{
			if ((GetTickCount() - g_cbPlayers[playerid][e_cbLastBurn]) >= 1500)
			{
	            g_cbPlayers[playerid][e_cbFlags] &= ~e_cbBurning;
			    CallLocalFunction("OnPlayerBurning", "dd", playerid, 0);
			}
		}
	}
    if (!(g_cbPlayers[playerid][e_cbFlags] & e_cbInitialized))
	{
 		g_cbPlayers[playerid][e_cbFlags] |= e_cbInitialized;

		GetPlayerHealth(playerid, g_cbPlayers[playerid][e_cbHealth]);
  		GetPlayerPos(playerid, g_cbPlayers[playerid][e_cbPosition][0], g_cbPlayers[playerid]

[e_cbPosition][1], g_cbPlayers[playerid][e_cbPosition][2]);
	}
	switch (GetPlayerState(playerid))
	{
	    case PLAYER_STATE_ONFOOT:
	    {
		  	if (g_bCallbacks & CB_OnPlayerTargetPlayer)
			{
		    	if (!g_cbPlayers[playerid][e_cbTarget] && GetPlayerTargetPlayer(playerid) == 

INVALID_PLAYER_ID)
				{
					g_cbPlayers[playerid][e_cbTarget] = INVALID_PLAYER_ID;
				}
	  			else if (GetPlayerTargetPlayer(playerid) != g_cbPlayers[playerid][e_cbTarget])
	  			{
	  		    	CallLocalFunction("OnPlayerTargetPlayer", "ddd", playerid, g_cbPlayers[playerid]

[e_cbTarget], GetPlayerWeapon(playerid));
	  		    	g_cbPlayers[playerid][e_cbTarget] = GetPlayerTargetPlayer(playerid);
				}
			}
			if (g_bCallbacks & CB_OnPlayerReloadWeapon)
			{
			    if (!(g_cbPlayers[playerid][e_cbFlags] & e_cbReloading) && (22 <= weaponid <= 34) && 

GetPlayerWeaponState(playerid) == WEAPONSTATE_RELOADING)
	    		{
      				g_cbPlayers[playerid][e_cbFlags] |= e_cbReloading;
   					CallLocalFunction("OnPlayerReloadWeapon", "ddd", playerid, weaponid, 

GetPlayerAmmo(playerid));
				}
				else if (GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING)
				{
				    g_cbPlayers[playerid][e_cbFlags] &= ~e_cbReloading;
				}
			}
			if (g_bCallbacks & CB_OnPlayerUseVending)
			{
				if (IsPlayerNearVending(playerid) && GetPlayerAnimationIndex(playerid) == 1660 && 

(GetTickCount() - g_cbPlayers[playerid][e_cbVending]) > 3500)
				{
					CallLocalFunction("OnPlayerUseVending", "d", playerid);
					g_cbPlayers[playerid][e_cbVending] = GetTickCount();
				}
			}
			if (g_bCallbacks & CB_OnPlayerFall)
			{
		        if (!(1130 <= GetPlayerAnimationIndex(playerid) <= 1132))
				{
					new Float:fZ, Float:fHP;

					GetPlayerPos(playerid, fZ, fZ, fZ);
					GetPlayerHealth(playerid, fHP);

					if (g_cbPlayers[playerid][e_cbHealth] > fHP && g_cbPlayers[playerid]

[e_cbPosition][2] > fZ)
					{
						CallLocalFunction("OnPlayerFall", "dff", playerid, floatsub

(g_cbPlayers[playerid][e_cbPosition][2], fZ), floatsub(g_cbPlayers[playerid][e_cbHealth], fHP));
					}
					GetPlayerHealth(playerid, g_cbPlayers[playerid][e_cbHealth]);
	   				GetPlayerPos(playerid, g_cbPlayers[playerid][e_cbPosition][0], 

g_cbPlayers[playerid][e_cbPosition][1], g_cbPlayers[playerid][e_cbPosition][2]);
				}
			}
		}
		case PLAYER_STATE_DRIVER:
		{
		    new
		        vehicleid = GetPlayerVehicleID(playerid);

			GetVehicleHealth(vehicleid, health);
			
			if (g_cbVehicles[vehicleid][e_cbCarHealth] > health && floatabs(g_cbVehicles[vehicleid]

[e_cbCarHealth] - health) >= POTENTIAL_CRASH_DAMAGE && floatabs(g_cbVehicles[vehicleid][e_cbCarSpeed] - 

CB_GetVehicleSpeed(vehicleid)) >= POTENTIAL_SPEED_DROP)
			{
			    new panels, doors, lights, tires;

				GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

			    if (g_cbVehicles[vehicleid][e_cbPanelData] != panels || g_cbVehicles[vehicleid]

[e_cbDoorData] != doors || g_cbVehicles[vehicleid][e_cbLightData] != lights)
			    {
					CallLocalFunction("OnPlayerCrashVehicle", "ddf", playerid, vehicleid, 

floatsub(g_cbVehicles[vehicleid][e_cbCarHealth], health));
				}
				g_cbVehicles[vehicleid][e_cbPanelData] = panels;
				g_cbVehicles[vehicleid][e_cbDoorData] = doors;
				g_cbVehicles[vehicleid][e_cbLightData] = lights;
			}
			g_cbVehicles[vehicleid][e_cbCarHealth] = health;
			g_cbVehicles[vehicleid][e_cbCarSpeed] = CB_GetVehicleSpeed(vehicleid);
		}
	}
	if (g_bCallbacks & CB_OnPlayerFPSChange)
	{
		if ((level = GetPlayerDrunkLevel(playerid)) < 100)
		{
    		SetPlayerDrunkLevel(playerid, 2000);
		}
		else if (g_cbPlayers[playerid][e_cbDrunk] != level)
	    {
	        new amount = g_cbPlayers[playerid][e_cbDrunk] - level;

		    if ((0 <= amount <= 200) && g_cbPlayers[playerid][e_cbFrames] != amount)
		    {
		        if (g_cbPlayers[playerid][e_cbFrames] < 0)
		        {
		            g_cbPlayers[playerid][e_cbFrames] = 0;
		        }
				CallLocalFunction("OnPlayerFPSChange", "ddd", playerid, g_cbPlayers[playerid]

[e_cbFrames], amount);
			}
			g_cbPlayers[playerid][e_cbFrames] = amount;
			g_cbPlayers[playerid][e_cbDrunk] = level;
		}
	}
	if (g_bCallbacks & CB_OnPlayerPacketLoss)
	{
		new
			Float:packet = NetStats_PacketLossPercent(playerid);

		if (floatcmp(packet, 0.0) != 0 && floatcmp(packet, g_cbPlayers[playerid][e_cbPacket]) != 0)
		{
		    CallLocalFunction("OnPlayerPacketLoss", "dff", playerid, g_cbPlayers[playerid][e_cbPacket], 

packet);
			g_cbPlayers[playerid][e_cbPacket] = packet;
		}
	}
	if (g_bCallbacks & CB_OnPlayerAnimationFinish)
	{
	    if (g_cbPlayers[playerid][e_cbFlags] & e_cbSetAnim && g_cbPlayers[playerid][e_cbAnimation] != 

GetPlayerAnimationIndex(playerid))
	    {
     		new
	            animname[32],
	            animlib[32];

	        g_cbPlayers[playerid][e_cbFlags] &= ~e_cbSetAnim;

	        GetAnimationName(g_cbPlayers[playerid][e_cbAnimation], animlib, sizeof(animlib), animname, sizeof

(animname));
	        CallLocalFunction("OnPlayerAnimationFinish", "dss", playerid, animlib, animname);
	    }
	}
	if (g_bCallbacks & CB_OnPlayerAntiReload || g_bCallbacks & CB_OnPlayerTakePicture)
	{
		if (g_cbPlayers[playerid][e_cbLastWeapon] != weaponid)
		{
		    new
				lastgun = g_cbPlayers[playerid][e_cbLastWeapon];

    		if ((22 <= lastgun <= 32) && lastgun != 25)
			{
				g_cbPlayers[playerid][e_cbShotCounter][g_aClipIndex[lastgun - 22]] = 0;
			}
			g_cbPlayers[playerid][e_cbLastWeapon] = weaponid;
			g_cbPlayers[playerid][e_cbLastAmmo] = GetPlayerAmmo(playerid);
		}
		else
		{
		    if (g_bCallbacks & CB_OnPlayerTakePicture && GetPlayerWeapon(playerid) == 43 && GetPlayerAmmo

(playerid) < g_cbPlayers[playerid][e_cbLastAmmo])
			{
			    CallLocalFunction("OnPlayerTakePicture", "d", playerid);
			    g_cbPlayers[playerid][e_cbLastAmmo] = GetPlayerAmmo(playerid);
		    }
			if ((22 <= weaponid <= 32) && weaponid != 25 && GetPlayerWeaponState(playerid) == 

WEAPONSTATE_RELOADING)
			{
				g_cbPlayers[playerid][e_cbShotCounter][g_aClipIndex[weaponid - 22]] = 0;
			}
		}
	}
	g_cbPlayers[playerid][e_cbFlags] |= e_cbUpdate;
	g_cbPlayers[playerid][e_cbLast] = GetTickCount();

	#if defined CB_OnPlayerUpdate
		return CB_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	new vehicleid = GetPlayerVehicleID(playerid);

	if (g_bCallbacks & CB_OnPlayerCrashVehicle)
	{
	    if (newstate == PLAYER_STATE_DRIVER)
	    {
	        GetVehicleHealth(vehicleid, g_cbVehicles[vehicleid][e_cbCarHealth]);
		}
	}
	if (g_bCallbacks & CB_OnPlayerCarJack)
	{
		if (newstate == PLAYER_STATE_DRIVER)
		{
			g_cbPlayers[playerid][e_cbLastVehicle] = vehicleid;
		}
		else if (newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
		{
			new
				driverid = CB_GetVehicleDriver(g_cbPlayers[playerid][e_cbLastVehicle]);

			if (driverid != INVALID_PLAYER_ID)
			{
			    CallLocalFunction("OnPlayerCarJack", "ddd", driverid, playerid, g_cbPlayers[playerid]

[e_cbLastVehicle]);
			}
			g_cbPlayers[playerid][e_cbLastVehicle] = INVALID_VEHICLE_ID;
		}
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

    if (g_bCallbacks & CB_OnPlayerHoldingKey)
	{
		if (!(g_cbPlayers[playerid][e_cbFlags] & e_cbHolding))
		{
		    g_cbPlayers[playerid][e_cbFlags] |= e_cbHolding;
	    	g_cbPlayers[playerid][e_cbTime] = GetTickCount();
			g_cbPlayers[playerid][e_cbTimer] = SetTimerEx("CB_HoldingTimer", 200, true, "dd", 

playerid, newkeys);
		}
	}
	if (g_bCallbacks & CB_OnPlayerAiming)
	{
		if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && (22 <= weaponid <= 43) && weaponid != 40)
		{
			if (!(g_cbPlayers[playerid][e_cbFlags] & e_cbAiming) && (newkeys & KEY_HANDBRAKE) & 

KEY_HANDBRAKE)
			{
                g_cbPlayers[playerid][e_cbFlags] |= e_cbAiming;
	    		CallLocalFunction("OnPlayerAiming", "ddd", playerid, weaponid, 1);
			}
			else if ((newkeys & KEY_HANDBRAKE) != KEY_HANDBRAKE && (oldkeys & KEY_HANDBRAKE) == 

KEY_HANDBRAKE)
			{
			    g_cbPlayers[playerid][e_cbFlags] &= ~e_cbAiming;
			    CallLocalFunction("OnPlayerAiming", "ddd", playerid, weaponid, 0);
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
	if (g_bCallbacks & CB_OnPlayerDisableCursor)
	{
		if (clickedid == Text:INVALID_TEXT_DRAW && g_cbPlayers[playerid][e_cbFlags] & e_cbSelection)
		{
	    	g_cbPlayers[playerid][e_cbFlags] &= ~e_cbSelection;
			CallLocalFunction("OnPlayerDisableCursor", "dd", playerid, g_cbPlayers[playerid]

[e_cbSelectColor]);
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
	if (g_bCallbacks & CB_OnPlayerBurning && weaponid == 37)
	{
	    if (!(g_cbPlayers[playerid][e_cbFlags] & e_cbBurning))
	    {
	        g_cbPlayers[playerid][e_cbFlags] |= e_cbBurning;
        	CallLocalFunction("OnPlayerBurning", "dd", playerid, 1);
		}
		g_cbPlayers[playerid][e_cbLastBurn] = GetTickCount();
	}
	return 1;
}

forward CB_UpdateAnim(playerid);
public CB_UpdateAnim(playerid)
{
    g_cbPlayers[playerid][e_cbAnimation] = GetPlayerAnimationIndex(playerid);
    g_cbPlayers[playerid][e_cbFlags] |= e_cbSetAnim;
	return 1;
}

forward CB_HoldingTimer(playerid, key);
public CB_HoldingTimer(playerid, key)
{
	new keys, ud, lr;

	GetPlayerKeys(playerid, keys, ud, lr);

	if (keys & key)
	{
	    g_cbPlayers[playerid][e_cbPresses]++;
	    CallLocalFunction("OnPlayerHoldingKey", "dd", playerid, key);
	}
	else
	{
	    if (g_bCallbacks & CB_OnPlayerReleaseKey && g_cbPlayers[playerid][e_cbPresses] > 0)
	    {
	    	CallLocalFunction("OnPlayerReleaseKey", "ddd", playerid, keys, (GetTickCount() - g_cbPlayers

[playerid][e_cbTime]));
		}
		KillTimer(g_cbPlayers[playerid][e_cbTimer]);

        g_cbPlayers[playerid][e_cbPresses] = 0;
        g_cbPlayers[playerid][e_cbFlags] &= ~e_cbHolding;
	}
}

forward CB_TabTimer();
public CB_TabTimer()
{
	for (new i = 0, l = GetMaxPlayers(); i != l; i ++)
	{
	    switch (GetPlayerState(i))
	    {
	        case PLAYER_STATE_ONFOOT, PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
	        {
				if (g_cbPlayers[i][e_cbFlags] & e_cbUpdate)
				{
		    		if (g_bCallbacks & CB_OnPlayerResume && g_cbPlayers[i][e_cbFlags] & e_cbPaused)
		    		{
						CallLocalFunction("OnPlayerResume", "dd", i, GetTickCount() - 

g_cbPlayers[i][e_cbTime]);
					}
		            g_cbPlayers[i][e_cbFlags] &= ~e_cbUpdate;
		            g_cbPlayers[i][e_cbFlags] &= ~e_cbPaused;
				}
				else if (g_bCallbacks & CB_OnPlayerPause)
				{
					if (!(g_cbPlayers[i][e_cbFlags] & e_cbPaused) && (GetTickCount() - 

g_cbPlayers[i][e_cbLast]) >= 4000)
					{
				    	g_cbPlayers[i][e_cbTime] = GetTickCount();
				    	g_cbPlayers[i][e_cbFlags] |= e_cbPaused;

						CallLocalFunction("OnPlayerPause", "d", i);
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

#define ApplyAnimation          CB_ApplyAnimation
#define SelectTextDraw          CB_SelectTextDraw
#define CancelSelectTextDraw    CB_CancelSelectTextDraw
#define OnFilterScriptInit  	CB_OnFilterScriptInit
#define OnGameModeInit 			CB_OnGameModeInit
#define OnPlayerDisconnect      CB_OnPlayerDisconnect
#define OnPlayerUpdate 			CB_OnPlayerUpdate
#define OnPlayerStateChange  	CB_OnPlayerStateChange
#define OnPlayerKeyStateChange  CB_OnPlayerKeyStateChange
#define OnPlayerWeaponShot      CB_OnPlayerWeaponShot
#define OnPlayerClickTextDraw   CB_OnPlayerClickTextDraw
#define OnPlayerTakeDamage      CB_OnPlayerTakeDamage

#if defined CB_OnPlayerUpdate
	forward CB_OnPlayerUpdate(playerid);
#endif

#if defined CB_OnFilterScriptInit
	forward CB_OnFilterScriptInit();
#endif

#if defined CB_OnGameModeInit
	forward CB_OnGameModeInit();
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
