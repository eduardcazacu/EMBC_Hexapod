#include "sequencer.h"
#include "pragma.h"




int freq = 40000000;

void main() {
    sequencer_init(freq);
    
    sequencer_wake();
    while(1) {
        sequencer_walkForward();
    }
}