#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gba.h>
#include "assets.h"
#include "fun.h"


ASSET(assets_fun_txt);
ASSET(pokemon_pal_bin);
ASSET(pokemon_img_bin);


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
	
	for (game.frame = 0;; game.frame++)
	{
		gameStep();
		VBlankIntrWait();
	}
}

void gameReset()
{
	// Set up video and backgrounds
	REG_DISPCNT = MODE_0 | BG0_ON | OBJ_ON | OBJ_1D_MAP;
	REG_BG0CNT = BG_PRIORITY(0) | CHAR_BASE(0) | BG_16_COLOR | SCREEN_BASE(30) | BG_SIZE(0);
	
	// Set up background
	for (u16 i = 0; i < 16; i++)
		BG_COLORS[i] = RGB5(i, i, i);
	
	for (u16 i = 0; i < 16*8*8/4; i++)
		((u16*) CHAR_BASE_BLOCK(0))[i] = rand() & 0xffff;
	
	for (u16 i = 0; i < 0x400; i++)
		((u16*) SCREEN_BASE_BLOCK(30))[i] = (rand() & 0xf) | CHAR_PALETTE(0);
	
	// Set up sprite
	for (u16 i = 0; i < ASSET_SIZE(pokemon_pal_bin) / 2; i++)
		OBJ_COLORS[i] = ((u16*) ASSET_START(pokemon_pal_bin))[i];
	
	for (u16 i = 0; i < ASSET_SIZE(pokemon_img_bin) / 2; i++)
		SPRITE_GFX[i] = ((u16*) ASSET_START(pokemon_img_bin))[i];
	
	// Hide all sprites
	for (u16 i = 0; i < 128; i++)
		OAM[i].attr0 = ATTR0_DISABLED;
	
	// Set up audio
	SNDSTAT = SNDSTAT_ENABLE;
	DMGSNDCTRL = DMGSNDCTRL_LVOL(7) | DMGSNDCTRL_RVOL(7) |
		DMGSNDCTRL_LSQR2 | DMGSNDCTRL_RSQR2 | DMGSNTCTRL_LNOISE | DMGSNDCTRL_RNOISE;
	DSOUNDCTRL = DSOUNDCTRL_DMG100;
	
	SQR2CTRL = 0 | SQR_DUTY(3) | 1<<8 | SQR_VOL(5);
	
	// Initialize game
	game.px = 0; game.py = 0;
}

void gameStep()
{
	// Movement
	const u16 keys = ~(REG_KEYINPUT);
	const s8 speed = (keys & KEY_B) ? 2 : 1;
	s8 dx = (!!(keys & KEY_RIGHT) - !!(keys & KEY_LEFT)) * speed;
	s8 dy = (!!(keys & KEY_DOWN) - !!(keys & KEY_UP)) * speed;
	
	if ((dx || dy) && game.frame % 4 == 0) {
		SQR2FREQ = 1<<15 | ((1<<11) - (1<<17)/440);
	}
	
	game.px += dx;
	game.py += dy;
	
	REG_BG0HOFS = game.px;
	REG_BG0VOFS = game.py;
	
	// Sprite tile
	static u8 spriteAnimation = 0;
	static u8 spriteFrame = 0;
	
	if (game.frame % 6 == 0)
	{
		spriteFrame++;
		spriteFrame %= 4;
	}
	
	if (dy > 0)
		spriteAnimation = 0;
	else if (dy < 0)
		spriteAnimation = 3;
	else if (dx < 0)
		spriteAnimation = 1;
	else if (dx > 0)
		spriteAnimation = 2;
	
	// Set up sprite
	static const u16 sx = (240 - 32) / 2;
	static const u16 sy = (160 - 32) / 2;
	const u16 spriteTile = (spriteAnimation * 4 + spriteFrame) * 4*4;
	OAM[0].attr0 = OBJ_Y(sy) | OBJ_16_COLOR | ATTR0_SQUARE;
	OAM[0].attr1 = OBJ_X(sx) | ATTR1_SIZE_32;
	OAM[0].attr2 = OBJ_CHAR(spriteTile) | OBJ_PRIORITY(0) | OBJ_PALETTE(0);
}
