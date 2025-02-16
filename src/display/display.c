#include "display.h"

// Função para exibir uma lista de textos, onde cada texto ocupa uma linha
void showTextDisplay(char *text[], int size)
{
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1};

    calculate_render_area_buffer_length(&frame_area);

    // Zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    int y = 0; // Posição vertical inicial
    for (int i = 0; i < size; i++)
    {
        // Desenha o texto na posição y
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 10; // Avança para a próxima linha (10 pixels de altura por linha)
    }

    // Renderiza o buffer no display
    render_on_display(ssd, &frame_area);
}