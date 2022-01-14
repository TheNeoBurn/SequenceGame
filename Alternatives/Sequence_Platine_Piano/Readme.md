# Sequence Platine Piano

This is a binary piano for my Arduino Sequence board.

You can play notes by using the 4 buttons as binary input:
- Pressing green adds 1 half step
- Pressing red adds 2 half stepps
- Pressing blue adds 4 half steps
- Pressing yellow adds 8 half steps
  
If no button is pressed the piano is silent.

You can define the (excluded) first note with START_NOTE as an index of N[]:
- START_NOTE 47: Is a B♮2 meaning the notes go from C3 to D4
