#pragma once

enum class KeyState { UP, PRESSED, DOWN, RELEASED };

struct Input
{
	KeyState jump, fall, left, right, yes, no;
};