/*
 * File:   example1.cc
 * Author: Meghna Basu
 * NetID : mxb171230 
 */
#include <stdint.h> // for uint32_t, uint64_t
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include  <sstream>
//#include <bits/stdc++.h>

#include "cdk.h"

#include <fstream>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 15
#define MATRIX_NAME_STRING "Binary File Contents"


using namespace std;

///*** For reading Header record ***////
class BinaryFileHeader
      {
      public:
	uint32_t magicNumber;
	uint32_t versionNumber;
	uint64_t numRecords;
      };

///*** For reading data records ***////
const int maxRecordStringLength = 25;
class BinaryFileRecord
      {
      public:
        uint8_t strLength;
	char stringBuffer[maxRecordStringLength]; 
      };



int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  const char 		*rowTitles[MATRIX_HEIGHT+1] = {"R0", "R1", "R2", "R3","R4", "R5"};
  const char 		*columnTitles[MATRIX_WIDTH+1] = {"C0", "C1", "C2", "C3"};
  int		boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  //  drawCDKMatrix(myMatrix, true);

  ///*** Read the header fro Binary File ***///
  BinaryFileHeader * headerRecord = new BinaryFileHeader (); //calling the default constructor  
  BinaryFileRecord * dataRecords [4];
  for (int i = 0; i<4; i++)
    {
      dataRecords[i] = new BinaryFileRecord ();
    }

 
  ifstream binInFile ("cs3377.bin", ios::in | ios::binary);
  if (binInFile.is_open())
    {
      binInFile.read((char *) headerRecord, sizeof(BinaryFileHeader)); // reading header records
      
      cout << "Magic Number: " << hex <<headerRecord->magicNumber<<endl;
      // cout << "Version Number: " << headerRecord->versionNumber<<endl;
      // cout << "Number of  Records: " << headerRecord->numRecords<<endl;
     
      /// *** Reading record data *** ////
      for (int i = 0; i < headerRecord->numRecords; i++)
	{
	  binInFile.read((char*)dataRecords[i], sizeof(BinaryFileRecord));
	  //cout <<"String Length :  "<<dataRecords[i]->strLength;
	  //cout <<"String : "<<dataRecords[i]->stringBuffer<<endl;
	}
    }
  else 
    {
      cout << " Error: Failed to open the binary file "<< endl;
      _exit (1);
    }

  
  ///*** Dipslay Header data ***///
  
  stringstream mNum;
  stringstream vNum;
  stringstream numRec;
  mNum << hex << headerRecord->magicNumber;
  vNum <<headerRecord->versionNumber;
  numRec<<headerRecord->numRecords;
  string magicNum = "Magic: Ox"+ mNum.str();
  string versionNum = "Version: "+ vNum.str();
  string numRecord = numRec.str();

  setCDKMatrixCell(myMatrix, 1, 1, magicNum.c_str() );
  setCDKMatrixCell(myMatrix, 1, 2, versionNum.c_str() );
  setCDKMatrixCell(myMatrix, 1, 3, numRecord.c_str() );

  ///*** Dipslay Record data ***///
  //    int j = 0;
  // for (int i = 2; i < (headerRecord->numRecords + 1); i++)
  //  {
	// setCDKMatrixCell(myMatrix, i, 1,dataRecords[j]->strLength);
  //            setCDKMatrixCell(myMatrix, i, 1, dataRecords[j]->stringBuffer);
  //    j++;
  //   }

   drawCDKMatrix(myMatrix, true);    /* required  */

  /* so we can see results */
  sleep (30);
  
  ///*** closing the binary file ***///
  binInFile.close();

  // Cleanup screen
  endCDK();
}
