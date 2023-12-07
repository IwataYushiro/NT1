#include <Novice.h>
#include <math.h>
#include <mmsystem.h>
#include <process.h>
#include <fstream>
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "winmm.lib")

DWORD WINAPI Threadfunc(void* px);
HWND hwMain;
const char kWindowTitle[] = "KAMATA ENGINE�N���C�A���g";

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

	// ������
	a.center.x = 400;
	a.center.y = 400;
	a.radius = 100;

	// �Ԃ���
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
			a.center.y -= 5;
		}
		if (keys[DIK_DOWN] != 0) {
			a.center.y += 5;
		}
		if (keys[DIK_RIGHT] != 0) {
			a.center.x += 5;
		}
		if (keys[DIK_LEFT] != 0) {
			a.center.x -= 5;
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

	SOCKET sConnect;
	struct sockaddr_in saConnect;
	char addr[20];

	WORD wPort = 8000;

	px = 0; // warning���
	// �t�@�C���ǂݍ���
	std::ifstream ifs("ip.txt");
	ifs.getline(addr, sizeof(addr));
	sConnect = socket(PF_INET, SOCK_STREAM, 0);

	ZeroMemory(&saConnect, sizeof(sockaddr_in));
	saConnect.sin_family = AF_INET;
	saConnect.sin_addr.s_addr = inet_addr(addr);
	saConnect.sin_port = htons(wPort);

	// �T�[�o�[�ɐڑ�
	if (connect(sConnect, (sockaddr*)(&saConnect), sizeof(saConnect)) == SOCKET_ERROR) {
		closesocket(sConnect);
		WSACleanup();

		return 1;
	}
}