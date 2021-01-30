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

// Get arguments from input, separated by delimiter
char** 
GetArgs(
    char                *a_str, 
    char                a_delim, 
    size_t              *count
)
{
	char** result = 0;
	char* tmp = a_str;
	char* last_comma = 0;
	char delim[2] = {a_delim, 0};

	// Count how many elements will be extracted.
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			(*count)++;
			last_comma = tmp;
		}
		tmp++;
	}

	// Add space for trailing token.
	*count += last_comma < (a_str + strlen(a_str) - 1);

	// Add space for terminating null string so caller
	// knows where the list of returned strings ends. 
	(*count)++;

	result = realloc(result, sizeof(char*) * (*count));

	if (result)
	{
		size_t idx = 0;
		char* token = strtok(a_str, delim);

		while (token)
		{
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		*(result + idx) = 0;
        
        // Decrement count so count is accurate to array size
	    (*count)--;
	}

	return result;
}

int 
command_handler(
    SOCKET              sock,
    char                *command)
{
    if (command == NULL)
        return HYPER_FAILED;

    size_t argc = 0;
    char** argv = GetArgs(command, ' ', &argc);
    if (argv == NULL)
        return HYPER_FAILED;

    for(unsigned int i = 0; i < numCommands; i++)
    {
        if (strcmp(command_list[i].command, argv[0]) == 0)
        {
            command_list[i].execute(sock, (const char**)argv, argc);
            free(argv);
            return HYPER_SUCCESS;
        }
    }

    free(argv);
    return HYPER_FAILED;
}

void
get_file(
    const SOCKET        sock,
    const char          **argv,
    const size_t        argc)
{
    HYPERSTATUS iResult = 0;
    unsigned short server_status = 0;

    if (argc < 2)
    {
        puts("Usage: get <source> <destination>");
        return;
    }

    char command[MAX_COMMAND_LENGTH];
    memset(command, 0, MAX_COMMAND_LENGTH);

    strncat(command, "SEND ", MAX_COMMAND_LENGTH - 1);
    strncat(command, argv[1], MAX_COMMAND_LENGTH - strlen(command) - 1);

    iResult = HyperSendCommand(sock, command);
    if (iResult != HYPER_SUCCESS)
    {
        puts("[-] HyperSendCommand failed");
        return;
    }
    
    iResult = HyperReceiveStatus(sock, &server_status);
    if (iResult != HYPER_SUCCESS)
    {
        puts("[-] HyperRecieveStatus failed");
        return;
    }
    else if (server_status >= 400)
    {
        printf("[-] %u BAD\n", server_status);
        return;
    }
    else
    {
        printf("[+] %u OK\n", server_status);
    }

    HYPERFILE lpBuffer = NULL;
    unsigned long ulTotalSize = 0;
    puts("[+] Recieving file...");
    iResult = HyperReceiveFile(sock, &lpBuffer, &ulTotalSize);
    if (iResult != HYPER_SUCCESS)
    {
        puts("[-] HyperRecieveFile failed");
        HyperMemFree(lpBuffer);
        return;
    }

    iResult = HyperWriteFile(argv[2], lpBuffer, ulTotalSize);
    if (iResult == HYPER_FAILED)
    {
        puts("[-] HyperWriteFile failed");
        HyperMemFree(lpBuffer);
        return;
    }
    HyperMemFree(lpBuffer);

    puts("[+] File written");
    return;
}

void
client_quit(
    const SOCKET        sock,
    const char          **argv,
    const size_t        argc)
{
    HyperSendCommand(sock, "QUIT");
    exit(HYPER_SUCCESS);
}

void
list_dir(
    const SOCKET        sock,
    const char          **argv,
    const size_t        argc)
{
    char buffer[4096] = { 0 };
    char listBuffer[4096] = { 0 };
    unsigned short status = 0;

    strncat(buffer, "LIST ", sizeof(buffer)-1);
    
    if (argc > 1)
        strncat(buffer, argv[1], sizeof(buffer)-strlen(buffer)-1);
    else
        strncat(buffer, ".", sizeof(buffer)-strlen(buffer)-1);

    HyperSendCommand(sock, buffer);

    HyperReceiveStatus(sock, &status);
    printf("[?] status code %du\n", status);

    HyperReceiveCommand(sock, listBuffer, sizeof(listBuffer));
    puts(listBuffer);
}

void server_handler(SOCKET sockServer)
{
    char command_buffer[MAX_COMMAND_LENGTH]; 
    memset(command_buffer, 0, MAX_COMMAND_LENGTH);

    while (1){
        fputs("hyper> ", stdout);
        fgets(command_buffer, MAX_COMMAND_LENGTH, stdin);
        
        /* Remove trailing new line */
        command_buffer[strcspn(command_buffer, "\n")] = 0;

        command_handler(sockServer, command_buffer);
    }

    return;
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
    
    for (int i = 0; i < MAX_CONNECTION_TRIES; i++)
    {
        iResult = HyperConnectServer(&sockServer, cpServerIP, usPort);
        if (iResult != HYPER_SUCCESS)
        {
            puts("[-] HyperConnectServer failed, retrying...");
        }
        else
        {
            puts("[+] Connected to server");
            server_handler(sockServer);
            break;
        }

        sleep(CONNECTION_TIMEOUT);
    }

    HyperCloseSocket(sockServer);
    HyperSocketCleanup();
    return HYPER_SUCCESS;
}
