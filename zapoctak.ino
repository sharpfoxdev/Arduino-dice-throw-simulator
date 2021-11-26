#include<funshield.h>

//global variables used by dice
int dice_type = 1; //for looping through array of dice types
int dice_throws = 2;
int dice_result = 0;
bool dice_in_config_mode = true; //when false, we are in normal (game) mode, when true, we are in config mode
static constexpr int DICE_TYPES_COUNT = 7; //number of dice types
static constexpr int DICE_MAX_THROWS = 9; //maximum amount of throws
int arr_dice_type_int[DICE_TYPES_COUNT] = {4, 6, 8, 10, 12, 20, 100}; //to find out number of faces of dice to calculate result


//arrays of strings for dice (number of throws and faces) used for printing on display
const char* arr_dice_type_strings[7] = {"d4 ", "d6 ", "d8 ", "d10", "d12", "d20", "d00"};
const char* arr_dice_throws_strings[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};


//arrays and constants for displaying characters on display
static constexpr int NUMBER_ZERO = 0xC0;
static constexpr int NUMBER_ONE = 0xF9;
static constexpr int NUMBER_TWO = 0xA4;
static constexpr int NUMBER_THREE = 0xB0;
static constexpr int NUMBER_FOUR = 0x99;
static constexpr int NUMBER_FIVE = 0x92;
static constexpr int NUMBER_SIX = 0x82;
static constexpr int NUMBER_SEVEN = 0xF8;
static constexpr int NUMBER_EIGHT = 0x80;
static constexpr int NUMBER_NINE = 0x90;
int arr_numbers[10] = {
  NUMBER_ZERO,  NUMBER_ONE,  NUMBER_TWO,  NUMBER_THREE,  NUMBER_FOUR,  
  NUMBER_FIVE,  NUMBER_SIX,  NUMBER_SEVEN,  NUMBER_EIGHT,  NUMBER_NINE
};
static constexpr int LETTER_D = 0xA1;

//global variables used by display
static constexpr int DISPLAY_POSITIONS_COUNT = 4;
int arr_positions[DISPLAY_POSITIONS_COUNT] = {0b0001, 0b0010, 0b0100, 0b1000};
int display_position = 0; //position which shines on display in loop
bool display_animation_mode = false; //true when throwing dice

//global variables used by buttons
static constexpr int BUTTONS_COUNT = 3;
int arr_buttons[BUTTONS_COUNT] = {button1_pin, button2_pin, button3_pin};
bool button1_was_pressed = false; //if the button was pressed in the last iteration of loop
bool button2_was_pressed = false;
bool button3_was_pressed = false;
long int button1_time_pressed; //time when we started to press button1 to calculate lenght of press

//global variables and constants used for animation
static constexpr int ANIMATION_DELAY = 70;
static constexpr int GLYPH_A = 0xFE;
static constexpr int GLYPH_B = 0xFD;
static constexpr int GLYPH_C = 0xFB;
static constexpr int GLYPH_D = 0xF7;
static constexpr int GLYPH_E = 0xEF;
static constexpr int GLYPH_F = 0xDF;
static constexpr int ANIMATION_GLYPHS_COUNT = 6;
int arr_animation_glyphs [ANIMATION_GLYPHS_COUNT] = {
  GLYPH_A, GLYPH_B, GLYPH_C, GLYPH_D, GLYPH_E, GLYPH_F
};
int animation_position = 0; 
long int animation_moved; //last time animation moved

void setup() {
  pinMode(latch_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  for(int i = 0; i < BUTTONS_COUNT; i++){
    pinMode(arr_buttons[i], INPUT);    
  }
}

void loop() {
  dice_try_throw(); //checks all buttons and decides accordingly what to do
  dice_try_change_number_of_throws(); 
  dice_try_change_type();
  if(display_animation_mode){
    display_animation();
  }
  else{
    if(dice_in_config_mode){
      display_settings();
    }
    else{
      display_result();
    }
  }
}

void dice_try_throw(){
  bool button1_is_pressed = !digitalRead(button1_pin); //current state of the button, true is pressed
  if(!button1_was_pressed && button1_is_pressed){ //wasnt pressed and now it is
    button1_was_pressed = true;
    dice_in_config_mode = false;
    display_animation_mode = true;
    button1_time_pressed = micros(); //starts measuring time of pressed button
  }
  else if(button1_was_pressed && button1_is_pressed){//was pressed last iteration and still is pressed, replaces while(button_pressed) loop
    //display was switched into animation mode, animation is executed
  }
  else if(button1_was_pressed && !button1_is_pressed){ //stopped pressing button
    long int time_elapsed = micros() - button1_time_pressed; //checks for elapsed time when the button was pressed
    long int dice_sum = 0;
    long long int generator_seed = time_elapsed; //making first seed from elapsed time
    for(int i = 0; i < dice_throws; i++){ //i-times throws the dice
      //generates number from elapsed time and then converts it to the type of dice
      generator_seed = generator(generator_seed); //after first iteration the new seed is the output from the first call of generator
      dice_sum += (generator_seed % arr_dice_type_int[dice_type])+1;
    }
    dice_result = dice_sum;
    button1_was_pressed = false;
    display_animation_mode = false;
  }
  else{//didnt press the button at all 
    
  }
}

long long int generator(long long int seed){
  //linear congruential generator of pseudo-random numbers
  long long int result = (((25214903917  * seed) + 13) % (281474976710656)) >> 33;
  return result;
}

void display_animation(){
  if(millis() - animation_moved > ANIMATION_DELAY){//time to move animation
    animation_position++;
    if(animation_position >= ANIMATION_GLYPHS_COUNT){
      animation_position = 0;
    }
    animation_moved = millis();
  }
  //glyph being displayed is one frame ahead of the one to its left
  int glyph = (animation_position+display_position) % ANIMATION_GLYPHS_COUNT;
  display_write_glyph(arr_animation_glyphs[glyph], arr_positions[display_position]);
  display_position++;
  if(display_position >= 4){
    display_position = 0;
  }
}

void display_result(){
  int position = DISPLAY_POSITIONS_COUNT - 1; //index of last position
  int number = dice_result; //copy of result
  while(position != display_position){
    number = number / 10; //gets to the correct digit from copy
    position--;
  }
  int digit = number % 10; //gets the digit, which should be shown on display on given display_position
  display_write_glyph(arr_numbers[digit], arr_positions[display_position]);
  if(display_position == 0){ //change of the position to the one that should be shown in the next iteration
    display_position = DISPLAY_POSITIONS_COUNT - 1;
  }
  else{
    display_position--;
  }
}

void dice_try_change_number_of_throws(){
  bool button2_is_pressed = !digitalRead(button2_pin);
  if(!button2_was_pressed && button2_is_pressed){
    dice_in_config_mode = true;
    dice_throws++;
    if(dice_throws > DICE_MAX_THROWS){
      dice_throws = 1;
    }
    button2_was_pressed = true;
  }
  else if(button2_was_pressed && button2_is_pressed){
    display_settings();
  }
  else if(button2_was_pressed && !button2_is_pressed){
    button2_was_pressed = false;
  }
  else{
    
  }
}

void dice_try_change_type(){
  bool button3_is_pressed = !digitalRead(button3_pin);
  if(!button3_was_pressed && button3_is_pressed){
    dice_in_config_mode = true;
    dice_type++;
    if(dice_type >= DICE_TYPES_COUNT){
      dice_type = 0;
    }
    button3_was_pressed = true;
  }
  else if(button3_was_pressed && button3_is_pressed){
    display_settings();
  }
  else if(button3_was_pressed && !button3_is_pressed){
    button3_was_pressed = false;
  }
  else{
    
  }
}

void display_settings(){
  if(display_position == 0){
    // shows the string of number of throws
    display_write_character(*(arr_dice_throws_strings[dice_throws]), display_position); 
  }
  else{
    // shows the correct position in string of "d" and number of faces of dice
    display_write_character(*(arr_dice_type_strings[dice_type] + display_position - 1), display_position); 
  }
  display_position++;
  if(display_position >= DISPLAY_POSITIONS_COUNT){
    display_position = 0;
  }
}

void display_write_character(unsigned char letter, int position)
{
  if (letter == (unsigned char)'d' || letter == (unsigned char)'D' ){
      display_write_glyph(LETTER_D, arr_positions[position]);
  }
  else if (letter >= (unsigned char)'0' && letter <= (unsigned char)'9' ){
      display_write_glyph(arr_numbers[letter - '0'], arr_positions[position]);
  }
  else{
    display_write_glyph(0xFF, arr_positions[position]); //space, blank glyph
  }
}

void display_write_glyph(byte glyph, byte position){
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, MSBFIRST, glyph);
  shiftOut(data_pin, clock_pin, MSBFIRST, position);
  digitalWrite(latch_pin, HIGH);
}
