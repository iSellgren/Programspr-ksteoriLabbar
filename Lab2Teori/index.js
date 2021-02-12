Array.prototype.iterator = function() {

    return{
        pos : 0,
        array : this,
        
        hasNext : function(){
            return this.pos < array.length
        },
        next : function(){
            return array[this.pos++]
        }
    }
}
var makeARandomNumber = function(){
    return Math.floor(Math.random() *100);
}
var array = Array(5).fill(0).map(makeARandomNumber);
console.log(array)
var it = array.iterator()
while(it.hasNext()){
    console.log(it.next())
}


