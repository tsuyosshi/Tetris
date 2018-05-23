#include "Siv3D.hpp"
#include "Player.h"

Vec2 Player::InputKey() {
	int dx = 0, dy = 0;
	if (Input::KeyRight.clicked)dx = 1;
	else if (Input::KeyLeft.clicked)dx = -1;
	else if (Input::KeyDown.clicked) dy = 1;
	return Vec2(dx, dy);
}



