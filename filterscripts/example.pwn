/*
	New SA-MP callbacks by Emmet

	This filterscripts simply defines all of the callbacks.
	You don't need this script to run callbacks.inc.
*/
#include <a_samp>
#include <callbacks>

public OnPlayerPause(playerid)
{
	return 1;
}

public OnPlayerUseVending(playerid)
{
	return 1;
}

public OnPlayerResume(playerid, time)
{
	return 1;
}

public OnPlayerHoldingKey(playerid, keys)
{
	return 1;
}

public OnPlayerReleaseKey(playerid, keys, time)
{
	return 1;
}

public OnPlayerFall(playerid, Float:height, Float:damage)
{
	return 1;
}

public OnPlayerPacketLoss(playerid, Float:oldloss, Float:newloss)
{
	return 1;
}

public OnPlayerCrashVehicle(playerid, vehicleid, Float:damage)
{
	return 1;
}

public OnPlayerFPSChange(playerid, oldfps, newfps)
{
	return 1;
}

public OnPlayerCarJack(playerid, targetid, vehicleid)
{
	return 1;
}

public OnPlayerWeaponEmpty(playerid, weaponid)
{
	return 1;
}

public OnPlayerFriendlyFire(playerid, targetid, weaponid)
{
	return 1;
}

public OnPlayerTargetPlayer(playerid, targetid, weaponid)
{
	return 1;
}

public OnPlayerDisableCursor(playerid, hovercolor)
{
	return 1;
}

public OnPlayerAntiReload(playerid, weaponid)
{
	return 1;
}

public OnPlayerAnimationFinish(playerid, animlib[], animname[])
{
    return 1;
}

public OnPlayerReloadWeapon(playerid, weaponid, ammo)
{
    return 1;
}

public OnPlayerBurning(playerid, status)
{
    return 1;
}

public OnPlayerAiming(playerid, weaponid, status)
{
    return 1;
}

public OnPlayerTakePicture(playerid)
{
    return 1;
}

