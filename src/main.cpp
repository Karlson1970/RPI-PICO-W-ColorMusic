#include "main.h"

#define RED_HSV         0     // красный в формате HSV
#define ORANGE_HSV      30    // Оранжевый
#define YELLOW_HSV      60    // желтый
#define GREEN_HSV       120   // зеленый
#define LIGHT_BLUE_HSV  180   // голубой
#define BLUE_HSV        240   // синий
#define VIOLET_HSV      270   // фиолетовый
#define LED_PIN    8  // Вход подключения ленты WS2812b (встроенный светодиод на pin16 RP2040Zero)
#define LED_COUNT 60  // количество светодиодов на ленте
#define PX1 62        // начальная координата вывода формы сигнала по x первый канал
#define PX2 65        // начальная координата вывода формы сигнала по x второй канал
#define PY1 16        // максимальное значение координаты по y для вывода формы сигнала
#define SAMPLES 128   // количество семплов для FFT

const uint8_t TitleBitmap [] PROGMEM = {
	// 'Заставка при первом включении, 128x48px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0e, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0e, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x70, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x70, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x70, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0e, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0xe0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x77, 0x70, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0xe0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x77, 0x70, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x77, 0x70, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0x0e, 0xe0, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x70, 0x07, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0x0e, 0xe0, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x70, 0x07, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0x0e, 0xe0, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x70, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 0x0e, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x07, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 0x0e, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x07, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 0x0e, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x07, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x0e, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x0e, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x0e, 0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x70, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x70, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77
};

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
arduinoFFT FFT = arduinoFFT(); // объявляем класс FFT 

double vRealCh1[SAMPLES];
double vImagCh1[SAMPLES];
double vRealCh2[SAMPLES]; 
double vImagCh2[SAMPLES];
int16_t waveCh1[SAMPLES];       // необработанные данные с аналоговых каналов 
int16_t waveCh2[SAMPLES];
uint16_t MaxCh1 = 0;
uint16_t MaxCh2 = 0;

// Пины энкодера
const uint encoderPinA = 15;
const uint encoderPinB = 14;
int16_t encoderPos = 0;
int8_t enccondition = 0;      //состояние энкодера 0 не было поворота ручки; 1 -поворот по часовой стрелке; -1 против часовой
uint8_t ModeShowProg = 127;   //Режим работы программы 
                              // 0 -126 - зарезервировано для режимов работы ленты без музыки. просто разные эффекты.
                              // 127 - пятицветный режим работы. музыка кодируется пятью цветами и выводится на ленту (класическая цветомузыка)
                              // 128 - режим когда музыка кодируется 30 цветов на канал.
                              // 129 - режим работы когда на ленту выводится уровень сигнала.
                              // 

// Функция рисует столбик на экране высотой заданой в x(номер столбца), y (0-100)
void DrawColumnDisplay(uint8_t x, uint8_t y) {
  y = map(y, 0, 100, 63, 15);    // Преобразуем входной параметр в высоту в пикселях
  x = x * 4;
  for (int i = 63; i > y; i--){
    u8g2.drawLine(x, i, x + 2, i);
  }   
}

void showWaveform() {          // Показываем форму сигнала
  for (int i = 0; i < 52; i++) {
    u8g2.drawLine(PX2 + i, PY1 - (waveCh1[i * 2]) / 256, PX2 + i + 1, 16 - (waveCh1[i * 2 + 1] / 256)); // Первый канал
    u8g2.drawLine(PX1 - i, PY1 - (waveCh2[i * 2]) / 256, PX1 - i - 1, 16 - (waveCh2[i * 2 + 1] / 256)); // Второй канал
  }
}

void ClearDisplayRandom(){    //Очищаем экран разными способами (выбирается случайно)
  uint8_t ModeClear = random(0,4);
  switch (ModeClear){
  case 0:
    for (int i = 0; i < 128; i++){
      u8g2.setDrawColor(1);
      u8g2.drawLine(i, 0, i, 63);
      u8g2.sendBuffer();
      u8g2.setDrawColor(2);
      u8g2.drawLine(i, 0, i, 63);
    }
  break;
  case 1:
    for (int i = 0; i < 64; i++){
      u8g2.setDrawColor(1);
      u8g2.drawLine(i, 0, i, 63);
      u8g2.drawLine(127 - i, 0, 127 - i, 63);
      u8g2.sendBuffer();
      u8g2.setDrawColor(2);
      u8g2.drawLine(i, 0, i, 63);
      u8g2.drawLine(127 - i, 0, 127 - i, 63);
    }
  break;
  case 2:
    for (int i = 0; i < 64; i++){
      u8g2.setDrawColor(1);
      u8g2.drawLine(0, i, 127, i);
      u8g2.sendBuffer();
      u8g2.setDrawColor(2);
      u8g2.drawLine(0, i, 127, i);
    }
  break;
  case 3:
     for (int i = 0; i < 32; i++){
      u8g2.setDrawColor(1);
      u8g2.drawLine(0, i, 127, i);
      u8g2.drawLine(0, 63 - i, 127, 63 - i);
      u8g2.sendBuffer();
      u8g2.setDrawColor(2);
      u8g2.drawLine(0, i, 127, i);
      u8g2.drawLine(0, 63 - i, 127, 63 - i);
    }
  break;
  }
  u8g2.setDrawColor(1);
}

void encoderA() {
  // Считываем значения с обоих пинов энкодера
  int a = digitalRead(encoderPinA);
  int b = digitalRead(encoderPinB);

// По изменению состояния A или B пина
// Определяем направление вращения энкодера
  if (a == HIGH && b == LOW) {  
    encoderPos++;
    enccondition = 1;  
  } 
}

void encoderB() {
// Считываем значения с обоих пинов энкодера
  int a = digitalRead(encoderPinA);
  int b = digitalRead(encoderPinB);

// По изменению состояния A или B пина
// Определяем направление вращения энкодера
  if (a == LOW && b == HIGH){
    encoderPos--;
    enccondition = -1;
  }
}

// Диапазон ввода параметра h (0 ~ 360), s (0 ~ 100), v (0 ~ 100), выход - цвет RGB в 32-хбитном формате
uint32_t hsv_to_rgb(int h,int s,int v)
{
    float C = 0, X = 0, Y = 0, Z = 0, R =0, G = 0, B = 0;
    int i = 0;
    float H = (float)(h);
    float S = (float)(s)/100.0;
    float V = (float)(v)/100.0;
    if(S == 0)
        R = G = B = V;
    else
    {
        H = H/60;
        i = (int)H;
        C = H - i;
        X = V * (1 - S);
        Y = V * (1 - S*C);
        Z = V * (1 - S*(1-C));
        switch(i){
            case 0 : R = V; G = Z; B = X; break;
            case 1 : R = Y; G = V; B = X; break;
            case 2 : R = X; G = V; B = Z; break;
            case 3 : R = X; G = Y; B = V; break;
            case 4 : R = Z; G = X; B = V; break;
            case 5 : R = V; G = X; B = Y; break;
        }
    }
    R = R * 255;
    G = G * 255;
    B = B * 255;
    uint32_t RGB32 = (int(R) << 16) | (int(G) << 8) | int(B); 
    return RGB32;
}

//Выбор режима работы цветомузыки
void Chance_ColorMusic(){
  uint16_t amplch1 = 0;
  uint16_t amplch2 = 0;
  
  uint32_t color_ch1, color_ch2;
  //проверяем все частоты от 800гц до 19600гц с шагом 400гц
  // i = 0 постоянная составляющая
  // i = 1 недостаточно хорошо определяется много шума
  //итого 48 частот
  for (uint8_t i = 2; i < 50; i++){
    if (vRealCh1[i] > 200) {
      vRealCh1[i] = vRealCh1[i] - 200;
    } else vRealCh1[i] = 0; // Все что ниже 200 считем шумом и не учитываем в подсчетах
    if (vRealCh2[i] > 200){
      vRealCh2[i] = vRealCh2[i] - 200;
    } else vRealCh2[i] = 0; // то же для второго канала
    amplch1 = constrain(vRealCh1[i], 0, 17000);   //обрежем сигнал по амплитуде  до 17000 будем считать это максимумом
    amplch2 = constrain(vRealCh2[i], 0, 17000);
    amplch1 = map(amplch1, 0, 17000, 0, 100);     // переведем амплитуду в проценты яркости
    amplch2 = map(amplch2, 0, 17000, 0, 100);
    if ((i + 1) % 3 == 0){                        // Берем каждый третий элемент массива
      uint8_t j = (i + 1) / 3 - 1;                // Находим знакоместо по x для вывода столбика
      DrawColumnDisplay(j, amplch1);              // Создаем столбик для первого канала
      DrawColumnDisplay(31-j, amplch2);         // Создаем столбик для второго канала
    }
    if (ModeShowProg == 127){
      ws2812fx.stop();
      switch (i) {
        case 2:  {// 800гц
          color_ch1 = hsv_to_rgb(RED_HSV, 100, amplch1);
          color_ch2 = hsv_to_rgb(RED_HSV, 100, amplch2);
          ws2812fx.setSegment(0, 0, 5, FX_MODE_STATIC, color_ch1, 30);
          ws2812fx.setSegment(1, 54, 59, FX_MODE_STATIC, color_ch2, 30,REVERSE);
          break;
        }
        case 6:  {//2400гц
          color_ch1 = hsv_to_rgb(ORANGE_HSV, 100, amplch1);
          color_ch2 = hsv_to_rgb(ORANGE_HSV, 100, amplch2);
          ws2812fx.setSegment(2, 6, 11, FX_MODE_STATIC, color_ch1, 30);
          ws2812fx.setSegment(3, 48, 53, FX_MODE_STATIC, color_ch2, 30,REVERSE);
          break;
        }
        case 10:   {//4000гц
          color_ch1 = hsv_to_rgb(GREEN_HSV, 100, amplch1);
          color_ch2 = hsv_to_rgb(GREEN_HSV, 100, amplch2);
          ws2812fx.setSegment(4, 12, 17, FX_MODE_STATIC, color_ch1, 30);
          ws2812fx.setSegment(5, 42, 47, FX_MODE_STATIC, color_ch2, 30,REVERSE);
          break;
        }
        case 16:    {//6400гц
          color_ch1 = hsv_to_rgb(BLUE_HSV, 100, amplch1);
          color_ch2 = hsv_to_rgb(BLUE_HSV, 100, amplch2);
          ws2812fx.setSegment(6, 18, 23, FX_MODE_STATIC, color_ch1, 30);
          ws2812fx.setSegment(7, 36, 41, FX_MODE_STATIC, color_ch2, 30,REVERSE);
          break;
        }
        case 25:    {//10000гц
          color_ch1 = hsv_to_rgb(VIOLET_HSV, 100, amplch1);
          color_ch2 = hsv_to_rgb(VIOLET_HSV, 100, amplch2);
          ws2812fx.setSegment(8, 24, 29, FX_MODE_STATIC, color_ch1, 30);
          ws2812fx.setSegment(9, 30, 35, FX_MODE_STATIC, color_ch2, 30,REVERSE);
          break;
        }
      }
      ws2812fx.start();
    }
    if (ModeShowProg == 128){
      if (i < 32){
        color_ch1 = hsv_to_rgb((i - 2) * 12, 100, amplch1);
        color_ch2 = hsv_to_rgb((i - 2) * 12, 100, amplch2);
        ws2812fx.setPixelColor(i - 2, color_ch1);
        ws2812fx.setPixelColor(61 - i,color_ch2);
      }
    }
  }
    if (ModeShowProg == 129){
      ws2812fx.stop();
      ws2812fx.removeActiveSegment(2);
      ws2812fx.removeActiveSegment(3);
      ws2812fx.removeActiveSegment(4);
      ws2812fx.removeActiveSegment(5);
      ws2812fx.removeActiveSegment(6);
      ws2812fx.removeActiveSegment(7);
      ws2812fx.removeActiveSegment(8);
      ws2812fx.removeActiveSegment(9);
      if (MaxCh1 > 48){
        MaxCh1 = MaxCh1 - 48;
      } else MaxCh1 = 0;
      if (MaxCh2 > 48){
        MaxCh2 = MaxCh2 - 48;
      } else MaxCh2 = 0;
      MaxCh1 = map(MaxCh1, 0, 2000, 0, LED_COUNT / 2 - 1);
      MaxCh2 = map(MaxCh2, 0, 2000, LED_COUNT - 1, LED_COUNT / 2);
        ws2812fx.setSegment(0, 0, MaxCh1, FX_MODE_STATIC, RED, 30);
      ws2812fx.setSegment(1, MaxCh2, LED_COUNT-1, FX_MODE_STATIC, RED, 30, REVERSE);
      ws2812fx.start();
      MaxCh1 = 0;
      MaxCh2 = 0;
    }
  if (ModeShowProg == 127 || ModeShowProg == 129){
    ws2812fx.service();
  }
}

void delay80ns(int t) {         // Функция задержки примерно t * 80ns = 400ns (0.4 us)
  volatile uint32_t x;          // Волатильная переменная для растягивания времени (объявлена volatile, чтобы оптимизатор ее не удалил)
  for (int i = 0; i < t; i++) {
    x++;                        // Инкрементирование 32-бит с большей временной нагрузкой, как раз 80нс
  }
}

void calculateFFT() {
  for (int i = 0; i < SAMPLES; i++) {
    waveCh1[i] = analogRead(26);                                      // Заполняем данными с аналогового входа массив
    waveCh2[i] = analogRead(28);                                      // то же для 2 канала     
    delay80ns(50);                                                    //задержка 0.4 микросекунды
  }
  for (int i = 0; i < SAMPLES; i++) {
    int16_t TempCh1 = waveCh1[i] - 2048;
    int16_t TempCh2 = waveCh2[i] - 2048;
    vRealCh1[i] = TempCh1;                                  // поскольку на АЦП сигнал подается со смещением то 0 находится на отметке 2048
    vRealCh2[i] = TempCh2;
    vImagCh1[i] = 0;
    vImagCh2[i] = 0;
    if (abs(TempCh1) > MaxCh1) MaxCh1 = abs(TempCh1); 
    if (abs(TempCh2) > MaxCh2) MaxCh2 = abs(TempCh2); 
  }  
  FFT.Windowing(vRealCh1, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); // применим окно Хэмминга к введенным данным VRealCh1.
  FFT.Windowing(vRealCh2, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); // тоже для 2 канала
  FFT.Compute(vRealCh1, vImagCh1, SAMPLES, FFT_FORWARD);              // преобразование Фурье для введенных данных. канал 1
  FFT.Compute(vRealCh2, vImagCh2, SAMPLES, FFT_FORWARD);              // канал 2
  FFT.ComplexToMagnitude(vRealCh1, vImagCh1, SAMPLES);                // вычисляем амплитуду (модуль) комплексного спектра канала 1.
  FFT.ComplexToMagnitude(vRealCh2, vImagCh2, SAMPLES);                // то же канал 2
}

void setup() {
// аппаратные прерывания от энкодера канал A и канал B
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), encoderB, RISING);
  randomSeed(analogRead(27)); // сменим зерно рандомайзера
//включаем внутренний датчик температуры
  //adc_init();
 // adc_set_temp_sensor_enabled(1);
  analogReadResolution(12); // Сообщим библиотеке ардуино что у нас 12bit разрешение ADC
  
  ws2812fx.init();
  ws2812fx.setBrightness(100);
  //ws2812fx.setSpeed(1000);
  //ws2812fx.setColor(0x007BFF);
  //ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();

  u8g2.begin();
  u8g2.setFont(u8g2_font_8x13_t_cyrillic);
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();        
  u8g2.clearBuffer();
  u8g2.setCursor(5, 0);
  u8g2.print("Color Music 1.0");
  u8g2.drawBitmap(0, 16, 16, 48, TitleBitmap);
  u8g2.sendBuffer();
  delay(3000);
  ClearDisplayRandom();
}

void loop() {
  calculateFFT();
  u8g2.clearBuffer();
  if (ModeShowProg > 126 && ModeShowProg < 130){
    ModeShowProg = ModeShowProg + enccondition;
    if (ModeShowProg == 130){
      ModeShowProg = 127;
    }
    if (ModeShowProg == 126){
      ModeShowProg = 129;
    }
    enccondition = 0;
    Chance_ColorMusic();
    if (ModeShowProg == 128){
        ws2812fx.show();
        delay(1);
    }
  }
  showWaveform();
  u8g2.sendBuffer();
  delay(1);  
}  