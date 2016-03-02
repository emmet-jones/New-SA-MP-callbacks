/*
	New SA-MP callbacks by Emmet_

	Description:
	  An include which offers a wide selection of callbacks for scripters to use.

	Start Date:
 	  01/26/2014 @ 11:58 AM.

	Last Update:
	  02/29/2016 @ 12:00 AM

	Callbacks:
	    OnPlayerPause(playerid)
	    Description: Called when a player is paused.

	    OnPlayerResume(playerid, time)
	    Description: Called when a player has resumed.

	    OnPlayerHoldingKey(playerid, keys)
	    Description: Called when a player begins holding a specific key.

	    OnPlayerReleaseKey(playerid, keys)
	    Description: Called when a player releases a key.

	    OnPlayerFall(playerid, Float:damage)
	    Description: Called when a player falls and loses damage.

	    OnPlayerPacketLoss(playerid, Float:newpacket, Float:oldpacket)
	    Description: Called when a player experiences packet loss.

	    OnPlayerUseVending(playerid, type)
	    Description: Called when a player uses a vending machine.

	    OnPlayerCrashVehicle(playerid, vehicleid, Float:damage)
	    Description: Called when a player crashes a vehicle.

	    OnPlayerFPSChange(playerid, oldfps, newfps)
	    Description: Called when a player's FPS rate changes.

	    OnPlayerJackVehicle(playerid, targetid, vehicleid)
	    Description: Called when a player jacks another player's vehicle.

	    OnPlayerEmptyWeapon(playerid, weaponid)
	    Description: Called when a player depletes all ammo in a weapon.

	    OnPlayerFriendlyFire(playerid, targetid, weaponid)
	    Description: Called when a player shoots at a teammate.

	    OnPlayerTargetPlayer(playerid, targetid, weaponid)
	    Description: Called when a player targets a player with their weapon.

		OnPlayerHideCursor(playerid, hovercolor)
	    Description: Called when a player cancels textdraw selection.

	    OnPlayerAntiReload(playerid, weaponid)
	    Description: Called when a player shoots without reloading their weapon.

	    OnPlayerAnimationPlay(playerid, animlib[], animname[])
	    Description: Called when an animation is played.

	    OnPlayerReloadWeapon(playerid, weaponid, ammo)
	    Description: Called when a player reloads their weapon.

        OnPlayerActionChange(playerid, oldaction, newaction)
	    Description: Called when a player's action change (see action list).

        OnPlayerRamPlayer(playerid, driverid, vehicleid, Float:damage)
	    Description: Called when a player rams another player.

		OnPlayerSprayAtVehicle(playerid, vehicleid)
		Description: Called wehn a player is spraying at a vehicle.

	    OnPlayerStartBurn(playerid)
	    Description: Called when a player is burning from fire.

	    OnPlayerStopBurn(playerid)
	    Description: Called when a player stops burning.

	    OnPlayerStartAim(playerid, weaponid)
	    Description: Called when a player is aiming a weapon.

	    OnPlayerStopAim(playerid)
	    Description: Called when a player stops aiming.

	    OnPlayerUseCamera(playerid)
		Description: Called when a player snaps a picture with a camera (weapon ID: 43).

		OnPlayerJump(playerid)
		Description: Called when a player jumps (SHIFT key).

  		OnPlayerUseGarage(playerid, vehicleid, type)
		Description: Called when a player uses a Pay'n'Spray or bomb shop.

		OnVehicleCreated(vehicleid, color1, color2)
		Description: Called when a vehicle is created by the server.

	Definitions:
	    POTENTIAL_CRASH_DAMAGE
	    Description: The maximum amount of damage a vehicle must take before being ruled as a crash.

		MAX_ACCUMULATED_SHOTS
		Description: The maximum amount of shots the player fired without reloading before calling OnPlayerAntiReload.
*/

/*
    native IsPlayerPaused(playerid);
	native IsPlayerBurning(playerid);
	native IsPlayerAiming(playerid);
	native IsPlayerSwimming(playerid);
	native IsPlayerSkydiving(playerid);
	native IsPlayerJumping(playerid);
	native IsPlayerNearVending(playerid);
	native IsPlayerNearGarage(playerid, type);
	native IsPlayerSprayingVehicle(playerid, vehicleid);
	native GetPlayerFPS(playerid);
	native GetPlayerAction(playerid);
	native GetPlayerPausedTime(playerid);
	native GetConnectedTime(playerid);
	native GetPlayerPacketLoss(playerid);
*/

#if !defined POTENTIAL_CRASH_DAMAGE
	#define POTENTIAL_CRASH_DAMAGE 		55.0
#endif

#if !defined MAX_ACCUMULATED_SHOTS
	#define MAX_ACCUMULATED_SHOTS 		5
#endif

#define PLAYER_ACTION_NONE      0
#define PLAYER_ACTION_SHOOTING  1
#define PLAYER_ACTION_SWIMMING  2
#define PLAYER_ACTION_SKYDIVING 3

#define VENDING_TYPE_SPRUNK     1
#define VENDING_TYPE_CANDY      2

#define GARAGE_BOMBSHOP         1
#define GARAGE_PAYNSPRAY        2

// --------------------------------------------------
// Enumerators
// --------------------------------------------------

enum _:e_PLAYER_TICK_ENUM {
	e_PLAYER_TICK,
	e_PLAYER_LAST_UPDATE,
	e_PLAYER_LAST_SHOT,
	e_PLAYER_LAST_JUMP,
	e_PLAYER_BURN_TICK,
	e_PLAYER_VENDING_TICK
};

enum e_PLAYER_DATA {
	e_PLAYER_FLAGS:e_bFlags,
	Float:e_fPacket,
	e_iHoldTimer,
  	e_iFrames,
  	e_iDrunk,
  	e_iMoney,
  	e_iTargetID,
  	e_iAction,
  	e_iAnimIndex,
  	e_iHoverColor,
  	e_iShotsFired[12],
  	e_iLastGun,
  	e_iLastAmmo,
   	e_iVehicleID
};

enum e_VEHICLE_DATA {
	e_iPanels,
	e_iDoors,
	e_iLights,
	Float:e_fHealth,
	Float:e_fSpeed
};

enum e_PLAYER_FLAGS (<<= 1) {
	e_bUpdate = 1,
	e_bPaused,
	e_bHoldKey,
	e_bSelectTD,
	e_bAnimSet,
	e_bKeyPress,
	e_bReloading,
	e_bBurning,
	e_bAiming
};

static
	s_aPlayerInfo[MAX_PLAYERS][e_PLAYER_DATA],
	s_aPlayerTickInfo[MAX_PLAYERS][e_PLAYER_TICK_ENUM],
	s_aVehicleInfo[MAX_VEHICLES][e_VEHICLE_DATA];

// --------------------------------------------------
// Arrays
// --------------------------------------------------

enum e_VENDING_MACHINES {
	e_VENDING_MACHINE_TYPE,
	Float:e_VENDING_MACHINE_X,
	Float:e_VENDING_MACHINE_Y,
	Float:e_VENDING_MACHINE_Z,
};

enum e_VEHICLE_GARAGES {
	e_VEHICLE_GARAGE_TYPE,
	Float:e_VEHICLE_GARAGE_X,
	Float:e_VEHICLE_GARAGE_Y,
	Float:e_VEHICLE_GARAGE_Z
};

static
	s_aVendingMachines[75][e_VENDING_MACHINES] = {
	    {VENDING_TYPE_SPRUNK, 2326.0273, -1645.9480, 14.8270},
	    {VENDING_TYPE_SPRUNK, 2060.1038, -1898.4542, 13.5538},
	    {VENDING_TYPE_SPRUNK, 1729.8389, -1943.8608, 13.5687},
	    {VENDING_TYPE_SPRUNK, 1929.5480, -1772.3799, 13.5469},
	    {VENDING_TYPE_SPRUNK, 1788.3971, -1369.2784, 15.7578},
	    {VENDING_TYPE_SPRUNK, 2352.9934, -1357.0977, 24.3984},
	    {VENDING_TYPE_SPRUNK, 1153.9095, -1460.9143, 15.7969},
	    {VENDING_TYPE_SPRUNK, 200.2008, -107.6411, 1.5513},
	    {VENDING_TYPE_SPRUNK, 1278.5946, 372.2171, 19.5547},
	    {VENDING_TYPE_SPRUNK, 2320.0242, 2532.0378, 10.8203},
	    {VENDING_TYPE_SPRUNK, 2086.5876, 2071.3574, 11.0579},
	    {VENDING_TYPE_SPRUNK, 1519.3347, 1055.2349, 10.8203},
	    {VENDING_TYPE_SPRUNK, 2503.1370, 1244.5089, 10.8203},
	    {VENDING_TYPE_SPRUNK, 373.8246, -178.9297, 1000.6328},
	    {VENDING_TYPE_SPRUNK, 2575.9084, -1284.4164, 1060.9844},
	    {VENDING_TYPE_SPRUNK, 495.9450, -23.5309, 1000.6797},
	    {VENDING_TYPE_SPRUNK, 501.8352, -2.2189, 1000.6797},
	    {VENDING_TYPE_SPRUNK, 2224.4141, -1153.4738, 1025.7969},
	    {VENDING_TYPE_SPRUNK, 2156.6990, 1606.7858, 999.9718},
	    {VENDING_TYPE_SPRUNK, 2209.1143, 1607.1713, 999.9691},
	    {VENDING_TYPE_SPRUNK, 2222.9924, 1606.8201, 999.9758},
	    {VENDING_TYPE_SPRUNK, -14.7329, 1176.1732, 19.5634},
	    {VENDING_TYPE_SPRUNK, -862.8334, 1537.4231, 22.5870},
	    {VENDING_TYPE_SPRUNK, -252.9273, 2597.9426, 62.8582},
	    {VENDING_TYPE_SPRUNK, -2419.5483, 985.9162, 45.2969},
	    {VENDING_TYPE_SPRUNK, -2419.5837, 984.5789, 45.2969},
	    {VENDING_TYPE_SPRUNK, -1349.3037, 492.3072, 11.1953},
	    {VENDING_TYPE_SPRUNK, -1981.6035, 142.6650, 27.6875},
	    {VENDING_TYPE_SPRUNK, -2005.6317, -490.8682, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2034.4124, -490.8690, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2063.2695, -490.8681, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2092.0920, -490.8683, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2119.7566, -423.4371, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2119.4058, -422.2168, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2097.2969, -397.5224, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2068.5955, -397.5209, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2039.8862, -397.5221, 35.5313},
	    {VENDING_TYPE_SPRUNK, -2011.1857, -397.5218, 35.5313},
	    {VENDING_TYPE_SPRUNK, -32.5050, -185.9060, 1003.5469},
	    {VENDING_TYPE_SPRUNK, -16.1550, -90.8514, 1003.5469},
	    {VENDING_TYPE_SPRUNK, -15.1533, -139.4374, 1003.5469},
	    {VENDING_TYPE_SPRUNK, -35.7115, -139.4262, 1003.5469},
        {VENDING_TYPE_SPRUNK, -19.0527, -57.0465, 1003.5469},
        {VENDING_TYPE_CANDY,  -75.2803, 1227.9558, 19.7359},
	    {VENDING_TYPE_CANDY,  -252.9929, 2599.7952, 62.8582},
	    {VENDING_TYPE_CANDY,  -1455.1244, 2592.4136, 55.8359},
	    {VENDING_TYPE_CANDY,  -1349.3669, 493.8326, 11.1953},
	    {VENDING_TYPE_CANDY,  -2229.1858, 287.1634, 35.3203},
	    {VENDING_TYPE_CANDY,  -33.9020, -186.0259, 1003.5469},
	    {VENDING_TYPE_CANDY,  -17.5500, -90.9689, 1003.5469},
	    {VENDING_TYPE_CANDY,  -16.4742, -139.5535, 1003.5469},
	    {VENDING_TYPE_CANDY,  -36.1270, -57.1352, 1003.5469},
		{VENDING_TYPE_CANDY,  1634.0918, -2238.2810, 13.5078},
	    {VENDING_TYPE_CANDY,  2480.8667, -1958.5234, 13.5830},
	    {VENDING_TYPE_CANDY,  2140.2676, -1161.4753, 23.9922},
	    {VENDING_TYPE_CANDY,  2153.8462, -1015.7134, 62.8837},
	    {VENDING_TYPE_CANDY,  662.4010, -551.4146, 16.3359},
	    {VENDING_TYPE_CANDY,  2271.7383, -77.2105, 26.5812},
	    {VENDING_TYPE_CANDY,  2845.7246, 1294.2957, 11.3906},
	    {VENDING_TYPE_CANDY,  1398.8699, 2223.3665, 11.0234},
	    {VENDING_TYPE_CANDY,  1659.4425, 1722.1097, 10.8281},
	    {VENDING_TYPE_CANDY,  2647.6428, 1128.9147, 11.1797},
	    {VENDING_TYPE_CANDY,  374.8246, 188.2364, 1008.3893},
	    {VENDING_TYPE_CANDY,  361.5366, 159.3573, 1008.3828},
	    {VENDING_TYPE_CANDY,  351.6461, 206.0936, 1008.3828},
	    {VENDING_TYPE_CANDY,  371.5602, 177.7127, 1019.9844},
	    {VENDING_TYPE_CANDY,  331.9078, 177.7589, 1019.9912},
	    {VENDING_TYPE_CANDY,  330.6430, 177.7591, 1019.9912},
	    {VENDING_TYPE_CANDY,  378.2992, -178.8477, 1000.6328},
	    {VENDING_TYPE_CANDY,  316.2407, -140.3572, 999.6016},
	    {VENDING_TYPE_CANDY,  500.6049, -2.1070, 1000.6797},
	    {VENDING_TYPE_CANDY,  2156.5854, 1607.8103, 999.9692},
	    {VENDING_TYPE_CANDY,  2202.4285, 1617.7483, 999.9766},
	    {VENDING_TYPE_CANDY,  2209.2668, 1620.4711, 999.9827},
	    {VENDING_TYPE_CANDY,  2223.1074, 1602.5835, 999.9734}
	},

	s_aVehicleGarages[][e_VEHICLE_GARAGES] = {
	    {GARAGE_PAYNSPRAY, -100.0488, 1117.3574, 19.7417},
		{GARAGE_PAYNSPRAY, -1420.5437, 2585.5051, 55.8433},
		{GARAGE_PAYNSPRAY, -2425.8203, 1022.0131, 50.3977},
		{GARAGE_PAYNSPRAY, -1904.3649, 284.2363, 41.0469},
	    {GARAGE_PAYNSPRAY, 2065.1960, -1831.4946, 13.5469},
		{GARAGE_PAYNSPRAY, 1025.0665, -1024.4612, 32.1016},
		{GARAGE_PAYNSPRAY, 720.1285, -457.2187, 16.3359},
		{GARAGE_PAYNSPRAY, 487.4250, -1739.0385, 10.8542},
		{GARAGE_PAYNSPRAY, 1974.4723, 2162.3350, 11.0703},
		{GARAGE_BOMBSHOP,  1849.0271, -1856.3948, 13.3828},
		{GARAGE_BOMBSHOP,  2005.9357, 2312.9939, 10.8203}
	}
;

// --------------------------------------------------
// Callbacks
// --------------------------------------------------

forward OnPlayerPause(playerid);
forward OnPlayerResume(playerid, time);
forward OnPlayerHoldingKey(playerid, keys);
forward OnPlayerReleaseKey(playerid, keys, time);
forward OnPlayerFall(playerid, Float:damage);
forward OnPlayerPacketLoss(playerid, Float:oldloss, Float:newloss);
forward OnPlayerCrashVehicle(playerid, vehicleid, Float:damage);
forward OnPlayerUseVending(playerid, type);
forward OnPlayerFPSChange(playerid, oldfps, newfps);
forward OnPlayerJackVehicle(playerid, targetid, vehicleid);
forward OnPlayerEmptyWeapon(playerid, weaponid);
forward OnPlayerFriendlyFire(playerid, targetid, weaponid);
forward OnPlayerTargetPlayer(playerid, targetid, weaponid);
forward OnPlayerHideCursor(playerid, hovercolor);
forward OnPlayerAntiReload(playerid, weaponid);
forward OnPlayerAnimationPlay(playerid, animlib[], animname[]);
forward OnPlayerReloadWeapon(playerid, weaponid, ammo);
forward OnPlayerActionChange(playerid, oldaction, newaction);
forward OnPlayerRamPlayer(playerid, driverid, vehicleid, Float:damage);
forward OnPlayerSprayAtVehicle(playerid, vehicleid);
forward OnPlayerStartBurn(playerid);
forward OnPlayerStopBurn(playerid);
forward OnPlayerStartAim(playerid, weaponid);
forward OnPlayerStopAim(playerid);
forward OnPlayerUseCamera(playerid);
forward OnPlayerJump(playerid);
forward OnPlayerUseGarage(playerid, vehicleid, type);
forward OnVehicleCreated(vehicleid, color1, color2);

// --------------------------------------------------
// Internal functions
// --------------------------------------------------

stock static Callback_GetFlag(playerid, e_PLAYER_FLAGS:flag)
{
	return s_aPlayerInfo[playerid][e_bFlags] & flag;
}

stock static Callback_SetFlag(playerid, e_PLAYER_FLAGS:flag, status)
{
	if(status)
		return s_aPlayerInfo[playerid][e_bFlags] |= flag;
	else
	    return s_aPlayerInfo[playerid][e_bFlags] &= ~flag;
}

stock static Callback_GetDriver(vehicleid)
{
	// Returns the driver of a vehicle (obvious).

	for(new i = 0, l = GetPlayerPoolSize(); i <= l; i ++)
	{
		if(IsPlayerInVehicle(i, vehicleid) && GetPlayerState(i) == PLAYER_STATE_DRIVER)
		{
		    return i;
		}
	}

	return INVALID_PLAYER_ID;
}

stock static Callback_GetMaxClip(weaponid)
{
	// Returns the clip capacity for the specified weapon (required by OnPlayerAntiReload).

	switch(weaponid)
	{
	    case 22:
			return 34;
	    case 23:
			return 17;
	    case 24, 27:
	        return 7;
	    case 28, 32:
	        return 100;
	    case 29, 30:
	        return 30;
		case 26:
		    return 4;
		case 31:
		    return 50;
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

stock static Callback_IsVehicleInPoint(vehicleid, Float:range, Float:x, Float:y, Float:z)
{
	// Checks if a vehicle is in range of a point.

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

// --------------------------------------------------
// External functions
// --------------------------------------------------

stock IsPlayerPaused(playerid)
{
	return Callback_GetFlag(playerid, e_bPaused);
}

stock IsPlayerBurning(playerid)
{
	return Callback_GetFlag(playerid, e_bBurning);
}

stock IsPlayerAiming(playerid)
{
	return Callback_GetFlag(playerid, e_bAiming);
}

stock IsPlayerSwimming(playerid)
{
    return (1538 <= GetPlayerAnimationIndex(playerid) <= 1544);
}

stock IsPlayerSkydiving(playerid)
{
	return (GetPlayerWeapon(playerid) == 46 && GetPlayerAnimationIndex(playerid) == 1134);
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
	return s_aPlayerInfo[playerid][e_iFrames];
}

stock GetPlayerPausedTime(playerid)
{
	return (GetConnectedTime(playerid) - s_aPlayerTickInfo[playerid][e_PLAYER_TICK]);
}

stock Float:GetPlayerPacketLoss(playerid)
{
	return s_aPlayerInfo[playerid][e_fPacket];
}

stock GetPlayerAction(playerid)
{
	return s_aPlayerInfo[playerid][e_iAction];
}

stock GetConnectedTime(playerid)
{
	return NetStats_GetConnectedTime(playerid);
}

stock IsPlayerNearVending(playerid, type)
{
	for(new i = 0; i < sizeof(s_aVehicleGarages); i ++)
	{
	    if(s_aVendingMachines[i][e_VENDING_MACHINE_TYPE] == type && IsPlayerInRangeOfPoint(playerid, 2.0, s_aVendingMachines[i][e_VENDING_MACHINE_X], s_aVendingMachines[i][e_VENDING_MACHINE_Y], s_aVendingMachines[i][e_VENDING_MACHINE_Z]))
	    {
	        return 1;
		}
	}

	return 0;
}

stock IsPlayerNearGarage(playerid, type)
{
	for(new i = 0; i < sizeof(s_aVehicleGarages); i ++)
	{
	    if(s_aVehicleGarages[i][e_VEHICLE_GARAGE_TYPE] == type && IsPlayerInRangeOfPoint(playerid, 8.0, s_aVehicleGarages[i][e_VEHICLE_GARAGE_X], s_aVehicleGarages[i][e_VEHICLE_GARAGE_Y], s_aVehicleGarages[i][e_VEHICLE_GARAGE_Z]))
	    {
	        return 1;
		}
	}

	return 0;
}

stock IsPlayerSprayingVehicle(playerid, vehicleid)
{
	new
	    index = GetPlayerAnimationIndex(playerid),
		Float:x,
		Float:y,
		Float:z,
		Float:a
	;

	if(IsVehicleStreamedIn(vehicleid, playerid))
	{
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, a);

		x += 2.0 * floatsin(-a, degrees);
		y += 2.0 * floatcos(-a, degrees);

		if(Callback_IsVehicleInPoint(vehicleid, 2.0, x, y, z))
		{
			return (1160 <= index <= 1163) || index == 1167 || index == 640;
		}
	}

	return 0;
}

// --------------------------------------------------
// Native hooks
// --------------------------------------------------

stock CB_CreateVehicle(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren = 0)
{
	new
	    vehicleid = CreateVehicle(modelid, x, y, z, rotation, color1, color2, respawn_delay, addsiren);

	if(vehicleid != INVALID_VEHICLE_ID)
	{
		CallLocalFunction("OnVehicleCreated", "iii", vehicleid, color1, color2);
	}

	return vehicleid;
}

stock CB_AddStaticVehicle(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2)
{
	new
	    vehicleid = AddStaticVehicle(modelid, x, y, z, rotation, color1, color2);

	if(vehicleid != INVALID_VEHICLE_ID)
	{
		CallLocalFunction("OnVehicleCreated", "iii", vehicleid, color1, color2);
	}

	return vehicleid;
}

stock CB_AddStaticVehicleEx(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren = 0)
{
	new
	    vehicleid = AddStaticVehicleEx(modelid, x, y, z, rotation, color1, color2, respawn_delay, addsiren);

	if(vehicleid != INVALID_VEHICLE_ID)
	{
		CallLocalFunction("OnVehicleCreated", "iii", vehicleid, color1, color2);
	}

	return vehicleid;
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
		s_aPlayerInfo[playerid][e_iHoverColor] = hovercolor;

		Callback_SetFlag(playerid, e_bSelectTD, true);
	}

	return ret;
}

stock CB_CancelSelectTextDraw(playerid)
{
	new
	    ret = CancelSelectTextDraw(playerid);

	if(ret)
	{
	    s_aPlayerInfo[playerid][e_iHoverColor] = 0;

		Callback_SetFlag(playerid, e_bSelectTD, false);
	}

	return ret;
}

// --------------------------------------------------
// SA-MP Callbacks
// --------------------------------------------------

public OnPlayerDisconnect(playerid, reason)
{
	if(Callback_GetFlag(playerid, e_bHoldKey))
	{
	    KillTimer(s_aPlayerInfo[playerid][e_iHoldTimer]);
	}

    for(new i = 0; i < e_PLAYER_TICK_ENUM; i ++)
	{
		s_aPlayerTickInfo[playerid][i] = 0;
	}

	s_aPlayerInfo[playerid][e_iAction] = PLAYER_ACTION_NONE;
	s_aPlayerInfo[playerid][e_iLastGun] = 0;
	s_aPlayerInfo[playerid][e_iLastAmmo] = 0;
  	s_aPlayerInfo[playerid][e_iFrames] = 0;
  	s_aPlayerInfo[playerid][e_iDrunk] = 0;
  	s_aPlayerInfo[playerid][e_iMoney] = 0;
  	s_aPlayerInfo[playerid][e_iTargetID] = 0;
  	s_aPlayerInfo[playerid][e_iAnimIndex] = 0;
  	s_aPlayerInfo[playerid][e_iHoverColor] = 0;
 	s_aPlayerInfo[playerid][e_fPacket] = 0.0;
 	s_aPlayerInfo[playerid][e_iVehicleID] = INVALID_VEHICLE_ID;
    s_aPlayerInfo[playerid][e_bFlags] = e_PLAYER_FLAGS:0;

    for(new i = 0; i < 12; i ++)
	{
		s_aPlayerInfo[playerid][e_iShotsFired][i] = 0;
	}

 	#if defined CB_OnPlayerDisconnect
 	    return CB_OnPlayerDisconnect(playerid, reason);
	#else
	    return 1;
	#endif
}

public OnFilterScriptInit()
{
	SetTimer("Callback_TabCheck", 600, true);

	#if defined CB_OnFilterScriptInit
	    return CB_OnFilterScriptInit();
	#else
		return 1;
	#endif
}

public OnGameModeInit()
{
 	SetTimer("Callback_TabCheck", 600, true);

	#if defined CB_OnGameModeInit
	    return CB_OnGameModeInit();
	#else
		return 1;
	#endif
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new
	    capacity = Callback_GetMaxClip(weaponid),
	    teamid = GetPlayerTeam(playerid);

	if((teamid != NO_TEAM && hittype == BULLET_HIT_TYPE_PLAYER) && GetPlayerTeam(hitid) == teamid)
	{
		CallLocalFunction("OnPlayerFriendlyFire", "iii", playerid, hitid, weaponid);
	}

	if(GetPlayerWeaponState(playerid) == WEAPONSTATE_LAST_BULLET && GetPlayerAmmo(playerid) == 1)
	{
		CallLocalFunction("OnPlayerEmptyWeapon", "ii", playerid, weaponid);
	}
	
	if((WEAPON_COLT45 <= weaponid <= WEAPON_TEC9) && weaponid != WEAPON_SHOTGUN)
	{
		new
			index = (weaponid - WEAPON_COLT45);

		s_aPlayerInfo[playerid][e_iShotsFired][index]++;

		if(s_aPlayerInfo[playerid][e_iShotsFired][index] - capacity >= MAX_ACCUMULATED_SHOTS)
		{
			CallLocalFunction("OnPlayerAntiReload", "ii", playerid, weaponid);
            s_aPlayerInfo[playerid][e_iShotsFired][index] = 0;
		}
	}

	if(s_aPlayerInfo[playerid][e_iAction] != PLAYER_ACTION_SHOOTING)
	{
    	CallLocalFunction("OnPlayerActionChange", "iii", playerid, s_aPlayerInfo[playerid][e_iAction], PLAYER_ACTION_SHOOTING);
    	s_aPlayerInfo[playerid][e_iAction] = PLAYER_ACTION_SHOOTING;
	}
	s_aPlayerTickInfo[playerid][e_PLAYER_LAST_SHOT] = GetConnectedTime(playerid);

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
		Float:health,
		animlib[32],
		animname[32];

	if(IsPlayerSkydiving(playerid) && s_aPlayerInfo[playerid][e_iAction] != PLAYER_ACTION_SKYDIVING)
	{
 		CallLocalFunction("OnPlayerActionChange", "iii", playerid, s_aPlayerInfo[playerid][e_iAction], PLAYER_ACTION_SKYDIVING);
		s_aPlayerInfo[playerid][e_iAction] = PLAYER_ACTION_SKYDIVING;
	}
	else if(!IsPlayerSkydiving(playerid) && s_aPlayerInfo[playerid][e_iAction] == PLAYER_ACTION_SKYDIVING)
	{
	    CallLocalFunction("OnPlayerActionChange", "iii", playerid, s_aPlayerInfo[playerid][e_iAction], PLAYER_ACTION_NONE);
	    s_aPlayerInfo[playerid][e_iAction] = PLAYER_ACTION_SKYDIVING;
	}
	else if(IsPlayerSwimming(playerid) && s_aPlayerInfo[playerid][e_iAction] != PLAYER_ACTION_SWIMMING)
	{
	    CallLocalFunction("OnPlayerActionChange", "iii", playerid, s_aPlayerInfo[playerid][e_iAction], PLAYER_ACTION_SWIMMING);
		s_aPlayerInfo[playerid][e_iAction] = PLAYER_ACTION_SWIMMING;
	}
	else if(!IsPlayerSwimming(playerid) && s_aPlayerInfo[playerid][e_iAction] == PLAYER_ACTION_SWIMMING)
	{
	    CallLocalFunction("OnPlayerActionChange", "iii", playerid, s_aPlayerInfo[playerid][e_iAction], PLAYER_ACTION_NONE);
	    s_aPlayerInfo[playerid][e_iAction] = PLAYER_ACTION_NONE;
	}
    else if(s_aPlayerInfo[playerid][e_iAction] == PLAYER_ACTION_SHOOTING && (GetConnectedTime(playerid) - s_aPlayerTickInfo[playerid][e_PLAYER_LAST_SHOT]) > 1200)
	{
	    CallLocalFunction("OnPlayerActionChange", "iii", playerid, s_aPlayerInfo[playerid][e_iAction], PLAYER_ACTION_NONE);
	    s_aPlayerInfo[playerid][e_iAction] = PLAYER_ACTION_NONE;
	}

	if((GetConnectedTime(playerid) - s_aPlayerTickInfo[playerid][e_PLAYER_BURN_TICK]) >= 1500 && Callback_GetFlag(playerid, e_bBurning))
	{
		Callback_SetFlag(playerid, e_bBurning, false);
 		CallLocalFunction("OnPlayerStopBurn", "i", playerid);
	}

	switch(GetPlayerState(playerid))
	{
		case PLAYER_STATE_ONFOOT:
		{
 			if(!s_aPlayerInfo[playerid][e_iTargetID] && GetPlayerTargetPlayer(playerid) == INVALID_PLAYER_ID)
			{
				s_aPlayerInfo[playerid][e_iTargetID] = INVALID_PLAYER_ID;
			}
			else if(GetPlayerTargetPlayer(playerid) != s_aPlayerInfo[playerid][e_iTargetID])
			{
  				CallLocalFunction("OnPlayerTargetPlayer", "iii", playerid, s_aPlayerInfo[playerid][e_iTargetID], GetPlayerWeapon(playerid));
				s_aPlayerInfo[playerid][e_iTargetID] = GetPlayerTargetPlayer(playerid);
			}

  			if(!Callback_GetFlag(playerid, e_bReloading) && (WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) && GetPlayerWeaponState(playerid) == WEAPONSTATE_RELOADING)
   			{
			   	Callback_SetFlag(playerid, e_bReloading, true);
				CallLocalFunction("OnPlayerReloadWeapon", "iii", playerid, weaponid, GetPlayerAmmo(playerid));
			}
			else if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING)
			{
   				Callback_SetFlag(playerid, e_bReloading, false);
			}

		    for(new i = 0; i < sizeof(s_aVendingMachines); i ++)
		    {
		        if(IsPlayerInRangeOfPoint(playerid, 2.0, s_aVendingMachines[i][e_VENDING_MACHINE_X], s_aVendingMachines[i][e_VENDING_MACHINE_Y], s_aVendingMachines[i][e_VENDING_MACHINE_Z]))
		        {
					if(GetPlayerAnimationIndex(playerid) == 1660 && (GetConnectedTime(playerid) - s_aPlayerTickInfo[playerid][e_PLAYER_VENDING_TICK]) > 3500)
					{
						CallLocalFunction("OnPlayerUseVending", "ii", playerid, s_aVendingMachines[i][e_VENDING_MACHINE_TYPE]);
						s_aPlayerTickInfo[playerid][e_PLAYER_VENDING_TICK] = GetConnectedTime(playerid);
						return 1;
					}
				}
			}

			if((GetConnectedTime(playerid) - s_aPlayerTickInfo[playerid][e_PLAYER_LAST_JUMP] >= 1000) && IsPlayerJumping(playerid))
			{
   				CallLocalFunction("OnPlayerJump", "i", playerid);
			    s_aPlayerTickInfo[playerid][e_PLAYER_LAST_JUMP] = GetConnectedTime(playerid);
			}
		}
		case PLAYER_STATE_DRIVER:
		{
	        new
	    		money = GetPlayerMoney(playerid);

			if(s_aPlayerInfo[playerid][e_iMoney] != money)
			{
	    	    if(GetPlayerCameraMode(playerid) == 15)
	        	{
	        	    switch(s_aPlayerInfo[playerid][e_iMoney] - money)
	            	{
	                	case 100:
		                {
                    	    if(IsPlayerNearGarage(playerid, GARAGE_PAYNSPRAY))
                    	    {
                    	        CallLocalFunction("OnPlayerUseGarage", "iii", playerid, GetPlayerVehicleID(playerid), GARAGE_PAYNSPRAY);
	                        }
	                    }
	                    case 500:
		                {
                    	    if(IsPlayerNearGarage(playerid, GARAGE_BOMBSHOP))
                    	    {
                    	        CallLocalFunction("OnPlayerUseGarage", "iii", playerid, GetPlayerVehicleID(playerid), GARAGE_BOMBSHOP);
	                        }
	                    }
	                }
	            }
	            s_aPlayerInfo[playerid][e_iMoney] = money;
	        }

	 		new
	   			vehicleid = GetPlayerVehicleID(playerid),
			   	panels,
			   	doors,
			   	lights,
			   	tires
	   		;

			GetVehicleHealth(vehicleid, health);

			if(s_aVehicleInfo[vehicleid][e_fHealth] > health && floatabs(s_aVehicleInfo[vehicleid][e_fHealth] - health) >= POTENTIAL_CRASH_DAMAGE && floatabs(s_aVehicleInfo[vehicleid][e_fSpeed] - Callback_GetSpeed(vehicleid)) >= 5.0)
			{
				GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

			    if(s_aVehicleInfo[vehicleid][e_iPanels] != panels || s_aVehicleInfo[vehicleid][e_iDoors] != doors || s_aVehicleInfo[vehicleid][e_iLights] != lights)
			    {
					CallLocalFunction("OnPlayerCrashVehicle", "iif", playerid, vehicleid, floatsub(s_aVehicleInfo[vehicleid][e_fHealth], health));
				}

				GetVehicleDamageStatus(vehicleid, s_aVehicleInfo[vehicleid][e_iPanels], s_aVehicleInfo[vehicleid][e_iDoors], s_aVehicleInfo[vehicleid][e_iLights], tires);
			}
			s_aVehicleInfo[vehicleid][e_fHealth] = health;
			s_aVehicleInfo[vehicleid][e_fSpeed] = Callback_GetSpeed(vehicleid);
		}
	}

    new
        difference = s_aPlayerInfo[playerid][e_iDrunk] - level;

	if(GetPlayerDrunkLevel(playerid) < 100)
	{
		SetPlayerDrunkLevel(playerid, 2000);
	}
	else if(s_aPlayerInfo[playerid][e_iDrunk] != level)
	{
	    if((0 <= difference <= 200) && s_aPlayerInfo[playerid][e_iFrames] != difference)
	    {
	        if(s_aPlayerInfo[playerid][e_iFrames] < 0)
	        {
	            s_aPlayerInfo[playerid][e_iFrames] = 0;
	        }
			CallLocalFunction("OnPlayerFPSChange", "iii", playerid, s_aPlayerInfo[playerid][e_iFrames], difference);
		}
		s_aPlayerInfo[playerid][e_iFrames] = difference;
		s_aPlayerInfo[playerid][e_iDrunk] = level;
	}

	new
		Float:packet = NetStats_PacketLossPercent(playerid);

	if(floatcmp(packet, 0.0) != 0 && floatcmp(packet, s_aPlayerInfo[playerid][e_fPacket]) != 0)
	{
	    CallLocalFunction("OnPlayerPacketLoss", "iff", playerid, s_aPlayerInfo[playerid][e_fPacket], packet);
		s_aPlayerInfo[playerid][e_fPacket] = packet;
	}

    if(Callback_GetFlag(playerid, e_bAnimSet) && s_aPlayerInfo[playerid][e_iAnimIndex] != GetPlayerAnimationIndex(playerid))
    {
        Callback_SetFlag(playerid, e_bAnimSet, false);

		GetAnimationName(s_aPlayerInfo[playerid][e_iAnimIndex], animlib, sizeof(animlib), animname, sizeof(animname));
        
        if(strlen(animlib) > 0 && strlen(animname) > 0)
        {
	        CallLocalFunction("OnPlayerAnimationPlay", "iss", playerid, animlib, animname);
		}
	}

    new
        ammo = GetPlayerAmmo(playerid);

    if(s_aPlayerInfo[playerid][e_iLastAmmo] != ammo)
    {
        if(GetPlayerWeapon(playerid) == WEAPON_CAMERA && s_aPlayerInfo[playerid][e_iLastAmmo] > ammo)
        {
            CallLocalFunction("OnPlayerUseCamera", "i", playerid);
        }
        else if(GetPlayerWeapon(playerid) == WEAPON_SPRAYCAN && s_aPlayerInfo[playerid][e_iLastAmmo] > ammo)
        {
			for(new i = 1, l = GetVehiclePoolSize(); i <= l; i ++)
			{
			    if(IsPlayerSprayingVehicle(playerid, i))
            	{
					CallLocalFunction("OnPlayerSprayAtVehicle", "ii", playerid, i);
					break;
				}
			}
		}
        s_aPlayerInfo[playerid][e_iLastAmmo] = ammo;
	}

	if(s_aPlayerInfo[playerid][e_iLastGun] != weaponid)
	{
	    new
			lastweapon = s_aPlayerInfo[playerid][e_iLastGun];

		if((WEAPON_COLT45 <= lastweapon <= WEAPON_TEC9) && lastweapon != WEAPON_SHOTGUN)
		{
			s_aPlayerInfo[playerid][e_iShotsFired][lastweapon - WEAPON_COLT45] = 0;
		}
		s_aPlayerInfo[playerid][e_iLastGun] = weaponid;
	}
	else
	{
		if((WEAPON_COLT45 <= weaponid <= WEAPON_TEC9) && weaponid != WEAPON_SHOTGUN && GetPlayerWeaponState(playerid) == WEAPONSTATE_RELOADING)
		{
			s_aPlayerInfo[playerid][e_iShotsFired][weaponid - WEAPON_COLT45] = 0;
		}
	}

	if(!Callback_GetFlag(playerid, e_bUpdate))
	{
		Callback_SetFlag(playerid, e_bUpdate, true);
	}

	s_aPlayerTickInfo[playerid][e_PLAYER_LAST_UPDATE] = GetConnectedTime(playerid);

	#if defined CB_OnPlayerUpdate
		return CB_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	new
		vehicleid = GetPlayerVehicleID(playerid);

	switch(newstate)
	{
    	case PLAYER_STATE_DRIVER:
	    {
    	    GetVehicleHealth(vehicleid, s_aVehicleInfo[vehicleid][e_fHealth]);
			s_aPlayerInfo[playerid][e_iVehicleID] = vehicleid;
		}
		case PLAYER_STATE_WASTED:
		{
		    s_aPlayerInfo[playerid][e_iAction] = PLAYER_ACTION_NONE;
		}
		case PLAYER_STATE_ONFOOT:
		{
		    if(oldstate == PLAYER_STATE_DRIVER && GetVehicleModel(s_aPlayerInfo[playerid][e_iVehicleID]))
		    {
		    	new
					driverid = Callback_GetDriver(s_aPlayerInfo[playerid][e_iVehicleID]);

				if(driverid != INVALID_PLAYER_ID)
				{
					CallLocalFunction("OnPlayerJackVehicle", "iii", driverid, playerid, s_aPlayerInfo[playerid][e_iVehicleID]);
				}

				s_aPlayerInfo[playerid][e_iVehicleID] = INVALID_VEHICLE_ID;
			}
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
	new
		weaponid = GetPlayerWeapon(playerid);

	if(GetPlayerState(playerid) != PLAYER_STATE_SPECTATING && GetPlayerState(playerid) != PLAYER_STATE_SPAWNED && GetPlayerState(playerid) != PLAYER_STATE_WASTED)
	{
		if(!Callback_GetFlag(playerid, e_bHoldKey))
		{
		    Callback_SetFlag(playerid, e_bHoldKey, true);

	    	s_aPlayerTickInfo[playerid][e_PLAYER_TICK] = GetConnectedTime(playerid);
			s_aPlayerInfo[playerid][e_iHoldTimer] = SetTimerEx("Callback_HoldTimer", 200, true, "ii", playerid, newkeys);
		}

		if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
		{
			if((WEAPON_COLT45 <= weaponid <= WEAPON_CAMERA) && weaponid != WEAPON_BOMB)
			{
				if(!Callback_GetFlag(playerid, e_bAiming) && (newkeys & KEY_HANDBRAKE) & KEY_HANDBRAKE)
				{
	                Callback_SetFlag(playerid, e_bAiming, true);
		    		CallLocalFunction("OnPlayerStartAim", "ii", playerid, weaponid);
				}
				else if((newkeys & KEY_HANDBRAKE) != KEY_HANDBRAKE && (oldkeys & KEY_HANDBRAKE) == KEY_HANDBRAKE)
				{
				    Callback_SetFlag(playerid, e_bAiming, false);
				    CallLocalFunction("OnPlayerStopAim", "i", playerid);
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
	if(clickedid == Text:INVALID_TEXT_DRAW && Callback_GetFlag(playerid, e_bSelectTD))
	{
	   	Callback_SetFlag(playerid, e_bSelectTD, false);
		CallLocalFunction("OnPlayerHideCursor", "ii", playerid, s_aPlayerInfo[playerid][e_iHoverColor]);
	}

	#if defined CB_OnPlayerClickTextDraw
		return CB_OnPlayerClickTextDraw(playerid, clickedid);
	#else
		return 0;
	#endif
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if(weaponid == WEAPON_COLLISION && issuerid == INVALID_PLAYER_ID && amount >= 4.95)
	{
		CallLocalFunction("OnPlayerFall", "if", playerid, amount);
	}

   	if(weaponid == WEAPON_VEHICLE && issuerid != INVALID_PLAYER_ID)
	{
    	CallLocalFunction("OnPlayerRamPlayer", "iiif", playerid, issuerid, GetPlayerVehicleID(issuerid), amount);
	}

	if(weaponid == WEAPON_FLAMETHROWER)
	{
 		if(!Callback_GetFlag(playerid, e_bBurning))
   		{
     		Callback_SetFlag(playerid, e_bBurning, true);
      		CallLocalFunction("OnPlayerStartBurn", "i", playerid);
		}
		s_aPlayerTickInfo[playerid][e_PLAYER_BURN_TICK] = GetConnectedTime(playerid);
	}

	#if defined CB_OnPlayerTakeDamage
		return CB_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}

// --------------------------------------------------
// Timers
// --------------------------------------------------

forward Callback_CheckAnimation(playerid);
public Callback_CheckAnimation(playerid)
{
	s_aPlayerInfo[playerid][e_iAnimIndex] = GetPlayerAnimationIndex(playerid);

	Callback_SetFlag(playerid, e_bAnimSet, true);
}

forward Callback_HoldTimer(playerid, key);
public Callback_HoldTimer(playerid, key)
{
    new
		keys,
		ud,
		lr;

    GetPlayerKeys(playerid, keys, ud, lr);

    if(keys & key)
    {
        Callback_SetFlag(playerid, e_bKeyPress, true);
        CallLocalFunction("OnPlayerHoldingKey", "ii", playerid, key);
    }
    else
    {
        if(Callback_GetFlag(playerid, e_bKeyPress))
       	{
           	CallLocalFunction("OnPlayerReleaseKey", "iii", playerid, key, GetConnectedTime(playerid) - s_aPlayerTickInfo[playerid][e_PLAYER_TICK]);
       	}

       	KillTimer(s_aPlayerInfo[playerid][e_iHoldTimer]);

    	Callback_SetFlag(playerid, e_bKeyPress, false);
    	Callback_SetFlag(playerid, e_bHoldKey, false);
    }
}

forward Callback_TabCheck();
public Callback_TabCheck()
{
	for(new i = 0, l = GetPlayerPoolSize(); i <= l; i ++)
	{
	    switch(GetPlayerState(i))
	    {
	        case PLAYER_STATE_ONFOOT, PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
	        {
				if(Callback_GetFlag(i, e_bUpdate))
				{
					if(Callback_GetFlag(i, e_bPaused))
  					{
						CallLocalFunction("OnPlayerResume", "ii", i, GetConnectedTime(i) - s_aPlayerTickInfo[i][e_PLAYER_TICK]);
					}

		            Callback_SetFlag(i, e_bUpdate, false);
		            Callback_SetFlag(i, e_bPaused, false);
				}

				if(!Callback_GetFlag(i, e_bPaused) && (GetConnectedTime(i) - s_aPlayerTickInfo[i][e_PLAYER_LAST_UPDATE]) >= 4000)
				{
				    CallLocalFunction("OnPlayerPause", "i", i);

					s_aPlayerTickInfo[i][e_PLAYER_TICK] = GetConnectedTime(i);

					Callback_SetFlag(i, e_bPaused, true);
				}
			}
		}
	}
}

// --------------------------------------------------
// Native hooks
// --------------------------------------------------

#if defined _ALS_CreateVehicle
	#undef CreateVehicle
#else
	#define _ALS_CreateVehicle
#endif

#if defined _ALS_AddStaticVehicle
	#undef AddStaticVehicle
#else
	#define _ALS_AddStaticVehicle
#endif

#if defined _ALS_AddStaticVehicleEx
	#undef AddStaticVehicleEx
#else
	#define _ALS_AddStaticVehicleEx
#endif

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

#define CreateVehicle           CB_CreateVehicle
#define AddStaticVehicle        CB_AddStaticVehicle
#define AddStaticVehicleEx      CB_AddStaticVehicleEx
#define ApplyAnimation          CB_ApplyAnimation
#define SelectTextDraw          CB_SelectTextDraw
#define CancelSelectTextDraw    CB_CancelSelectTextDraw

// --------------------------------------------------
// Callback hooks
// --------------------------------------------------

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
