#include "Pch.h"

#ifdef DEBUG
const bool IS_WINDOW_MODE = true;
#else
const bool IS_WINDOW_MODE = false;
#endif

const int  SCREEN_WIDTH = 640;						//�E�B���h�E�̕�
const int  SCREEN_HEIGHT = 480;						//�E�B���h�E�̍���

const VECTOR ZERO_VECTOR = { 0.0f, 0.0f, 0.0f };		//�[���x�N�g��

const VECTOR RIGHT = { 1.0f, 0.0f, 0.0f };			//�E
const VECTOR LEFT = { -1.0f, 0.0f, 0.0f };			//��