/***********\
|* Options *|
\***********/

// This defines notes (C -> B♮)
const int N[] = { 
      16,   17,   18,   19,   21,   22,   23,   24,   26,   28,   29,   31,
      33,   35,   37,   39,   41,   44,   46,   49,   52,   55,   58,   62,
      65,   69,   73,   78,   82,   87,   92,   98,  104,  110,  117,  123,
     131,  139,  147,  156,  165,  175,  185,  196,  208,  220,  233,  247,  
     262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,  
     523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,
    1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 
    2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 
    4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902
};
// Define here how many colors to use
const int PINS_CNT = 4;
// Define the button pins to use (ordered)
const int PINS_BTN[] = { 3, 5, 7, 9 };
// Define the LED pins to use (ordered)
const int PINS_LED[] = { 2, 4, 6, 8 };
// Define the time in milliseconds a color is shown
const int SHOW_LEN = 400;
// Define notes to use (ordered, e.g. C major scale: 262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880, 988, ...)
const int NOTES[] = { 262, 330, 392, 494 };
// Define the audio pin to use (set to -1 to disable)
const int NOTE_PIN = 10;
// Define here how long a note is played (in milliseconds)
const int NOTE_LEN = 100;
// Define an analog pin to get some entropy from (set to -1 to disable)
const int RND_PIN = 0;
// Define here the maximum color sequence count
const int SEQ_CNT = 25;

/***********/

int sequence[SEQ_CNT];
int currLength = 1;

///////////////////////////////////////////////////
// This function is run at the start of our program
void setup() {
  // Set pin modes for all used pins
  for (int i = 0; i < PINS_CNT; i++) {
    pinMode(PINS_BTN[i], INPUT_PULLUP);
    pinMode(PINS_LED[i], OUTPUT);
  }
  if (NOTE_PIN >= 0) pinMode(NOTE_PIN, OUTPUT);

  // Use an analog input to seed the random function
  if (RND_PIN >= 0) randomSeed(analogRead(RND_PIN));

  // Define a random sequence for this instance
  for (int i = 0; i < SEQ_CNT; i++) {
    sequence[i] = random(PINS_CNT);
  }

  delay(500);
}

/////////////////////////////////////////////////////////////////
// Plays a note by its index and a length declaration as fraction 
void playNote(int index, int divider) {
  int len = 1000 / divider;
  tone(NOTE_PIN, N[index], len - 10);
  delay(len);
}

/////////////////////////////////////////////////////////////////////////////
// This function is used to play a sliding note from one frequency to another
void playSlide(int freq1, int freq2, int step) {
  if (NOTE_PIN < 0) return;
  if (freq1 < freq2) {
    for (int i = freq1; i < freq2; i += step) {
      tone(NOTE_PIN, i, 10);
      delay(10);
    }    
  } else {
    for (int i = freq1; i > freq2; i -= step) {
      tone(NOTE_PIN, i, 10);
      delay(10);
    }
  }
}

/////////////////////////////////////////
// This function turns all LEDs on or off
void setAll(bool on) {
  for (int i = 0; i < PINS_CNT; i++) {
    digitalWrite(PINS_LED[i], on ? HIGH : LOW);
  }
}

/////////////////////////////////////////////////
// This function shows a color and plays its note
void showColor(int index) {
  digitalWrite(PINS_LED[index], HIGH);
  delay(50);
  if (NOTE_PIN >= 0 && NOTE_LEN > 10) {
    tone(NOTE_PIN, NOTES[index], NOTE_LEN - 10);
    delay(NOTE_LEN);
  } else {
    delay(100);
  }
  delay(SHOW_LEN);
  digitalWrite(PINS_LED[index], LOW);
}

void playDone() {
  playNote(33, 8);
  playNote(33, 8);
  playNote(45, 8);
}

//////////////////////////////////////////////////////////
// This function turns on all LEDs and plays a loose sound
void showLoose() {
  setAll(true);

  playSlide(440, 660, 8);
  delay(100);
  playSlide(440, 587, 6);
  delay(100);
  playSlide(440, 523, 3);
  for (int n = 0; n < 4; n++) {
    playSlide(523, 500, 3);
    playSlide(500, 523, 3);
  } 
}

///////////////////////////////////////////////////////////
// This function plays a victory song in a specified height
void showWin(int h) {
  if (NOTE_PIN >= 0) {
    // Play a little song
    playNote(h, 8);
    playNote(h, 8);
    playNote(h, 8);
    playNote(h + 7, 8);
    playNote(h, 8);
    playNote(h + 5, 8);
    playNote(h, 8);
    playNote(h + 3, 8);
  } else {
    // No audio pin => flash random LEDs
    int i = random(PINS_CNT);
    
  }
}

/////////////////////////////////////////////////////////////////////////////////
// This function waits until one of the buttons was pressed and returns its index
int waitForKey() {
  int key = -1;
  while (key == -1) {
    delay(5);
    for (int i = 0; i < PINS_CNT; i++) {
      if (digitalRead(PINS_BTN[i]) == LOW) {
        key = i;
        break;
      }
    }
  }
  return key;
}

//////////////////////////////////////////
// This function is called again and again
void loop() {
  // Display the current first colors of the sequence
  for (int i = 0; i < currLength; i++) {
    showColor(sequence[i]);
    delay(50);
  }

  // Process inputs
  for (int curr = 0; curr < currLength; curr++) {
    // Wait for a key press
    int key = waitForKey();
    if (key == sequence[curr]) {
      // That was the correct key, show color and goto next
      showColor(sequence[curr]);
      delay(10);
    } else {
      // That was the wrong key, show user lost
      showLoose();
      // ... then blink all LEDs until reset
      bool on = true;
      while (true) {
        on = on ? false : true;
        setAll(on);
      }
    }
  }

  // The user got all colors right => play done sound and goto next level
  playDone();
  currLength++;

  if (currLength > SEQ_CNT) {
    // The sequence ended => show user won until reset
    int led = 0;
    bool on = false;
    while (true) {
      int height = 29 + random(6) * 5;
      led++;
      if (led == PINS_CNT) {
        led = 0;
        on = !on;
      }
      digitalWrite(PINS_LED[led], on ? HIGH : LOW);
      delay(5);
      showWin(height);
      //delay(50);
    }
  }
}
