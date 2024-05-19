#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>
#include <string>
#include <thread>
#include <chrono>
#include <conio.h>

#define TIME_PORT	27015

void main()
{

	// Initialize Winsock (Windows Sockets).

	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "Time Client: Error at WSAStartup()\n";
		return;
	}

	// Client side:
	// Create a socket and connect to an internet address.

	SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
		std::cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a client to communicate on a network, it must connect to a server.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called server. 
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(TIME_PORT);
	bool stop = false;
	// Send and receive data.
	while (!stop)
	{
		std::time_t lastMeasurementTime = 0;
		DWORD time = 0;
		int number;
		char sendBuff[255] = "HI";
		std::cout << "please choose number betwen 1 - 14 : " << endl;
		std::cout << "1.Get Time" << endl;
		std::cout << "2.Get Time Without Date " << endl;
		std::cout << "3.Get Time Since Epoch " << endl;
		std::cout << "4.Get Client To Server Delay Estimation " << endl;
		std::cout << "5.Measure RTT " << endl;
		std::cout << "6.Get Time Without Date Or Seconds" << endl;
		std::cout << "7.Get Year" << endl;
		std::cout << "8.Get Month And Day" << endl;
		std::cout << "9.Get Seconds Since Begining Of Month" << endl;
		std::cout << "10.Get Week Of Year" << endl;
		std::cout << "11.Get Day light Savings" << endl;
		std::cout << "12.Get Time Without Date In City" << endl;
		std::cout << "13.Measure Time Lap" << endl;
		std::cout << "14.Exit" << endl;
		cin >> number;
		int bytesSent = 0;
		int bytesRecv = 0;
		switch(number){
			case 1:
				strcpy(sendBuff,"1");
				break;
			case 2:
				strcpy(sendBuff, "2");
				break;
			case 3:
				strcpy(sendBuff, "3");
				break;
			case 4:
			{
				strcpy(sendBuff, "4");
				DWORD lastTick;
				for (int i = 0; i < 100; i++)
				{


					// Asks the server what's the currnet time.
					// The send function sends data on a connected socket.
					// The buffer to be sent and its size are needed.
					// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
					// The two last arguments hold the details of the server to communicate with. 
					// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).

					bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
					if (SOCKET_ERROR == bytesSent)
					{
						std::cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}
					//std::cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

				}
				for (int j = 0; j < 100; j++) {
					char recBuff[255];

					bytesRecv = recv(connSocket, recBuff, 255, 0);
					if (SOCKET_ERROR == bytesRecv)
					{
						std::cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}

					recBuff[bytesRecv] = '\0';
					//cout << recBuff << endl;
					if (j % 2 == 1) {
						time += std::stoul(recBuff) - lastTick;
					}
					lastTick = std::stoul(recBuff);

				}
				time = time;
				std::cout <<"the sum of hefresh is" << time <<"the average of hefreshem"<< time/99<< endl;
			}
				break;
			case 5:

				strcpy(sendBuff, "5");
				for (int i = 0; i < 100; i++)
				{
					DWORD startTick = GetTickCount();
					char recBuff[255];
					//cout << "start" << startTick << endl;
					// Asks the server what's the currnet time.
					// The send function sends data on a connected socket.
					// The buffer to be sent and its size are needed.
					// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
					// The two last arguments hold the details of the server to communicate with. 
					// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).

					bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
					if (SOCKET_ERROR == bytesSent)
					{
						std::cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}
					//std::cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

					// Gets the server's answer using simple recieve (no need to hold the server's address).
					bytesRecv = recv(connSocket, recBuff, 255, 0);
					if (SOCKET_ERROR == bytesRecv)
					{
						std::cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}

					recBuff[bytesRecv] = '\0';

					DWORD lastTick = GetTickCount();
					//cout << "last" << lastTick << endl;

						time += (lastTick-startTick);
						//std::cout << time << endl;
					}
				
				time = time;
				std::cout <<"full time of 100 requestsis " << time <<" the average "<<time/100<< endl;
				break;
			case 6:
				strcpy(sendBuff, "6");
				break;
			case 7:
				strcpy(sendBuff, "7");
				break;
			case 8:
				strcpy(sendBuff, "8");
				break;
			case 9:
				strcpy(sendBuff, "9");
				break;
			case 10:
				strcpy(sendBuff, "10");
				break;
			case 11:
				strcpy(sendBuff, "11");
				break;
			case 12:
				int countryNumber;
				std::cout << "please choose number betwen 1 - 5 : " << endl;
				std::cout << "1.Doha" << endl;
				std::cout << "2.Prague" << endl;
				std::cout << "3.New York" << endl;
				std::cout << "4.Berlin" << endl;
				std::cout << "5.Other" << endl;
				cin >> countryNumber;
				switch (countryNumber) {
				case 1:
					strcpy(sendBuff, "12,1");
					break;
				case 2:
					strcpy(sendBuff, "12,2");
					break;
				case 3:
					strcpy(sendBuff, "12,3");
					break;
				case 4:
					strcpy(sendBuff, "12,4");
					break;
				case 5:
					strcpy(sendBuff, "12,5");
					break;
				}
				break;
			case 13:
				char recBuff[255];
				strcpy(sendBuff, "13");
				bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
				bytesRecv = recv(connSocket, recBuff, 255, 0);
				if (SOCKET_ERROR == bytesRecv)
				{
					std::cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}

				recBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
				std::cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recBuff << "\" message.\n";
				lastMeasurementTime = 0;
				while (true) {
					std::cout << "Click any key to stop" << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(1)); // Check every second
					if (std::time(nullptr) - lastMeasurementTime >= 10800) {
						std::time_t currentTime = std::time(nullptr);
						std::cout << "Measurement stopped after 3 minutes." << std::endl;
						strcpy(sendBuff, "endOfTime");
					}
					else {
						if (_kbhit()) {
							//bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
							strcpy(sendBuff, "13");

						}
					}
					break;
				}
				break;
			case 14:
				stop = true;
				break;
		
		}
			

	
		char recvBuff[255];

		// Asks the server what's the currnet time.
		// The send function sends data on a connected socket.
		// The buffer to be sent and its size are needed.
		// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
		// The two last arguments hold the details of the server to communicate with. 
		// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
		if (number != 4 && number != 5 && number!=14)
		{
			bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
			if (SOCKET_ERROR == bytesSent)
			{
				std::cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}
			std::cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

			// Gets the server's answer using simple recieve (no need to hold the server's address).
			bytesRecv = recv(connSocket, recvBuff, 255, 0);
			if (SOCKET_ERROR == bytesRecv)
			{
				std::cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}

			recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
			std::cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
		}
		}

}