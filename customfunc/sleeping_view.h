#pragma once

#define RAIN_MAX_COLUMNS 10  
#define RAIN_MAX_ROWS 20
#define CHAR_SPACING 12
#define CHAR_SIZE 12
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define FALL_SPEED 3
#define FADE_STEPS 30

void draw_matrix_code_rain(painter_device_t display, painter_font_handle_t font);
void init_matrix_code_rain(void);
void update_matrix_code_rain(void);

typedef struct {
    int x;
    int y;
    char character;
    int fade;
} CodeDrop;     
