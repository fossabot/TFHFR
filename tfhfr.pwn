#include                <a_samp>

#define                 BitFlag_Get(%0,%1)              ((%0) & (%1))   // Returns zero (false) if the flag isn't set.
#define                 BitFlag_On(%0,%1)               ((%0) |= (%1))  // Turn on a flag.
#define                 BitFlag_Off(%0,%1)              ((%0) &= ~(%1)) // Turn off a flag.
#define                 BitFlag_Toggle(%0,%1)           ((%0) ^= (%1))  // Toggle a flag (swap true/false).

#define                 FIXES_ServerVarMsg              (0)
#include                <fixes>

#include                <YSI\y_ini>
#include                <YSI\y_dialog>

#include                <sscanf2>

#undef                  MAX_PLAYERS
#define                 MAX_PLAYERS                     (10)
#define                 MAX_USERNAME                    (MAX_PLAYER_NAME + 1)
#define                 MAX_PASSWORD                    (65)
#define                 MAX_SALT                        (16)
#define                 MAX_DATE                        (17 + 1)

#define                 MELEE_WEAPON                    (1001)
#define                 HANDGUN_WEAPON                  (1002)
#define                 SMG_WEAPON                      (1003)
#define                 SHOTGUN_WEAPON                  (1004)
#define                 RIFLE_WEAPON                    (1005)
#define                 SNIPER_WEAPON                   (1006)
#define                 OTHER_WEAPON                    (1007)

enum account{
    username[MAX_USERNAME],
    password[MAX_PASSWORD],
    salt[MAX_SALT],
    meleekills,
    handgunkills,
    shotgunkills,
    smgkills,
    riflekills,
    sniperkills,
    deaths,
    skins,
    referredby[MAX_USERNAME],
    referralpoints,
    playingtime,
    dateregistered[MAX_DATE]
}

enum PlayerFlags:(<<=1){
    LOGGED_IN = 1,
}


enum {
    LOAD_DATA, SAVE_DATA, EMPTY_DATA
}

enum {
    EMPTY, LOGIN, REGISTER, SHORT_REGISTER, REFERRAL_REGISTER, NOREFERRAL_REGISTER
}

new 
    PA[MAX_PLAYERS][account],
    PlayerFlags:PF[MAX_PLAYERS char]
    ;

AP(const name[]){
    new string[15 + MAX_USERNAME];
    format(string, sizeof string, "Accounts/%s.ini", name);
    return string;
}

AccountQuerries(const playerid, const type){
    switch(type){
        case LOAD_DATA:{
            inline LoadData(string:name[],string:value[]){
                INI_String("Password", PA[playerid][password], MAX_PASSWORD);
                INI_String("Salt", PA[playerid][salt], MAX_SALT);
                INI_Int("Melee", PA[playerid][meleekills]);
                INI_Int("Handgun", PA[playerid][handgunkills]);
                INI_Int("Shotgun", PA[playerid][shotgunkills]);
                INI_Int("SMG", PA[playerid][smgkills]);
                INI_Int("Rifle", PA[playerid][riflekills]);
                INI_Int("Sniper", PA[playerid][sniperkills]);
                INI_Int("Deaths", PA[playerid][deaths]);
                INI_Int("Skin", PA[playerid][skins]);
                INI_String("Referredby", PA[playerid][referredby]);
                INI_Int("Referralpoints", PA[playerid][referralpoints]);
                INI_Int("PlayingTime", PA[playerid][playingtime]);
                INI_String("DateRegistered", PA[playerid][dateregistered]);
            }
            INI_ParseFile(AP(PA[playerid][username]), using inline LoadData);
        }
        case SAVE_DATA:{

            new INI:File = INI_Open(AP(PA[playerid][username]));

            INI_SetTag(File, "Information");
            INI_WriteString(File, "DateRegistered", PA[playerid][dateregistered]);
            INI_WriteInt(File, "PlayingTime", PA[playerid][playingtime]);
            INI_WriteInt(File, "Referralpoints", PA[playerid][referralpoints]);
            INI_WriteString(File, "Referredby", PA[playerid][referredby]);
            INI_WriteInt(File, "Skin", PA[playerid][skins]);
            INI_WriteInt(File, "Deaths", PA[playerid][deaths]);
            INI_WriteInt(File, "Sniper", PA[playerid][sniperkills]);
            INI_WriteInt(File, "Rifle", PA[playerid][riflekills]);
            INI_WriteInt(File, "SMG", PA[playerid][smgkills]);
            INI_WriteInt(File, "Shotgun", PA[playerid][shotgunkills]);
            INI_WriteInt(File, "Handgun", PA[playerid][handgunkills]);
            INI_WriteInt(File, "Melee", PA[playerid][meleekills]);
            INI_WriteString(File, "Salt", PA[playerid][salt]);
            INI_WriteString(File, "Password", PA[playerid][password]);
            INI_Close(File);
        }
        case EMPTY_DATA:{
            format(PA[playerid][username], MAX_USERNAME, "");
            format(PA[playerid][password], MAX_PASSWORD, "");
            format(PA[playerid][salt], MAX_SALT, "");
            format(PA[playerid][referredby], MAX_USERNAME, "");
            format(PA[playerid][dateregistered], MAX_DATE, "");
            PA[playerid][meleekills] = PA[playerid][handgunkills] = PA[playerid][shotgunkills] =
            PA[playerid][smgkills] = PA[playerid][riflekills] = PA[playerid][sniperkills] =
            PA[playerid][deaths] = PA[playerid][skins] = PA[playerid][referralpoints] =
            PA[playerid][playingtime] = 0;

            BitFlag_Off(PF{ playerid }, LOGGED_IN);
        }
    }
    return 1;
}

doSalt(playerid){
    new container[MAX_SALT];
    for(new i = 0, j = MAX_SALT; i < j; i++){
        format(container[i], MAX_SALT, "%d", random(10));
    }
    format(PA[playerid][salt], MAX_SALT, "%s", container);
    return 1;
}

PlayerDialog(playerid, dialog){
    switch(dialog){
        case REGISTER:{
            inline register(pid, dialogid, response, listitem, string:inputtext[]){
                #pragma unused pid, dialogid, listitem
                if(response){
                    doSalt(playerid);
                    if(strlen(inputtext) >= 6 && strlen(inputtext) <= 12){
                        SHA256_PassHash(inputtext, PA[playerid][salt], PA[playerid][password], MAX_PASSWORD);
                        PlayerDialog(playerid, REFERRAL_REGISTER);
                    }else{
                        PlayerDialog(playerid, SHORT_REGISTER);
                    }
                }
            }
            Dialog_ShowCallback(playerid, using inline register, DIALOG_STYLE_PASSWORD, "The Four Horsemen - Registration", "Welcome to The Four Horseman Deathmatch Arena\nRegister your password below to start playing.", "Submit");
        }
        case SHORT_REGISTER:{
            inline register(pid, dialogid, response, listitem, string:inputtext[]){
                #pragma unused pid, dialogid, listitem
                if(response){
                    doSalt(playerid);
                    if(strlen(inputtext) >= 6 && strlen(inputtext) <= 12){
                        SHA256_PassHash(inputtext, PA[playerid][salt], PA[playerid][password], MAX_PASSWORD);
                        PlayerDialog(playerid, REFERRAL_REGISTER);
                    }else{
                        PlayerDialog(playerid, SHORT_REGISTER);
                    }
                }
            }
            Dialog_ShowCallback(playerid, using inline register, DIALOG_STYLE_PASSWORD, "The Four Horsemen - Registration", "Your password is too short.\nPlease retype it and make sure that it is longer than 5 characters and shorter than 13 characters", "Submit");
        }
        case REFERRAL_REGISTER:{
            inline register(pid, dialogid, response, listitem, string:inputtext[]){
                #pragma unused pid, dialogid, listitem
                if(response){
                    format(PA[playerid][referredby], MAX_USERNAME, "%s", inputtext);
                    if(fexist(AP(PA[playerid][referredby]))){
                        format(PA[playerid][dateregistered], MAX_DATE, "%s", getdate());
                        AccountQuerries(playerid, SAVE_DATA);
                    }else{
                        format(PA[playerid][referredby], MAX_USERNAME, "");
                        PlayerDialog(playerid, NOREFERRAL_REGISTER);
                    }
                }else{
                    format(PA[playerid][dateregistered], MAX_DATE, "%s", getdate());
                    AccountQuerries(playerid, SAVE_DATA);
                }
            }
            Dialog_ShowCallback(playerid, using inline register, DIALOG_STYLE_INPUT, "The Four Horseman - Registration", "Please type below who referred you into our server to give them credit", "Submit", "Skip");
        }
        case NOREFERRAL_REGISTER:{
            inline register(pid, dialogid, response, listitem, string:inputtext[]){
                #pragma unused pid, dialogid, listitem
                if(response){
                    format(PA[playerid][referredby], MAX_USERNAME, "%s", inputtext);
                    if(fexist(AP(PA[playerid][referredby]))){
                        format(PA[playerid][dateregistered], MAX_DATE, "%s", getdate());
                        AccountQuerries(playerid, SAVE_DATA);
                    }else{
                        format(PA[playerid][referredby], MAX_USERNAME, "");
                        PlayerDialog(playerid, NOREFERRAL_REGISTER);
                    }
                }else{
                    format(PA[playerid][dateregistered], MAX_DATE, "%s", getdate());
                    AccountQuerries(playerid, SAVE_DATA);
                }
            }
            Dialog_ShowCallback(playerid, using inline register, DIALOG_STYLE_INPUT, "The Four Horseman - Registration", "Our system had not found this user in our system.\nPlease retype and remember that our system is case sensitive.", "Submit", "Skip");
        }
    }
    return 1;
}

regulatePlayerOnConnect(playerid){
    SetSpawnInfo(playerid, NO_TEAM, 299, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
    TogglePlayerSpectating(playerid, true);
    TogglePlayerControllable(playerid, false);
    return 1;
}

PlayerWeapon(const weaponid){
    new weapontype;
    if(weaponid >= 0 && weaponid <= 15){weapontype = MELEE_WEAPON;}
    return weapontype;
}

main(){}

public OnGameModeInit(){
    UsePlayerPedAnims();
    EnableStuntBonusForAll(0);
    DisableInteriorEnterExits();
    SetGameModeText("Deathmatch Arena");
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
            case MELEE_WEAPON:{

            }
        }
    }
    return 1;
}