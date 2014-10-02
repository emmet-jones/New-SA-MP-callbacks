New-SA-MP-callbacks
===================

This is a collection of useful callbacks for SA-MP, that works with every SA-MP version.

List of Callbacks
===================
The following callbacks are included with this library:

```
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
```

Functions
===================

External functions for this library:

```
// Returns 1 if the player is paused.
stock IsPlayerPaused(playerid);

// Returns 1 if the player is holding the key.
stock IsPlayerHoldingKey(playerid, keys);

// Returns the player's packet loss.
stock Float:GetPlayerPacketLoss(playerid);

// Returns the time that the player has been paused.
stock GetPlayerPausedTime(playerid);

// Returns 1 if the player is near any vending machine.
stock IsPlayerNearVending(playerid);

// Returns the player's FPS amount.
stock GetPlayerFPS(playerid);
```

Example
===================

```
public OnPlayerResume(playerid, time)
{
    new
        str[48];

    format(str, sizeof(str), "You've been paused for %d milliseconds.", time);
    return SendClientMessage(playerid, -1, str);
}

public OnPlayerCrashVehicle(playerid, vehicleid, Float:damage)
{ 
    SendClientMessage(playerid, 0xFF0000FF, "You have crashed your vehicle!");
    SendClientMessage(playerid, 0xFF0000FF, "Learn how to drive, please!");
    return 1;
}
```
