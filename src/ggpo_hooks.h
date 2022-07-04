/*based on https://github.com/pond3r/ggpo/blob/master/src/apps/vectorwar/vectorwar.h*/

// #include "ggponet.h"

enum BattleBinaryInputs {
 INPUT_NORMAL_ATTACK  = (1 << 0),
 INPUT_SPECIAL_ATTACK = (1 << 1),
 INPUT_DIGITAL_BLOCK  = (1 << 2),
 INPUT_DIGITAL_JUMP   = (1 << 3),
 INPUT_TAUNT          = (1 << 4)
}

void Battle_Init();
void Battle_InitSpectator();
void Battle_DrawCurrentFrame();
void Battle_AdvanceFrame();
void Battle_Idle();
void Battle_DisconnectPlayer();
void Battle_Exit();
