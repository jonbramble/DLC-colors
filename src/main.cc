#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../include/DLC_colors.h"


int main(int argc, char* argv[]) {
	/*int c, index;
	char* closed; 
	char* prima;
	char* appname = argv[0];
	vector<char*> ifiles;

	int one = 1, fd;
	int multiplefiles = (argc > 1);*/
	
	
	/*while((c = getopt(argc, argv, "p:c:")) != -1){			//taken from gnu example
		switch(c){
		case 'p':
			prima = optarg;
			break;
		case 'c':
			closed = optarg;
			break;
		default:
			fprintf(stderr, "usage: %s [-p background] [-c closed] others.tif ...\n", appname);
			exit(-1);
			
		}
	}*/

     

	//printf ("prima = %s, closed = %s\n ", prima, closed);

	//for (index = optind; index < argc; index++){
       // 	printf ("Image files %s\n", argv[index]);
	//	ifiles.push_back(argv[index]);
	//}


	DLC colors;
	colors.Test();
	
	
	return 0;
}
