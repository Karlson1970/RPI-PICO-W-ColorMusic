# RPI-PICO-W-ColorMusic
 Цветомузыка для Raspberry Pi Pico (RP2040)

 Дисплей SSD1306 I2C SDA - GPIO4 SCL - GPIO5.
 Светодиодная лента WS2812B - GPIO8.
 Encoder GPIO15 GPIO14.
 Аналоговые входы звукового сигнала подключены к ADC0 и ADC2. Поскольку это звук. то на вход сигнал подается со смещением. Нулем считатся половина напряжения. 
 Схема подключения в папке schematic.
 На дисплее отображается форма сигнала и спектр сигнала полученный с помощью FFT.
 С помощью энкодера можно изменить режим цветомузыки.
 1 режим -6 светодиодов по 5 частотам на каждый канал звука. 800гц - красный. 2400гц - оранжевый. 4000гц - зеленый. 6400гц - Синий. 10000гц - фиолетовый.
 (для всех частот указан пик, есстественно они захватывают соседние частоты. диапозон каждого канала можно проверить генератором).
 2 режим -30 частот на каждый канал. Каждая частота выводится своим цветом. шаг 400гц начиная с частоты 800гц (пик).
 3 режим - Индикатор максимальной громкости на каждый канал.

 Код ужасен. Написан по принципу лишь бы заработал. Оптимизацией и улучшением займусь как только появится время.
