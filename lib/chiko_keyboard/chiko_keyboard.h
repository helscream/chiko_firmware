#ifndef __SB_KEYBOARD__
#define __SB_KEYBOARD__

enum KEYS { HOME,
            END,
            LEFT_KEY,
            RIGHT_KEY,
            UP,
            DOWN,
            PGUP,
            PGDN,
            UNKNOWN,
            SPACE,
            BACKSPACE,
            ENTER,
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
            EIGHT,
            NINE,
            NOKEY,
            MINUS,
            PLUS
};


KEYS getKeyPressed(void) {
  if (Serial.available() > 0) {

    char inChar = Serial.read();
    // Serial.printf("%d,", inChar);

    switch (inChar) {
      
      case 27:  // Special Character
/*
        char inChar2 = Serial.read();
        Serial.printf("%d,", inChar2);

        switch (inChar2) {
          case 91:  // Start Special Character
            char inChar3 = Serial.read();
            Serial.printf("%d\n", inChar3);
            switch (inChar3) {
              case 72:  // Home
                return HOME;
                break;
              case 70:  // End
                return END;
                break;
              case 65:  // UP
                return UP;
                break;
              case 66:  // DOWN
                return DOWN;
                break;
              case 68:  // LEFT_KEY
                return LEFT_KEY;
                break;
              case 67:  // RIGHT_KEY
                return RIGHT_KEY;
                break;
              case 53:  // PG UP
                return PGUP;
                break;
              case 54:  // PG DOWN
                return PGDN;
                break;
              default:
                Serial.printf("%d:", inChar3);
                return UNKNOWN;
                break;
            }
            break;
        }
        */
        break;

      case 49:  // ONE
        return ONE;
        break;
      case 50:  // TWO
        return TWO;
        break;
      case 51:  // THREE
        return THREE;
        break;
      case 52:  // FOUR
        return FOUR;
        break;
      case 53:  // FIVE
        return FIVE;
        break;
      case 54:  // SIX
        return SIX;
        break;
      case 55:  // SEVEN
        return SEVEN;
        break;
      case 56:  // EIGHT
        return EIGHT;
        break;
      case 57:  // NINE
        return NINE;
        break;
      case 13:  // Enter
        return ENTER;
        break;
      case 127:  // BACKSPACE
        return BACKSPACE;
        break;
      case 43:  // +
        return PLUS;
        break;
      case 45:  // -
        return MINUS;
        break;
    }
    Serial.printf("\n");
  }
  return NOKEY;
}


#endif