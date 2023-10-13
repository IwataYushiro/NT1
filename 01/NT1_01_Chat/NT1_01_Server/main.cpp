#include <winsock2.h> /* WinSockのヘッダファイル */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main()
{
	SOCKET listen_s;
	SOCKET s;

	SOCKADDR_IN from;
	int fromlen;
	u_short uport;

	/* ポート番号の入力 */
	printf("使用するポート番号--> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* リスンソケットをオープン */
	listen_s = socket(AF_INET, SOCK_STREAM, 0); /*　☆socket関数　*/
	if (listen_s == INVALID_SOCKET) {
		printf("リスンソケットオープンエラー");
		WSACleanup();
		return;
	}
	printf("リスンソケットをオープンしました\n");

	SOCKADDR_IN saddr;
	/* ソケットに名前を付ける */
	/*　☆スライドを参考にSOCKADDR_IN型変数「saddr」の設定　*/
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = INADDR_ANY;

	/*　☆bind関数を実行　*/
	if (bind(listen_s,(SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("bindエラー");
		closesocket(listen_s);
		return SOCKET_ERROR;
	}
	printf("bind成功です");

	/* クライアントからの接続待ちの状態にする */
	/*　☆listen関数を実行　*/
	if (listen(listen_s, 0) == SOCKET_ERROR) {
		printf("listenエラー\n");
		closesocket(listen_s);
		return SOCKET_ERROR;
	}
	printf("listen成功です\n");

	/* 接続を待機する */
	printf("acceptで待機します\n");
	fromlen = (int)sizeof(from);
	/*　☆SOCKADDR_IN型変数「from」を使ってaccept関数を実行　*/
	s = accept(listen_s, (SOCKADDR*)&from, (int*)sizeof(fromlen));
		if (s == INVALID_SOCKET) {
			printf("acceptエラー\n");
			closesocket(listen_s);
			return;
		}

	printf("%sが接続してきました\n", inet_ntoa(from.sin_addr));
	printf("accepet関数成功です\n");

	/* リスンソケットはもう不要 */
	closesocket(listen_s);

	/* 会話開始 */
	printf("会話開始\n");

	while (1) {

		/*　☆スライドを参考に送受信処理を入力　*/
		int nRcv;
		char szBuf[1024];

		nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';

		printf("受信--> %s\n", szBuf);
		printf("送信-->");

		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(s, szBuf, (int)strlen(szBuf), 0);
	}

	/* ソケットを閉じる */
	closesocket(s);
	WSACleanup();

	return 0;
}
