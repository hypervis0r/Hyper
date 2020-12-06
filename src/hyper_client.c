#include "hyper_client.h"

void usage(void)
{
    puts( 
        
        "    __  __                       \n"                     
        "   / / / /_  ______  ___  _____  \n"
        "  / /_/ / / / / __ \\/ _ \\/ ___/\n"
        " / __  / /_/ / /_/ /  __/ /      \n"
        "/_/ /_/\\__  / ____/\\___/_/     \n"
        "      /____/_/                   \n"
    );
    puts("Usage: hyper <IP ADDRESS> <PORT>");
}

int main(int argc, char **argv)
{
    HYPERSTATUS iResult = 0;
    SOCKET sockServer = 0;
    char *cpServerIP = NULL;
    unsigned short usPort = 0;

    if (argc < 3)
    {
        usage();
        return HYPER_FAILED;
    }
    else
    {
        cpServerIP = argv[1];
        usPort = (unsigned short)strtoul(argv[2], NULL, 0);
    }
       
    iResult = HyperNetworkInit();
    if (iResult != HYPER_SUCCESS)
    {
        puts("[-] HyperNetworkInit failed");
        return HYPER_FAILED;
    }
    else
        puts("[+] Hyper NetAPI Initialized");

    iResult = HyperConnectServer(&sockServer, cpServerIP, usPort);
    if (iResult != HYPER_SUCCESS)
    {
        puts("[-] HyperConnectServer failed");
        return HYPER_FAILED;
    }
    else
        puts("[+] Connected to server");

    iResult = HyperSendCommand(sockServer, "SEND");
    if (iResult != HYPER_SUCCESS)
    {
        puts("[-] HyperSendCommand failed");
        return HYPER_FAILED;
    }
    else
        puts("[+] Command sent");

    HYPERFILE lpBuffer = NULL;
    unsigned long ulTotalSize = 0;
    puts("[+] Recieving file...");
    iResult = HyperRecieveFile(sockServer, &lpBuffer, &ulTotalSize);
    if (iResult != HYPER_SUCCESS)
    {
        puts("[-] HyperRecieveFile failed");
        HyperMemFree(lpBuffer);
        return HYPER_FAILED;
    }
    else
        puts("[+] File recieved");

    iResult = HyperWriteFile("./testresult.jpg", lpBuffer, ulTotalSize);
    if (iResult == HYPER_FAILED)
    {
        puts("[-] HyperWriteFile failed");
        HyperMemFree(lpBuffer);
        return HYPER_FAILED;
    }
    HyperMemFree(lpBuffer);

    puts("[+] File written");

    HyperCloseSocket(sockServer);
    HyperSocketCleanup();
    return HYPER_SUCCESS;
}