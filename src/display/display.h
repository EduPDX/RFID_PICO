#ifndef DISPLAY_H
#define DISPLAY_H

#include "src/ssd1306/ssd1306_i2c.h"
#include "src/ssd1306/ssd1306.h"

// Função para exibir uma lista de textos, onde cada texto ocupa uma linha
void showTextDisplay(char *text[], int size);

#endif // DISPLAY_H