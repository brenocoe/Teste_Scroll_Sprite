#include <Arduino.h>
#include <TFT_eSPI.h>
#include "roda_num_black.h"
#include "roda_num_red.h"

// declaracao de prototipos
void update_altitude();
void show_altitude();
void show_point();
void show_number(int dig, int num, int step);
void print_number(int dig, int angle, float alt);
void print_number_void(int dig);

// declaracao de variaveis locais
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite painelD0 = TFT_eSprite(&tft);
TFT_eSprite disco0 = TFT_eSprite(&tft);
TFT_eSprite painelD1 = TFT_eSprite(&tft);
TFT_eSprite disco1 = TFT_eSprite(&tft);
TFT_eSprite painelD2 = TFT_eSprite(&tft);
TFT_eSprite disco2 = TFT_eSprite(&tft);
TFT_eSprite painelD3 = TFT_eSprite(&tft);
TFT_eSprite disco3 = TFT_eSprite(&tft);

int larg_jan[4] = {60, 60, 60, 60};
int alt_jan[4] = {90, 90, 90, 90};
int larg_img[4] = {400, 400, 400, 400};
int alt_img[4] = {400, 400, 400, 400};
int pivot_x[4] = {-145, -145, -145, -145};
int pivot_y[4] = {44, 44, 44, 44};
int start_angle[4] = {0, 0, 0, 0};
int pos_x[4] = {25, 95, 165, 235};
int pos_y[4] = {40, 40, 40, 40};
int step[4] = {9, 9, 18, 18};
int displayed_number[4] = {0, 0, 0, 0};
bool descendo = true;
float Alt_Max = 12345;
float Alt_Min = 0;
float Altitude = Alt_Max;
float Dif_Alt = 318.97 / 3; // aprox 350km/h ou 318.97ft/seg
int espera = 1000 / 3;      // 1/3 seg

void setup()
{
  //
  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_WHITE);
  //
  painelD0.createSprite(larg_jan[0], alt_jan[0]);
  painelD0.setSwapBytes(true);
  painelD0.setPivot(pivot_x[0], pivot_y[0]);
  disco0.createSprite(larg_img[0], alt_img[0]);
  disco0.setSwapBytes(true);
  //
  painelD1.createSprite(larg_jan[1], alt_jan[1]);
  painelD1.setSwapBytes(true);
  painelD1.setPivot(pivot_x[1], pivot_y[1]);
  disco1.createSprite(larg_img[1], alt_img[1]);
  disco1.setSwapBytes(true);
  //
  painelD2.createSprite(larg_jan[2], alt_jan[2]);
  painelD2.setSwapBytes(true);
  painelD2.setPivot(pivot_x[2], pivot_y[2]);
  disco2.createSprite(larg_img[2], alt_img[2]);
  disco2.setSwapBytes(true);
  //
  painelD3.createSprite(larg_jan[3], alt_jan[3]);
  painelD3.setSwapBytes(true);
  painelD3.setPivot(pivot_x[3], pivot_y[3]);
  disco3.createSprite(larg_img[3], alt_img[3]);
  disco3.setSwapBytes(true);
  //
  for (size_t i = 0; i < 4; i++)
    show_number(i, 9, step[i]);
  for (size_t i = 0; i < 4; i++)
    show_number(i, 0, step[i]);
}

void loop()
{
  update_altitude();
  show_altitude();
}

void update_altitude()
{
  if (descendo)
    Altitude -= Dif_Alt;
  else
    Altitude += Dif_Alt;

  if (Altitude < Alt_Min)
  {
    Altitude = Alt_Min;
    descendo = false;
    delay(espera * 3);
  }
  else if (Altitude > Alt_Max)
  {
    Altitude = Alt_Max;
    descendo = true;
    delay(espera * 3);
  }
  else
  {
    delay(espera);
  }
}

void show_altitude()
{
  //
  String Num_Display = "";
  String d0 = "";
  String d1 = "";
  String d2 = "";
  String d3 = "";
  //
  if (Altitude >= 1000)
  {
    // divide por 1000
    // mostra duas casas decimais
    Num_Display += "0" + String(Altitude / 1000, 2);
    d0 = Num_Display.substring(Num_Display.length() - 5, Num_Display.length() - 4);
    d1 = Num_Display.substring(Num_Display.length() - 4, Num_Display.length() - 3);
    d2 = Num_Display.substring(Num_Display.length() - 2, Num_Display.length() - 1);
    d3 = Num_Display.substring(Num_Display.length() - 1, Num_Display.length());
    //
    show_number(3, d3.toInt(), step[3]);
    show_number(2, d2.toInt(), step[2]);
    show_number(1, d1.toInt(), step[1]);
    show_number(0, d0.toInt(), step[0]);
  }
  else
  {
    // mostra centena inteira
    Num_Display += "000" + String(Altitude, 0);
    d0 = Num_Display.substring(Num_Display.length() - 4, Num_Display.length() - 3);
    d1 = Num_Display.substring(Num_Display.length() - 3, Num_Display.length() - 2);
    d2 = Num_Display.substring(Num_Display.length() - 2, Num_Display.length() - 1);
    d3 = Num_Display.substring(Num_Display.length() - 1, Num_Display.length());
    //
    show_number(3, d3.toInt(), 36);
    show_number(2, d2.toInt(), 36);
    show_number(1, d1.toInt(), 36);
    show_number(0, d0.toInt(), 36);
  }
}

void show_point()
{
  if (Altitude >= 1000)
    tft.fillSmoothCircle(159, 124, 5, TFT_RED, TFT_WHITE);
  else
    tft.fillSmoothCircle(159, 124, 5, TFT_WHITE, TFT_WHITE);
}

void show_number(int dig, int num, int step)
{
  //
  int angle = start_angle[dig];
  int dif = num - displayed_number[dig];
  //
  if ((dig == 0) && (displayed_number[dig] == 0) && (num == 0) && (dif == 0))
    print_number_void(dig);
  //
  if ((displayed_number[dig] == 0) && (num == 9))
  {
    for (size_t j = 0; j < 36; j += step)
    {
      angle -= step;
      print_number(dig, angle, Altitude);
    }
    displayed_number[dig] = num;
    start_angle[dig] = -36;
    dif = 0;
  }
  //
  if ((displayed_number[dig] == 9) && (num == 0))
  {
    for (size_t j = 0; j < 36; j += step)
    {
      angle += step;
      print_number(dig, angle, Altitude);
    }
    displayed_number[dig] = num;
    start_angle[dig] = 0;
    dif = 0;
  }
  //
  if (dif > 0)
  {
    for (size_t i = displayed_number[dig]; i < num; i++)
    {
      for (size_t j = 0; j < 36; j += step)
      {
        angle += step;
        print_number(dig, angle, Altitude);
      }
    }
    displayed_number[dig] = num;
    start_angle[dig] = angle;
  }
  //
  if (dif < 0)
  {
    for (size_t i = displayed_number[dig]; i > num; i--)
    {
      for (size_t j = 0; j < 36; j += step)
      {
        angle -= step;
        print_number(dig, angle, Altitude);
      }
    }
    displayed_number[dig] = num;
    start_angle[dig] = angle;
  }
  //
  show_point();
}

void print_number(int dig, int angle, float alt)
{
  switch (dig)
  {
  case 0:
    painelD0.fillScreen(TFT_WHITE);
    if (alt >= 1000.0)
      disco0.pushImage(0, 0, larg_img[dig], alt_img[dig], roda_num_black);
    else
      disco0.pushImage(0, 0, larg_img[dig], alt_img[dig], roda_num_red);
    disco0.pushRotated(&painelD0, angle, TFT_WHITE);
    painelD0.pushSprite(pos_x[dig], pos_y[dig]);
    break;
  case 1:
    painelD1.fillScreen(TFT_WHITE);
    if (alt >= 1000.0)
      disco1.pushImage(0, 0, larg_img[dig], alt_img[dig], roda_num_black);
    else
      disco1.pushImage(0, 0, larg_img[dig], alt_img[dig], roda_num_red);
    disco1.pushRotated(&painelD1, angle, TFT_WHITE);
    painelD1.pushSprite(pos_x[dig], pos_y[dig]);
    break;
  case 2:
    painelD2.fillScreen(TFT_WHITE);
    if (alt >= 1000.0)
      disco2.pushImage(0, 0, larg_img[dig], alt_img[dig], roda_num_black);
    else
      disco2.pushImage(0, 0, larg_img[dig], alt_img[dig], roda_num_red);
    disco2.pushRotated(&painelD2, angle, TFT_WHITE);
    painelD2.pushSprite(pos_x[dig], pos_y[dig]);
    break;
  case 3:
    painelD3.fillScreen(TFT_WHITE);
    if (alt >= 1000.0)
      disco3.pushImage(0, 0, larg_img[dig], alt_img[dig], roda_num_black);
    else
      disco3.pushImage(0, 0, larg_img[dig], alt_img[dig], roda_num_red);
    disco3.pushRotated(&painelD3, angle, TFT_WHITE);
    painelD3.pushSprite(pos_x[dig], pos_y[dig]);
    break;
  default:
    break;
  }
}

void print_number_void(int dig)
{
  switch (dig)
  {
  case 0:
    painelD0.fillScreen(TFT_WHITE);
    painelD0.pushSprite(pos_x[dig], pos_y[dig]);
    break;
  case 1:
    painelD1.fillScreen(TFT_WHITE);
    painelD1.pushSprite(pos_x[dig], pos_y[dig]);
    break;
  case 2:
    painelD2.fillScreen(TFT_WHITE);
    painelD2.pushSprite(pos_x[dig], pos_y[dig]);
    break;
  case 3:
    painelD3.fillScreen(TFT_WHITE);
    painelD3.pushSprite(pos_x[dig], pos_y[dig]);
    break;
  default:
    break;
  }
}