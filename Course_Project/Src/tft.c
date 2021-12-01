#include "tft.h"


extern float emissivity;


void FloatToCharEmissivity(float n){
	int tmp;
	tmp = (int)(n*100);
	char a[3];
	a[0] = (tmp/10)%10 + 48;
	a[1] = tmp%10 + 48;
	a[2] = 0;
	Draw_String(74, 60, WHITE, BLACK, &a[0], 2);
}
/*
 * The function translates from the float data type to char and displays this value on the display
*
* Input data:
* src is a variable that needs to be converted to a char array
* x and y are the coordinates of the upper left point of the symbol rendering
 */
void FloatToCharTemperature(float src, uint16_t x, uint16_t y){
	int tmp;
	if (src<0){
		tmp = (int)(-(src)*100);
		char a[7];
		a[0] = '-';
		a[1] = (tmp/1000)%10 + 48;
		a[2] = (tmp/100)%10 + 48;
		a[3] = '.';
		a[4] = (tmp/10)%10+48;
		a[5] = tmp%10+48;
		a[6] = 0;
		Draw_String(x, y, WHITE, BLACK, &a[0], 2);
	}
	else{
		tmp = (int)(src*100);
		char a[7];
		a[0] = (tmp/10000)%10 + 48;
		a[1] = (tmp/1000)%10 + 48;
		a[2] = (tmp/100)%10 + 48;
		a[3] = '.';
		a[4] = (tmp/10)%10+48;
		a[5] = tmp%10+48;
		a[6] = 0;
		Draw_String(x, y, WHITE, BLACK, &a[0], 2);
	}
}

void TFT_init(){
	spi1_master_init();
	LED_on();

	RESET_ACTIVE();
	Micro_tick_delay(1);

	RESET_IDLE();
	Micro_tick_delay(40);

	tft_write_cmd(0x01,1,1); // software reset
// necessary to wait 5msec before sending new command
// it works without delay at all

	tft_display_normal_mode();
// necessary to wait 5msec before sending new command
// it works without delay at all
	tft_pixel_format();
//	not necessary to wait

	tft_sleep_out();
// necessary to wait 5msec before sending new command
// it works without delay at all
	tft_display_on();
//	not necessary to wait

	tft_color_ALL(BLACK);
	Draw_String(10, 20, WHITE, BLACK, "t", 2);
	Draw_String(26, 28, WHITE, BLACK, "air", 1);
	Draw_String(50, 20, WHITE, BLACK, "=", 2);
	Draw_String(10, 40, WHITE, BLACK, "t", 2);
	Draw_String(26, 48, WHITE, BLACK, "object", 1);
	Draw_String(74, 40, WHITE, BLACK, "=", 2);
	Draw_String(10, 60, WHITE, BLACK, "E=0.99", 2);
}


void Micro_tick_delay(uint32_t tick){
	tick*=48;
	while (tick--);
}

void TickDelay(uint32_t tick){
	while(tick--);
}

void tft_write_cmd(uint8_t cmd, uint8_t *data, uint8_t size) {
	if (size == 0) {
		return;
	}

	DC_COMMAND();
	spi1_SendDataDMA_1Byte(&cmd, 1);
	Micro_tick_delay(1);

	DC_DATA();
	spi1_SendDataDMA_1Byte(&data[0], size);
	Micro_tick_delay(8); // experimental value
}

void tft_sleep_out() {
	DC_COMMAND();
	uint8_t data=Sleep_out;
	spi1_SendDataDMA_1Byte(&data, 1);
	Micro_tick_delay(3100); // experimental value
}

void tft_display_off() {
	tft_write_cmd(Display_OFF, 0, 0);
}

void tft_display_on() {
	uint8_t data=ROTATION;
	tft_write_cmd(Display_ON, &data, 1);
}

void tft_display_normal_mode() {
	DC_COMMAND();
	uint8_t data=Normal_Display_mode;
	spi1_SendDataDMA_1Byte(&data, 1);
}

void tft_pixel_format() {
	uint8_t data = Pixel_16_bits;
	tft_write_cmd(Pixel_Format_Set, &data, 1);
}

//column - y
void tft_set_column(uint16_t col_start, uint16_t col_end) {
	uint8_t data_column[4] = {	(uint8_t)((col_start >> 8) & 0xFF),
								(uint8_t)(col_start & 0xFF),
								(uint8_t)((col_end >> 8) & 0xFF),
								(uint8_t)(col_end & 0xFF)};

	tft_write_cmd(Column_Address_Set, &data_column[0], 4);
}

//row - x
void tft_set_row(uint16_t row_start, uint16_t row_end) {
	uint8_t data_row[4] = {	(uint8_t)(row_start >> 8),
							(uint8_t)(row_start & 0xFF),
							(uint8_t)(row_end >> 8),
							(uint8_t)(row_end & 0xFF)};
	tft_write_cmd(Page_Adress_Set, &data_row[0], 4);
}

void tft_ram_write(uint8_t *data, uint8_t size) {
	tft_write_cmd(Memory_Write, &data[0], size);
}

void tft_set_region(uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end) { // selecting an area
	uint8_t data = 0x0;
	tft_write_cmd(Memory_Access_Control, &data, 1);

	tft_set_column(col_start, col_end);
	tft_set_row(row_start, row_end);
	tft_ram_write(&data, 1);
}

void tft_color_ALL(uint16_t color) {
	tft_set_region(0,X_SIZE,0,Y_SIZE);
	uint8_t color_array[4];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for (int i = 0; i<76800; i++)
	{
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}

void tft_color_XY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color) {
	if (x1>X_SIZE || y1>Y_SIZE || x2>X_SIZE || y2>Y_SIZE)
		return;
	if (x1>x2) swap(&x1,&x2);
	if (y1>y2) swap(&y1,&y2);
	tft_set_region(x1,x2,y1,y2);
	int count_pixels = (x2-x1+1)*(y2-y1+1);
	uint8_t color_array[2];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for(uint32_t i = 0; i < count_pixels+1; i++){
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}

void swap(uint16_t *a, uint16_t *b) {
	uint16_t t;
	t=*a;
	*a=*b;
	*b=t;
}
