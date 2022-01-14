/*
 * @Author: NeoBurn (github.com/TheNeoBurn)
 * 
 * This is a binary piano for my Arduino Sequence board.
 * 
 * You can play notes by using the 4 buttons as binary input:
 *  - Pressing green adds 1 half step
 *  - Pressing red adds 2 half stepps
 *  - Pressing blue adds 4 half steps
 *  - Pressing yellow adds 8 half steps
 *  
 * If no button is pressed the piano is silent.
 *  
 * You can define the (excluded) first note with START_NOTE as an index of N[]:
 *  - START_NOTE 47: Is a B♮2 meaning the notes go from C3 to D4
 */

// Define witch pins to use
#define BTN_G 9
#define LED_G 8
#define BTN_R 7
#define LED_R 6
#define BTN_B 5
#define LED_B 4
#define BTN_Y 3
#define LED_Y 2
#define TONE 10

// Define witch note to add the selected keys to (excluded)
#define START_NOTE 47

// Define the number of cycles a key must be pressed before it is applied
#define DEBOUNCE 5

// This defines notes (C -> B♮)
const int N[] = { 
      16,   17,   18,   19,   21,   22,   23,   24,   26,   28,   29,   31, // C0 -> B♮0
      33,   35,   37,   39,   41,   44,   46,   49,   52,   55,   58,   62, // C1 -> B♮1
      65,   69,   73,   78,   82,   87,   92,   98,  104,  110,  117,  123, // C2 -> B♮2
     131,  139,  147,  156,  165,  175,  185,  196,  208,  220,  233,  247, // C3 -> B♮3
     262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494, // C4 -> B♮4
     523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988, // C5 -> B♮5
    1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, // C6 -> B♮6
    2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, // C7 -> B♮7
    4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902  // C8 -> B♮8
};

void setup() {
  // Set pin types
  pinMode(LED_G, OUTPUT);
  pinMode(BTN_G, INPUT_PULLUP);
  pinMode(LED_R, OUTPUT);
  pinMode(BTN_R, INPUT_PULLUP);
  pinMode(LED_B, OUTPUT);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(LED_Y, OUTPUT);
  pinMode(BTN_Y, INPUT_PULLUP);
  pinMode(TONE, OUTPUT);
}

// Global variable to remember the last key
int lastKey = 0;
int newKey = 0;
int newCount = 0;
 
void loop() {
  // Read the current key combination
  int curr = 0;
  if (digitalRead(BTN_G) == LOW) curr |= 1;
  if (digitalRead(BTN_R) == LOW) curr |= 2;
  if (digitalRead(BTN_B) == LOW) curr |= 4;
  if (digitalRead(BTN_Y) == LOW) curr |= 8;

  if (curr == newKey && newKey != lastKey) {
    newCount++;
  } else {
    newKey = curr;
    newCount = 0;
  }

  if (newCount > DEBOUNCE) {
    // Stop the previous selected tone
    noTone(TONE);

    // Update the LEDs
    digitalWrite(LED_G, (curr & 1) != 0 ? HIGH : LOW);
    digitalWrite(LED_R, (curr & 2) != 0 ? HIGH : LOW);
    digitalWrite(LED_B, (curr & 4) != 0 ? HIGH : LOW);
    digitalWrite(LED_Y, (curr & 8) != 0 ? HIGH : LOW);

    // Start the new tone (if any key is selected)
    if (curr != 0) {
      tone(TONE, N[START_NOTE + curr]);
    }

    // Remember the current key
    lastKey = curr;

    // Reset the counter
    newCount = 0;
  }

  // Wait for a few milliseconds to give the tone a chance
  delay(5);
}
