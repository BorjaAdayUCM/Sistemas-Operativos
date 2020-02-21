#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

//extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes)
{
    int i = 0, c;
    while ((c = getc(origin)) != EOF && i < nBytes){
    	if (putc(c, destination) == EOF) return -1;
    	i++;
    }
    return i;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char* loadstr(FILE * file)
{
	int i = 1; int caracter;
	caracter = getc(file);
	while (caracter != (int)'\0' && caracter != EOF) {
		caracter = getc(file);
		i++;
	}
	if (caracter == EOF) return NULL;
	char* cadena = (char*)malloc(i * sizeof(char));
	if (cadena == NULL) return NULL;
	fseek(file, -i, SEEK_CUR);
	if (fread(cadena, sizeof(char), i, file) != i) return NULL;
	return cadena;
}


/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry* readHeader(FILE * tarFile, int *nFiles)
{
	stHeaderEntry* cabecera = NULL;
	int  nr_files = 0;
	/* ...  Read  the  number  of  files (N) from  tarfile  and
	store  it in  nr_files  ... */
	if (fread(&nr_files, sizeof(int), 1, tarFile) != 1) return NULL;
    if (nr_files <= 0) return NULL;
	/*  Allocate  memory  for  the  array */
	cabecera = (stHeaderEntry*)malloc(nr_files * sizeof(stHeaderEntry));
	if (cabecera == NULL) return NULL;
	/*...  Read  the (pathname ,size) pairs  from  tarFile  and
	store  them in the  array  ...*/
	for (int i = 0; i < nr_files; i++) {
		cabecera[i].name = loadstr(tarFile);
		if (fread(&cabecera[i].size, sizeof(unsigned int), 1, tarFile) != 1) return NULL;
	}
	/* Store  the  number  of  files  in the  output  parameter  */
	(*nFiles) = nr_files;
	return cabecera;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */

int createTar(int nFiles, char *fileNames[], char tarName[])
{
	int offData = 0;

	//Crear archivo tar
	FILE * file = fopen(tarName, "w+");
	if (file == NULL) return EXIT_FAILURE;

	//Reservar espacio para la cabecera
	stHeaderEntry* array = (stHeaderEntry*)malloc(nFiles * sizeof(stHeaderEntry));
	if (array == NULL) return EXIT_FAILURE;

	//calcular offData y dar valores a name en el array
	for(int i = 0; i < nFiles; i++){
		array[i].name = fileNames[i];
		offData += (strlen(fileNames[i]) + 1);
	}
	offData += sizeof(int) + (nFiles * sizeof(unsigned int));

	//Escribir el contenido de los ficheros
	fseek(file, offData, SEEK_SET);
	for(int i = 0; i <  nFiles; i++){
		FILE *inputFile = fopen(fileNames[i],"r");
		if (inputFile == NULL) return EXIT_FAILURE;
		array[i].size = copynFile(inputFile, file, INT_MAX);
		if (array[i].size == -1) return EXIT_FAILURE;
		if (fclose(inputFile) != 0) return EXIT_FAILURE;
	}
	fseek(file, 0, SEEK_SET);
	if (fwrite(&nFiles, sizeof(int), 1, file) != 1) return EXIT_FAILURE;

	//Escribir la cabecera
	for(int i = 0; i <  nFiles; i++){
		if (fwrite(array[i].name, strlen(array[i].name), 1, file) != 1) return EXIT_FAILURE;
		if (fputc('\0', file) == EOF) return EXIT_FAILURE;
		if (fwrite(&array[i].size, sizeof(unsigned int), 1, file) != 1) return EXIT_FAILURE;
	}

	// Cerrar mtar
	if (fclose(file) != 0) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname

 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[])
{
	int nFiles;
	FILE * file = fopen(tarName, "r");
	if (file == NULL) return -1;
	stHeaderEntry* cabecera = readHeader(file, &nFiles);
	for(int i = 0; i < nFiles; i++)
	{
		FILE * newFich = fopen(cabecera[i].name, "w");
		if (newFich == NULL) return - 1;
		if (copynFile(file, newFich, cabecera[i].size) == -1) return EXIT_FAILURE;
		fseek(file, -1, SEEK_CUR);
		if (fclose(newFich) != 0) return EXIT_FAILURE;
	}
	if (fclose(file) != 0) return EXIT_FAILURE;
	free(cabecera);
	return EXIT_SUCCESS;
}
