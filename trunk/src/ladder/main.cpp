#include <windows.h>
#include <wininet.h>

#include <stdio.h>

extern "C"
{
#include "md5.h"
#include "hmac_md5.h"
}

void WinError(void)
{
	LPVOID lpMsgBuf;
	
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);
	
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
	// Free the buffer.
	LocalFree( lpMsgBuf );

	exit(0);
}

void InetError(void)
{
	DWORD	a, size = 256;
	char	err[256];
	InternetGetLastResponseInfo(&a, err, &size);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	MessageBox(NULL, (LPCTSTR)err, "Error", MB_OK | MB_ICONINFORMATION );

	exit(0);
}

void md5(char *buf, int length, unsigned char *output)
{
	MD5_CTX		md5;
	int			i;

	MD5_Init(&md5);
	MD5_Update(&md5, buf, length);
	MD5_Final(output, &md5);
}

void md5_print(char *buf)
{
	int		*ibuf = (int*)buf;

    int             i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 32; j = j + 8)
            printf("%02x", (ibuf[i] >> j) & 0xFF);
    printf("\n");
    fflush(stdout);
}

void md5_sprint(char *buf, char *output, int start)
{
	int		*ibuf = (int*)buf;

    int             i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 32; j = j + 8, start += 2)
            sprintf(output + start, "%02x", (ibuf[i] >> j) & 0xFF);

	output[start] = '\0';
}

void Ladder_SendLadderStats(char *username, char *password, char *map, int time);

int main(void)
{
	Ladder_SendLadderStats("test", "test", "haste0204", 600);
	return 0;
}

HINTERNET	inet;

HINTERNET Ladder_OpenConnection(void)
{
	inet = InternetOpen("HASTE", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if(!inet) WinError();

	HINTERNET	url = InternetConnect(inet, "www.divzero-games.com", INTERNET_DEFAULT_HTTP_PORT, " ", NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if(!url) 
	{
		InternetCloseHandle(inet);
		WinError();
	}

	return url;
}

void Ladder_CloseConnection(HINTERNET url)
{
	InternetCloseHandle(url);
	InternetCloseHandle(inet);
}

DWORD Ladder_SendRequest(HINTERNET url, char *request, char *buf, int length)
{
	DWORD	read;

	HINTERNET	req = HttpOpenRequest(url, "GET", request, NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, NULL);
	if(!req) WinError();
	
	HttpSendRequest(req, NULL, 0, NULL, 0);

	InternetReadFile(req, buf, length, &read);
	InternetCloseHandle(req);

	return read;
}

/*
==========================================================================================
Ladder_SendLadderStats

* ladder.php sends a random string to the client
* the map MD5 cheksum is appended to this string (=salt)
* the client scrambles the time string in the formt "timexx:yy:zz"
* the scrambled time string is encoded with the derived salt

* a ladder update is send to ladder.php in the form ladder.php?username+md5_password+mapname+time+encoded_time

TODO: use 32 characters for the scrambled time
TODO: use a scrambled version of the salt
TODO: scramble the sent encoded_time MD5
==========================================================================================
*/

void Ladder_SendLadderStats(char *username, char *password, char *map, int time)
{
	unsigned char	md5_password[32];
	char			php[256] = "/ladder.php";

	char			buf[4096], 
					map_crc[512] = "blabla",
					time_str[32] = "", 
					scrambled_time[32] = "",
					encoded_time[512];

	HINTERNET		url;

	int				scramble[12] = {4, 8, 2, 11, 3, 9, 1, 10, 5, 7, 0, 6}, i;

	sprintf(time_str, "%08d", time);

	memset(buf, 0, 4096);
	memset(md5_password, 0, 32);

	md5(password, strlen(password), md5_password);

	url = Ladder_OpenConnection();
	Ladder_SendRequest(url, php, buf, 4096);
	Ladder_CloseConnection(url);

	strcat(map_crc, buf);

	strcat(php, "?");
	strcat(php, username);

	strcat(php, "+");
	md5_sprint((char*)md5_password, php, strlen(php));

	strcat(php, "+");
	strcat(php, (char*)map);

	strcat(php, "+");
	strcat(php, time_str);

	sprintf(time_str, "t5)!%08d", time);
	
	for(i = 0; i < 12; i++)
		scrambled_time[i] = time_str[scramble[i]];

	//TODO: get the real map MD5 CRC value
	hmac_md5((unsigned char*)scrambled_time, strlen(scrambled_time), (unsigned char*)map_crc, strlen(map_crc), (unsigned char*)encoded_time);

	strcat(php, "+");
	md5_sprint((char*)encoded_time, php, strlen(php));

	memset(buf, 0, 4096);
	url = Ladder_OpenConnection();
	Ladder_SendRequest(url, php, buf, 4096);
	Ladder_CloseConnection(url);

	printf("%s\n", buf);
}