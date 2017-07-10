check_lets();

var let_left = let0();
var let_center = let1();
var let_right = let2();

if(!let_left && !let_center && !let_right){
   // console.log('_1')
   forward(100);
}else if(!let_left && let_center && let_right){
   // console.log('_2')
   backward(400);
   turnleft(400);
}else if(!let_left && !let_center && let_right){
   // console.log('_3')
   backward(200);
   turnleft(400);
}else if(let_left && let_center && !let_right){
   // console.log('_4')
   backward(200);
   turnright(400);
}else if(let_left && !let_center && !let_right){
   // console.log('_5')
   backward(200);
   turnright(300);
}else if(let_left && let_center && let_right){
   // console.log('_6')
   backward(400);
   turnleft(400);
}else if(!let_left && let_center && !let_right){
   // console.log('_7')
   backward(200);
   turnleft(200);
}else if(let_left && !let_center && let_right){
   // console.log('_8')
   backward(600);
   turnleft(400);
}
