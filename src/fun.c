#include <gba.h>
#include "assets.h"
#include "fun.h"


game_t game;


int main()
{
	irqInit();
	irqEnable(IRQ_VBLANK);
	
	gameRun();
	
	// Should never reach
	return 0;
}

void gameRun()
{
	gameReset();
	
	for (u32 frame = 0;; frame++)
	{
		gameStep();
		VBlankIntrWait();
	}
}

void gameReset()
{
	// Set up video and backgrounds
	REG_DISPCNT = MODE_0 | BG2_ON | OBJ_ON;
	REG_BG2CNT = BG_PRIORITY(1) | CHAR_BASE(0) | BG_16_COLOR | SCREEN_BASE(31) | BG_SIZE(0);
	
	// Sprite palette
	OBJ_COLORS[0] = RGB5(31, 0, 0);
	OBJ_COLORS[1] = RGB5(0, 31, 0);
	OBJ_COLORS[2] = RGB5(0, 0, 31);
	
	// Sprite tiles
	for (u16 i = 0; i < 0x2000; i++)
		SPRITE_GFX[i] = 0x0101;
	
	// Audio
	SNDSTAT = SNDSTAT_ENABLE;
	DMGSNDCTRL = DMGSNDCTRL_LVOL(7) | DMGSNDCTRL_RVOL(7) |
		DMGSNDCTRL_LSQR2 | DMGSNDCTRL_RSQR2 | DMGSNTCTRL_LNOISE | DMGSNDCTRL_RNOISE;
	DSOUNDCTRL = DSOUNDCTRL_DMG100;
	
	SQR2CTRL = 0 | SQR_DUTY(2) | 1<<8 | SQR_VOL(10);
	
	// game
	game.px = 0; game.py = 0;
}

void gameStep()
{
	static const s16 speed = 3;
	
	u16 keys = ~(REG_KEYINPUT);
	game.px += (!!(keys & KEY_RIGHT) - !!(keys & KEY_LEFT)) * speed;
	game.py += (!!(keys & KEY_DOWN) - !!(keys & KEY_UP)) * speed;
	
	OAM[0].attr0 = OBJ_Y(game.py) | OBJ_256_COLOR | ATTR0_SQUARE;
	OAM[0].attr1 = OBJ_X(game.px) | ATTR1_SIZE_16;
	OAM[0].attr2 = OBJ_CHAR(0) | OBJ_PRIORITY(0);
}
