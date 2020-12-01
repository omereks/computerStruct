#include <stdio.h>
#include <string.h>
#define BOMF (char)0xff;
#define BOMS (char)0xfe;


/**creating int i = 1 wich is 0x1 and then creating a char pointer to point the first byte of i
 * if it was little endian the first byte should be 0x01 = 1
 * if it was big endian the first byte should be 0x00 = 0
 *
 * @return 0 if our system is Littel Endian else return 1
 */
int is_big_endian(const char* bom) {
    // check the bom if it fffe or feff
	if (bom[0] == 0xff && bom[1] == 0xfe) {
        return 0;
    }
    return 1;
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
    FILE *fileInput, *fileOutput;
    fileInput = fopen(fileNameInput, "rb");
	fileOutput = fopen(fileNameOutput, "wb");
	
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

	fread(buffer, 1, sizeof(buffer), fileInput);
	int isBigEndian = is_big_endian(buffer);		
	fwrite(buffer, 1, sizeof(buffer), fileOutput);

	//check if windows
	if (strcmp(sysInput,"-win") == 0 || strcmp(sysOutput, "-win") == 0)
	{
		//windows to unix:
		if ((strcmp(sysInput,"-win") == 0) && (strcmp(sysOutput,"-unix") == 0))
		{
			while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
			{
				//big endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isBigEndian == 1))
				{
					char nextBuffer[2];
					fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x00 ) && (nextBuffer[1] == 0x0a) && (isBigEndian == 1))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
						}
						else
						{
							charSwap(&nextBuffer[0], &nextBuffer[1]);
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
						}
					}
				}



				//littel endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isBigEndian == 0))
				{
					char nextBuffer[2];
					fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x0a ) && (nextBuffer[1] == 0x00) && (isBigEndian == 0))
					{
						if (strcmp(KeepOrSwap, "-keep") == 0)
						{
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
						}
						else
						{
							charSwap(&nextBuffer[0], &nextBuffer[1]);
							fwrite(nextBuffer, 1, sizeof(nextBuffer), fileOutput);
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
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isBigEndian == 1))
				{
					char nextBuffer[2];
					fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x00 ) && (nextBuffer[1] == 0x0a) && (isBigEndian == 1))
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
				}



				//littel endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isBigEndian == 0))
				{
					char nextBuffer[2];
					fread(nextBuffer, 1 , sizeof(nextBuffer), fileInput);
					if ((nextBuffer[0] == 0x0a ) && (nextBuffer[1] == 0x00) && (isBigEndian == 0))
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
				//big endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isBigEndian == 1))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndBig, 1, sizeof(windowsEndBig), fileOutput);
					}
					else
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
					}
					
				}
				//littel endian
				if ((buffer[0] == 0x0d ) && (buffer[1] == 0x00) && (isBigEndian == 0))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
					}
					else
					{
						fwrite(windowsEndBig, 1, sizeof(windowsEndBig), fileOutput);
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

		//unix to windows
		if ((strcmp(sysInput,"-unix") == 0) && (strcmp(sysOutput,"-win") == 0))
		{
			while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
			{
				//big endian
				if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0a) && (isBigEndian == 1))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndBig, 1, sizeof(windowsEndBig), fileOutput);
					}
					else
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
					}
					
				}
				//littel endian
				if ((buffer[0] == 0x0a ) && (buffer[1] == 0x00) && (isBigEndian == 0))
				{
					if (strcmp(KeepOrSwap, "-keep") == 0)
					{
						fwrite(windowsEndLittle, 1, sizeof(windowsEndLittle), fileOutput);
					}
					else
					{
						fwrite(windowsEndBig, 1, sizeof(windowsEndBig), fileOutput);
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

		return;
	}
	
	// unix or mac
	//mac to unix
	if (strcmp(sysInput,"-mac") == 0)
	{
		while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
		{
			if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0d) && (isBigEndian == 1))
			{
				buffer[1] = 0x0a;
			}
			if ((buffer[1] == 0x00 ) && (buffer[0] == 0x0d) && (isBigEndian == 0))
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
	if (strcmp(sysInput,"-unix") == 0)
	{
		while (fread(buffer, 1 , sizeof(buffer), fileInput) > 0)
		{
			if ((buffer[0] == 0x00 ) && (buffer[1] == 0x0a) && (isBigEndian == 1))
			{
				buffer[1] = 0x0d;
			}
			if ((buffer[1] == 0x00 ) && (buffer[0] == 0x0a) && (isBigEndian == 0))
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