
extern unsigned int _error_code_ ;

void _general_protection_fault_(void)
{
    if ( (_error_code_ & 0x1) == 0x0);
        //external event didn't occur
    else if ((_error_code_ & 0x1) == 0x1);
        //External error occur

    else  ;
    


}


