#include <stdio.h>
#include <string.h>
#define BOMF (char)0xff
#define BOMS (char)0xfe


/**creating int i = 1 wich is 0x1 and then creating a char pointer to point the first byte of i
 * if it was little endian the first byte should be 0x01 = 1
 * if it was big endian the first byte should be 0x00 = 0
 *
 * @return 0 if our system is Littel Endian else return 1
 */
int is_little_endian(char* bom) {
    // check the bom if it fffe or feff
	
	if ((char)bom[0] == (char)0xfe && (char)bom[1] == (char)0xff) {
	 	return 1;
    }
    return 0;
}

void charSwap(char* a, char* b){
    char temp = *a;
    *a = *b;
    *b = temp;
}

void copyFileToFile(char* fileNameInput, char* fileNameOutput, char* KeepOrSwap){
	char buffer[2];
	char newBuffer[2];
    FILE *fileInput, *fileOutput;
    fileInput = fopen(fileNameInput, "rb");
	fileOutput = fopen(fileNameOutput, "wb");
    
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
	//printf ("%d /n/n/n", isLittleEndian);
	if (strcmp(KeepOrSwap, "-keep") == 0)
	{
		fwrite(buffer, 1, sizeof(buffer), fileOutput);	}
	else if (strcmp(KeepOrSwap, "-swap") == 0)
	{
		charSwap(&buffer[0], &buffer[1]);
		fwrite(buffer, 1, sizeof(buffer), fileOutput);
	}
	
	//fwrite(buffer, 1, sizeof(buffer), fileOutput);

	//check if windows
	if (strcmp(sysInput,"-win") == 0 || strcmp(sysOutput, "-win") == 0)
	{
		//windows to unix:
		if ((strcmp(sysInput,"-win") == 0) && (strcmp(sysOutput,"-unix") == 0))
		{
			while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
			{
				//big endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isLittleEndian == 1))
				{
					char nextBuffer[2];
					fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x00 ) && (nextBuffer[1] == 0x0a))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
							fread(buffer, 1 , sizeof(nextBuffer), fileInput);
						}
						else if (strcmp(KeepOrSwap, "-swap") == 0)
						{
							charSwap(&nextBuffer[0], &nextBuffer[1]);
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
							fread(buffer, 1 , sizeof(nextBuffer), fileInput);
						}
					}
				}



				//littel endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
				{
					char nextBuffer[2];
					fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x0a ) && (nextBuffer[1] == 0x00))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
							fread(buffer, 1 , sizeof(nextBuffer), fileInput);
						}
						else if (strcmp(KeepOrSwap, "-swap") == 0)
						{
							charSwap(&nextBuffer[0], &nextBuffer[1]);
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
							fread(buffer, 1 , sizeof(nextBuffer), fileInput);
						}
					}
				}

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
			fclose(fileInput);
			fclose(fileOutput);
			return;
		}
		

		
		//windows to mac:
		if ((strcmp(sysInput,"-win") == 0) && (strcmp(sysOutput,"-mac") == 0))
		{
			while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
			{
				//big endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isLittleEndian == 1))
				{
					char nextBuffer[2];
					fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x00 ) && (nextBuffer[1] == 0x0a))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(buffer, 1, sizeof(buffer), fileOutput);
							fread(buffer, 1 , sizeof(buffer), fileInput);
						}
						else if (strcmp(KeepOrSwap, "-swap") == 0)
						{
							charSwap(&buffer[0], &buffer[1]);
							fwrite(buffer, 1, sizeof(buffer), fileOutput);
							fread(buffer, 1 , sizeof(buffer), fileInput);
						}
					}
				}



				//littel endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
				{
					char nextBuffer[2];
					fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x0a ) && (nextBuffer[1] == 0x00))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(buffer, 1, sizeof(buffer), fileOutput);
							fread(buffer, 1 , sizeof(buffer), fileInput);
						}
						else if (strcmp(KeepOrSwap, "-swap") == 0)
						{
							charSwap(&buffer[0], &buffer[1]);
							fwrite(buffer, 1, sizeof(buffer), fileOutput);
							fread(buffer, 1 , sizeof(buffer), fileInput);
						}
					}
				}

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
				//big endian
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
				//littel endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isLittleEndian == 0))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
						flagMW = 0;
					}
					else if (strcmp(KeepOrSwap, "-swap") == 0)
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
				//big endian
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
				//littel endian
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
			//for big
			if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isLittleEndian == 1))
			{
				buffer[1] = 0x0a;
			}
			//for little
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
			//for big
			if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0a) && (isLittleEndian == 1))
			{
				buffer[1] = 0x0d;
			}
			//for little
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

int main(int argc, char* argv[]){

	//copyFileToFile2("./unix_input-utf-16.txt", "stest_win_to_unix_swap.txt", "-win", "-unix", "-keep");

	FILE *fileInput, *fileOutput;
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
			return 0;
		}
	}
	
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