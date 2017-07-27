//////////////////////////////////////////////////////////////////////////////
bool CheckforValidity(byte* array1) {
  int arrayLength = array1[0];
  if (arrayLength > 10) {
    int count = 0;
    for (int i = 1; i < arrayLength; i++) {
      byte currval = array1[i];
      if (currval == 1) {
        count = count + 1;
      }
    }
    if (count > 2) {
      return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
}


///// Compare two arrays with abit of built in hysteresis /////
// todo: variable hysteresis

bool compare_arrays(byte* array1, byte* array2) {
  //Serial.println("compare array");
  int length1 = array1[0];
  int length2 = array2[0];
  int count = 0;

  //byte currVal = 0xFF;
  //byte lastVal = 0xFF;

  //if (length1 == length2) {
  for (int i = 1; i < length1; i++) {
    if (array1[i] == array2[i]) {
    } else {
      if (array1[i] == array2[i] + 1) {
      } else {
        if (array1[i] == array2[i] + 2) {
          //stop
        } else {
          if (array1[i] == array2[i] - 1) {
          } else {
            if (array1[i] == array2[i] - 2) {
              //Serial.println("Checking byte" + String(i));
            } else {
              count = count + 1;
            }
          }
        }
      }
    }
  }
  //    return true;
  //    Serial.println("true");
  // }
  // return false;
  //Serial.println("false");
  Serial.print("Count is:");
  Serial.println(count);
  if (count > 1) {
    return false;
  } else {
    return true;
  }
}
///////////////////////////////////////////////////////////////////////

/////////////////////Print in Human Readable Format/////////////////////
void printDataHuman(byte* inputArray) {
  byte char1 = 45;
  int arrayLength = inputArray[0];

  boolean state = true;

  for (int i = 1; i < arrayLength - 1; i++) {
    byte counter = inputArray[i];
    for (int j = 0; j < counter; j++) {
      if (state == true) {
        Serial.print("`");
      } else {
        Serial.print(".");
      }
    }
    state = !state;
    Serial.print("|");

  }
  Serial.println("");
}

void printDataHuman_old(byte* inputArray) {
  int arrayLength = inputArray[0];

  byte byte1 = 0;
  byte byte2 = 0;

  for (int i = 1; i < (arrayLength - 2); i++) {
    byte1 = inputArray[i];
    byte2 = inputArray[i + 1];
    if (byte1 == 0 && byte2 == 0) {
      Serial.print("_");
    } else {
      if (byte1 == 1 && byte2 == 1) {
        Serial.print("Â¯");
      } else {
        if ((byte1 == 0 && byte2 == 1) || (byte1 == 1 && byte2 == 0))
          Serial.print("|");
      }
    }

  }
}
////////////////////////////////////////////////////////////////////////

/////////////////////Prints an Array Byte wise/////////////////////
void printDataByte(byte* inputArray, int arraysize) {
  //Serial.print("Recieved IR Packet:");
  Serial.print("{");
  for (int i = 0; i < arraysize; i++) {
    Serial.print(inputArray[i]);
    if (i < arraysize - 1) {
      Serial.print(",");
    } else {
      Serial.println("}");
    }
  }
  //Serial.println("");
}
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
bool checkForData(byte* inputArray, byte arraysize, bool printArray) {
  int i = 0;
  byte lastValue = 0xFF;
  byte currValue = 0xFF;

  for (i = 0; i < arraysize - 1; i++) {
    currValue = lastValue & inputArray[i];
    lastValue = currValue;
  }

  //Serial.println(currValue);
  //if(currValue<255){
  if (printArray == true) {
    //printDataBits();
  }
  //byte nextStep[recvBuff];
  //nextStep==irDataIn;
  //clipData(nextStep);
  //}
  return true;
}
/////////////////////////////////////////////////////////////////////

/////////////////////Prints Input Array bit-wise/////////////////////
void printDataBits(byte* inputArray, int arrayLength) {
  Serial.print("Recieved Packet:");
  for (int i = 0; i < arrayLength; i++) {
    for (int j = 0; j <= 7; j++) {
      byte currByte = inputArray[i];
      Serial.print(bitRead(currByte, j));
      //Serial.print("0x");
      //Serial.print(inputArray[i],HEX);
      //Serial.print(",");

    }
  }
  Serial.println("");
}
///////////////////////////////////////////////////////////////////////


