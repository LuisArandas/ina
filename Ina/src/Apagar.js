/*
    Luis Arandas 2018
*/

outlets=1;
function bang()
{
if(this.patcher.box){
outlet(0, "script delete " + this.patcher.box.varname);
}
}
