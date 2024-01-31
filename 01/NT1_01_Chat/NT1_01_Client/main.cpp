#include <winsock2.h> /* WinSockのヘッダファイル */
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment ( lib, "WSock32.lib" ) /* WinSockライブラリの指定 */
#pragma comment ( lib, "ws2_32.lib" ) /* WinSockライブラリの指定 */

int main()
{
	WSADATA wsaData;
	/* WinSockの初期化 */
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		/* 初期化エラー */
		printf("WinSockの初期化に失敗しました\n");
		return 1;
	}

	SOCKET sock;
	u_short uport = 8000;
	
	/* ソケットをオープン */
	sock = socket(AF_INET, SOCK_DGRAM, 0);/*　☆socket関数　*/
	if (sock == INVALID_SOCKET) {
		printf("ソケットオープンエラー\n");
		return 1;
	}
	printf("リスンソケットをオープンしました\n");

	SOCKADDR_IN send_addr;
	/*　☆スライドを参考にSOCKADDR_IN型変数「saddr」の設定　*/
	memset(&send_addr, 0, sizeof(SOCKADDR_IN));
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(uport);
	inet_pton(send_addr.sin_family, "192.168.0.13", &send_addr.sin_addr.S_un.S_addr);

	SOCKADDR_IN recv_addr;
	/*　☆スライドを参考にSOCKADDR_IN型変数「saddr」の設定　*/
	memset(&recv_addr, 0, sizeof(SOCKADDR_IN));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(uport);
	inet_pton(recv_addr.sin_family, "192.168.0.13", &recv_addr.sin_addr.S_un.S_addr);

	int fromlen=(int)sizeof(recv_addr);
	/*　☆connectはいらない	　*/
	printf("サーバーに接続できました\n");

	while (1) {

		/*　スライドを参考に送受信処理を入力　*/
		int nRcv = 0;
		
		char szBuf[1024];

		printf("送信-->");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		sendto(sock, szBuf, sizeof(szBuf), 0,
			(struct sockaddr*)&send_addr, sizeof(send_addr));
		memset(&szBuf, 0, sizeof(szBuf));
		nRcv = recvfrom(sock, szBuf, sizeof(szBuf), 0, (sockaddr*)&recv_addr, &fromlen);
		if (nRcv >= 0 && nRcv < 1024) szBuf[nRcv] = '\0';
		printf("受信--> %s\n", szBuf);
		
	}

	/* ソケットを閉じる */
	closesocket(sock);
	WSACleanup();

    return 0;
}
