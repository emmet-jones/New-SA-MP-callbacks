/*
	New SA-MP callbacks by Emmet
	Created on Sunday, January 26, 2014 at 11:58 AM.
*/

#if !defined POTENTIAL_CRASH_DAMAGE
	#define POTENTIAL_CRASH_DAMAGE (55.0)
#endif

#if !defined POTENTIAL_SPEED_DROP
	#define POTENTIAL_SPEED_DROP (5.0)
#endif

#define IsCallbackUsed(%0) \
	(g_CallbackInfo & %0)

#define CB_GetPlayerFlag(%0,%1) \
	(g_aCallbackFlags[(%0)] & %1)

// Player variables
enum e_cbPlayerData {
	e_cbTime,
	e_cbTimer,
	e_cbPresses,
	e_cbLast,
 	e_cbVending,
  	e_cbFrames,
  	e_cbDrunk,
  	e_cbTarget,
	Float:e_cbHealth,
	Float:e_cbPacket,
	Float:e_cbPosition[3]
};

// Player flags
enum e_cbPlayerFlags (<<= 1) {
	e_cbInitialized = 1,
	e_cbUpdate,
	e_cbPaused,
	e_cbHolding,
	e_cbSelection
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
	CB_OnPlayerAntiReload
};

static
    e_cbCallbackFlags: g_CallbackInfo,
    e_cbPlayerFlags: g_aCallbackFlags[MAX_PLAYERS],
	g_cbCallbackData[MAX_PLAYERS][e_cbPlayerData];

static
	g_aLastVehicle[MAX_PLAYERS] = {INVALID_PLAYER_ID, ...},
	g_aVehicleDamage[MAX_VEHICLES][3 char],
	Float:g_aVehicleHealth[MAX_VEHICLES],
	Float:g_aVehicleSpeed[MAX_VEHICLES],
	g_aFiredShots[MAX_PLAYERS][5],
	g_aLastWeapon[MAX_PLAYERS];

static const
	g_aClipData[] = {34, 17, 7, 1, 4, 7, 100, 30, 30, 50, 100, 1, 1},
	g_aClipSlots[] = {2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 4, 6, 6};

static const Float:g_aVendingMachines[][] = {
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
};

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
forward OnPlayerDisableCursor(playerid);

// Calls when a player shoots more ammo than their weapon's clip can hold.
forward OnPlayerAntiReload(playerid, weaponid);

stock IsPlayerNearVending(playerid)
{
    for (new i = 0; i < sizeof(g_aVendingMachines); i ++)
	{
        if (GetPlayerDistanceFromPoint(playerid, g_aVendingMachines[i][0], g_aVendingMachines[i][1], g_aVendingMachines[i][2]) > 3.0)
            continue;

		else
		    return i;
    }
	return 0;
}

stock IsPlayerPaused(playerid)
	return GetCallbackFlag(playerid, CB_PAUSED);

stock GetPlayerFPS(playerid)
	return (g_cbCallbackData[playerid][e_cbFrames]);

stock GetPlayerPausedTime(playerid)
	return (g_cbCallbackData[playerid][e_cbTime]);

stock CB_SelectTextDraw(playerid, hovercolor)
{
	CB_SetPlayerFlag(playerid, e_cbSelection, true);
	return SelectTextDraw(playerid, hovercolor);
}

stock CB_CancelSelectTextDraw(playerid)
{
	CB_SetPlayerFlag(playerid, e_cbSelection, false);
	return CancelSelectTextDraw(playerid);
}

stock static CB_SetPlayerFlag(playerid, e_cbPlayerFlags:flag, bool:status)
{
	if (status)
	    g_aCallbackFlags[playerid] |= flag;

	else
	    g_aCallbackFlags[playerid] &= ~flag;

	return 0;
}

stock static CB_GetVehicleDriver(vehicleid)
{
	for (new i = 0, l = GetMaxPlayers(); i != l; i ++)
	{
		if (GetPlayerState(i) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(i) == vehicleid)
		    return i;
	}
	return INVALID_PLAYER_ID;
}

stock static Float:CB_GetVehicleSpeed(vehicleid)
{
	static
	    Float:x,
	    Float:y,
	    Float:z,
		Float:speed;

	GetVehicleVelocity(vehicleid, x, y, z);
	speed = floatsqroot((x * x) + (y * y) + (z * z)) * 100;

	return speed;
}

stock static CB_SearchCallbacks()
{
	enum e_CustomCallbacks {
	    e_CallbackName[32 char],
	    e_cbCallbackFlags: e_CallbackFlag
	};
	static const g_aCustomCallbacks[][e_CustomCallbacks] = {
	    {!"OnPlayerPause", CB_OnPlayerPause},
		{!"OnPlayerResume", CB_OnPlayerResume},
		{!"OnPlayerHoldingKey", CB_OnPlayerHoldingKey},
		{!"OnPlayerReleaseKey", CB_OnPlayerReleaseKey},
	    {!"OnPlayerFall", CB_OnPlayerFall},
		{!"OnPlayerPacketLoss", CB_OnPlayerPacketLoss},
		{!"OnPlayerUseVending", CB_OnPlayerUseVending},
		{!"OnPlayerCrashVehicle", CB_OnPlayerCrashVehicle},
	    {!"OnPlayerFPSChange", CB_OnPlayerFPSChange},
		{!"OnPlayerCarJack", CB_OnPlayerCarJack},
		{!"OnPlayerWeaponEmpty", CB_OnPlayerWeaponEmpty},
		{!"OnPlayerFriendlyFire", CB_OnPlayerFriendlyFire},
	    {!"OnPlayerTargetPlayer", CB_OnPlayerTargetPlayer},
		{!"OnPlayerDisableCursor", CB_OnPlayerDisableCursor},
		{!"OnPlayerAntiReload", CB_OnPlayerAntiReload}
	};
	for (new i = 0; i != sizeof(g_aCustomCallbacks); i ++)
	{
		if (funcidx(g_aCustomCallbacks[i][e_CallbackName]) == -1)
		    continue;

		g_CallbackInfo |= g_aCustomCallbacks[i][e_CallbackFlag];
	}
}

public OnFilterScriptInit()
{
    CB_SearchCallbacks();

	if (IsCallbackUsed(CB_OnPlayerPause) || IsCallbackUsed(CB_OnPlayerResume))
		SetTimer("CB_TabTimer", 600, true);

	#if defined CB_OnFilterScriptInit
	    return CB_OnFilterScriptInit();
	#else
		return 1;
	#endif
}

public OnGameModeInit()
{
	CB_SearchCallbacks();

	if (IsCallbackUsed(CB_OnPlayerPause) || IsCallbackUsed(CB_OnPlayerResume))
		SetTimer("CB_TabTimer", 600, true);

	#if defined CB_OnGameModeInit
	    return CB_OnGameModeInit();
	#else
		return 1;
	#endif
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if ((IsCallbackUsed(CB_OnPlayerFriendlyFire)) && (hittype == BULLET_HIT_TYPE_PLAYER && GetPlayerTeam(playerid) == GetPlayerTeam(hitid) && GetPlayerTeam(playerid) != 0 && GetPlayerTeam(playerid) != NO_TEAM))
		CallLocalFunction("OnPlayerFriendlyFire", "ddd", playerid, hitid, weaponid);

	if ((IsCallbackUsed(CB_OnPlayerWeaponEmpty)) && (22 <= weaponid <= 38 && GetPlayerWeaponState(playerid) == WEAPONSTATE_LAST_BULLET && GetPlayerAmmo(playerid) == 1))
		CallLocalFunction("OnPlayerWeaponEmpty", "dd", playerid, weaponid);

	if ((IsCallbackUsed(CB_OnPlayerAntiReload)) && (22 <= weaponid <= 34) && (weaponid != 25 && weaponid != 33 && weaponid != 34))
	{
		new
			index = (weaponid - 22);

		if (++ g_aFiredShots[playerid][g_aClipSlots[index]] > g_aClipData[index] && (g_aFiredShots[playerid][g_aClipSlots[index]] - g_aClipData[index]) > 2)
		{
		    g_aFiredShots[playerid][g_aClipSlots[index]] = 0;
			CallLocalFunction("OnPlayerAntiReload", "dd", playerid, weaponid);
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
	static
	    weaponid,
	    amount,
	    level,
	    Float:packet;

    if (!CB_GetPlayerFlag(playerid, e_cbInitialized))
	{
 		g_aCallbackFlags[playerid] |= e_cbInitialized;

		GetPlayerHealth(playerid, g_cbCallbackData[playerid][e_cbHealth]);
  		GetPlayerPos(playerid, g_cbCallbackData[playerid][e_cbPosition][0], g_cbCallbackData[playerid][e_cbPosition][1], g_cbCallbackData[playerid][e_cbPosition][2]);
	}
	switch (GetPlayerState(playerid))
	{
	    case PLAYER_STATE_ONFOOT:
	    {
			if (IsCallbackUsed(CB_OnPlayerAntiReload))
			{
				if ((weaponid = GetPlayerWeapon(playerid)) != g_aLastWeapon[playerid])
				{
		    		if ((22 <= g_aLastWeapon[playerid] <= 34) && (g_aLastWeapon[playerid] != 25 && g_aLastWeapon[playerid] != 33 && g_aLastWeapon[playerid] != 34))
						g_aFiredShots[playerid][g_aClipSlots[g_aLastWeapon[playerid] - 22]] = 0;

					g_aLastWeapon[playerid] = weaponid;
				}
				else if ((22 <= weaponid <= 34) && (weaponid != 25 && weaponid != 33 && weaponid != 34) && GetPlayerWeaponState(playerid) == WEAPONSTATE_RELOADING) {
					g_aFiredShots[playerid][g_aClipSlots[weaponid - 22]] = 0;
				}
			}
		  	if (IsCallbackUsed(CB_OnPlayerTargetPlayer))
			{
		    	if (g_cbCallbackData[playerid][e_cbTarget] == 0 && GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID)
		        	g_cbCallbackData[playerid][e_cbTarget] = INVALID_PLAYER_ID;

	  			else if (GetPlayerTargetPlayer(playerid) != g_cbCallbackData[playerid][e_cbTarget])
	  			{
	  		    	g_cbCallbackData[playerid][e_cbTarget] = GetPlayerTargetPlayer(playerid);
	  		    	CallLocalFunction("OnPlayerTargetPlayer", "ddd", playerid, g_cbCallbackData[playerid][e_cbTarget], GetPlayerWeapon(playerid));
				}
			}
			if (IsCallbackUsed(CB_OnPlayerUseVending))
			{
				if (IsPlayerNearVending(playerid) && GetPlayerAnimationIndex(playerid) == 1660 && (GetTickCount() - g_cbCallbackData[playerid][e_cbVending]) > 3500)
				{
					CallLocalFunction("OnPlayerUseVending", "d", playerid);
					g_cbCallbackData[playerid][e_cbVending] = GetTickCount();
				}
			}
			if (IsCallbackUsed(CB_OnPlayerFall))
			{
		        if (GetPlayerAnimationIndex(playerid) < 1130 || GetPlayerAnimationIndex(playerid) > 1132)
				{
					static
					    Float:health,
						Float:x,
						Float:y,
	   					Float:z;

					GetPlayerPos(playerid, x, y, z);
					GetPlayerHealth(playerid, health);

					if (g_cbCallbackData[playerid][e_cbHealth] > health && g_cbCallbackData[playerid][e_cbPosition][2] > z)
						CallLocalFunction("OnPlayerFall", "dff", playerid, floatsub(g_cbCallbackData[playerid][e_cbPosition][2], z), floatsub(g_cbCallbackData[playerid][e_cbHealth], health));

					GetPlayerHealth(playerid, g_cbCallbackData[playerid][e_cbHealth]);
	   				GetPlayerPos(playerid, g_cbCallbackData[playerid][e_cbPosition][0], g_cbCallbackData[playerid][e_cbPosition][1], g_cbCallbackData[playerid][e_cbPosition][2]);
				}
			}
		}
		case PLAYER_STATE_DRIVER:
		{
		    new
		        vehicleid = GetPlayerVehicleID(playerid),
		        Float:health;

			GetVehicleHealth(vehicleid, health);

			if ((floatcmp(g_aVehicleHealth[vehicleid], health) > 0 && floatabs(g_aVehicleHealth[vehicleid] - health) >= POTENTIAL_CRASH_DAMAGE) && floatabs(g_aVehicleSpeed[vehicleid] - CB_GetVehicleSpeed(vehicleid)) >= POTENTIAL_SPEED_DROP)
			{
			    new
					damage[4];

				GetVehicleDamageStatus(vehicleid, damage[0], damage[1], damage[2], damage[3]);

			    for (new i = 0; i < 3; i ++) if (g_aVehicleDamage[vehicleid]{i} != damage[i]) {
					CallLocalFunction("OnPlayerCrashVehicle", "ddf", playerid, vehicleid, floatsub(g_aVehicleHealth[vehicleid], health));

					break;
				}
				for (new i = 0; i < 3; i ++) if (g_aVehicleDamage[vehicleid]{i} != damage[i]) {
					g_aVehicleDamage[vehicleid]{i} = damage[i];
				}
			}
			g_aVehicleHealth[vehicleid] = health;
			g_aVehicleSpeed[vehicleid] = CB_GetVehicleSpeed(vehicleid);
		}
	}
	if (IsCallbackUsed(CB_OnPlayerFPSChange))
	{
		if ((level = GetPlayerDrunkLevel(playerid)) < 100)
    		SetPlayerDrunkLevel(playerid, 2000);

		else if (g_cbCallbackData[playerid][e_cbDrunk] != level)
	    {
		    if ((amount = g_cbCallbackData[playerid][e_cbDrunk] - level) && g_cbCallbackData[playerid][e_cbFrames] != amount && 0 <= amount <= 200)
				CallLocalFunction("OnPlayerFPSChange", "ddd", playerid, g_cbCallbackData[playerid][e_cbFrames], amount);

			g_cbCallbackData[playerid][e_cbFrames] = amount;
			g_cbCallbackData[playerid][e_cbDrunk] = level;
		}
	}
	if (IsCallbackUsed(CB_OnPlayerPacketLoss))
	{
		packet = NetStats_PacketLossPercent(playerid);

		if (floatcmp(packet, 0.0) != 0 && floatcmp(packet, g_cbCallbackData[playerid][e_cbPacket]) != 0) {
		    CallLocalFunction("OnPlayerPacketLoss", "dff", playerid, g_cbCallbackData[playerid][e_cbPacket], packet);

			g_cbCallbackData[playerid][e_cbPacket] = packet;
		}
	}
	CB_SetPlayerFlag(playerid, e_cbUpdate, true);
	g_cbCallbackData[playerid][e_cbLast] = GetTickCount();

	#if defined CB_OnPlayerUpdate
		return CB_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (IsCallbackUsed(CB_OnPlayerCarJack))
	{
		if (newstate == PLAYER_STATE_DRIVER)
			g_aLastVehicle[playerid] = GetPlayerVehicleID(playerid);

		else if (newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
		{
			static
				driverid = INVALID_PLAYER_ID;

			if ((driverid = CB_GetVehicleDriver(g_aLastVehicle[playerid])) != INVALID_PLAYER_ID)
			    CallLocalFunction("OnPlayerCarJack", "ddd", driverid, playerid, g_aLastVehicle[playerid]);

			g_aLastVehicle[playerid] = INVALID_VEHICLE_ID;
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
    if (IsCallbackUsed(CB_OnPlayerHoldingKey))
	{
		if (!CB_GetPlayerFlag(playerid, e_cbHolding))
		{
	    	CB_SetPlayerFlag(playerid, e_cbHolding, true);

	    	g_cbCallbackData[playerid][e_cbTime] = GetTickCount();
			g_cbCallbackData[playerid][e_cbTimer] = SetTimerEx("CB_HoldingTimer", 200, true, "dd", playerid, newkeys);
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
	if ((IsCallbackUsed(CB_OnPlayerDisableCursor)) && (clickedid == Text:INVALID_TEXT_DRAW && g_aCallbackFlags[playerid] & e_cbSelection))
	    CallLocalFunction("OnPlayerDisableCursor", "d", playerid);

	#if defined CB_OnPlayerClickTextDraw
		return CB_OnPlayerClickTextDraw(playerid, clickedid);
	#else
		return 0;
	#endif
}

forward CB_HoldingTimer(playerid, key);
public CB_HoldingTimer(playerid, key)
{
	static
	    keys[3];

	GetPlayerKeys(playerid, keys[0], keys[1], keys[2]);

	if (keys[0] & key)
	{
	    g_cbCallbackData[playerid][e_cbPresses]++;
	    CallLocalFunction("OnPlayerHoldingKey", "dd", playerid, key);
	}
	else
	{
	    if (IsCallbackUsed(CB_OnPlayerReleaseKey) && g_cbCallbackData[playerid][e_cbPresses] > 0)
	    	CallLocalFunction("OnPlayerReleaseKey", "ddd", playerid, keys, GetTickCount() - g_cbCallbackData[playerid][e_cbTime]);

        g_cbCallbackData[playerid][e_cbPresses] = 0;

        KillTimer(g_cbCallbackData[playerid][e_cbTimer]);
	    CB_SetPlayerFlag(playerid, e_cbHolding, false);
	}
}

forward CB_TabTimer();
public CB_TabTimer()
{
	for (new i = 0, l = GetMaxPlayers(); i != l; i ++)
	{
	    if (GetPlayerState(i) != PLAYER_STATE_ONFOOT && GetPlayerState(i) != PLAYER_STATE_DRIVER && GetPlayerState(i) != PLAYER_STATE_PASSENGER)
	        continue;

		if (CB_GetPlayerFlag(i, e_cbUpdate))
		{
		    if (IsCallbackUsed(CB_OnPlayerResume) && CB_GetPlayerFlag(i, e_cbPaused))
		    	CallLocalFunction("OnPlayerResume", "dd", i, GetTickCount() - g_cbCallbackData[i][e_cbTime]);

		    CB_SetPlayerFlag(i, e_cbUpdate, false);
		    CB_SetPlayerFlag(i, e_cbPaused, false);
		}
		else if (IsCallbackUsed(CB_OnPlayerPause))
		{
			if (!CB_GetPlayerFlag(i, e_cbPaused) && (GetTickCount() - g_cbCallbackData[i][e_cbLast]) >= 2000)
			{
		    	g_cbCallbackData[i][e_cbTime] = GetTickCount();

				CallLocalFunction("OnPlayerPause", "d", i);
				CB_SetPlayerFlag(i, e_cbPaused, true);
			}
		}
	}
}

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

#define SelectTextDraw          CB_SelectTextDraw
#define CancelSelectTextDraw    CB_CancelSelectTextDraw

#define OnFilterScriptInit  	CB_OnFilterScriptInit
#define OnGameModeInit 			CB_OnGameModeInit
#define OnPlayerUpdate 			CB_OnPlayerUpdate
#define OnPlayerStateChange  	CB_OnPlayerStateChange
#define OnPlayerKeyStateChange  CB_OnPlayerKeyStateChange
#define OnPlayerWeaponShot      CB_OnPlayerWeaponShot
#define OnPlayerClickTextDraw   CB_OnPlayerClickTextDraw

#if defined CB_OnPlayerUpdate
	forward CB_OnPlayerUpdate(playerid);
#endif

#if defined CB_OnFilterScriptInit
	forward CB_OnFilterScriptInit();
#endif

#if defined CB_OnGameModeInit
	forward CB_OnGameModeInit();
#endif

#if defined CB_OnPlayerStateChange
	forward CB_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

#if defined CB_OnPlayerKeyStateChange
	forward CB_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

#if defined CB_OnPlayerWeaponShot
	forward CB_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
#endif

#if defined CB_OnPlayerClickTextDraw
	forward CB_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif
