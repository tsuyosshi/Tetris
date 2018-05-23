# include <Siv3D.hpp>
#include "Mino.h"
#include "CurrentMino.h"
#include "NextMino.h"
#include "Player.h"
#include "Field.h"

short Init[100][100];//仮 後で要修正


void Main()
{
	CurrentMino currentMino(0, Vec2(5, 0), -1, false);
	NextMino preNextMino(0, 1, 2);
	NextMino nextMino;
	Field field(Init);
	const Font font(30);
	Graphics::SetBackground(Palette::Black);
	Stopwatch time;
	time.start();
	while (System::Update())
	{
 		Stopwatch stopwatch;
		Stopwatch IsStop;
		stopwatch.start();
		while (System::Update()) {
			field.DrawField();
			currentMino.DrawMino();
			preNextMino.DrawMino(font);
			currentMino.MoveMino(field.field);
			font(L"Time", time.ms()).draw(300, 10, Palette::Red);
			if (stopwatch.ms() >= 250) {
				if (!currentMino.IsConflict(Vec2(currentMino.pos.x, currentMino.pos.y + 1), field.field))currentMino.FreeFall(field.field);
				stopwatch.restart();
			}
			if (Input::KeyX.clicked) {
				currentMino.RightRotateMino(field.field);
			}
			if (Input::KeyZ.clicked) {
				currentMino.LeftRotateMino(field.field);
			}
			if (Input::KeyUp.clicked) {
				currentMino.ForceFall(field.field);
				field.ConstructField(currentMino.pos, currentMino);
				field.VanishColumn();
				field.DrawField();
				currentMino = CurrentMino(preNextMino.second, Vec2(5, 0), currentMino.holdMinoNumber, false);
				double minoIndex = std::min(6, (int)(Random() * 7));
				preNextMino = NextMino(preNextMino.second, preNextMino.third, minoIndex);
				break;
			}
			if (Input::KeyShift.clicked) {
				if (!currentMino.isUsedHold) {
					if (currentMino.holdMinoNumber != -1) {
						currentMino = CurrentMino(currentMino.holdMinoNumber, Vec2(5, 0), currentMino.minoNumber, true);
						break;
					}
					else {
						currentMino = CurrentMino(preNextMino.second, Vec2(5, 0), currentMino.minoNumber, true);
						preNextMino = NextMino(preNextMino.second, preNextMino.third, rand() % 7);
						break;
					}
				}
			}
			if (currentMino.IsStop(field.field))IsStop.start();
			else IsStop.reset();
			if (field.score >= 40) {
				font(L"Clear").draw(10, 10, Palette::Yellow);
				WaitKey();
				return;
			}
			if (IsStop.ms() >= 1000 && currentMino.IsStop(field.field)) {
				IsStop.reset();
				if(currentMino.GameOver()) {
					font(L"GameOver").draw(10, 10, Palette::Red);
					WaitKey(); 
					return ;
				}
				field.ConstructField(currentMino.pos, currentMino);
				field.VanishColumn();
				field.DrawField();
				currentMino = CurrentMino(preNextMino.second, Vec2(5, 0), currentMino.holdMinoNumber, false);
				preNextMino = NextMino(preNextMino.second, preNextMino.third, rand() % 7);
				break;
			}
			font(L"Score ", 40-field.score).draw(300, 320);
			font(L"Hold").draw(500, 50);
			if (currentMino.holdMinoNumber != -1) {
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) {
						if (currentMino.mino[currentMino.holdMinoNumber][i][j] == 1) {
							Rect(500 + j * 20, 100 + i * 20, 20, 20).draw(Color(64, 64, 255));
						}
					}
				}
			}
			else {
				font(L"NULL").draw(500, 100,Palette
				::Red);
			}
		}
	}
}
