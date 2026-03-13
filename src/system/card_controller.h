#ifndef CARD_CONTROLLER_H
#define CARD_CONTROLLER_H

void startGame(int players, int cards_per_player);
void gameControllerTask(void *pvArguments);
extern volatile bool isDeckEmpty;
#endif
