// 312350192 Omer Eckstein
#include <stdio.h>
#include <string.h>

/**
 * check if the the txt is in big or little endian by the BOM
 *
 * @return 0 if our system is big Endian else return 1
 */
int is_little_endian(char* bom) {
    // check the bom if it fffe or feff
	if ((char)bom[0] == (char)0xfe && (char)bom[1] == (char)0xff) {
	 	return 1;
    }
    return 0;
}
/**
 * @brief swaping chars by pointer 
 * 
 * @param a the first char
 * @param b the second char
 */
void charSwap(char* a, char* b){
    char temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief first method for copying file into onther on the same OS
 * 
 * @param fileNameInput the name of the input file
 * @param fileNameOutput the name of the output file
 * @param KeepOrSwap is either "-keep" or "-swap"
 */
void copyFileToFile(char* fileNameInput, char* fileNameOutput, char* KeepOrSwap){
	char buffer[2];
	char newBuffer[2];
    FILE *fileInput, *fileOutput;
    fileInput = fopen(fileNameInput, "rb");
	fileOutput = fopen(fileNameOutput, "wb");
	//overing the input file and copy it into the output file
	while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
	{
		if (strcmp(KeepOrSwap, "-keep") == 0)
		{
			fwrite(buffer, 1, sizeof(buffer), fileOutput);
		}
		else
		{
			charSwap(&buffer[0], &buffer[1]);
			fwrite(buffer, 1, sizeof(buffer), fileOutput);
		}	
	}
	fclose(fileOutput);
	fclose(fileInput);
	return;
}

/**
 * @brief for the second and third methods, i divided to cases:
 * 1. the same OS - sending to the first method
 * 2. windows to unix
 * 3. windows to mac
 * 4. mac to windows
 * 5. unix to windows
 * 6. mac to unix
 * 7. unix to mac
 * in every case above i divded by big an little endian, and then dvided again into keep or swap
 * 
 * @param fileNameInput the name of the input file
 * @param fileNameOutput the name of the output file
 * @param sysInput the inputs OS its either "-mac", "-unix", "-win"
 * @param sysOutput the output OS its either "-mac", "-unix", "-win"
 * @param KeepOrSwap is either "-keep" or "-swap"
 */
void copyFileToFile2(char* fileNameInput, char* fileNameOutput, char* sysInput, char* sysOutput, char* KeepOrSwap){
	//is the same OS
	if (strcmp(sysInput,sysOutput) == 0)
	{
		copyFileToFile(fileNameInput, fileNameOutput, KeepOrSwap);
		return;
	}
	
	char buffer[2];
    FILE *fileInput = fopen(fileNameInput, "rb");
	FILE *fileOutput = fopen(fileNameOutput, "wb");
     

	
	char windowsEndBig[4];
	windowsEndBig[0]=0x00;
	windowsEndBig[1]=0x0d;
	windowsEndBig[2]=0x00;
	windowsEndBig[3]=0x0a;

	
	char windowsEndLittle[4];
	windowsEndLittle[0]=0x0d;
	windowsEndLittle[1]=0x00;
	windowsEndLittle[2]=0x0a;
	windowsEndLittle[3]=0x00;

	int c = fread(buffer, 1, sizeof(buffer), fileInput);
	int isLittleEndian = is_little_endian(buffer);		
	//copying the BOM
	if (strcmp(KeepOrSwap, "-keep") == 0)
	{
		fwrite(buffer, 1, sizeof(buffer), fileOutput);	}
	else if (strcmp(KeepOrSwap, "-swap") == 0)
	{
		charSwap(&buffer[0], &buffer[1]);
		fwrite(buffer, 1, sizeof(buffer), fileOutput);
	}
	
	//check if windows
	if (strcmp(sysInput,"-win") == 0 || strcmp(sysOutput, "-win") == 0)
	{
		//windows to unix:
		if ((strcmp(sysInput,"-win") == 0) && (strcmp(sysOutput,"-unix") == 0))
		{
			int i = 1;
			while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
			{
				if((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isLittleEndian == 1)){
					i = fread(buffer, 1 , sizeof(buffer), fileInput);
				}
				if((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isLittleEndian == 0)){
					i = fread(buffer, 1 , sizeof(buffer), fileInput);
				}
				//little endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isLittleEndian == 1))
				{
					char nextBuffer[2];
					i = fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x00 ) && (nextBuffer[1] == 0x0a))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
							i = fread(buffer, 1 , sizeof(nextBuffer), fileInput);
						}
						else if (strcmp(KeepOrSwap, "-swap") == 0)
						{
							charSwap(&nextBuffer[0], &nextBuffer[1]);
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
							i = fread(buffer, 1 , sizeof(nextBuffer), fileInput);
						}
					}
				}



				//big endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
				{
					char nextBuffer[2];
					i = fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x0a ) && (nextBuffer[1] == 0x00))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
							i = fread(buffer, 1 , sizeof(nextBuffer), fileInput);
						}
						else if (strcmp(KeepOrSwap, "-swap") == 0)
						{
							charSwap(&nextBuffer[0], &nextBuffer[1]);
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
							i = fread(buffer, 1 , sizeof(nextBuffer), fileInput);
						}
					}
				}
				if (i > 0)
				{
					//if its not end of the line
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(buffer, 1, sizeof(buffer), fileOutput);
					}
					else if (strcmp(KeepOrSwap, "-swap") == 0)
					{
						charSwap(&buffer[0], &buffer[1]);
						fwrite(buffer, 1, sizeof(buffer), fileOutput);
					}
				}
				
				
			}
			fclose(fileInput);
			fclose(fileOutput);
			return;
		}
		

		
		//windows to mac:
		if ((strcmp(sysInput,"-win") == 0) && (strcmp(sysOutput,"-mac") == 0))
		{
			int i = 1;
			while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
			{
				if((buffer[0] == 0x00 ) && (buffer[1] == 0x0a) && (isLittleEndian == 1)){
					i = fread(buffer, 1 , sizeof(buffer), fileInput);
				}
				if((buffer[0] == 0x0a ) && (buffer[1] == 0x00) && (isLittleEndian == 0)){
					i = fread(buffer, 1 , sizeof(buffer), fileInput);
				}
				
				//little endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isLittleEndian == 1))
				{
					char nextBuffer[2];
					i = fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x00 ) && (nextBuffer[1] == 0x0a))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(buffer, 1, sizeof(buffer), fileOutput);
							i = fread(buffer, 1 , sizeof(buffer), fileInput);
						}
						else if (strcmp(KeepOrSwap, "-swap") == 0)
						{
							charSwap(&buffer[0], &buffer[1]);
							fwrite(buffer, 1, sizeof(buffer), fileOutput);
							i = fread(buffer, 1 , sizeof(buffer), fileInput);
						}
					}
				}



				//big endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
				{
					char nextBuffer[2];
					i = fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x0a ) && (nextBuffer[1] == 0x00))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(buffer, 1, sizeof(buffer), fileOutput);
							i = fread(buffer, 1 , sizeof(buffer), fileInput);
						}
						else if (strcmp(KeepOrSwap, "-swap") == 0)
						{
							charSwap(&buffer[0], &buffer[1]);
							fwrite(buffer, 1, sizeof(buffer), fileOutput);
							i = fread(buffer, 1 , sizeof(buffer), fileInput);
						}
					}
				}

				//if its not end of the line
				if (i > 0)
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(buffer, 1, sizeof(buffer), fileOutput);
					}
					else if (strcmp(KeepOrSwap, "-swap") == 0)
					{
						charSwap(&buffer[0], &buffer[1]);
						fwrite(buffer, 1, sizeof(buffer), fileOutput);
					}/* code */
				}
				
				
			}
			fclose(fileInput);
			fclose(fileOutput);
			return;
		}

		//mac to windows
		if ((strcmp(sysInput,"-mac") == 0) && (strcmp(sysOutput,"-win") == 0))
		{
			while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
			{
				int flagMW = 1;
				//little endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isLittleEndian == 1))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndBig, 1, sizeof(windowsEndBig), fileOutput);
						flagMW = 0;
					}
					else if (strcmp(KeepOrSwap, "-swap") == 0)
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
						flagMW = 0;
					}
					
				}
				//big endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
						flagMW = 0;
					}
					else if (strcmp(KeepOrSwap, "-swap") == 1)
					{
						fwrite(windowsEndBig, 1, sizeof(windowsEndBig), fileOutput);
						flagMW = 0;
					}
				}
				//if its not end of the line
				if (flagMW == 1)
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(buffer, 1, sizeof(buffer), fileOutput);
					}
					else if (strcmp(KeepOrSwap, "-swap") == 0)
					{
						charSwap(&buffer[0], &buffer[1]);
						fwrite(buffer, 1, sizeof(buffer), fileOutput);
					}
				}
			}
			fclose(fileInput);
			fclose(fileOutput);
			return;
		}

		//unix to windows
		if ((strcmp(sysInput,"-unix") == 0) && (strcmp(sysOutput,"-win") == 0))
		{
			while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
			{
				int flagUW = 0;
				//little endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0a) && (isLittleEndian == 1))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndBig, 1, sizeof(windowsEndBig), fileOutput);
						flagUW = 1;
					}
					else if (strcmp(KeepOrSwap, "-swap") == 0)
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
						flagUW = 1;
					}
					
				}
				//big endian
				if ((buffer[0] == 0x0a ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
						flagUW = 1;
					}
					else if (strcmp(KeepOrSwap, "-swap") == 0)
					{
						fwrite(windowsEndBig, 1, sizeof(windowsEndBig), fileOutput);
						flagUW = 1;
					}
				}
				//if its not end of the line
				if(flagUW == 0)
				{
						if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(buffer, 1, sizeof(buffer), fileOutput);
					}
					else if (strcmp(KeepOrSwap, "-swap") == 0)
					{
						charSwap(&buffer[0], &buffer[1]);
						fwrite(buffer, 1, sizeof(buffer), fileOutput);
					}
				}
			}
			fclose(fileInput);
			fclose(fileOutput);
			return;
		}

		return;
	}
	
	// unix or mac
	//mac to unix
	if ((strcmp(sysInput,"-mac") == 0) && (strcmp(sysOutput,"-unix") == 0))
	{
		while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
		{
			//for little
			if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isLittleEndian == 1))
			{
				buffer[1] = 0x0a;
			}
			//for big
			if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
			{
				buffer[0] = 0x0a;
			}
			
			if (strcmp(KeepOrSwap, "-keep") == 0)
			{
				fwrite(buffer, 1, sizeof(buffer), fileOutput);
			}
			else if (strcmp(KeepOrSwap, "-swap") == 0)
			{
				charSwap(&buffer[0], &buffer[1]);
				fwrite(buffer, 1, sizeof(buffer), fileOutput);
			}
		}
		fclose(fileInput);
		fclose(fileOutput);
	}
	//unix to mac
	if ((strcmp(sysInput,"-unix") == 0) && (strcmp(sysOutput,"-mac") == 0))
	{
		while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
		{
			//for little
			if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0a) && (isLittleEndian == 1))
			{
				buffer[1] = 0x0d;
			}
			//for big
			if ((buffer[0] == 0x0a ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
			{
				buffer[0] = 0x0d;
			}
			
			if (strcmp(KeepOrSwap, "-keep") == 0)
			{
				fwrite(buffer, 1, sizeof(buffer), fileOutput);
			}
			else if (strcmp(KeepOrSwap, "-swap") == 0)
			{
				charSwap(&buffer[0], &buffer[1]);
				fwrite(buffer, 1, sizeof(buffer), fileOutput);
			}
		}
		fclose(fileInput);
		fclose(fileOutput);
	}
	
	return;
}
/**
 * @brief  if there is only 2 or 4 args i add "-keep"
 * 
 * i divided to cases:
 * 1. the same OS - sending to the first method copyFileToFile
 * 2. windows to unix- sending to the first method copyFileToFile2
 * 3. windows to mac- sending to the first method copyFileToFile2
 * 4. mac to windows- sending to the first method copyFileToFile2
 * 5. unix to windows- sending to the first method copyFileToFile2
 * 6. mac to unix- sending to the first method copyFileToFile2
 * 7. unix to mac- sending to the first method copyFileToFile2
 * in every case above i divded by big an little endian, and then dvided again into keep or swap 
 */
int main(int argc, char* argv[]){
	FILE *fileInput;
	//check that the argument ar correct
	if ((argc < 3) || (argc == 4))
	{
		return 0;
	}
	else
	{
		fileInput = fopen(argv[1], "rb");
		if (fileInput == NULL)
		{
			fclose(fileInput);
			return 0;
		}
	}
	//switching for the right case
	if (argc == 3)
	{
		copyFileToFile(argv[1], argv[2], "-keep");
	}
	if (argc == 5)
	{
		copyFileToFile2(argv[1], argv[2], argv[3], argv[4], "-keep");
	}
	if (argc == 6)
	{
		copyFileToFile2(argv[1], argv[2], argv[3], argv[4], argv[5]);
	}
    return 0;
}