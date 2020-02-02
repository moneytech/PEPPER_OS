#include "caches.h"
#include "../../initialisation/io.h"

/*
    The memory type range registers (MTRRs) provide a mechanism for 
    associating the memory types with physical-address ranges in system memory.
    They allow the processor to optimize operations for different types of memory such as RAM, 
    ROM, frame-buffer memory, and memory-mapped I/O devices.
*/

/*
    N'importe quel changements des valeurs des registres MTRRs  ,requiert
    un système d'exploitation capable de repéter le chargement et l'activation le processus 
*/

/*
    Lors de la réinitialisation , les processeurs éffacent les indicateurs plus valide
    dans le MTRR et effece l'indicateur E dans le MSR IA32_MTRR_DEF_TYPE pour désactiver tout les MSR
    (Tout les autres bits des MTRR ne sont plusdéfinis)

    Avant l'initialisation , des MTRR , le BIOS doit initialiser toutes les plages fixes 
    des variables en fonction des mémoires connus .
*/

/*
    Tout les processeurs doivent avoir les mêmes valeurs MTRR

    le système d'exploiatation initialise le MP jusqu'à ce que le registre MTRRdeftype soit à zero
    une fois que tout les processeur ont chargés leurs MTRR ,
    
     une synchroisation empêche d'autre accès à la mémoire jusqu'à ce que tout 
     les processeurs indiquent que les MTRR sont activés.
*/

//Faire en sorte que chaque processeur puisse charger leurs MTRR ,  le OS peut maintenant activer leurs MTTRs

inline uint32_t EnablingMTRRs() {
    cli;
}
