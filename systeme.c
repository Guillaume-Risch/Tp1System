
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "cpu.h"
#include "asm.h"
#include "systeme.h"


/**********************************************************
** Structures de données de l'ordonnanceur (représentation
** d'un ensemble de processus).
**
** SUIVEZ L'ORDRE DES QUESTIONS DU SUJET DE TP.
***********************************************************/

#define MAX_PROCESS  (20)   /* nb maximum de processus  */

typedef enum {
    EMPTY = 0,              /* processus non-prêt       */
    READY = 1,              /* processus prêt           */
} STATE;                    /* État d'un processus      */

typedef struct {
    PSW    cpu;             /* mot d'état du processeur */
    STATE  state;           /* état du processus        */
} PCB;                      /* Un Process Control Block */

PCB process[MAX_PROCESS];   /* table des processus      */

int current_process = -1;   /* nu du processus courant  */
int nb_ready        =  0;   /* nb de processus prêts    */


/**********************************************************
** Changer l'état d'un processus.
**
** SUIVEZ L'ORDRE DES QUESTIONS DU SUJET DE TP.
***********************************************************/

void change_state(int p, STATE new_state) {
    assert((0 <= p) && (p < MAX_PROCESS));
    STATE old_state = process[p].state;
    process[p].state = new_state;
    if (old_state == READY) nb_ready--;
    if (new_state == READY) nb_ready++;
}


/**********************************************************
** Ordonnancer l'exécution des processus.
**
** SUIVEZ L'ORDRE DES QUESTIONS DU SUJET DE TP.
***********************************************************/

PSW scheduler(PSW cpu) {
    printf("Fonction scheduler() à terminer\n");
    return cpu;
}


/**********************************************************
** Démarrage du système (création d'un programme)
***********************************************************/

PSW system_init(void) {

    printf("Booting\n");
    
    /*** création d'un programme P1 ***/
    
    PSW P1_cpu = {
        .PC = 20, // le code démarre au début du segment
        .SB = 20, // le segment débute à l'adr physique 20
        .SE = 30, // et se termine à l'adresse 30
                  // les autres registres sont à zéro
    };
    
    assemble(P1_cpu.SB, "prog1.asm");
    
    return P1_cpu;
}


/**********************************************************
** Traitement des appels au système
***********************************************************/


PSW sysc_exit(PSW cpu){
    exit(EXIT_SUCCESS);
    return cpu; 
}

PSW sysc_puti(PSW cpu){
    printf("Entier : %d \n", cpu.RI.arg);
    return cpu;
}


enum {
    SYSC_EXIT = 100,   // fin du processus courant
    SYSC_PUTI = 200,   // afficher le contenu de Ri
};

static PSW process_system_call(PSW cpu) {
    // suivant l'argument de sysc Ri, Rj, ARG
    switch (cpu.RI.arg) {
        case SYSC_EXIT:
            cpu = sysc_exit(cpu);
            break;
        case SYSC_PUTI:
            cpu = sysc_puti(cpu);
            break;
        default:
            printf("Appel système inconnu %d\n", cpu.RI.arg);
            break;
    }
    return cpu;
}


/**********************************************************
** Traitement des interruptions par le système (mode système)
***********************************************************/

PSW process_interrupt(PSW cpu) {
    printf("num_INT %d \n", cpu.IN );
    switch (cpu.IN) {
        case INT_SEGV:
        printf("Error SEGV %d \n", INT_SEGV);
            exit(EXIT_SUCCESS);
            break;
        case INT_INST:
        printf("Error INST %d \n", INT_INST);
            exit(EXIT_SUCCESS);
            break;
        case INT_TRACE:
            dump_cpu(cpu); 
            break;
        case INT_SYSC:
            cpu = process_system_call(cpu);
            break;
        case INT_KEYBOARD:
            break;
        default:
            break;
    }
    return cpu;
}
