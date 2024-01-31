#include <Novice.h>
#include <math.h>
#include <mmsystem.h>
#include <process.h>

HWND hwMain;
const char kWindowTitle[] = "第三回課題1";

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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1024, 768);

	hwMain = GetDesktopWindow();

	a.center.x = 400;
	a.center.y = 400;
	a.radius = 100;

	b.center.x = 200;
	b.center.y = 200;
	b.radius = 50;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
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
		/// ↓更新処理ここから
		///

		float distance = sqrtf(
		  (float)pow((double)a.center.x - (double)b.center.x, 2) +
		  (float)pow((double)a.center.y - (double)b.center.y, 2));

		if (distance <= a.radius + b.radius) {
			color = BLUE;
		} else
			color = RED;
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::DrawEllipse(
		  (int)a.center.x, (int)a.center.y, (int)a.radius, (int)a.radius, 0.0f, WHITE,
		  kFillModeSolid);
		Novice::DrawEllipse(
		  (int)b.center.x, (int)b.center.y, (int)b.radius, (int)b.radius, 0.0f, color,
		  kFillModeSolid);
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();

	return 0;
}
// eof
