short O_SER = 5;
short O_RCLK = 6;
short O_SRCLK = 7;
bool light_map[8][8];
              //r1,0   c4,1   c6,2   r4,3   c1,4   r2,5   c7,6   c8,7   r5,8   r7,9   c2,10  c3,11  r8,12  c5,13  r6,14  r3,15
bool line[64] = {false, false, false, true, true , true , false, false, true , true , true , false, true , false, true , true, };
short ColumnPin[8] = {4, 10, 11, 1, 13, 2, 6, 7};
short RowPin[8] =       {0, 5, 15, 3, 8, 14, 9, 12};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( O_SER,  OUTPUT );
  pinMode( O_RCLK, OUTPUT );
  pinMode( O_SRCLK, OUTPUT );

  digitalWrite(O_RCLK, LOW);
     for(int i = 0; i < 64;i++){
       digitalWrite(O_SER, LOW);
       //クロックパルスを立ち上げる
      digitalWrite(O_SRCLK, HIGH);
      digitalWrite(O_SRCLK, LOW);
       //-----------------------------------------
     }
     digitalWrite(O_RCLK, HIGH);
   for(int i = 0; i < sizeof(light_map) / sizeof(light_map[0]); i++){
    for (int j = 0; j < sizeof(light_map[0]) / sizeof(light_map[0][0]); j++){
      light_map[i][j] = false;
    }
  }
  for (int shift_num = 0; shift_num < 4; shift_num++){
    for (int i = 0; i < sizeof(RowPin) / sizeof(RowPin[0]); i++){
      line[RowPin[i] + 16 * shift_num] = true;
    }
  }
  for (int shift_num = 0; shift_num < 4; shift_num++){
    for (int i = 0; i < sizeof(ColumnPin) / sizeof(ColumnPin[0]); i++){
      line[ColumnPin[i] + 16 * shift_num] = false;
    }
  }
}             

int count = 0;
int count_r = 0;
int count_c = 0;
int stute = 0;
/*
void debugText(){
    for (int shift_num = 1; shift_num < 5; shift_num++){
      for (int l = 0; l < sizeof(RowPin) / sizeof(RowPin[0]); l++){
        if(line[RowPin[l] * shift_num] != true){
          Serial.print("Row " + String(shift_num) + " : " + String(l) + ", ");
        }
        if(line[ColumnPin[l] * shift_num] != false){
          Serial.print("Col " + String(shift_num) + " : " + String(l) + ", ");
        }
      }
    }
    Serial.println();
  }
void loop() {
  line[20] = true;
digitalWrite(O_RCLK, LOW);
  for(int i = 63; i != -1;i--){
    if (line[i] == true){
      digitalWrite(O_SER, HIGH);
    }
    else{
      digitalWrite(O_SER, LOW);
    }
    //クロックパルスを立ち上げる
    digitalWrite(O_SRCLK, HIGH);
    digitalWrite(O_SRCLK, LOW);
    //-----------------------------------------
  }
  digitalWrite(O_RCLK, HIGH);
  debugText();
}*/
void loop() {
  light_map[0][0] = true;
    for(int i = 0; i < sizeof(light_map) / sizeof(light_map[0]); i++){
      for (int j = 0; j < sizeof(light_map[0]) / sizeof(light_map[0][0]); j++){

        if ( light_map[i][j] == true){
          line[ColumnPin[j]] = true;
          line[RowPin[i]] = false;
        }
      }
      digitalWrite(O_RCLK, LOW);
      for(int c = 63; c != -1;c--){
        if (line[c] == true){
          digitalWrite(O_SER, HIGH);
        }
        else{
          digitalWrite(O_SER, LOW);
        }
        //クロックパルスを立ち上げる
        digitalWrite(O_SRCLK, HIGH);
        digitalWrite(O_SRCLK, LOW);
        //-----------------------------------------
      }
      digitalWrite(O_RCLK, HIGH);
      for (int shift_num = 0; shift_num < 4; shift_num++){
        for (int i = 0; i < sizeof(RowPin) / sizeof(RowPin[0]); i++){
          line[RowPin[i] + 16 * shift_num] = true;
        }
      }
      for (int shift_num = 0; shift_num < 4; shift_num++){
        for (int i = 0; i < sizeof(ColumnPin) / sizeof(ColumnPin[0]); i++){
          line[ColumnPin[i] + 16 * shift_num] = false;
        }
      }
    }
    Serial.println();
    delay(0.1);
}
