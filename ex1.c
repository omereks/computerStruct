‫‪//‬‬ 312350192 Omer Eckstein



int is_big_endian(){
	unsigned int i = 1;
	char *firstDig = (char*) &i;
	if (firstDig[0] == 1)
		// Litel endian
		return 0;
	// Big endian
	return 1; 
}
