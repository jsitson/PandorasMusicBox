// Arduino Test

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NUMNOTES 12
#define SCALE 7
#include "pitches.h"
#include <LiquidCrystal.h>
const int rs = 25, en = 3, d4 = 4, d5 = 23, d6 = 2, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/* MUSIC GLOSSARY
  Western music generally divides music into 12 tones:
  C, C#/Db, D, D#/Eb, E, F, F#/Gb, G, G#/Ab, A, A#/Bb, B
    '#' reads as 'sharp'
    'b' reads as 'flat'
    if there's a '/' between notes, that means they fundamentally are the same notes, 
      but in terms of music theory, they are technically different
    since we only care about the sound of the note, for duplicates we will use the names:
      C#, Eb, F#, Ab, Bb
  These 12 tones make up one "Octave"
  A "major scale" consists of 7 out of these 12 notes
  There are 12 major scales for the 12
  A major scale is made up of a pattern of whole steps and half steps
  A "Half step" is defined as a distance of 1 between 2 notes
    for example: 
      F is 1 half step above E
      G is 3 half steps above Bb
    ** the 12 tones are cyclical, so C is one half step above B and vise-versa 
  A "Whole step" is defined as a distance of 2 between 2 notes    
    for example: 
      C is 1 whole step above D
      G is 2 whole steps above B
  A "chromatic scale" is a scale consisting entirely of half steps
  In this code we use the terms "scale" and "key" interchangeably
    for example "C major scale" == "key of C"
  The note name of a given scale is the "root" of the scale
    for example, F# is the root of the F# Major scale
  In our instrument of 12 notes, we have C as the note with the lowest frequency and B as the highest
  
  Not every combination of notes fits into a major scale or chromatic,
  but this program will find the closest scale based on user input
*/

typedef struct note {
  char * name;  // name of the note
  int frequency;  // # times the note was played by the user
  int piezoNum; // acts as the index of an array of notes for scale-ordering purposes
}note;

typedef struct scaleNote {
  char * name;  // name of the note
  int piezoNum; // acts as the index of an array of notes for scale-ordering purposes
}scaleNote;

note list[NUMNOTES] = {
  {.name = "C",  .frequency = 0, .piezoNum = 0} , {.name = "C#", .frequency = 0, .piezoNum = 1} ,
  {.name = "D",  .frequency = 0, .piezoNum = 2} , {.name = "Eb", .frequency = 0, .piezoNum = 3} , 
  {.name = "E",  .frequency = 0, .piezoNum = 4} , {.name = "F",  .frequency = 0, .piezoNum = 5} , 
  {.name = "F#", .frequency = 0, .piezoNum = 6} , {.name = "G",  .frequency = 0, .piezoNum = 7} , 
  {.name = "Ab", .frequency = 0, .piezoNum = 8} , {.name = "A",  .frequency = 0, .piezoNum = 9} , 
  {.name = "Bb", .frequency = 0, .piezoNum = 10}, {.name = "B",  .frequency = 0, .piezoNum = 11}
};

scaleNote scaleList[NUMNOTES][SCALE] = {
  {{.name = "C", .piezoNum = 0} , {.name = "D", .piezoNum = 2} , {.name = "E", .piezoNum = 4} , {.name = "F", .piezoNum = 5} ,
   {.name = "G", .piezoNum = 7} , {.name = "A", .piezoNum = 9} , {.name = "B", .piezoNum = 11}},
  {{.name = "C#", .piezoNum = 1} , {.name = "Eb", .piezoNum = 3} , {.name = "F",  .piezoNum = 5}, {.name = "F#", .piezoNum = 6} , 
   {.name = "Ab", .piezoNum = 8} , {.name = "Bb", .piezoNum = 10} , {.name = "C", .piezoNum = 0}},
  {{.name = "D", .piezoNum = 2} , {.name = "E", .piezoNum = 4} , {.name = "F#", .piezoNum = 6} , {.name = "G", .piezoNum = 7} ,
    {.name = "A", .piezoNum = 9} , {.name = "B", .piezoNum = 11} , {.name = "C#", .piezoNum = 1}} ,
  {{.name = "Eb", .piezoNum = 3} , {.name = "F", .piezoNum = 5} , {.name = "G", .piezoNum = 7} , {.name = "Ab", .piezoNum = 8} , 
    {.name = "Bb", .piezoNum = 10} , {.name = "C", .piezoNum = 0} , {.name = "D", .piezoNum = 2}},
  {{.name = "E", .piezoNum = 4} , {.name = "F#", .piezoNum = 6} , {.name = "Ab", .piezoNum = 8} , {.name = "A", .piezoNum = 9} , 
    {.name = "B", .piezoNum = 11} , {.name = "C#", .piezoNum = 1} , {.name = "Eb", .piezoNum = 3}},
  {{.name = "F", .piezoNum = 5} , {.name = "G", .piezoNum = 7} , {.name = "A", .piezoNum = 9} , {.name = "Bb", .piezoNum = 10} , 
    {.name = "C", .piezoNum = 0} , {.name = "D", .piezoNum = 2} , {.name = "E", .piezoNum = 4}},
  {{.name = "F#", .piezoNum = 6} , {.name = "Ab", .piezoNum = 8} , {.name = "Bb", .piezoNum = 10} , {.name = "B", .piezoNum = 11} , 
   {.name = "C#", .piezoNum = 1} , {.name = "Eb", .piezoNum = 3} , {.name = "F", .piezoNum = 5}},
  {{.name = "G", .piezoNum = 7} , {.name = "A", .piezoNum = 9} , {.name = "B", .piezoNum = 11} , {.name = "C", .piezoNum = 0},
   {.name = "D", .piezoNum = 2} , {.name = "E", .piezoNum = 4} , {.name = "F#", .piezoNum = 6}},
  {{.name = "Ab", .piezoNum = 8} , {.name = "Bb", .piezoNum = 10} , {.name = "C", .piezoNum = 0}, {.name = "C#", .piezoNum = 1} , 
   {.name = "Eb", .piezoNum = 3} , {.name = "F",  .piezoNum = 5}, {.name = "G", .piezoNum = 7}},
  {{.name = "A", .piezoNum = 9} , {.name = "B", .piezoNum = 11} , {.name = "C#", .piezoNum = 1}, 
   {.name = "D", .piezoNum = 2} , {.name = "E", .piezoNum = 4} , {.name = "F#", .piezoNum = 6} , {.name = "Ab", .piezoNum = 8}},
  {{.name = "Bb", .piezoNum = 10} , {.name = "C", .piezoNum = 0} , {.name = "D", .piezoNum = 2} , {.name = "Eb", .piezoNum = 3},
   {.name = "F", .piezoNum = 5} , {.name = "G", .piezoNum = 7} , {.name = "A", .piezoNum = 9}},
  {{.name = "B", .piezoNum = 11} , {.name = "C#", .piezoNum = 1} , {.name = "Eb", .piezoNum = 3} ,
  {.name = "E", .piezoNum = 4} , {.name = "F#", .piezoNum = 6} , {.name = "Ab", .piezoNum = 8} , {.name = "Bb", .piezoNum = 10}}
};

char closestKey[10];
int endingCon = 0;

// helper function to swap 2 indexes of an array
void swap(note *array, int i, int j)
{
  note temp = array[i]; 
  array[i] = array[j]; 
  array[j]= temp;
}

// sort the user's inputted frequency array from highest to lowest, (based on the selection sort algorithm)
void sortByFrequency(note *array, int n)
{
  int j, start, indxOfMax;
  for (start = 0; start < n; start++){
    indxOfMax = start;
    for (j = start + 1; j < n; j++){
      if (array[j].frequency > array[indxOfMax].frequency)
        indxOfMax = j;
      swap(array, start, indxOfMax);
    }
  }
}

// like previous, but sorting by the Piezo Number (C = 0, B = 11), to make key recognition simpler
void sortByPiezoNum(note *array, int n)
{
  int j, start, indxOfMin;
  for (start = 0; start < SCALE; start++){
    indxOfMin = start;
    for (j = start + 1; j < SCALE; j++){
      if (array[j].piezoNum < array[indxOfMin].piezoNum)
        indxOfMin = j;
      swap(array, start, indxOfMin);
    }
  }
}

//function to find how many halfstep intervals are in the inputted frequency array
int findHalfSteps(note * list)
{
  int i, num = 0;
  //loop through all 7 of the most frequent notes in the inputted array
  for (i = 0; i < SCALE; i++) 
    //if the current and following note are 1 half step appart, increment number of half steps 
    if ((list[(i+1)%NUMNOTES].piezoNum) - list[i].piezoNum == 1)
      num++;
  return num;
}

 
 // Function to find the closest Major scale to the inputted 
void findKey (scaleNote scaleList[NUMNOTES][SCALE], note * list, char * closestKey)
{
  int i, sameNotes = 0, sameNotesMax = 0, noteInScale;
  int scalePos = 0, inputPos = 0, startingNote, rootIsInKey = 0, curKey;
  
  // loop through all 12 major scales
  for (curKey = 0; curKey < NUMNOTES; curKey++)
  {
    // reset these counters to 0 for every key
    inputPos = scalePos = sameNotes = rootIsInKey = 0;
    // to find the index where C/C# is in every key
    if (curKey < 8)  // Keys C major ~ G major
      startingNote = (curKey+1)/2; 
    if (curKey >= 8) // Keys Ab major ~ B major
      startingNote = (curKey+2)/2;
    
    // loop through each note of the scale to count how many notes in common with the input
    while ((inputPos < SCALE) && (scalePos < SCALE))
    {
      // index starting where C/C# is in the key and works with each itteration
      noteInScale = (SCALE - startingNote + scalePos)%SCALE;
      // comparing the input note with the note in the key
      if (list[inputPos].piezoNum < (scaleList[curKey][noteInScale].piezoNum))
        inputPos++;
      else if(list[inputPos].piezoNum > (scaleList[curKey][noteInScale].piezoNum))
        scalePos++;
      else if (list[inputPos].piezoNum == (scaleList[curKey][noteInScale].piezoNum))
      {
        // we'll only consider a scale is close if the root of the scale is in the inputted notes
        if (strcmp(list[inputPos].name, scaleList[curKey]->name) == 0)
          rootIsInKey = 1;        
        sameNotes++;  inputPos++;   scalePos++;
      }
    }
    // if we find a scale with more notes in common with the input, change the closestKey
    if ((sameNotes > sameNotesMax) && rootIsInKey)
    {
      sameNotesMax = sameNotes;
      strcpy(closestKey, scaleList[curKey]->name);
    }
  }
}



//can change once the minumum thresholds are found by the instrument keys on the piezo
const int threshold = 3;    // minimum reading of the sensors that generates a note

// notes to play, corresponding to the 3 sensors:
int notes[] = {
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4,
  NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4
};

void setup() {

}

void loop() {
  
  for (int thisSensor = 0; thisSensor < 12; thisSensor++) {
    // get a sensor reading:
    int sensorReading = analogRead(thisSensor);

    // if the sensor is pressed hard enough:
    if (sensorReading > threshold) {
      Serial.println(sensorReading);
      // play the note corresponding to this sensor:
      list[thisSensor].frequency++;
      endingCon++;
      tone(8, notes[thisSensor], 100);
      delay(50);
      Serial.println(list[thisSensor].name);
      Serial.println(list[thisSensor].frequency);
    }
    if (endingCon>=20)
    {
      sortByFrequency(list, NUMNOTES);
      sortByPiezoNum(list, NUMNOTES); 
    
      // We consider the inputted notes to be closer to a chromatic if there are 5 half steps
      if (findHalfSteps(list) >= 5)
        strcpy(closestKey, "Chromatic");
      else
        findKey(scaleList, list, closestKey);
      lcd.print(closestKey);
      delay(200);
    }
    }
    return 0;
  }


    
