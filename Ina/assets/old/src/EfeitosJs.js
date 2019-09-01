/*
 Luis Arandas 2018
*/

 autowatch = 1;

 inlets = 12;
 outlets = 12;
 var efeitos = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
 var nome = "inv ";

setinletassist(0,"Efeito1");
setinletassist(1,"Efeito2");
setinletassist(2,"Efeito3");
setinletassist(3,"Efeito4");
setinletassist(4,"Efeito5");
setinletassist(5,"Efeito6");
setinletassist(6,"Efeito7");
setinletassist(7,"Efeito8");
setinletassist(8,"Efeito9");
setinletassist(9,"Efeito10");
setinletassist(10,"Efeito11");
setinletassist(11,"Efeito12");

 function msg_int(val){
    if (inlet == 0){   
        efeitos[1] = val;
        outlet(0, nome + val);
        } 
    else if (inlet == 1){
        efeitos[2] = val;
        outlet(1, nome + val);
        }
    else if (inlet == 2){
        efeitos[3] = val;
        outlet(2, nome + val);
        }
    else if (inlet == 3){
        efeitos[4] = val;
        outlet(3, nome + val);
        }    
    else if (inlet == 4){
        efeitos[5] = val;
        outlet(4, nome + val);
        }
    else if (inlet == 5){
        efeitos[6] = val;
        outlet(5, nome + val);
        }
    else if (inlet == 6){
        efeitos[7] = val;
        outlet(6, nome + val);
        }
    else if (inlet == 7){
        efeitos[8] = val;
        outlet(7, nome + val);
        }    
    else if (inlet == 8){
        efeitos[9] = val;
        outlet(8, nome + val);
        }
    else if (inlet == 9){
        efeitos[10] = val;
        outlet(9, nome + val);
        }
    else if (inlet == 10){
        efeitos[11] = val;
        outlet(10, nome + val);
        }
    else if (inlet == 11){
        efeitos[12] = val;
        outlet(11, nome + val);
        }      
}

/*

i didnt get this to work, feel free to do it
lambda pointer to array 

autowatch = 1;

var size = 12;

inlets = size;
outlets = size;
var efeitos = Array.from(Array(size), () => 0);
var nome = "inv ";

for (var i = 0; i < size; i++) {
    setinletassist(i, "Efeito" + (i+1));
}

function msg_ist(val){
    efeitos[inlet] = val;
    outlet(inlet, nome + val);
}

*/