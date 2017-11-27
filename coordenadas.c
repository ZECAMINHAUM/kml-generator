#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum { false, true } bool;
struct tdados{
	float  lat, longi, alt, velo;
};
void recebe(char nome[10]);
FILE* abre (char modo, char caminho[30]);
float calcula (float  lat1, float  longi1, float  lat2, float  longi2);
main (){
	FILE *arq, *arq2;
	char nome[10];
	int i, reg, cont;
	float  lat1, longi1, lat2, longi2, Dst, Rmedia;
	bool veri = true, veri2 = true;
	struct tdados GPS;

	printf("\n\t\tBEM VINDO");
	printf("\n\nincializando...");
	sleep(1);
	system("cls");

	do{
	system("cls");

	printf("\n\t\t%c SISTEMA DE COORDENADAS %c", 185, 204);
	recebe(nome);
	arq = abre('l', nome);

	if(arq == NULL){
		printf("\nNAO FOI POSSIVEL ABRIR O ARQUIVO");
		veri = false;
	}
	}while(veri == false);
	cont = 0;
	i = 1;
	fscanf(arq, "%d", &reg);
	printf("O NUMERO DE REGISTRO DO ARQUIVO: %d", reg);
	sleep(1);
	system("cls");
	fseek(arq, 5, SEEK_SET);
	while(!feof(arq)){
		fscanf(arq,"%f %f %f %f", &GPS.lat, &GPS.longi, &GPS.alt, &GPS.velo);
		fflush(stdin);
		printf("\nCoordenada %i: %.4f %.4f %.2f %.5f", i, GPS.lat, GPS.longi, GPS.alt, GPS.velo);
		sleep(2);
		if(veri2 == true){
			lat1 += GPS.lat;
		 	longi1+= GPS.longi;
		}else if (veri2 == false){
			lat2 += GPS.lat;
			longi2 += GPS.longi;
		}
		veri2 = false;
		Rmedia += GPS.velo;
		cont++;
		i++;
	}
	system("cls");
	Dst = calcula(lat1, longi1, lat2, longi2);
	Rmedia = (Rmedia/cont)*3.6;
	printf("\nDISTANCIA PERCORRIDA DE %.3f KM", Dst);
	printf("\nVELOCIDADE MEDIA DE %.2f KM/HORA\n", Rmedia);
	getchar();
	system("cls");
	i = 0;
	arq2 = abre ('l', "Gmap.kml");
	fseek(arq, 5, SEEK_SET);
	fseek(arq2, 381, SEEK_SET);
	while(!feof(arq)){
		fscanf(arq, "%f %f %f %f", &GPS.lat, &GPS.longi, &GPS.alt, &GPS.velo);
		fflush(stdin);
		fprintf(arq2, "\n\t\t%.5f, %.5f", GPS.lat, GPS.longi);
	}
	fclose(arq);
	fclose(arq2);
	arq2 = abre('e', "Gmap.kml");
	fprintf(arq2, "\n\t  </coordinates>\n\t</LineString>\n  </Placemark>\n</kml>");
	fclose(arq2);
	printf("\nAQUIVO KML PRONTO...");
	sleep(1);
	printf("\nAPERTE QUALQUER TECLA PARA SAIR");
	getchar();
}
void recebe (char nome[10]){
	do{
		printf("\n\nPOR FAVOR, DIGITE O NOME DO ARQUIVO: ");
		scanf("%s", nome);
		fflush(stdin);
	}while((strlen(nome) < 1)||(strlen(nome) > 10));
}
FILE* abre (char modo, char caminho[30]){
	FILE *arq;
	if (modo == 'l'){
		arq = fopen(caminho, "r+");
	}
	if (modo == 'e'){
		arq = fopen(caminho, "a");
	}
}
float calcula (float  lat1, float  longi1, float  lat2, float  longi2){

	float  a, b, dlat, dlong, Dst, r = 6371.0;

	lat1 = lat1 * (M_PI / 180.0);
	longi1 = longi1 * (M_PI / 180.0);
	lat2 = lat2 * (M_PI / 180.0);
	longi2 = longi2 * (M_PI / 180.0);

	dlat = (lat2 +(lat1 * -1)) / 2;
	dlong = (longi2 + (longi1 * -1)) / 2;

	a = sin(dlat) * sin(dlat) + cos(lat1) * cos(lat2) * sin(dlong) * sin(dlong);
	b = 2 * atan2(sqrt(a), sqrt(1 + (a * -1)));

	Dst = (r * b * 1000) / 1000;
	return Dst;
}
