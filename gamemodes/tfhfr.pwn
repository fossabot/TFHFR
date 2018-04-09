

#include                <a_samp>

#define                 FIXES_ServerVarMsg              (0)
#include                <fixes>

#include                "server.inc"

#include                "header.inc"

#include                "commands.inc"

main(){}

public OnGameModeInit(){
    UsePlayerPedAnims();
    DisableInteriorEnterExits();
    ServerQuery(EMPTY_DATA);
    if(!fexist((INFO_PATH))){
        getdate(SA[launchyear], SA[launchmonth], SA[launchdate]);
        ServerQuery(SAVE_DATA);
    }
    else{ServerQuery(LOAD_DATA);}
    return 1;
}

public OnGameModeExit(){
    foreach( new playerid : Player){
        if(BitFlag_Get(PF{ playerid }, LOGGED_IN)){
            OnPlayerDisconnect(playerid, 0);
        }
    }
    return 1;
}

public OnPlayerConnect(playerid){
    regulatePlayerOnConnect(playerid);
    AccountQuerries(playerid, EMPTY_DATA);
    PF{ playerid } = PlayerFlags:0;
    GetPlayerName(playerid, PA[playerid][username], MAX_USERNAME);
    if(fexist(AP(PA[playerid][username]))){
        AccountQuerries(playerid, LOAD_DATA);
        PlayerDialog(playerid, LOGIN);
    }else{PlayerDialog(playerid, REGISTER);}
    if(GetPlayerPoolSize() > SA[mostonline]){
        SA[mostonline] = GetPlayerPoolSize();
        ServerQuery(SAVE_DATA;        
    }
    return 1;
}

public OnPlayerDisconnect(playerid, reason){
    if(BitFlag_Get(PF{ playerid }, LOGGED_IN)){
        AccountQuerries(playerid, SAVE_DATA);
        AccountQuerries(playerid, EMPTY_DATA);
    }
    return 1;
}

public OnPlayerDeath(playerid, killerid, reason){
    if(killerid != INVALID_PLAYER_ID){
        switch(PlayerWeapon(reason)){
            case MELEE_WEAPON:{PA[killerid][meleekills]++;}
            case HANDGUN_WEAPON:{PA[killerid][handgunkills]++;}
            case SMG_WEAPON:{PA[killerid][smgkills]++;}
            case SHOTGUN_WEAPON:{PA[killerid][shotgunkills]++;}
            case RIFLE_WEAPON:{PA[killerid][riflekills]++;}
            case SNIPER_WEAPON:{PA[killerid][sniperkills]++;}
            case OTHER_WEAPON:{PA[playerid][otherkills]++;}
        }
        PA[playerid][deaths]++;
        SA[serverfatality]++;
        AccountQuerries(killerid, SAVE_DATA);
        AccountQuerries(playerid, SAVE_DATA);
        KillCount(playerid);
        ServerQuery(SAVE_DATA;        
    }
    return 1;
}

#include                 "impl.inc"
#include                <YSI\y_timers>

task regularcheck[300000]()
{
    foreach(new playerid : Player){
        if(BitFlag_Get(PF{ playerid }, LOGGED_IN)){
            doPlayerCheck(playerid);
            KillCount(playerid);
            AccountQuerries(playerid, SAVE_DATA);
        }
    }
    return 1;
}

task hourlycheck[3600000]()
{
    foreach(new playerid : Player){
        if(BitFlag_Get(PF{ playerid }, LOGGED_IN)){
            PA[playerid][playingtime]++;
            AccountQuerries(playerid, SAVE_DATA);
        }
    }
    return 1;
}