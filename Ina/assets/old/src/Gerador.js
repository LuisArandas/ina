/*
    Luis Arandas 2018 
*/

autowatch = 1;
inlets = 1;
outlets = 2;


function msg_int(val){
    if (val == 0){   
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador");
        }
    else if (val == 1){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[1]");
        }
	else if (val == 2){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[2]");
        }
	else if (val == 3){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[3]");
        }
	else if (val == 4){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[4]");
        }
	else if (val == 5){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[5]");
        }
	else if (val == 6){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[6]");
        }
	else if (val == 7){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[7]");
        }
	else if (val == 8){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[8]");
        }
	else if (val == 9){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[9]");
        }
	else if (val == 10){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[10]");
        }
	else if (val == 11){
        outlet(0, "Gerador.maxpat");
		outlet(1, "Gerador[11]");
        }

}

/*

i didnt get this to work, feel free to do it


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