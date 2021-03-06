#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ebu.h"

void dumpTTI(struct EBU_TTI* tti){
	printf("SGN: %hX\n",tti->SGN);
	printf("SN : %02hX%02hX\n",tti->SN[1],tti->SN[0]);
	printf("EBN: %hX\n",tti->EBN);
	printf("CS : %hX\n",tti->CS);
	printf("TCI: %02d:%02d:%02d:%02d\n",tti->TCI.hours,tti->TCI.minutes,tti->TCI.seconds,tti->TCI.frames);
	printf("TCO: %02d:%02d:%02d:%02d\n",tti->TCO.hours,tti->TCO.minutes,tti->TCO.seconds,tti->TCO.frames);
	printf("VP : %hX\n",tti->VP);
	printf("JC : %hX\n",tti->JC);
	printf("CF : %hX\n",tti->CF);

	int i = 0;
	int j = 0;
	for(i = 0; i < 112; i++){
		if(tti->TF[i] == 0X1E){
			tti->TF[i] = 0x8A;
		}
		if(tti->TF[i] >= 0x20 && tti->TF[i] < 0x80){
			j++;
		}
		if(tti->TF[i] == 0x8A || tti->TF[i] == 0x8F){
			printf("Char Count : %d\t%s\n",j,tti->TF);
			j = 0;
			if(tti->TF[i] == 0x8F){
				break;
			}
		}

	}


	printf("TF : %.112s\n",tti->TF);
}

int main(int argc, const char** argv) {
	char * output = NULL;
	int full = 0;
	int i = 0;
	for (i = 1; i < argc; ++i)
	{
		if(!strcmp(argv[i],"-f")){
			full = 1;
		}
		else{
			output = (char *) argv[i];
		}
	}

	if(output == NULL){
		if(output == NULL)
			printf("no output set\n");
		printf("Usage: %s [-f] input.stl\n",argv[0]);
		return 0;
	}

	FILE* source = fopen(output,"r");
	if(source == NULL){
		printf("Error: Source not loaded\n");
		fclose(source);
		return 1;
	}

	struct EBU* ebu = parseEBU(source);
	fclose(source);

	isBelleNuit(ebu);
	

	printf("CPN: %.3s\n",ebu->gsi.CPN);
	printf("DFC: %.8s\n",ebu->gsi.DFC);
	printf("DSC: %c\n",ebu->gsi.DSC);
	printf("CCT: %.2s\n",ebu->gsi.CCT);
	printf("LC : %.2s\n",ebu->gsi.LC);
	printf("OPT: %.32s\n",ebu->gsi.OPT);
	printf("OET: %.32s\n",ebu->gsi.OET);
	printf("TPT: %.32s\n",ebu->gsi.TPT);
	printf("TET: %.32s\n",ebu->gsi.TET);
	printf("TN : %.32s\n",ebu->gsi.TN);
	printf("TCD: %.32s\n",ebu->gsi.TCD);
	printf("SLR: %.16s\n",ebu->gsi.SLR);
	printf("CD : %.6s\n",ebu->gsi.CD);
	printf("RD : %.6s\n",ebu->gsi.RD);
	printf("RN : %.2s\n",ebu->gsi.RN);
	printf("TNB: %.5s\n",ebu->gsi.TNB);
	printf("TNS: %.5s\n",ebu->gsi.TNS);
	printf("TNG: %.3s\n",ebu->gsi.TNG);
	printf("MNC: %.2s\n",ebu->gsi.MNC);
	printf("MNR: %.2s\n",ebu->gsi.MNR);
	printf("TCS: %hX\n",ebu->gsi.TCS);	

	struct EBU_TC *tc = charToTC(ebu->gsi.TCP);
	printf("TCP: %02hd:%02hd:%02hd:%02hd\n", tc->hours,tc->minutes,tc->seconds,tc->frames);
	free(tc);
	tc = charToTC(ebu->gsi.TCF);
	printf("TCF: %02hd:%02hd:%02hd:%02hd\n", tc->hours,tc->minutes,tc->seconds,tc->frames);
	free(tc);

	printf("TND: %hX\n",ebu->gsi.TND);
	printf("DSN: %hX\n",ebu->gsi.DSN);
	printf("CO : %.3s\n",ebu->gsi.CO);
	printf("PUB: %.32s\n",ebu->gsi.PUB);
	printf("EN : %.32s\n",ebu->gsi.EN);
	printf("ECD: %.32s\n",ebu->gsi.ECD);

	if(full == 1){
		char TNB[6];
		strncpy(TNB,ebu->gsi.TNB,5);
		TNB[5] = '\0';
		int nTNB = atoi(TNB);
		printf("%d\n",nTNB);
		for(i = 0; i < nTNB; i++){
			dumpTTI(&(ebu->tti[i]));
		}
	}
	
	free(ebu);

	return 0;
}