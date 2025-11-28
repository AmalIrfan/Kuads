#include <cmath>
#include <sstream>
#include <string>

namespace rl {
#include "raylib.h"
#include "raymath.h"
} // namespace rl

struct Vector2i {
	int x, y;

	Vector2i() {}
	Vector2i(int x, int y) : x(x), y(y) {}
};

class Renderer {
protected:
	int width, height;
	std::string title;
	int fps;
	Vector2i canvasOffset;
	Vector2i canvasSize;
	int canvasScale;
	Vector2i canvasCursor;
	rl::Color *canvas;
	Vector2i palleteCursor;
	Vector2i palleteSize;
	rl::Color *pallete;

public:
	Renderer(int w, int h, std::string t, int fps)
			: width(w), height(h), title(t), fps(fps) {
		rl::InitWindow(w, h, t.c_str());
		rl::SetTargetFPS(fps);
			}
	void draw() {
		rl::BeginDrawing();
		rl::ClearBackground(rl::GetColor(0xFF0000FF));
		// draw the canvas
		for (int i = 0; i < canvasSize.x; i++) {
			for (int j = 0; j < canvasSize.y; j++) {
				rl::DrawRectangle(i * canvasScale + canvasOffset.x,
					j * canvasScale + canvasOffset.y, canvasScale,
					canvasScale, canvas[i + j * canvasSize.x]);
				rl::DrawRectangleLines(
					i * canvasScale + canvasOffset.x, j * canvasScale + canvasOffset.y,
					canvasScale, canvasScale,
					rl::GetColor(0x000000FF));
			}
		}
		rl::DrawRectangleLines(canvasOffset.x, canvasOffset.y,
			canvasSize.x * canvasScale,
			canvasSize.y * canvasScale, rl::GetColor(0xFFFF00FF));
		rl::DrawRectangleLines(canvasCursor.x * canvasScale + canvasOffset.x,
			canvasCursor.y * canvasScale + canvasOffset.y, canvasScale, canvasScale,
			rl::GetColor(0x00FFFFFF));

		std::stringstream ss;
		// display grid coords
		for (int i = 0; i < canvasSize.x; i++) {
			ss << std::hex << std::uppercase << i;
			rl::DrawText(ss.str().c_str(), i * canvasScale + canvasOffset.x,
				canvasOffset.y - 16, 16, rl::GetColor(0xFFFFFFFF));
			ss.str("");
		}
		for (int j = 0; j < canvasSize.y; j++) {
			ss << std::hex << std::uppercase << j;
			rl::DrawText(ss.str().c_str(), canvasOffset.x - 16,
				j * canvasScale + canvasOffset.y, 16,
				rl::GetColor(0xFFFFFFFF));
			ss.str("");
		}
		// display the cursor coords in the right corner of the canvas
		ss << std::hex << std::uppercase << canvasCursor.x << canvasCursor.y;
		rl::DrawText(ss.str().c_str(), canvasSize.x * canvasScale + canvasOffset.x,
			canvasSize.y * canvasScale + canvasOffset.y, 16,
			rl::GetColor(0xFFFF00FF));
		// display pallete left of the canvas
		for (int i = 0; i < palleteSize.x; i++) {
			for (int j = 0; j < palleteSize.y; j++) {
				rl::DrawRectangle((i - palleteSize.x - 2) * canvasScale + canvasOffset.x,
					j * canvasScale + canvasOffset.y, canvasScale,
					canvasScale, pallete[i + j * palleteSize.x]);
				rl::DrawRectangleLines(
					(i - palleteSize.x - 2) * canvasScale + canvasOffset.x,
					j * canvasScale + canvasOffset.y,
					canvasScale, canvasScale,
					rl::GetColor(0x000000FF));
			}
		}
		// display the pallete cursor
		rl::DrawRectangleLines(
			(palleteCursor.x - palleteSize.x - 2) * canvasScale + canvasOffset.x,
			palleteCursor.y * canvasScale + canvasOffset.y,
			canvasScale, canvasScale,
			rl::GetColor(0x00FFFFFF));
		rl::EndDrawing();
	}
};

class App : public Renderer {
protected:
	int sinceInput;
	bool saved;

public:
	App()
			: Renderer(640, 480, "Hello, World!", 60), sinceInput(0), saved(false) {
		canvasSize = {16, 16};
		canvasScale = 16;
		canvasOffset = {width / 2 - canvasSize.x * canvasScale / 2,
						height / 2 - canvasSize.y * canvasScale / 2};
		canvasCursor = {0, 0};
		palleteSize = {8, 4};
		palleteCursor = {0, 0};
		loadCanvas();
		loadPallete();
	}
	void loadCanvas() {
		rl::Image img = rl::LoadImage("canvas.png");
		canvas = new rl::Color[canvasSize.x * canvasSize.y];
		for (int i = 0; i < canvasSize.x; i++) {
			for (int j = 0; j < canvasSize.y; j++) {
				canvas[i + j * canvasSize.x] = ((rl::Color*)img.data)[i + j * canvasSize.x];
			}
		}
		rl::UnloadImage(img);
	}
	void loadPallete() {
		rl::Image img = rl::LoadImage("pallete.png");
		rl::Color* colors = rl::LoadImageColors(img);
		pallete = new rl::Color[palleteSize.x * palleteSize.y];
		for (int i = 0; i < palleteSize.x; i++) {
			for (int j = 0; j < palleteSize.y; j++) {
				pallete[i + j * palleteSize.x] = colors[i + j * palleteSize.x];
			}
		}
		rl::UnloadImage(img);
		rl::UnloadImageColors(colors);
	}
	~App() {
		delete[] canvas;
		delete[] pallete;
	}
	void saveCanvas() {
		rl::Image img = {.data = canvas,
			.width = canvasSize.x,
			.height = canvasSize.y,
			.mipmaps = 1,
			.format = rl::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

		rl::ExportImage(img, "canvas.png");
		saved = true;
	}
	void update() {
		auto IsKeyPressedOrRepeat = [](rl::KeyboardKey key) {
			return rl::IsKeyPressed(key) || rl::IsKeyPressedRepeat(key);
		};
		sinceInput++;
		if (IsKeyPressedOrRepeat(rl::KEY_W)) {
			sinceInput = 0;
			canvasCursor.y--;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_S)) {
			sinceInput = 0;
			canvasCursor.y++;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_A)) {
			sinceInput = 0;
			canvasCursor.x--;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_D)) {
			sinceInput = 0;
			canvasCursor.x++;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_Q)) {
			sinceInput = 0;
			canvasCursor.y--;
			canvasCursor.x--;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_E)) {
			sinceInput = 0;
			canvasCursor.y--;
			canvasCursor.x++;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_Z)) {
			sinceInput = 0;
			canvasCursor.y++;
			canvasCursor.x--;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_C)) {
			sinceInput = 0;
			canvasCursor.y++;
			canvasCursor.x++;
		}
		if (canvasCursor.x < 0) {
			canvasCursor.x = canvasSize.x - 1;
		}
		if (canvasCursor.y < 0) {
			canvasCursor.y = canvasSize.y - 1;
		}
		if (canvasCursor.x >= canvasSize.x) {
			canvasCursor.x = 0;
		}
		if (canvasCursor.y >= canvasSize.y) {
			canvasCursor.y = 0;
		}
		if (IsKeyDown(rl::KEY_RIGHT_SHIFT) | IsKeyDown(rl::KEY_LEFT_SHIFT)) {
			sinceInput = 0;
			canvas[canvasCursor.x + canvasCursor.y * canvasSize.x] = pallete[palleteCursor.x + palleteCursor.y * palleteSize.x];
		}
		if (IsKeyDown(rl::KEY_RIGHT_CONTROL) | IsKeyDown(rl::KEY_LEFT_CONTROL)) {
			sinceInput = 0;
			canvas[canvasCursor.x + canvasCursor.y * canvasSize.x] = rl::GetColor(0x00000000);
		}
		if (sinceInput > 120) {
			if (!saved)
				saveCanvas();
		} else if (saved) {
			saved = false;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_UP)) {
			palleteCursor.y--;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_DOWN)) {
			palleteCursor.y++;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_LEFT)) {
			palleteCursor.x--;
		}
		if (IsKeyPressedOrRepeat(rl::KEY_RIGHT)) {
			palleteCursor.x++;
		}
		if (palleteCursor.x < 0) {
			palleteCursor.x = palleteSize.x - 1;
		}
		if (palleteCursor.y < 0) {
			palleteCursor.y = palleteSize.y - 1;
		}
		if (palleteCursor.x >= palleteSize.x) {
			palleteCursor.x = 0;
		}
		if (palleteCursor.y >= palleteSize.y) {
			palleteCursor.y = 0;
		}
	}
	void run() {
		while (!rl::WindowShouldClose()) {
			update();
			draw();
		}
	}
};

int main(void) {
	App app;
	app.run();
	return 0;
}
