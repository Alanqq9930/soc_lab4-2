#include "fir.h"

void __attribute__ ( ( section ( ".mprjram" ) ) ) initfir() {
	//initial your fir
	for(int n=0;n<N;n++){
	    outputsignal[n] = 0;
	    inputbuffer[n] = 0;
	}
	
}

int* __attribute__ ( ( section ( ".mprjram" ) ) ) fir(){
	initfir();
	//write down your fir
	for(int i=0;i<N;i++){
	    for(int k=N-1;k>0;k--){
	    	inputbuffer[k] = inputbuffer[k-1];
	    }
	    inputbuffer[0] = inputsignal[i];
	    for(int j=0;j<N;j++){
	    	outputsignal[i] += inputbuffer[j] * taps[j];
	    }
	}
	
	return outputsignal;
}
