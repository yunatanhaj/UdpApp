#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <ctime>
#include <chrono>
#include <map>
#include <iomanip>
#include <sstream>
#include <thread>

#define TIME_PORT	27015
struct Timezone {
	int offsetHours;
	int offsetMinutes;
};
std::time_t lastMeasurementTime = 0;
char* GetTime();
char* MeasureTimeLap();
const char* GetTimeWithoutDateInCity(string city);
char* GetDaylightSavings();
char* GetWeekOfYear();
char* GetSecondsSinceBeginingOfMonth();
char* GetMonthAndDay();
char* GetYear();
char* GetTimeWithoutDateOrSeconds();
char* GetTimeSinceEpoch();
char* GetTimeWithoutDate();
void main()
{
	// Initialize Winsock (Windows Sockets).

	// Create a WSADATA object called wsaData.
	// The WSADATA structure contains information about the Windows 
	// Sockets implementation.
	WSAData wsaData;

	// Call WSAStartup and return its value as an integer and check for errors.
	// The WSAStartup function initiates the use of WS2_32.DLL by a process.
	// First parameter is the version number 2.2.
	// The WSACleanup function destructs the use of WS2_32.DLL by a process.
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "Time Server: Error at WSAStartup()\n";
		return;
	}

	// Server side:
	// Create and bind a socket to an internet address.

	// After initialization, a SOCKET object is ready to be instantiated.

	// Create a SOCKET object called m_socket. 
	// For this application:	use the Internet address family (AF_INET), 
	//							datagram sockets (SOCK_DGRAM), 
	//							and the UDP/IP protocol (IPPROTO_UDP).
	SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Check for errors to ensure that the socket is a valid socket.
	// Error detection is a key part of successful networking code. 
	// If the socket call fails, it returns INVALID_SOCKET. 
	// The "if" statement in the previous code is used to catch any errors that
	// may have occurred while creating the socket. WSAGetLastError returns an 
	// error number associated with the last error that occurred.
	if (INVALID_SOCKET == m_socket)
	{
		std::cout << "Time Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a server to communicate on a network, it must first bind the socket to 
	// a network address.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called serverService. 
	sockaddr_in serverService;
	// Address family (must be AF_INET - Internet address family).
	serverService.sin_family = AF_INET;
	// IP address. The sin_addr is a union (s_addr is a unsigdned long (4 bytes) data type).
	// INADDR_ANY means to listen on all interfaces.
	// inet_addr (Internet address) is used to convert a string (char *) into unsigned int.
	// inet_ntoa (Internet address) is the reverse function (converts unsigned int to char *)
	// The IP address 127.0.0.1 is the host itself, it's actually a loop-back.
	serverService.sin_addr.s_addr = INADDR_ANY;	//inet_addr("127.0.0.1");
	// IP Port. The htons (host to network - short) function converts an
	// unsigned short from host to TCP/IP network byte order (which is big-endian).
	serverService.sin_port = htons(TIME_PORT);

	// Bind the socket for client's requests.

	// The bind function establishes a connection to a specified socket.
	// The function uses the socket handler, the sockaddr structure (which
	// defines properties of the desired connection) and the length of the
	// sockaddr structure (in bytes).
	if (SOCKET_ERROR == bind(m_socket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		std::cout << "Time Server: Error at bind(): " << WSAGetLastError() << endl;
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	// Waits for incoming requests from clients.

	// Send and receive data.
	sockaddr client_addr;
	int client_addr_len = sizeof(client_addr);
	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[255];
	char recvBuff[255];

	// Get client's requests and answer them.
	// The recvfrom function receives a datagram and stores the source address.
	// The buffer for data to be received and its available size are 
	// returned by recvfrom. The fourth argument is an idicator 
	// specifying the way in which the call is made (0 for default).
	// The two last arguments are optional and will hold the details of the client for further communication. 
	// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
	std::cout << "Time Server: Wait for clients' requests.\n";
	bool flag = true;
	while (true)
	{
		bytesRecv = recvfrom(m_socket, recvBuff, 255, 0, &client_addr, &client_addr_len);
		if (SOCKET_ERROR == bytesRecv)
		{
			std::cout << "Time Server: Error at recvfrom(): " << WSAGetLastError() << endl;
			closesocket(m_socket);
			WSACleanup();
			return;
		}

		recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
		if (strcmp(recvBuff, "1") == 0)
		{
			std::cout << "Time Server: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";

			// Answer client's request by the current time.

			// Get the current time.
			//time_t timer;
			//time(&timer);
			// Parse the current time to printable string.
			strcpy(sendBuff,GetTime());
			//sendBuff[strlen(sendBuff) - 1] = '\0'; //to remove the new-line from the created string
		}
		else if (strcmp(recvBuff, "2") == 0)
		{
			strcpy(sendBuff, GetTimeWithoutDate());
		}
		else if (strcmp(recvBuff, "3") == 0)
		{
			strcpy(sendBuff, GetTimeSinceEpoch());
		}
		else if (strcmp(recvBuff, "5") == 0) {
			flag = false;
			strcpy(sendBuff, "OK");
			bytesSent = sendto(m_socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&client_addr, client_addr_len);
			for (int i = 1; i < 100; i++)
			{
				bytesRecv = recvfrom(m_socket, recvBuff, 255, 0, &client_addr, &client_addr_len);
				bytesSent = sendto(m_socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&client_addr, client_addr_len);


			}
		}
		else if (strcmp(recvBuff, "12,1") == 0|| strcmp(recvBuff, "12,2") == 0|| strcmp(recvBuff, "12,3") == 0|| strcmp(recvBuff, "12,4") == 0) {
			if (recvBuff[3] == '1') {
				strcpy(sendBuff, GetTimeWithoutDateInCity("Doha"));

			}
			if (recvBuff[3] == '2') {
				strcpy(sendBuff, GetTimeWithoutDateInCity("Prague"));

			}
			if (recvBuff[3] == '3') {
				strcpy(sendBuff, GetTimeWithoutDateInCity("New York"));

			}
			if (recvBuff[3] == '4') {
				strcpy(sendBuff, GetTimeWithoutDateInCity("Berlin"));

			}
			if (recvBuff[3] == '5') {
				strcpy(sendBuff, GetTimeWithoutDateInCity("Other"));

			}
		}
		else if (strcmp(recvBuff, "4") == 0) {
			flag = false;
			DWORD startTick = GetTickCount();
			std::snprintf(sendBuff, sizeof(sendBuff), "%lu", startTick);
			bytesSent = sendto(m_socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&client_addr, client_addr_len);
			for (int i = 1; i < 100; i++)
			{
				bytesRecv = recvfrom(m_socket, recvBuff, 255, 0, &client_addr, &client_addr_len);
				startTick = GetTickCount();
				std::snprintf(sendBuff, sizeof(sendBuff), "%lu", startTick);
				bytesSent = sendto(m_socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&client_addr, client_addr_len);
				//cout << startTick << endl;

			}
		}
		else if (strcmp(recvBuff, "13") == 0) {
			static char buffer[80];

			std::time_t currentTime = std::time(nullptr);

			if (lastMeasurementTime == 0) {
				lastMeasurementTime = currentTime;
				std::strcpy(buffer, "Measurement started.");
				bytesSent = sendto(m_socket, buffer, (int)strlen(buffer), 0, (const sockaddr*)&client_addr, client_addr_len);
			/*while (true) {
					std::this_thread::sleep_for(std::chrono::seconds(1)); // Check every second
					if (std::time(nullptr)- lastMeasurementTime>=180) {
						std::time_t currentTime = std::time(nullptr);
							std::cout << "Measurement stopped after 3 minutes." << std::endl;
						}
						*/
				bytesRecv = recvfrom(m_socket, recvBuff, 255, 0, &client_addr, &client_addr_len);
				if (strcmp(recvBuff, "13") == 0){
					std::time_t currentTime = std::time(nullptr);
					std::time_t timeLap = currentTime - lastMeasurementTime;
					std::snprintf(sendBuff, 80, "%ld seconds elapsed.", timeLap);
					}
				}
			lastMeasurementTime = 0;
		}
		else if (strcmp(recvBuff, "6") == 0)
		{
			strcpy(sendBuff, GetTimeWithoutDateOrSeconds());
		}
		else if (strcmp(recvBuff, "7") == 0)
		{
			strcpy(sendBuff, GetYear());
		}
		else if (strcmp(recvBuff, "8") == 0)
		{
		strcpy(sendBuff, GetMonthAndDay());
		}
		else if (strcmp(recvBuff, "9") == 0)
		{
		strcpy(sendBuff, GetSecondsSinceBeginingOfMonth());
		}
		else if (strcmp(recvBuff, "10") == 0)
		{
		strcpy(sendBuff, GetWeekOfYear());
		}
		else if (strcmp(recvBuff, "11") == 0)
		{
		strcpy(sendBuff, GetDaylightSavings());
		}
		// Sends the answer to the client, using the client address gathered
		// by recvfrom. 
		if (flag) {
			bytesSent = sendto(m_socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&client_addr, client_addr_len);
			if (SOCKET_ERROR == bytesSent)
			{
				std::cout << "Time Server: Error at sendto(): " << WSAGetLastError() << endl;
				closesocket(m_socket);
				WSACleanup();
				return;
			}

			std::cout << "Time Server: Sent: " << bytesSent << "\\" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
		}
		flag = true;
	}

	// Closing connections and Winsock.
	std::cout << "Time Server: Closing Connection.\n";
	closesocket(m_socket);
	WSACleanup();
}
struct tm* timeFunction()//helper function to return the time and not to write the code 9 times 
{
	time_t now = time(nullptr);
	struct tm* timeinfo = localtime(&now);
	return timeinfo;
}
char* GetTime() {
	
	struct tm* timeinfo = timeFunction();

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
	return buffer;
}

// Function to get the current time without the date
char* GetTimeWithoutDate() {
	struct tm* timeinfo = timeFunction();
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);

	return buffer;
}

// Function to get the current time in seconds since epoch
char* GetTimeSinceEpoch() {
	char strTime[80];
	sprintf(strTime, "%d", time(nullptr));
	return strTime;
}


// Function to get the current time without date or seconds
char* GetTimeWithoutDateOrSeconds() {
	struct tm* timeinfo = timeFunction();

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);

	return buffer;
}

// Function to get the year
char* GetYear() {
	struct tm* timeinfo = timeFunction();
	char strYear[80];
	sprintf(strYear, "%d", timeinfo->tm_year + 1900); // Years since 1900
	return strYear;
}

// Function to get the month and day
char* GetMonthAndDay() {
	struct tm* timeinfo = timeFunction();

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%m-%d", timeinfo);

	return buffer;
}

// Function to get the number of seconds since the beginning of the month
char* GetSecondsSinceBeginingOfMonth() {
	struct tm* timeinfo = timeFunction();
	char strSecondeSinceBeginingOfMonth[80];
	sprintf(strSecondeSinceBeginingOfMonth, "%d", timeinfo->tm_sec + timeinfo->tm_min * 60 + timeinfo->tm_hour * 3600);

	return strSecondeSinceBeginingOfMonth;
}

// Function to get the week of the year
char* GetWeekOfYear() {
	struct tm* timeinfo = timeFunction();
	// Get the day of the year and calculate the week
	int dayOfYear = timeinfo->tm_yday;
	int weekOfYear = (dayOfYear + 7 - timeinfo->tm_wday) / 7;
	char strWeekOfYear[80];
	sprintf(strWeekOfYear, "%d", weekOfYear);

	return strWeekOfYear;
}

// Function to check if daylight savings time is in effect
char* GetDaylightSavings() {
	struct tm* timeinfo = timeFunction();
	char strDaylightSaving[80];
	sprintf(strDaylightSaving, "%d", timeinfo->tm_isdst > 0);
	return strDaylightSaving;
}

// Function to get the time in a specific city
const char* GetTimeWithoutDateInCity(string city) {
	std::map<std::string, Timezone> cityTimezones = {
		   {"Doha", {3, 0}},        // ???? (????)
		   {"Prague", {1, 0}},      // ???? (?'???)
		   {"New York", {-5, 0}},   // ??? ???? (???"?)
		   {"Berlin", {1, 0}}       // ????? (??????)
	};

	Timezone tz;
	if (cityTimezones.find(city) != cityTimezones.end()) {
		tz = cityTimezones[city];
	}
	else {
		tz = { 0, 0 }; // ?? ???? ?? ????? ??????, ????? ???? ??? UTC
	}

	std::time_t now = std::time(nullptr);
	now += tz.offsetHours * 3600 + tz.offsetMinutes * 60;
	std::tm localTime = *std::localtime(&now);

	char buffer[10];
	std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &localTime);
	return buffer;
}

// Function to measure the time lapse between two requests
char* MeasureTimeLap() {
	static char buffer[80];

	std::time_t currentTime = std::time(nullptr);

	if (lastMeasurementTime == 0) {
		lastMeasurementTime = currentTime;
		std::strcpy(buffer, "Measurement started.");
	}
	else {
		std::time_t timeLap = currentTime - lastMeasurementTime;
		if (timeLap <= 180) {
			lastMeasurementTime = 0; // Reset the measurement
			std::snprintf(buffer, 80, "%ld seconds elapsed.", timeLap);
		}
		else {
			lastMeasurementTime = 0; // Stop the measurement
			std::strcpy(buffer, "Measurement stopped after 3 minutes.");
		}
	}

	return buffer;
}
