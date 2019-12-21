

#ifndef _VIDEO_H_
    #define _VIDEO_H_

    #define LOADING_COLOR 0X01 //Couleur dédié pour le chargement d'une fonctionnalité
    #define READY_COLOR 0X07 //Couleur dédié pour une action qui c'est déjà produite
    #define ADVICE_COLOR 0X06 //Couleur pour les conseils
    #define ERROR_COLOR 0x04 //Couleur pour les erreurs

    #define VIDEO_MEM 0xb8000

    
    static int CURSOR_Y = 0 ;
    static int CURSOR_X = 0;

    
    void  write_string( unsigned char colour, const char *string );
    void  pepper_screen() ;
    void  scrollup() ;
    void  putchar(unsigned char color , unsigned const char c) ;

    /*Print ADDRESS*/

    void  print_address (unsigned char color , unsigned int adress_) ;


#endif // !VIDEO_H
