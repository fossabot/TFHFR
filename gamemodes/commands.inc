#include                                                <YSI/y_commands>

#include                                                <sscanf2>

YCMD:serverstatus(playerid, params[], help) 
{
    #pragma unused params, help
    format(string, sizeof string, "Server Version: %d.%d.%d | Freeroam\
    \nPlayers Online: %d | Launch Date: %d/%d/%d", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, GetPlayerPoolSize(),
    SA[launchmonth], SA[launchdate], SA[launchyear]);
    return 1;
}