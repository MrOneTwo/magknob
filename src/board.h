#ifndef BOARD_H
#define BOARD_H

#define ENCODER_WRAP_VALUE (1024)

void board_init(void);
int board_encoder_get_counter(void);

#endif // BOARD_H
