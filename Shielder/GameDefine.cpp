#include "Pch.h"

#ifdef DEBUG
const bool IS_WINDOW_MODE = true;
#else
const bool IS_WINDOW_MODE = false;
#endif

const int  SCREEN_WIDTH = 640;						//ウィンドウの幅
const int  SCREEN_HEIGHT = 480;						//ウィンドウの高さ

const VECTOR ZERO_VECTOR = { 0.0f, 0.0f, 0.0f };		//ゼロベクトル

const VECTOR RIGHT = { 1.0f, 0.0f, 0.0f };			//右
const VECTOR LEFT = { -1.0f, 0.0f, 0.0f };			//左