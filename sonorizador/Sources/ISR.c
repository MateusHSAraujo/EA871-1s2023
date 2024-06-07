#include "ISR.h"
#include "stdint.h"
#include "TPM.h"
#include "derivative.h"

#define C_7 2093
#define C_6	1046
#define C_5	523
#define G_6S 1661
#define G_6	1567
#define G_5S 831
#define G_5	784
#define G_4	392
#define G_4S 415
#define A_5 880
#define A_5S 932
#define A_6S 1864
#define A_4S 466
#define E_6 1318
#define E_5 659
#define D_5S 622
#define D_5 587
#define D_6S 1244
#define D_6 1175
#define F_5 698
#define F_6 1398
#define B_5 988

typedef struct SongTag1
{
	uint16_t* notes;  	 //<! buffer de dados
	uint8_t* ticks;
	uint8_t noteToPlay ;       
	uint8_t lastNoteIdx;
	uint8_t ps;
	uint16_t mod;
} Song_type;
static uint8_t counter = 1;

static Song_type victoryFanfare;
static uint16_t victoryFanfare_notes[]={ C_6 , C_6 , C_6 , C_6 , G_5S , A_5S , C_6 , 0 , A_5S, C_6};
static uint8_t victoryFanfare_ticks[]={          1  , 1 ,  2    , 2 ,    2  ,  1 ,   1  , 1 ,  3 };

static Song_type winnerSong;
static uint16_t winnerSong_notes[]={G_4, C_5 , E_5 , G_5 , C_6 , E_6 , G_6 , E_6 , G_4S , C_5  , D_5S , G_5S , C_6 , D_6S , G_6S , D_6S , A_4S , D_5, F_5, A_5S, D_6 , F_6 , A_6S , A_6S , A_6S , A_6S, C_7 };
static uint8_t winnerSong_ticks[]={       1  ,  1  ,  1  ,  1 ,   1  ,  2 ,   2  ,  1   ,  1   ,  1   ,  1   ,  1  ,   1  ,  2   ,   2  ,  1   ,  1 ,  1,   1 ,  1  ,  1  ,  2   ,  1   ,  1   ,  1   ,   3  };

static Song_type looserSong;
static uint16_t looserSong_notes[]={ C_6 , G_5 , E_5 , A_5 , B_5 , A_5 , G_5S , A_5S , G_5S, G_5 , F_5, G_5};
static uint8_t looserSong_ticks[]={          3  , 2 ,  2    , 2 ,    2  ,  2 ,   2  ,   2  ,  1  ,  1 ,  3 };


void initSongs(){
	
	victoryFanfare.notes = victoryFanfare_notes;
	victoryFanfare.ticks = victoryFanfare_ticks;
	victoryFanfare.noteToPlay = 0;
	victoryFanfare.lastNoteIdx = 9;
	victoryFanfare.mod=21800;
	
	winnerSong.notes = winnerSong_notes;
	winnerSong.ticks = winnerSong_ticks;
	winnerSong.noteToPlay = 0;
	winnerSong.lastNoteIdx = 26;
	winnerSong.mod=21845;
	
	looserSong.notes = looserSong_notes;
	looserSong.ticks = looserSong_ticks;
	looserSong.noteToPlay = 0;
	looserSong.lastNoteIdx = 11;
	looserSong.mod=21845;
	
}

uint8_t adjustTick (Song_type *song){
	uint8_t idx = song->noteToPlay;
	if (idx==song->lastNoteIdx) return 1;
	return song->ticks[idx];
}

int8_t nextNote (Song_type *song){
	if (song->noteToPlay > song->lastNoteIdx) return -1;
	
	uint16_t noteFreq = song->notes[song->noteToPlay];
	uint8_t tick = adjustTick(song); 
	TPM_config_especifica (0, (song->mod)*tick, 0b1111, 0, 0, 0, 0, 0, 0b111);
	TPM_CH_config_especifica (0, 0, 0b0111, TPM0_CNT);
	
	if (noteFreq!=0){
		uint16_t mod1 = 20971520 / (noteFreq * 32);
		TPM_config_especifica (1, mod1, 0b1111, 0, 0, 0, 0, 0, 0b101);
		TPM_CH_config_especifica (1, 1, 0b1010, mod1/2); //MS_ELS = 1010 -> EPWM high true
	} else {
		TPM_CH_config_especifica (1, 1, 0b0000, 0); 
	}
	song->noteToPlay++;
	return 0;
}

void FTM0_IRQHandler () {
	
		if (TPM0_STATUS & TPM_STATUS_CH0F_MASK){ // Canal de temporarizacao 
				TPM0_C0SC |= TPM_CnSC_CHF_MASK; //Limpa flag
				
				if (counter == 0){
					if (victoryFanfare.noteToPlay==0){
						TPM_config_especifica (0, victoryFanfare.mod, 0b1111, 0, 0, 0, 0, 0, 0b111);
						TPM_CH_config_especifica (0, 0, 0b0111, TPM0_CNT); //MS_ELS = 0111 -> OC set output on match
					}
					if (nextNote(&victoryFanfare)==-1){
						TPM_config_especifica (0, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111);
						TPM_CH_config_especifica (1, 1, 0b0000, 0);
						counter++;
					}
				}
				if (counter==0){
					if (winnerSong.noteToPlay==0){
						TPM_config_especifica (0, winnerSong.mod, 0b1111, 0, 0, 0, 0, 0, 0b111);
						TPM_CH_config_especifica (0, 0, 0b0111, TPM0_CNT); //MS_ELS = 0111 -> OC set output on match
					}
					if (nextNote(&winnerSong)==-1){
						TPM_config_especifica (0, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111);
						TPM_CH_config_especifica (1, 1, 0b0000, 0);
					}
				}
				
				if (counter==1){
					if (looserSong.noteToPlay==0){
						TPM_config_especifica (0, looserSong.mod*3, 0b1111, 0, 0, 0, 0, 0, 0b111);
						TPM_CH_config_especifica (0, 0, 0b0111, TPM0_CNT); //MS_ELS = 0111 -> OC set output on match
					}
					if (nextNote(&looserSong)==-1){
						TPM_config_especifica (0, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111);
						TPM_CH_config_especifica (1, 1, 0b0000, 0);
					}
				}
		}
		
	return;
}
