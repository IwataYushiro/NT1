#include <Novice.h>
#include <math.h>
#include <mmsystem.h>
#include <process.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "winmm.lib")

DWORD WINAPI Threadfunc(void*);
HWND hwMain;
const char kWindowTitle[] = "KAMATA ENGINE�T�[�o";

typedef struct {
	float x;
	float y;
} Vector2;

typedef struct {
	Vector2 center;
	float radius;
} Circle;

Circle a, b;
Vector2 center = {100, 100};
char keys[256] = {0};
char preKeys[256] = {0};
int color = RED;

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WSADATA wdData;
	static HANDLE hThread;
	static DWORD dwID;

	// ���C�u�����̏�����
	Novice::Initialize(kWindowTitle, 1024, 768);

	hwMain = GetDesktopWindow();

	a.center.x = 400;
	a.center.y = 400;
	a.radius = 100;

	b.center.x = 200;
	b.center.y = 200;
	b.radius = 50;

	// winsock������
	WSAStartup(MAKEWORD(2, 0), &wdData);

	// �f�[�^�𑗎�M�������X���b�h�iWinMain�̗���Ɋ֌W�Ȃ����삷�鏈���̗���j�Ƃ��Đ����B
	// �f�[�^����M���X���b�h�ɂ��Ȃ��Ɖ����f�[�^����M����܂�RECV�֐��Ŏ~�܂��Ă��܂��B
	hThread = (HANDLE)CreateThread(NULL, 0, &Threadfunc, (LPVOID)&a, 0, &dwID);

	// �E�B���h�E�́~�{�^�����������܂Ń��[�v
	while (Novice::ProcessMessage() == 0) {
		// �t���[���̊J�n
		Novice::BeginFrame();

		// �L�[���͂��󂯎��
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		if (keys[DIK_UP] != 0) {
			b.center.y -= 5;
		}
		if (keys[DIK_DOWN] != 0) {
			b.center.y += 5;
		}
		if (keys[DIK_RIGHT] != 0) {
			b.center.x += 5;
		}
		if (keys[DIK_LEFT] != 0) {
			b.center.x -= 5;
		}

		///
		/// ���X�V������������
		///

		float distance = sqrtf(
		  (float)pow((double)a.center.x - (double)b.center.x, 2) +
		  (float)pow((double)a.center.y - (double)b.center.y, 2));

		if (distance <= a.radius + b.radius) {
			color = BLUE;
		} else
			color = RED;
		///
		/// ���X�V���������܂�
		///

		///
		/// ���`�揈����������
		///
		Novice::DrawEllipse(
		  (int)a.center.x, (int)a.center.y, (int)a.radius, (int)a.radius, 0.0f, WHITE,
		  kFillModeSolid);
		Novice::DrawEllipse(
		  (int)b.center.x, (int)b.center.y, (int)b.radius, (int)b.radius, 0.0f, color,
		  kFillModeSolid);
		///
		/// ���`�揈�������܂�
		///

		// �t���[���̏I��
		Novice::EndFrame();

		// ESC�L�[�������ꂽ�烋�[�v�𔲂���
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ���C�u�����̏I��
	Novice::Finalize();

	// winsock�I��
	WSACleanup();

	return 0;
}

// �ʐM�X���b�h�֐�
DWORD WINAPI Threadfunc(void* px) {

	SOCKET sWait, sConnect;                // �ҋ@�p�Ɛڑ��p
	struct sockaddr_in saLocal, saConnect; // �ҋ@�p�Ɛڑ��p
	WORD wPort = 8000;
	int iLen = sizeof(saConnect); // accept�֐��Ŏg�p

	px = 0; // warning���
	// �ҋ@�\�P�b�g�쐬
	sWait = socket(PF_INET, SOCK_STREAM, 0);

	ZeroMemory(&saLocal, sizeof(saLocal));

	// �ҋ@�\�P�b�g�Ƀ|�[�g8000�ԕR�Â���bind�֐���
	// �����œn��SOCKADDR_IN�\���̂�ݒ�
	saLocal.sin_family = AF_INET;
	saLocal.sin_addr.s_addr = INADDR_ANY;
	saLocal.sin_port = htons(wPort);

	if (bind(sWait, (LPSOCKADDR)&saLocal, sizeof(saLocal)) == SOCKET_ERROR) {
		closesocket(sWait);
		SetWindowText(hwMain, L"�ڑ��ҋ@�\�P�b�g���s");
		return 1;
	}

	if (listen(sWait, 2) == SOCKET_ERROR) {
		closesocket(sWait);
		SetWindowText(hwMain, L"�ڑ��ҋ@�\�P�b�g���s");
		return 1;
	}

	// sConnect�Őڑ��󂯓���
	sConnect = accept(sWait, (LPSOCKADDR)(&saConnect), &iLen);

	if (sConnect == INVALID_SOCKET) {
		shutdown(sConnect, 2);
		closesocket(sConnect);
		shutdown(sWait, 2);
		closesocket(sWait);
		SetWindowText(hwMain, L"�\�P�b�g�ڑ����s");
		return 1;
	}

	// �ڑ��҂��\�P�b�g���
	shutdown(sWait, 2);
	closesocket(sWait);

	while (1) {
		// �f�[�^��M
		int nRcv = recv(sConnect, (char*)&a, sizeof(Circle), 0);

		if (nRcv == SOCKET_ERROR)
			break;

		// �f�[�^���M
		send(sConnect, (const char*)&b, sizeof(Circle), 0);
	}

	shutdown(sConnect, 2);
	closesocket(sConnect);

	return 0;
}
// eof