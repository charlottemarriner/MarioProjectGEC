#pragma once

//Screen Dimensions
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 416

//Game Constants
#define MOVEMENTSPEED 0.05f
#define JUMP_FORCE_DECREMENT 450.0f
#define INITIAL_JUMP_FORCE 400.0f
#define GRAVITY 96.0f

//Map Dimensions
#define MAP_HEIGHT 13
#define MAP_WIDTH 17

//Character 
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

//Screenshake
#define SCREENSHAKE_DURATION 0.25f

//Countdowns
#define ENEMYSPAWNCOUNTDOWN 10.0f
#define BONUS_COIN_LEVEL_COUNTDOWN 10.0f
#define MULTIPLIER_COUNTDOWN 2.0f

//Enemy speeds :
//Koopa default injured time / speed
#define INJURED_TIME 5.0f
#define KOOPA_SPEEP 0.005f
#define KOOPA_SPEEP_INJURED 0.01f
//Coin speed
#define COIN_SPEED 0.01f
//Crab Speeds
#define CRAB_SPEED 0.01f
#define CRAB_SPEED_INJURED 0.015f

#define ANIMATION_DELAY 0.1f

//Game Levels
#define KOOPA_NUM_LEVEL1 3
#define KOOPA_NUM_LEVEL2 5
#define KRAB_NUM_LEVEL4 4
#define KOOPA_NUM_LEVEL5 2
#define KRAB_NUM_LEVEL5 4
#define LEVEL5_TOTAL_ENEMIES 6
