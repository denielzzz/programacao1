#include <stdio.h>
#include "libConfeitaria.h"

int main()
{
    frigobar frig;    
    frigobar *f;

    f = criaFrigobar(&frig);
    alocaProdutos(f);
    destroiFrigobar(f);

    return 0;
}