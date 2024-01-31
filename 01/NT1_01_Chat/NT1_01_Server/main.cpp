#include <winsock2.h> /* WinSockのヘッダファイル */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment ( lib, "WSock32.lib" ) /* WinSockライブラリの指定 */

int main()
{
	WSADATA wsaData;
	/* WinSockの初期化 */
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		/* 初期化エラー */
		printf("WinSockの初期化に失敗しました\n");
		return 1;
	}

	int fromlen;
	u_short uport;

	/* ポート番号の入力 */
	printf("使用するポート番号--> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* リスンソケットをオープン */
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0); /*　☆socket関数　*/
	printf("リスンソケットをオープンしました\n");
	if (sock == INVALID_SOCKET) {
		printf("ソケットオープンエラー\n");
		return 1;
	}

	SOCKADDR_IN send_addr;
	/* ソケットに名前を付ける */
	/*　☆スライドを参考にSOCKADDR_IN型変数「saddr」の設定　*/
	memset(&send_addr, 0, sizeof(SOCKADDR_IN));
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(uport);
	send_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	/*　☆bind関数を実行　*/
	bind(sock, (sockaddr*)&send_addr, sizeof(send_addr));
	fromlen = (int)sizeof(send_addr);

	/* 会話開始 */
	printf("会話開始\n");

	while (1) {

		/*　☆スライドを参考に送受信処理を入力　*/
		int nRcv;
		char szBuf[1024];

		nRcv = recvfrom(sock, szBuf, sizeof(szBuf)-1, 0, (sockaddr*)&send_addr, &fromlen);
		if (nRcv >= 0) szBuf[nRcv] = '\0';

		printf("受信--> %s\n", szBuf);
		memset(&szBuf, 0, sizeof(szBuf));
		printf("送信-->");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);
		sendto(sock, szBuf, sizeof(szBuf), 0,
			(sockaddr*)&send_addr, sizeof(send_addr));
	}

	/* ソケットを閉じる */
	closesocket(sock);
	WSACleanup();

	return 0;
}
