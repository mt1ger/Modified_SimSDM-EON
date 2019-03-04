#include <iostream>
#include <string>
#include "ModulationFormats1.h"

using namespace std;

/****************************************
 * Modulation Format VS Reachability:
 * BPSK:       5540km   1bit/symbol
 * QPSK:       5540km   2bits/symbol
 * 16QAM:      2375km   4bits/symbol
 * 64QAM:      916km    6bits/symbol
 ****************************************/


unsigned int ModulationFormats::spectralslots_computation (unsigned int BitsPerSymbol, unsigned int bm_SpectralSlots) {
	unsigned int SpectralSlots;
	// "bm" means before modulation
	if (bm_SpectralSlots % BitsPerSymbol != 0) {
		SpectralSlots = bm_SpectralSlots / BitsPerSymbol + 1;	
	}
	else SpectralSlots = bm_SpectralSlots / BitsPerSymbol;

	return SpectralSlots;
}


double ModulationFormats::search_link_weight (unsigned int predecessor, unsigned int successor) {
	return network->NodesWeight[predecessor][successor];
}


void ModulationFormats::mf_chosen (vector<int> & shortestPath, unsigned int * occupiedSpectralSlots, unsigned int * dataSize, string *MF, unsigned int *mfTimes) {
	double TotalDist = 0; 
	double Dist = 0;
	unsigned int am_SpectralSlots;


	for (int i = 1; i < shortestPath.size (); i++) {
		Dist = search_link_weight (shortestPath[i - 1], shortestPath[i]);
		TotalDist = TotalDist + Dist;
	}

	if (TotalDist > 2375 && TotalDist <= 5540) {
		m_Format = QPSK;
		*mfTimes = 2;
		am_SpectralSlots = spectralslots_computation (*mfTimes, *occupiedSpectralSlots);
		*MF = "QPSK";
	}
	else if (TotalDist > 916 && TotalDist <= 2375) {
		m_Format = QAM16;
		*mfTimes = 4;
		*MF = "16QAM";
		am_SpectralSlots = spectralslots_computation (*mfTimes, *occupiedSpectralSlots);
	}
	else if (TotalDist <= 916) {
		m_Format = QAM64;
		*mfTimes = 6;
		*MF = "64QAM";
		am_SpectralSlots = spectralslots_computation (*mfTimes, *occupiedSpectralSlots);
	}

	*occupiedSpectralSlots = am_SpectralSlots;
}

