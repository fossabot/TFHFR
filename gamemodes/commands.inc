#include                                                <YSI\y_commands>

#include                                                <sscanf2>

YCMD:serverstatus(playerid, params[], help) 
{
    #pragma unused params, help
    new string[79 + (14*3)];
    format(string, sizeof string, "Server Version: %d.%d.%d | Gamemode: Freeroam\nOnline Count: %d | Fatalities: %d", 
    MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, SA[mostonline], SA[serverfatality]);
    SCM(playerid, -1, string);
    return 1;
}