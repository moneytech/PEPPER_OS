#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"
#include "kernel/initialisation/io.h"
#include "kernel/memory_management/paging/paging.h"
#include "stdlib/video.h"

unsigned int main() {
  pepper_screen();
  cli;

  init_gdt();

  kprintf(2, READY_COLOR, "Chargement des segments\n");

  init_idt();

  if (DetectPSE32bit & DetectPGE & DetectPAT & DetectMTRR & DetectMSR) {
	 kprintf(2, READY_COLOR, "PSE 32bit , PAT, MTRRs and PGE detected \n");

	 if (LinearAddress == 0)
		kprintf(2, READY_COLOR,
		        "Linear address with max width 32bits supported ,\n");
	 else
		kprintf(2, READY_COLOR,
		        "Linear address with max width 48bits supported ,\n");

	 if (PhysicalAddress == 0)
		kprintf(2, READY_COLOR,
		        "Physical address with max width 32bits supported\n");
	 else
		kprintf(2, READY_COLOR, "Physical address with width 36bits supported\n");

	 init_paging();

	 kprintf(2, READY_COLOR, "Enabling 32-bit paging\n");

	 kprintf(3, READY_COLOR,
	         "Map kernel at physical address [%]\nInitialisation de 4Mo de "
	         "memoire physique\n",
	         get_phyaddr((virtaddr_t)(main)));

	 kprintf(2, READY_COLOR, "Physical memory manager loaded\n");
  }

  sti;
  while (1)
	 ;
  return 0;
}