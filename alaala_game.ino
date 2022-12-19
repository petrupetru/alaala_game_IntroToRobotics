#include<LiquidCrystal.h>
#include"LedControl.h"
#include <time.h>
#include <string.h>
#include<EEPROM.h>
#include <ezBuzzer.h>


//states
const byte sInit = 0;
const byte sGame = 1;
const byte sGameOver = 2;
const byte sMenu = 3;
const byte sLeaderboard = 4;
const byte sAbout = 5;
const byte sHow = 6;
const byte sSettings = 7;
const byte sSelectLevel = 8;

//joystick pins
const int xPin = A0;
const int yPin = A1;
const int pinSW = 2;

//matrix pins
const byte dinPin =12;
const byte clockPin = 7;
const byte loadPin = 10;
const byte matrixSize = 8;
bool matrixChanged = true;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);


//lcd pins
const byte RS = 9;
const byte enable = 8;
const byte d4 = A4;
const byte d5 = A5;
const byte d6 = 5;
const byte d7 = 4;

const byte buzzPin = 3;
const int buzzBeepFreq = 5;
const int buzzScrollFreq = 5;
const int buzzBeepTime = 150;
const int buzzInitTime = 500;
const int buzzInitFreq = 200;
const byte numBright =  5;
const int maxAnalog = 255;
const int maxBrightMatix = 15;



const byte moveInterval = 100;
unsigned long long lastMoved = 0;

const unsigned int debounceDelay = 50;


byte xPos = 0;
byte yPos = 0;
byte brightnessLCDPin = 6;
byte matrixBrightness = 10;
int lcdBrightness = 100;
byte matrixBrightnessSelector = 4;
byte lastMBSelector;
byte lcdBrightnessSelector = 4;
byte lastLCDSelector;
byte soundSelector = 0;
byte difficultySelector = 0;
const bool hard = true;
const bool easy = false;
byte boardSize = 8;
byte cellSize = 1;
const int startDelay = 1500;
const int showDelay = 1500;
bool cleared = false;
byte level = 1;
byte maxLevel = 1;
byte maxLives = 3;
byte lives = 1;
byte selected = 0;
bool boardCells[matrixSize][matrixSize];
bool boardCellsSelected[matrixSize][matrixSize];
int blinkPlayerInterval = 400;
unsigned long long lastBlinkPlayer = 0;
bool playerShow = true;


byte xLastPos = 0;
byte yLastPos = 1;
bool joyMovedX = false;
bool joyMovedY = false;
const int minThreshold = 200;
const int maxThreshold = 800;
byte lastButtonValue = LOW;
byte buttonValue = LOW;
byte buttonState = LOW;
byte lastButtonState = LOW;
unsigned long long lastDebounceTime = 0;

byte xFood = 0;
byte yFood = 0;
byte state = 0;

const int nameSize = 3;
char name[] = "DEF";
int score = 0;
unsigned int initDuration = 1;
unsigned int gameDuration = 15;
unsigned long  timeRemaing = gameDuration;
unsigned long long timeStart = 0;

const int leaderboardSize = 5; 
char leaderboardNames[leaderboardSize][3] = {"DEF", "DEF", "DEF", "DEF", "DEF"};
int leaderboardScores[leaderboardSize] = {0, 0, 0, 0, 0};
int highscore = 0;
const char about[] = "github.com/petrupetru";


int xValue = 0;
int yValue = 0;



/*MENU:
  (0)Play
  (1)Leaderboard -> 1,2,3,4,5
  (2)Settings ->  (0)Name:
                  (1)LCD Brightness
                  (2)Matrix Brightness
                  (3)Sound -> On / Off
  (3)About
  (4)How To Play
*/
const int displayColumns = 16;
const int displayRows = 2;
const int menuSize = 5;
const int settingsSize = 6;
const int howToSize = 8;


String menu[menuSize] = {
  "Play",
  "Leaderboard",
  "Settings",
  "About",
  "How to Play?"
};

String settings[settingsSize] = {
  "Name",
  "Dificulty",
  "Matrix Brightness",
  "LCD Brightess",
  "Sounds",
  "Reset Game"
};
//byte howToSelector = 0;
byte menuSelector = 0;
byte leaderboardSelector = 0;
byte settingsSelector = 0;
byte howToSelector = 0;
byte levelSelector = 1;
bool lockSettings = false;
char howTo[howToSize][16] = {
  "Alaala.",
  "Memorize the",
  "pattern shown",
  "on screen,",
  "then use the",
  "joystick to",
  "draw it exactly",
  "how it was."
};

bool show = true;
int blinkCursorInterval = 400;
unsigned long long lastBlinkCursor = 0;
bool cursorShow = false;
byte nameIndex = 0;
byte lastNameIndex = 0;
int res = 0;
int lastRes = 0;
byte resetContor = 0;
unsigned long long aboutStartTime;
char toShow[16];
byte offset = 0;
bool reset = false;
unsigned long long startGameOverTime = 0;
LiquidCrystal lcd(RS,enable,d4,d5,d6,d7);
byte selectArrow[] = {
  B00000,
  B10000,
  B11000,
  B11100,
  B11110,
  B11100,
  B11000,
  B10000
};
byte crown[] = {
  B00000,
  B00000,
  B10001,
  B10101,
  B11111,
  B11111,
  B00000,
  B00000
};

byte full[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte line[] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
};
byte heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

byte heartEmpty[] = {
  B00000,
  B01010,
  B10101,
  B10001,
  B01010,
  B00100,
  B00000,
  B00000
};



byte matrixByte[matrixSize] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

byte board[matrixSize] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

byte matrixByteEmpty[matrixSize] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};


const uint64_t matrixInit = {0x007f415d457d01ff};
const uint64_t matrixEmpty = {0x0000000000000000}; 
const uint64_t matrixLeaderboard = {0x7e18187e7effffff};
const uint64_t matrixSettings = {0xc0e07e3f33303818};
const uint64_t matrix1 = {0x7e7e181818783818};
const uint64_t matrix2 = {0x7e3e180c06667e3c};
const uint64_t matrix3 = {0x7c7e027c7c027e7c};
const uint64_t matrixOk = {0x2070d88c06030100};

ezBuzzer buzzer(buzzPin);




//song from:
//https://create.arduino.cc/projecthub/akkuadlakha5/play-songs-using-arduino-0d90ed
int melody[]={NOTE_C6,NOTE_D6,NOTE_E6,NOTE_F6,NOTE_G6,0,NOTE_E6,NOTE_C6,0,NOTE_G6,0,NOTE_F6,NOTE_D6,0,
  NOTE_F6,0,NOTE_D6,NOTE_B5,0,NOTE_F6,0,NOTE_E6,NOTE_C6,0,
  NOTE_G5,0,NOTE_C6,0,NOTE_F6,0,NOTE_E6,NOTE_G6,NOTE_C6,0,
  NOTE_E6,NOTE_F6,NOTE_E6,NOTE_F6,NOTE_E6,NOTE_F6,NOTE_E6,NOTE_C6,NOTE_D6,0,
  NOTE_C6,NOTE_D6,NOTE_E6,NOTE_F6,NOTE_G6,0,NOTE_E6,NOTE_C6,0,NOTE_G6,0,NOTE_F6,NOTE_D6,0,
  NOTE_F6,0,NOTE_D6,NOTE_B5,0,NOTE_F6,0,NOTE_E6,NOTE_C6,0,
  NOTE_A5,0,NOTE_A6,0,NOTE_G6,NOTE_F6,NOTE_E6,NOTE_F6,NOTE_G6,0,NOTE_C6,NOTE_C6,0,
  NOTE_E6,NOTE_F6,NOTE_E6,NOTE_C6,NOTE_F6,NOTE_E6,NOTE_C6,NOTE_A6,NOTE_G6,0,
  NOTE_G5,NOTE_G5,NOTE_F6,NOTE_E6,NOTE_D6,NOTE_E6,NOTE_C6,0,0
};
int noteDurations[]={ 8,8,8,8,8,10,8,8,10,8,10,4,8,2,
  8,10,8,8,10,8,10,4,8,2,
  8,10,8,10,8,10,4,8,4,6,
  8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,10,8,8,10,8,10,4,8,2,
  8,10,8,8,10,8,10,4,8,2,
  8,10,8,10,8,8,8,8,4,10,8,8,4,
  8,8,8,8,8,8,8,8,2,4,
  8,8,8,8,8,8,6,1,4
};

int gameOverSound[] = {
  0, NOTE_G4, NOTE_C4,
};
int gameOverDurations[] = {
  8, 4, 8,
};

int highscoreSound[] = {
  0, NOTE_A5, NOTE_B5, NOTE_C5, NOTE_B5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5,
};
int highscoreSoundDuration[] = {
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
};


int noteLength;


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  EEPROM.get(0, leaderboardNames);
  int addressScores = sizeof(leaderboardNames);
  EEPROM.get(addressScores, leaderboardScores);
  int addressName = addressScores + sizeof(leaderboardScores);
  highscore = leaderboardScores[0];
  EEPROM.get(addressName, name);
  int addressMB = addressName + sizeof(name);
  EEPROM.get(addressMB, matrixBrightnessSelector);
  int addressLCD = addressMB + sizeof(lcdBrightnessSelector);
  EEPROM.get(addressLCD, lcdBrightnessSelector);
  int addressSound = addressLCD + sizeof(soundSelector);
  EEPROM.get(addressSound, soundSelector);
  int addressDif = addressSound + sizeof(soundSelector);
  EEPROM.get(addressDif, difficultySelector);
  int addressMaxLevel = addressDif + sizeof(difficultySelector);
  EEPROM.get(addressMaxLevel, maxLevel);


  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightnessSelector * (maxBrightMatix / numBright));
  lc.clearDisplay(0);
  
  pinMode(buzzPin,OUTPUT);
  pinMode(brightnessLCDPin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP);

  lcd.begin(16,2);
  lcd.createChar(1, selectArrow);
  lcd.createChar(2, crown);
  lcd.createChar(3, full);
  lcd.createChar(4, line);
  lcd.createChar(5, heart);
  lcd.createChar(6, heartEmpty);

  noteLength = sizeof(noteDurations) / sizeof(int);

  
}

void loop() {
  buzzer.loop();
  analogWrite(brightnessLCDPin, lcdBrightnessSelector * (maxAnalog / numBright));

  readButton();
  if (buttonValue == 1 && lastButtonValue == 0 && state != sGame) {
    beep(buzzBeepFreq);
  }

  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  stateSelector(state);
  

  if(matrixChanged == true){
    updateByteMatrix();
    matrixChanged = false;
  }
  lastButtonValue = buttonValue;
}

void stateSelector(byte state) {
  if (state == sInit) {
    stateInit();
  }
  else if (state == sGame) {
    stateGame();
  }
  else if (state == sGameOver) {
    stateGameOver();
  }
  else if (state == sMenu) {
    stateMenu();
  }
  else if (state == sLeaderboard) {
    stateLeaderboard();
  }
  else if (state == sAbout) {
    stateAbout();
  }
  else if (state == sSettings) {
    stateSettings();
  }
  else if (state == sHow) {
    stateHowToPlay();
  }
  else if (state == sSelectLevel) {
    stateSelectLevel();
  }
  
}

void stateGameOver() {

  if (millis() - startGameOverTime < startDelay) {
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Level reached:");
    lcd.print(level);
  }
  else{
    highscore = leaderboardScores[0];
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Score:");
    lcd.print(score);
    lcd.print(" ");
    lcd.write(2);
    lcd.print(":");
    lcd.print(highscore);  
    lcd.print("     ");
  }
  
  if(buttonValue == 1 && lastButtonValue == 0){
    displayImage(matrixEmpty);
    matrixChanged = true;
    lcd.clear();
    state = sMenu;
  }
}



void stateMenu() {
  displayImage(matrixInit);
  updatePositionX(menuSelector, menuSize, 1);
  lcd.setCursor(0, 0);
  if(menuSelector < menuSize - 1){
    lcd.write(1);
    lcd.print(menu[menuSelector]);
    lcd.setCursor(0, 1);
    lcd.print(' ');
    lcd.print(menu[menuSelector + 1]);
  }
  else {
    lcd.print(' ');
    lcd.print(menu[menuSelector - 1]);
    lcd.setCursor(0, 1);
    lcd.write(1);
    lcd.print(menu[menuSelector]);
  }
  if(buttonValue == 1 && lastButtonValue == 0) {
    if (menuSelector == 0){
      lcd.clear();
      //displayImage(matrixEmpty);
      state = sSelectLevel;
      

    }
    if (menuSelector == 1) {
      lcd.clear();
      displayImage(matrixEmpty);
      EEPROM.get(0, leaderboardNames);
      int address_arr2 = sizeof(leaderboardNames);
      EEPROM.get(address_arr2, leaderboardScores);
      state = sLeaderboard;
    }
    if (menuSelector == 2) {
        lcd.clear();
        displayImage(matrixEmpty);
        state = sSettings;
    }
    if (menuSelector == 3) {
      for (int i = 0; i < displayColumns; i++) {
        toShow[i] = about[i];
      }
      aboutStartTime = millis();
      lcd.clear();
      displayImage(matrixEmpty);
      state = sAbout;
    }
    if (menuSelector == 4) {
      lcd.clear();
      displayImage(matrixEmpty);
      state = sHow;
    }
    
  }
}

void stateInit(){
  displayImage(matrixInit);
  if (initDuration - millis()/1000 > 0) {
    lcd.setCursor(0, 0);
    lcd.print("Hello, ");
    lcd.print(name);
    lcd.print("! ");

  }
  else {
    for (int i; i < matrixSize; i++)
      matrixByte[i] = matrixByteEmpty[i];
    lcd.clear();
    state = sMenu;
  }
}

void stateGame() {
  if (buzzer.getState() == BUZZER_IDLE && soundSelector == 1) {
    int length = sizeof(noteDurations) / sizeof(int);
    buzzer.playMelody(melody, noteDurations, length); // playing
  }
  lcd.setCursor(13, 0);
  for (int i = 0; i < maxLives; i++) {
    if (i < lives) {
      lcd.write(5);
    }
    else {
      lcd.write(6);
    }
  }
  if (millis() - timeStart < startDelay / 3) {
    displayImage(matrix3);
    lcd.setCursor(0, 0);
    lcd.print("READY");
  }
  else if (millis() - timeStart < (startDelay / 3) * 2) {
    displayImage(matrix2);
    lcd.setCursor(0, 0);
    lcd.print("SET  ");
  }
  else if (millis() - timeStart < startDelay) {
    displayImage(matrix1);
    lcd.setCursor(0, 0);
    lcd.print("GO   ");
  }
  else if (millis() - timeStart < startDelay + showDelay) {
    for (int i = 0; i < matrixSize; i++) {
      matrixByte[i] = board[i];
    }
    matrixChanged = true;
    lcd.setCursor(0, 0);
    lcd.print("Memorize the");
    lcd.setCursor(0, 1);
    lcd.print(" pattern");

  }
  else {
    if (!cleared) {
      displayImage(matrixEmpty);
      lcd.clear();
      cleared = true;
    }
    lcd.setCursor(0, 0);
    lcd.print("Level: ");
    lcd.print(level);
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
    updateGamePositions();
    if (buttonValue == 1 && lastButtonValue == 0 && level < boardSize * boardSize) {
      
      if (boardCells[xPos][yPos] == 1) {
        score += 1;
        selected++;
        boardCellsSelected[xPos][yPos] = 1;
        boardCells[xPos][yPos] = 0;

        if (selected == level) {
          score += 10;
          selected = 0;
          level++;
          buildBoard();
          cleared = false;
          matrixChanged = true;
          timeStart = millis();
          lcd.clear();
        }

      }
      else {
        if(lives == 1) {
          if(level >= boardSize * boardSize) {
            score += 100;
          }
          if (level > maxLevel) {
            maxLevel = level;
          }
          updateEEPROM();
          state = sGameOver;
          if (buzzer.getState() != BUZZER_IDLE) {
            buzzer.stop() ; // stop
          }

          for (int i = 0; i < matrixSize; i++)
            board[i] = matrixByteEmpty[i];
          for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
              boardCells[i][j] = 0;
              boardCellsSelected[i][j] = 0;
            }
          }
          cleared  = true;
          selected = 0;
          lcd.clear();
          highscore = leaderboardScores[0];

          if (score > highscore) {
            const uint64_t smiley = {0x3c4299a581a5423c};
            displayImage(smiley);
            if (buzzer.getState() == BUZZER_IDLE && soundSelector == 1) {
              int length = sizeof(highscoreSoundDuration) / sizeof(int);
              buzzer.playMelody(highscoreSound, highscoreSoundDuration, length); // playing
            }

          }
          else {
            const uint64_t sad = {0x3c42a59981a5423c};
            displayImage(sad);
            if (buzzer.getState() == BUZZER_IDLE && soundSelector == 1) {
              int length = sizeof(gameOverDurations) / sizeof(int);
              buzzer.playMelody(gameOverSound, gameOverDurations, length); // playing
            }
          }
          startGameOverTime = millis();
        }
        else {
          lives--;
        }
        
      }
    }
  }


}

void stateSelectLevel() {

  updatePositionY(levelSelector, maxLevel, 1);
  lcd.setCursor(0, 0);
  lcd.print("Starting level: ");
  lcd.setCursor(0, 1);
  lcd.write(1);
  lcd.print(' ');
  lcd.print(levelSelector + 1);

  if(buttonValue == 1 && lastButtonValue == 0) {
    level = levelSelector + 1;
    score = (level - 1) * 10 + ((level - 1) * level) / 2;
    lcd.clear();
    displayImage(matrixEmpty);
    timeStart = millis();
    if(difficultySelector == easy) {
      boardSize = 4;
    }
    else {
      boardSize = 8;
    }
    cellSize = matrixSize / boardSize;
    cleared = false;
    buildBoard();
    if (difficultySelector == 1) {
      lives = 3;
    }
    else {
      lives = 1;
    }
    state = sGame;

  }
  
}

void stateLeaderboard() {
  displayImage(matrixLeaderboard);
  updatePositionX(leaderboardSelector, leaderboardSize, 2);
  lcd.setCursor(0, 0);
  if (leaderboardSelector < leaderboardSize - 2) {
    int score1 = leaderboardScores[leaderboardSelector];
    int score2 = leaderboardScores[leaderboardSelector + 1];
    char name1[nameSize + 1];
    char name2[nameSize + 1];
    for (int k = 0; k < nameSize; k++) {
      name1[k] = leaderboardNames[leaderboardSelector][k];
      name2[k] = leaderboardNames[leaderboardSelector + 1][k];
    }
 
    lcd.write(1);
    for (int k = 0; k < nameSize; k++) {
      lcd.print(name1[k]);
    }
    lcd.print(": ");
    lcd.print(score1);
    lcd.setCursor(0, 1);
    lcd.print(' ');
    for (int k = 0; k < nameSize; k++) {
      lcd.print(name2[k]);
    }
    lcd.print(": ");
    lcd.print(score2);


  }
  else {
    int score1 = leaderboardScores[leaderboardSelector - 1];
    int score2 = leaderboardScores[leaderboardSelector];
    char name1[nameSize];
    char name2[nameSize];
    for (int k = 0; k < nameSize; k++) {
      name1[k] = leaderboardNames[leaderboardSelector - 1][k];
      name2[k] = leaderboardNames[leaderboardSelector][k];
    }
    lcd.print(' ');
    for (int k = 0; k < nameSize; k++) {
      lcd.print(name1[k]);
    }
    lcd.print(": ");
    lcd.print(score1);
    lcd.setCursor(0, 1);
    lcd.write(1);
    for (int k = 0; k < nameSize; k++) {
      lcd.print(name2[k]);
    }
    lcd.print(": ");
    lcd.print(score2);
  }

  if (back()) {
    beep(buzzBeepFreq);
    lcd.clear();
    displayImage(matrixEmpty);
    state = sMenu;
  }

}

void stateSettings() {
  /*0  "Name",
    1  "Matrix Brightness",
    2  "LCD Brightess",
    3  "Sounds",
    4  "Reset Game"
  */
  
  displayImage(matrixSettings);
  if(settingsSelector == 0) {
    if (lockSettings) {
      lcd.setCursor(0, 0);
      lcd.print(' ');
      lcd.print("Name:");
      lcd.setCursor(0, 1);
      lcd.write(1);
      updatePositionY(nameIndex, nameSize, 1);
      lcd.setCursor(nameIndex + 1, 1);
      unsigned long long currentTime = millis();
      res = updateLetter();
      if(res == 1){
        name[nameIndex] = nextChar(name[nameIndex]);
      }
      if(res == -1){
        name[nameIndex] = prevChar(name[nameIndex]);
      }
      if (currentTime - lastBlinkCursor >= blinkCursorInterval) {
        lastBlinkCursor = currentTime;
        if(cursorShow) {
          cursorShow = false;
          lcd.print(' ');
          lcd.setCursor(nameIndex + 1, 1);
        }
        else {
          cursorShow = true;
          lcd.print(name[nameIndex]);
          lcd.setCursor(nameIndex + 1, 1);
        }        
      }
      if (nameIndex != lastNameIndex) {
        lcd.setCursor(1, 1);
        lcd.print(name);
        lcd.setCursor(nameIndex + 1, 1);
      }
      lastNameIndex = nameIndex;
      
      
    }
    else {
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.print("Name:");
      lcd.setCursor(0, 1);
      lcd.print(' ');
      lcd.print(name);
    }


  }
  else if (settingsSelector == 1) {
    if (lockSettings) {
      updatePositionY(difficultySelector, 2, 1);
      lcd.setCursor(0, 0);
      lcd.print(' ');
      lcd.print("Difficulty");
      lcd.setCursor(0, 1);
      lcd.write(1);
      if (difficultySelector == hard) {
        lcd.print("HARD");   
      }
      else {
        lcd.print("EASY");
      }
      
    }
    else {
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.print("Difficulty");
      lcd.setCursor(0, 1);
      lcd.print(' ');
      if (difficultySelector == hard) {
        lcd.print("HARD");   
      }
      else {
        lcd.print("EASY");
      }
    }

  }
  else if (settingsSelector == 2) {
    
    if (lockSettings) {
      updatePositionY(matrixBrightnessSelector, numBright, 1);
      if (lastMBSelector != matrixBrightnessSelector) {

        lc.setIntensity(0, matrixBrightnessSelector * (maxBrightMatix / numBright));
      }
      lcd.setCursor(0, 0);
      lcd.print(' ');
      lcd.print("Matrix Brightness");
      lcd.setCursor(0, 1);
      lcd.write(1);
      for (int i = 0; i <= numBright; i++) {
        if(i <= matrixBrightnessSelector) {
          lcd.write(3);
        }
        else {
          lcd.print(' ');
        }
      }
      lastMBSelector = matrixBrightnessSelector;
    }
    else {
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.print("Matrix Brightness");
      lcd.setCursor(0, 1);
      lcd.print(' ');
      for (int i = 0; i <= numBright; i++) {
        if(i <= matrixBrightnessSelector) {
          lcd.write(3);
        }
        else {
          lcd.print(' ');
        }
      }

    }
    
  }
  else if (settingsSelector == 3) {
    if (lockSettings) {
      updatePositionY(lcdBrightnessSelector, numBright, 1);
      if (lastLCDSelector != lcdBrightnessSelector) {

        lc.setIntensity(0, lcdBrightnessSelector * (maxAnalog / numBright));
      }
      lcd.setCursor(0, 0);
      lcd.print(' ');
      lcd.print("LCD Brightness");
      lcd.setCursor(0, 1);
      lcd.write(1);
      for (int i = 0; i <= numBright; i++) {
        if(i <= lcdBrightnessSelector) {
          lcd.write(3);
        }
        else {
          lcd.print(' ');
        }
      }
      lastLCDSelector = lcdBrightnessSelector;
    }
    else {
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.print("LCD Brightness");
      lcd.setCursor(0, 1);
      lcd.print(' ');
      for (int i = 0; i <= numBright; i++) {
        if(i <= lcdBrightnessSelector) {
          lcd.write(3);
        }
        else {
          lcd.print(' ');
        }
      }

    }
  }
  else if (settingsSelector == 4) {
    if (lockSettings) {
      updatePositionY(soundSelector, 2, 1);
      lcd.setCursor(0, 0);
      lcd.print(' ');
      lcd.print("Sound");
      lcd.setCursor(0, 1);
      lcd.write(1);
      if (soundSelector == 0) {
        lcd.print("OFF");   
      }
      else {
        lcd.print("ON ");
      }
      
    }
    else {
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.print("Sound");
      lcd.setCursor(0, 1);
      lcd.print(' ');
      if (soundSelector == 0) {
        lcd.print("OFF");   
      }
      else {
        lcd.print("ON ");
      }
    }
    
  }
  else if (settingsSelector == 5) {
    
    if (lockSettings) {
      lcd.setCursor(0, 0);
      lcd.print("  Are you sure?");
      if(resetContor == 0) {
        
        lcd.setCursor(0, 1);
        lcd.print("Move ");
        lcd.write(4);
        lcd.write(1);
        lcd.print(" 3 times.");
      }
      
      if (right()) {
        resetContor += 1;
      }
      if(resetContor != 0) {
        lcd.setCursor(0, 1);
        for (int i = 0; i < resetContor; i++) {
          lcd.print("  ");
          lcd.write(4);
          lcd.write(1);
        }
        lcd.print("                ");
      }
      if(back()){
        beep(buzzBeepFreq);
        resetContor = 0;
      }
      
      if (resetContor == 3) {
        resetContor = 0;
        lockSettings = false;
        settingsSelector = 0;
        menuSelector = 0;
        reset = true;        
        lcd.clear();
        displayImage(matrixEmpty);
        state = sMenu;
        name[0] = 'D';
        name[1] = 'E';
        name[2] = 'F';
        matrixBrightnessSelector = 4;
        lcdBrightnessSelector = 4;
        lc.setIntensity(0, matrixBrightnessSelector * (maxBrightMatix / numBright));
        //analogWrite(brightnessLCDPin, lcdBrightnessSelector * (maxAnalog / numBright));
        soundSelector = 0;
        leaderboardSelector = 0;
        difficultySelector = 0;
        for (int i = 0; i < leaderboardSize; i++) {
          leaderboardScores[i] = 0;
          for(int j = 0; j < nameSize; j++) {
            leaderboardNames[i][j] = '-';
          }
        }
        updateEEPROM();


      }
      

    }
    else {
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.print("Reset Game");
      lcd.setCursor(0, 1);
      lcd.print("                ");      
    }
    
  }


  if (buttonValue == 1 && lastButtonValue == 0) {
    lcd.clear();
    if (lockSettings == false) {
      if(settingsSelector == 0) {
        lcd.setCursor(1, 1);
        lcd.print(name);
      }
      lockSettings = true;
    }
      
    else {
      lockSettings = false;
      resetContor = 0;
      updateEEPROM();
    }
  }
  if (!lockSettings) {
    updatePositionX(settingsSelector, settingsSize, 1);
    if (back()) {
      beep(buzzBeepFreq);
      lcd.clear();
      displayImage(matrixEmpty);
      state = sMenu;
    }
  }

  

}

void stateAbout() {
  if((millis() - aboutStartTime) >= 700)  {  //0.7 s
    offset++;
    aboutStartTime = millis();

    if(offset > sizeof(about) - displayColumns - 1) {
      offset = 0;
    }
    for (int i = 0; i < displayColumns; i++) {
      toShow[i] = about[i + offset];
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("By Burdusa Petru");
  lcd.setCursor(0, 1);
  displayImage(matrixInit);
  lcd.print(toShow);
  if (back()) {
    beep(buzzBeepFreq);
    lcd.clear();
    displayImage(matrixEmpty);
    state = sMenu;
  }
}

void stateHowToPlay() {
  displayImage(matrixInit);
  updatePositionX(howToSelector, howToSize, 1);
  lcd.setCursor(0, 0);
  if(howToSelector < howToSize - 1){
    lcd.write(1);
    lcd.print(howTo[howToSelector]);
    lcd.setCursor(0, 1);
    lcd.print(' ');
    lcd.print(howTo[howToSelector + 1]);
  }
  else {
    lcd.print(' ');
    lcd.print(howTo[howToSelector - 1]);
    lcd.setCursor(0, 1);
    lcd.write(1);
    lcd.print(howTo[howToSelector]);
  }
  if (back()) {
    beep(buzzBeepFreq);
    lcd.clear();
    displayImage(matrixEmpty);
    state = sMenu;
  }
}

void readButton() {
  buttonState = digitalRead(pinSW);
  if(buttonState != lastButtonState) {
    lastDebounceTime = millis();
    lastButtonState = buttonState;
  }
  if(millis() - lastDebounceTime > debounceDelay){
    buttonValue = !buttonState;
  }
}


void updateEEPROM() {
  if (!reset) {
    for (int i = 0; i < nameSize; i++){
      leaderboardNames[leaderboardSize - 1][i] = name[i];
    }
    leaderboardScores[leaderboardSize - 1] = score;

  }
  reset = false;
  
  for (int i = 0; i < leaderboardSize; i++) {
    for(int j = i + 1; j < leaderboardSize; j++) {
      if (leaderboardScores[j] > leaderboardScores[i]){
        int aux = leaderboardScores[j];
        char auxName[nameSize];
        for (int k = 0; k < nameSize; k++){
          auxName[k] = leaderboardNames[j][k];
        }
        leaderboardScores[j] = leaderboardScores[i];
        leaderboardScores[i] = aux;
        for (int k = 0; k < nameSize; k++){
          leaderboardNames[j][k] = leaderboardNames[i][k];
        }
        for (int k = 0; k < nameSize; k++){
          leaderboardNames[i][k] = auxName[k];
        }
      }
    }
  }

  EEPROM.put(0, leaderboardNames);
  int addressScores = sizeof(leaderboardNames);
  EEPROM.put(addressScores, leaderboardScores);
  int addressName = addressScores + sizeof(leaderboardScores);
  EEPROM.put(addressName, name);
  int addressMB = addressName + sizeof(name);
  EEPROM.put(addressMB, matrixBrightnessSelector);
  int addressLCD = addressMB + sizeof(matrixBrightnessSelector);
  EEPROM.put(addressLCD, lcdBrightnessSelector);
  int addressSound = addressLCD + sizeof(lcdBrightnessSelector);
  EEPROM.put(addressSound, soundSelector);
  int addressDif = addressSound + sizeof(soundSelector);
  EEPROM.put(addressDif, difficultySelector);
  int addressMaxLevel = addressDif + sizeof(difficultySelector);
  EEPROM.put(addressMaxLevel, maxLevel);

}

void updateByteMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixByte[row]);
  }
}

int updateLetter(){
  int res = 0;
  if(xValue >= minThreshold && xValue <= maxThreshold){
    joyMovedX = false;
  }
  else if(xValue > maxThreshold && joyMovedX == false){
    joyMovedX = true;  
    res =  1;
  }
  else if(xValue < minThreshold && joyMovedX == false){
    joyMovedX = true;
    res = -1;
  }
  return res;
}

void updatePositionX(byte &selector, const int &size, int bias) {
  //xLastPos = xPos;
  if(xValue >= minThreshold && xValue <= maxThreshold){
    joyMovedX = false;
  }
  else if(xValue > maxThreshold && joyMovedX == false){
    beep(buzzScrollFreq);
    lcd.clear();
    if(selector > 0) {
      selector--;
    }
    else {
      selector = size - bias;
    }
    joyMovedX = true;  
  }
  else if(xValue < minThreshold && joyMovedX == false){
    beep(buzzScrollFreq);
    lcd.clear();
    if(selector < size - bias) {
      selector++;
    }
    else {
      selector = 0;
    }
    joyMovedX = true;
  }
  

}

void updatePositionY(byte &selector, const int &size, int bias) {

  if(yValue >= minThreshold && yValue <= maxThreshold){
    joyMovedY = false;
  }
  else if(yValue < minThreshold && joyMovedY == false){
    beep(buzzScrollFreq);
    if(selector > 0) {
      selector--;
    }
    else {
      selector = size - bias;
    }
    joyMovedY = true;  
  }
  else if(yValue > maxThreshold && joyMovedY == false){
    beep(buzzScrollFreq);
    if(selector < size - bias) {
      selector++;
    }
    else {
      selector = 0;
    }
    joyMovedY = true;
  }
  
  

}


void updateGamePositions() {

  xLastPos = xPos;
  yLastPos = yPos;
  if(xValue >= minThreshold && xValue <= maxThreshold){
    joyMovedX = false;
  }
  else if(xValue > maxThreshold && joyMovedX == false){
    if(xPos > 0) {
      xPos--;
    }
    else {
      xPos = boardSize - 1;
    }
    joyMovedX = true;  
  }
  else if(xValue < minThreshold && joyMovedX == false){
    if(xPos < boardSize - 1) {
      xPos++;
    }
    else {
      xPos = 0;
    }
    joyMovedX = true;
  }
  if (joyMovedX == false){
    if(yValue >= minThreshold && yValue <= maxThreshold){
      joyMovedY = false;
    }
    else if(yValue > maxThreshold && joyMovedY == false){
      if(yPos > 0) {
        yPos--;
      }
      else {
        yPos = boardSize - 1;
      }
      joyMovedY = true;  
    }
    else if(yValue < minThreshold && joyMovedY == false){
      if(yPos < boardSize - 1) {
        yPos++;
      }
      else {
        yPos = 0;
      }
      joyMovedY = true;
    }
  }
  int rowBoard = 0;
  int rowBoardN = 0;
  long long currentTime = millis();
  if (currentTime - lastBlinkPlayer > blinkPlayerInterval || xPos != xLastPos || yPos != yLastPos) {
    for (int i = 0; i < boardSize; i++) {
      for(int j = 0; j < cellSize; j++) {
          rowBoard += boardCellsSelected[xLastPos][i] * (1 << i * cellSize + j);
          rowBoardN += boardCellsSelected[xPos][i] * (1 << i * cellSize + j);
      }
    }

    for(int i = 0; i < cellSize; i++) {
      matrixByte[xLastPos * cellSize + i] = rowBoard; 
    }
    lastBlinkPlayer = currentTime;
    if (playerShow) {
      for(int i = 0; i < cellSize; i++) {
        for(int j = 0; j < cellSize; j++) {
          matrixByte[xPos * cellSize + i] = matrixByte[xPos * cellSize + i] | (1 << (yPos * cellSize + j));
        }
      }
      playerShow = false;
    }
    else {
      for(int i = 0; i < cellSize; i++) {
        matrixByte[xPos * cellSize + i] = rowBoardN;
      }
      playerShow = true;
    }
    matrixChanged = true;
  }
}




void displayImage(uint64_t image) {
  for (int i = 0; i < matrixSize; i++) {
    byte row = (image >> i * 8) & 0xFF;
    matrixByte[i] = row;
  }
  matrixChanged = true;
}

bool beep(int freq) {
  if (soundSelector == 1) {
    tone(buzzPin, freq, buzzBeepTime);
  }
}


bool back() {
  bool moved = false;
  if(yValue >= minThreshold && yValue <= maxThreshold){
    joyMovedY = false;
  }
  else if(yValue < minThreshold && joyMovedY == false){
    joyMovedY = true; 
    moved = true; 
  }
  return moved;
}

bool right() {
  bool moved = false;
  if(yValue >= minThreshold && yValue <= maxThreshold){
    joyMovedY = false;
  }
  else if(yValue > maxThreshold && joyMovedY == false){
    joyMovedY = true;  
    moved = true;
  }
  return moved;
}


char nextChar(char c) {
  if (c != 'Z') {
    c++;
  }
  else{
    c = 'A';
  }
  return c;
}

char prevChar(char c) {
  if (c != 'A') {
    c--;
  }
  else{
    c = 'Z';
  }
  return c;
}

void buildBoard() {
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
        boardCells[i][j] = 0;
        boardCellsSelected[i][j] = 0;
    }
  }
  for (int i = 0; i < matrixSize; i++) {
      matrixByte[i] = 0;
      board[i] = 0;
  }
  for (int i = 0; i < level; i++) {
    byte xCell = random() %  boardSize;
    byte yCell = random() %  boardSize;
    if(boardCells[xCell][yCell] == 1) {
      i--;
    }
    else {
      boardCells[xCell][yCell] = 1;
      for (int k = 0; k < cellSize; k++) {
        for (int l = 0; l < cellSize; l++) {
          board[xCell * cellSize + k] = board[xCell * cellSize + k] + (1 << yCell * cellSize + l);
        }
      }
    }
  }
}
