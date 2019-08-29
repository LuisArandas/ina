/*
	Luis Arandas 2018
*/

outlets = 1;

function bang()
{
	this.patcher.apply(iterfun);
}

function iterfun(b) {
	if (b.varname != ""){
	outlet(0, b.varname);
	}	
	return true;	
}

iterfun.local=1; 

//aplydeep se quiser todos mesmo