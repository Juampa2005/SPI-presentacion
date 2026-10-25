
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// Funciones proporcionadas por el controlador de la pantalla.
// Deben implementarse o adaptarse a tu driver ST7735.
void tft_init(void);
void tft_fill_screen(uint16_t color);
void tft_draw_pixel(int x, int y, uint16_t color);

// Colores RGB565
#define BLACK  0x0000
#define WHITE  0xFFFF
#define RED    0xF800
#define YELLOW 0xFFE0
#define BLUE   0x001F

// Dibuja un corazon sencillo mediante dos circulos
// y un triangulo inferior.
static void draw_heart(void) {
    // Circulos superiores
    for (int y = 35; y <= 65; y++) {
        for (int x = 25; x <= 65; x++) {
            int dx = x - 35;
            int dy = y - 45;
            if (dx * dx + dy * dy <= 100)
                tft_draw_pixel(x, y, RED);

            dx = x - 55;
            if (dx * dx + dy * dy <= 100)
                tft_draw_pixel(x, y, RED);
        }
    }

    // Punta inferior del corazon
    for (int y = 45; y <= 95; y++) {
        int half_width = (95 - y) * 2 / 3;
        for (int x = 45 - half_width; x <= 45 + half_width; x++) {
            if (x >= 0 && x < 128 && y >= 0 && y < 160)
                tft_draw_pixel(x, y, RED);
        }
    }
}

static void draw_smile(void) {
    // Cara circular
    for (int y = 25; y <= 105; y++) {
        for (int x = 24; x <= 104; x++) {
            int dx = x - 64;
            int dy = y - 65;
            if (dx * dx + dy * dy <= 40 * 40)
                tft_draw_pixel(x, y, YELLOW);
        }
    }

    // Ojos
    for (int y = 48; y < 56; y++) {
        for (int x = 45; x < 53; x++)
            tft_draw_pixel(x, y, BLACK);
        for (int x = 75; x < 83; x++)
            tft_draw_pixel(x, y, BLACK);
    }

    // Sonrisa aproximada
    for (int x = 43; x <= 85; x++) {
        int dx = x - 64;
        int y = 76 + (dx * dx) / 25;
        if (y < 105)
            tft_draw_pixel(x, y, BLACK);
    }
}

static void execute_command(char *command) {
    // Elimina espacios iniciales
    while (*command == ' ' || *command == '\t')
        command++;

    // Elimina salto de linea
    command[strcspn(command, "\r\n")] = '\0';

    if (strcmp(command, "heart") == 0) {
        tft_fill_screen(BLACK);
        draw_heart();
        printf("Corazon dibujado.\n");
    }
    else if (strcmp(command, "smile") == 0) {
        tft_fill_screen(BLACK);
        draw_smile();
        printf("Carita dibujada.\n");
    }
    else if (strcmp(command, "clear") == 0) {
        tft_fill_screen(BLACK);
        printf("Pantalla limpiada.\n");
    }
    else if (strcmp(command, "help") == 0) {
        printf("Comandos disponibles:\n");
        printf("  heart - dibuja un corazon\n");
        printf("  smile - dibuja una carita\n");
        printf("  clear - limpia la pantalla\n");
        printf("  help  - muestra los comandos\n");
    }
    else if (command[0] != '\0') {
        printf("Comando desconocido. Escribe help.\n");
    }
}

int main(void) {
    stdio_init_all();
    sleep_ms(2000);

    // Inicializa el controlador de la pantalla SPI
    tft_init();
    tft_fill_screen(BLACK);

    printf("\nREPL de pantalla SPI\n");
    printf("Escribe help para ver los comandos.\n");

    char command[64];

    while (true) {
        printf("> ");

        if (fgets(command, sizeof(command), stdin) != NULL) {
            execute_command(command);
        }
        else {
            sleep_ms(10);
        }
    }

    return 0;
}
