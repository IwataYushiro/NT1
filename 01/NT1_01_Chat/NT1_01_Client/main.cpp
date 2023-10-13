#include <winsock2.h> /* WinSockのヘッダファイル */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	SOCKET s;
	SOCKADDR_IN saddr;
	u_short uport;
	char szServer[1024] = { 0 };
	HOSTENT* lpHost;
	unsigned int addr;

	/* ポート番号の入力 */
	printf("使用するポート番号 --> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* サーバ名またはサーバのIPアドレスを入力 */
	printf("IPアドレス -->");
	scanf_s("%s", szServer, 1024);
	fflush(stdin);

	/* ソケットをオープン */
	s = socket(AF_INET, SOCK_STREAM, 0);/*　☆socket関数　*/
		if (s == INVALID_SOCKET) {
			printf("ソケットオープンエラー\n");
			return INVALID_SOCKET;
		}

	/* サーバーを名前で取得する */
	lpHost = gethostbyname(szServer);
	if (lpHost == NULL) {
		/* サーバーをIPアドレスで取得する */
		addr = inet_addr(szServer);
		lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
	}

	if (lpHost == NULL) {
		printf("Hostが見つかりません\n");
		closesocket(s);
		return NULL;
	}

	/*　☆スライドを参考にSOCKADDR_IN型変数「saddr」の設定　*/
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = lpHost->h_addrtype;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);

	/*　☆connect関数を実行　*/
	if ( connect(s,(SOCKADDR*)&saddr,sizeof(saddr)) == SOCKET_ERROR) {
		printf("サーバーと接続できませんでした\n");
		closesocket(s);
		return SOCKET_ERROR;
	}
	printf("サーバーに接続できました\n");

	while (1) {

		/*　スライドを参考に送受信処理を入力　*/
		int nRcv;
		char szBuf[1024];

		printf("送信-->");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(s, szBuf, (int)strlen(szBuf), 0);
		
		nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';
		printf("受信--> %s\n", szBuf);
		
	}

	/* ソケットを閉じる */
	closesocket(s);
	WSACleanup();

    return 0;
}
