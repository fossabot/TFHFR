#include                <YSI\y_ini>
#include                <YSI\y_dialog>

#define                 BitFlag_Get(%0,%1)              ((%0) & (%1))   // Returns zero (false) if the flag isn't set.
#define                 BitFlag_On(%0,%1)               ((%0) |= (%1))  // Turn on a flag.
#define                 BitFlag_Off(%0,%1)              ((%0) &= ~(%1)) // Turn off a flag.
#define                 BitFlag_Toggle(%0,%1)           ((%0) ^= (%1))  // Toggle a flag (swap true/false).

#define                 INFO_PATH                       ("info.ini")

#define                 MAX_USERNAME                    (MAX_PLAYER_NAME + 1)
#define                 MAX_PASSWORD                    (65)
#define                 MAX_SALT                        (16)
#define                 MAX_DATE                        (17 + 1)
#define                 MAX_VIP                         (3)

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
    otherkills,
    deaths,
    skins,
    referredby[MAX_USERNAME],
    referralpoints,
    playingtime,
    monthregistered,
    dateregistered,
    yearregistered,

    // personal data
    cash,
    credits,
    vip[MAX_VIP],
    vipexpirymonth,
    vipexpirydate,
    vipexpiryyear,
    vippoints,
    rewardpoints,
    frpoints,
    hitpoints,
    bool: privileged,
    privilexpirymonth,
    privilexpirydate,
    privilexpiryyear,
}

enum server{
    mostonline,
    serverfatality,
    launchmonth,
    launchdate,
    launchyear
}

enum PlayerFlags:(<<=1){
    LOGGED_IN = 1,
}


enum {
    LOAD_DATA, SAVE_DATA, EMPTY_DATA
}

enum {
    EMPTY, LOGIN, REGISTER, SHORT_REGISTER, REFERRAL_REGISTER, NOREFERRAL_REGISTER, EMPTY_FORVIP,

    RANDOM_SPAWN
}

new 
    PA[MAX_PLAYERS][account],
    PlayerFlags:PF[MAX_PLAYERS char],
    SA[server]
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
                INI_Int("MonthReg", PA[playerid][monthregistered]);
                INI_Int("DateReg", PA[playerid][dateregistered]);
                INI_Int("YearReg", PA[playerid][yearregistered]);
            
                INI_Int("Cash", PA[playerid][cash]);
                INI_Int("Credits", PA[playerid][credits]);
                INI_String("VIP", PA[playerid][vip], MAX_VIP);
                INI_Int("VIPExpiryMonth", PA[playerid][vipexpirymonth]);
                INI_Int("VIPExpiryDate", PA[playerid][vipexpirydate]);
                INI_Int("VIPExpiryYear", PA[playerid][vipexpiryyear]);
                INI_Int("VIPPoints", PA[playerid][vippoints]);
                INI_Int("Rewardpoints", PA[playerid][rewardpoints]);
                INI_Int("FRPoints", PA[playerid][frpoints]);
                INI_Int("hitpoints", PA[playerid][hitpoints]);
                INI_Bool("Privileged", PA[playerid][privileged]);
                INI_Int("PrivilExpiryMonth", PA[playerid][privilexpirymonth]);
                INI_Int("PrivilExpiryDate", PA[playerid][privilexpirydate]);
                INI_Int("PrivilExpiryYear", PA[playerid][privilexpiryyear]);
            }
            INI_ParseFile(AP(PA[playerid][username]), using inline LoadData);
        }
        case SAVE_DATA:{

            new INI:File = INI_Open(AP(PA[playerid][username]));

            INI_SetTag(File, "Information");
            INI_WriteInt(File, "PrivilExpiryYear", PA[playerid][privilexpiryyear]);
            INI_WriteInt(File, "PrivilExpiryDate", PA[playerid][privilexpirydate]);
            INI_WriteInt(File, "PrivilExpiryMonth", PA[playerid][privilexpirymonth]);
            INI_WriteBool(File, "Privileged", PA[playerid][privileged]);
            INI_WriteInt(File, "Hitpoints", PA[playerid][hitpoints]);
            INI_WriteInt(File, "FRPoints", PA[playerid][frpoints]);
            INI_WriteInt(File, "Rewardpoints", PA[playerid][rewardpoints]);
            INI_WriteInt(File, "VIPPoints", PA[playerid][vippoints]);
            INI_WriteInt(File, "VIPExpiryYear", PA[playerid][vipexpiryyear]);
            INI_WriteInt(File, "VIPExpiyDate", PA[playerid][vipexpirydate]);
            INI_WriteInt(File, "VIPExpiryMonth", PA[playerid][vipexpirymonth]);
            INI_WriteString(File, "VIP", PA[playerid][vip]);
            INI_WriteInt(File, "Credits", PA[playerid][credits]);
            INI_WriteInt(File, "Cash", PA[playerid][cash]);

            INI_WriteInt(File, "YearRegistered", PA[playerid][yearregistered]);
            INI_WriteInt(File, "DateRegistered", PA[playerid][dateregistered]);
            INI_WriteInt(File, "MonthRegistered", PA[playerid][monthregistered]);
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
            format(PA[playerid][vip], MAX_VIP, "");
            PA[playerid][meleekills] = PA[playerid][handgunkills] = PA[playerid][shotgunkills] =
            PA[playerid][smgkills] = PA[playerid][riflekills] = PA[playerid][sniperkills] =
            PA[playerid][deaths] = PA[playerid][skins] = PA[playerid][referralpoints] =
            PA[playerid][playingtime] = PA[playerid][cash] = PA[playerid][credits] =
            PA[playerid][vippoints] = PA[playerid][rewardpoints] = PA[playerid][frpoints] =
            PA[playerid][hitpoints] = PA[playerid][monthregistered] = PA[playerid][dateregistered] = 
            PA[playerid][yearregistered] = PA[playerid][vipexpirymonth] = PA[playerid][vipexpirydate] =
            PA[playerid][vipexpiryyear] = PA[playerid][privilexpirymonth] = PA[playerid][privilexpirydate] = 
            PA[playerid][privilexpiryyear] = 0;

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
            Dialog_ShowCallback(playerid, using inline register, DIALOG_STYLE_PASSWORD, "The Four Horsemen Project - Registration", "Welcome to The Four Horseman Deathmatch Arena\nRegister your password below to start playing.", "Submit");
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
            Dialog_ShowCallback(playerid, using inline register, DIALOG_STYLE_PASSWORD, "The Four Horsemen Project- Registration", "Your password is too short.\nPlease retype it and make sure that it is longer than 5 characters and shorter than 13 characters", "Submit");
        }
        case REFERRAL_REGISTER:{
            inline register(pid, dialogid, response, listitem, string:inputtext[]){
                #pragma unused pid, dialogid, listitem
                if(response){
                    format(PA[playerid][referredby], MAX_USERNAME, "%s", inputtext);
                    if(fexist(AP(PA[playerid][referredby]))){
                        getdate(PA[playerid][yearregistered], PA[playerid][monthregistered], PA[playerid][dateregistered]);
                        AccountQuerries(playerid, SAVE_DATA);
                        doSpawn(playerid, RANDOM_SPAWN);
                    }else{
                        format(PA[playerid][referredby], MAX_USERNAME, "");
                        PlayerDialog(playerid, NOREFERRAL_REGISTER);
                    }
                }else{
                    getdate(PA[playerid][yearregistered], PA[playerid][monthregistered], PA[playerid][dateregistered]);
                    AccountQuerries(playerid, SAVE_DATA);
                    doSpawn(playerid, RANDOM_SPAWN);
                }
            }
            Dialog_ShowCallback(playerid, using inline register, DIALOG_STYLE_INPUT, "The Four Horsemen Project - Registration", "Please type below who referred you into our server to give them credit", "Submit", "Skip");
        }
        case NOREFERRAL_REGISTER:{
            inline register(pid, dialogid, response, listitem, string:inputtext[]){
                #pragma unused pid, dialogid, listitem
                if(response){
                    format(PA[playerid][referredby], MAX_USERNAME, "%s", inputtext);
                    if(fexist(AP(PA[playerid][referredby]))){
                        getdate(PA[playerid][yearregistered], PA[playerid][monthregistered], PA[playerid][dateregistered]);
                        AccountQuerries(playerid, SAVE_DATA);
                        doSpawn(playerid, RANDOM_SPAWN);
                    }else{
                        format(PA[playerid][referredby], MAX_USERNAME, "");
                        PlayerDialog(playerid, NOREFERRAL_REGISTER);
                    }
                }else{
                    getdate(PA[playerid][yearregistered], PA[playerid][monthregistered], PA[playerid][dateregistered]);
                    AccountQuerries(playerid, SAVE_DATA);
                    doSpawn(playerid, RANDOM_SPAWN);
                }
            }
            Dialog_ShowCallback(playerid, using inline register, DIALOG_STYLE_INPUT, "The Four Horsemen Project - Registration", "Our system had not found this user in our system.\nPlease retype and remember that our system is case sensitive.", "Submit", "Skip");
        }
        case LOGIN:{
            inline login(pid, dialogid, response, listitem, string:inputtext[]){
                #pragma unused pid, dialogid, listitem
                if(response){
                    new ret_pass[MAX_PASSWORD];
                    SHA256_PassHash(inputtext, PA[playerid][salt], ret_pass, MAX_PASSWORD);
                    if(strcmp(PA[playerid][password], ret_pass, TRUE) == 0){
                        doSpawn(playerid, RANDOM_SPAWN);
                    }
                }
            }
            Dialog_ShowCallback(playerid, using inline login, DIALOG_STYLE_PASSWORD, "The Four Horsemen Project - Login", "Type in your password below so you can start going back to your progress again", "Login");
        }
        case EMPTY_FORVIP:{
            inline empty(pid, dialogid, response, listitem, string:inputtext[]){
                #pragma unused pid, dialogid, response, listitem, inputtext
            }
            Dialog_ShowCallback(playerid, using inline empty, DIALOG_STYLE_MSGBOX, "The Four Horsemen Project - VIP Expired", "Your vip has expired. If you wish to be a vip again. You can use your in-game credits to pay for it", "Close");
        }
    }
    return 1;
}

regulatePlayerOnConnect(playerid){
    SetSpawnInfo(playerid, NO_TEAM, 299, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
    TogglePlayerSpectating(playerid, true);
    TogglePlayerControllable(playerid, false);
    SetPlayerScore(playerid, 0);
    return 1;
}

PlayerWeapon(const weaponid){
    new weapontype;
    if(weaponid >= 0 && weaponid <= 15) weapontype = MELEE_WEAPON;
    else if(weaponid >= 22 && weaponid <= 24) weapontype = HANDGUN_WEAPON;
    else if(weaponid >= 25 && weaponid <= 27) weapontype = SHOTGUN_WEAPON;
    else if(weaponid >= 28 && weaponid <= 29 || weaponid == 32) weapontype = SMG_WEAPON;
    else if(weaponid >= 30 && weaponid <= 31) weapontype = RIFLE_WEAPON;
    else if(weaponid >= 33 && weaponid <= 34) weapontype = SNIPER_WEAPON;
    else weapontype = OTHER_WEAPON;
    return weapontype;
}

KillCount(const playerid){ 
    SetPlayerScore(playerid, PA[playerid][meleekills] + PA[playerid][handgunkills] + PA[playerid][smgkills] + PA[playerid][shotgunkills] +
    PA[playerid][riflekills] + PA[playerid][sniperkills] + PA[playerid][otherkills]);
    return 1;
}

doSpawn(const playerid, const type){
    switch(type){
        case RANDOM_SPAWN:{
            SetPlayerPos(playerid, 0.0, 0.0, 1.0);
            TogglePlayerSpectating(playerid, FALSE);
            TogglePlayerControllable(playerid, FALSE);
            doPlayerCheck(playerid);
            KillCount(playerid);
        }
    }
    return 1;
}

doPlayerCheck(const playerid){
    if(strcmp(PA[playerid][vip], "NV", TRUE) != 0){
        new d, m, y, expired;
        getdate(y, m, d);
        if(PA[playerid][vipexpiryyear] > y){
            expired = 1;
        }else if(PA[playerid][vipexpiryyear] == y){
            if(PA[playerid][vipexpirymonth] > m){
                expired = 1;
            }else if(PA[playerid][vipexpirymonth] == m){
                if(PA[playerid][vipexpirydate] >= d){
                    expired = 1;
                }
            }
        }
        if(expired == 1){
            format(PA[playerid][vip], MAX_VIP, "NV");
            PlayerDialog(playerid, EMPTY_FORVIP);
        }
    }
    return 1;
}

ServerQuery(const type){
    switch(type){
        case SAVE_DATA:{
            new INI:File = INI_Open(INFO_PATH);

            INI_SetTag(File, "Information");
            INI_WriteInt(File, "LaunchYear", SA[launchyear]);
            INI_WriteInt(File, "LaunchDate", SA[launchdate]);
            INI_WriteInt(File, "LaunchMonth", SA[launchmonth]);
            INI_WriteInt(File, "Fatality", SA[serverfatality]);
            INI_WriteInt(File, "MostOnline", SA[mostonline]);

            INI_Close(File);
        }
        case LOAD_DATA:{
            inline load(string:name[], string:value[]){
                INI_Int("MostOnline", SA[mostonline]);
                INI_Int("Fatality", SA[serverfatality]);
                INI_Int("LaunchMonth", SA[launchmonth]);
                INI_Int("LaunchDate", SA[launchdate]);
                INI_Int("LaunchYear", SA[launchyear]);
            }
            INI_ParseFile(INFO_PATH, using inline load);
        }
        case EMPTY_DATA:{
            SA[mostonline] = SA[serverfatality] = SA[launchmonth] =
            SA[launchdate] = SA[launchyear] = 0;
        }
    }
    return 1;
}