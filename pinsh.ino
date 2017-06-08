/*
 * Command shell to control Arduino
 * 
 * aa
 *
 * Uses this library: 
 *  https://github.com/joshmarinacci/CmdArduino/
 */

#include <Cmd.h>

int start_pin =  1; int end_pin   = 19;

void setup() {
  Serial.begin(9600);         cmdInit(&Serial);
  cmdAdd("get"   ,  get_pin); cmdAdd("set"  ,set_pin);
  cmdAdd("toggle",  toggle) ; cmdAdd("pulse",pulse  );
  cmdAdd("showall",show_all); cmdAdd("help", help   );

  for (int pin = start_pin ; pin <= end_pin ; pin++) 
  { pinMode( pin, OUTPUT); }
}

void loop() { cmdPoll(); }

int is_valid_pin(int pin_number) {
  Stream *s = cmdGetStream();
  if (pin_number == NULL) { s->println("pin_number is NULL. returning.\n"); return 0; }
  if (pin_number == -1)   { s->println("pin_number = -1. returning.\n");    return 0; }
  if (pin_number < start_pin || pin_number > end_pin)
  { s->println("pin_number must be in range 1-19"); return 0; }
  return 1;
}

int is_valid_value(int value) {
  Stream *s = cmdGetStream();
  if (value != HIGH && value != LOW) {
    s->println("Value must be \"high\" or \"low\". returning."); return 0; }
  return 1;
}

void get_pin(int arg_cnt, char **args) { 
  Stream  *s = cmdGetStream(); int pin = -1;
  if (arg_cnt != 2) {
    s->println("usage: get <pin (1-19)>"); return; }
  pin = cmdStr2Num(args[1], 10); // translate ascii to integer 
  if (!is_valid_pin(pin)) { return; }
  s->print("pin ");
  s->print(pin); 
  s->print(" = "); 
  s->print(digitalRead(pin));
  s->print("\n");
  return;
}

int str2highlow(char *cmd) {
  if (strncmp( cmd, "high", strlen("high")) == 0) { return 1 ; }
  if (strncmp( cmd, "low" , strlen("low"))  == 0) { return 0 ; }   
  return -1;
}

void set_pin(int arg_cnt, char **args) {
  Stream *s = cmdGetStream(); int pin = -1; int pin_value = 0;
  if (arg_cnt != 3) {
    s->println("usage: set <pin (1-19)> <high|low>"); return; 
  }
  pin = cmdStr2Num(args[1], 10); // translate incoming ascii to integer
  if ( !is_valid_pin(pin)) { return; }
  pin_value = str2highlow(args[2]);
  if (!is_valid_value(pin_value)) { return; }
  s->print("setting pin "); s->print(pin); s->print("->"); s->print(args[2]); 
  s->print("\n");
  digitalWrite(pin, pin_value);
  return;
}

void toggle(int arg_cnt, char **args) {
  Stream *s = cmdGetStream(); int pin = -1;
  if (arg_cnt != 2) {
    s->println("usage: toggle <pin_number");
    return;
  }
  pin = cmdStr2Num(args[1], 10); // translate incoming ascii to integer
  if ( !is_valid_pin(pin)) { return; }
  s->print("toggling pin "); s->print(pin); s->print("\n");
  digitalWrite(pin,!digitalRead(pin));
  return;
}

void pulse(int arg_cnt, char **args)
{ // toggle, wait, toggle back
  Stream *s = cmdGetStream(); int pin; int mils;
  if (arg_cnt != 3) { s->println("usage: pulse <pin_number> <milliseconds>"); return; }
  s->print("pulsing pin "); s->print(pin); s->print(" for # milliseconds.");
  return;
}

void show_all(int arg_cnt, char **args)
{
  Stream *s = cmdGetStream(); int pin;
  if (arg_cnt != 1) { s->println("usage: showall"); return; }
  for ( pin=start_pin; pin <= end_pin; pin++ ) {
    s->print(pin); s->print(" = "); s->print(digitalRead(pin)); s->print("\n");
  }
}

void help(int arg_cnt, char **args)
{
  Stream *s = cmdGetStream(); 
  s->println("help");
}


